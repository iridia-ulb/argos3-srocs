#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>

#include <list>
#include <array>
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <iostream>

#define WIDTH 1024
#define HEIGHT 768
#define BYTES_PER_PIXEL 2
#define BUFFER_SIZE (WIDTH * HEIGHT * BYTES_PER_PIXEL)

bool bSignalRecv = false;

void handler(int n_signal) {
  bSignalRecv = true;
  /* allow the user to kill application immediately with ctrl-c */
  std::signal(SIGINT, SIG_DFL);
  /* ignore other signals and attempt to allow the robot to shutdown */
  std::signal(SIGABRT, SIG_IGN);
  std::signal(SIGFPE, SIG_IGN);
  std::signal(SIGILL, SIG_IGN);
  std::signal(SIGSEGV, SIG_IGN);
  std::signal(SIGTERM, SIG_IGN);
}

int main(int argc, char **argv) {
   /* capture signals */
   std::signal(SIGINT, handler);
   std::signal(SIGABRT, handler);
   std::signal(SIGFPE, handler);
   std::signal(SIGILL, handler);
   std::signal(SIGSEGV, handler);
   std::signal(SIGTERM, handler);

   /* check and parse arguments */
   if( argc != 3 ) {
      std::cerr << "usage: " << argv[0] << " SINK SOURCE" << std::endl;
      std::exit(-1);
   }

   /* open the sink device */
   int sink = ::open(argv[1], O_WRONLY);
   if (sink < 0) {
      std::cerr << "failed to open sink device: " << std::strerror(errno) << std::endl;
      std::exit(-2);
   }

   /* open source file */
   int source = ::open(argv[2], O_RDONLY);
   if(source < 0 ) {
      std::cerr << "failed to open source file: " << std::strerror(errno) << std::endl;
      std::exit(-3);
   }

   /* load frames */
   std::list<std::array<char, BUFFER_SIZE> > lst_frames;
   for(;;) {
      lst_frames.emplace_back();
      if(::read(source, lst_frames.back().data(), BUFFER_SIZE) != BUFFER_SIZE) {
         lst_frames.pop_back();
         break;
      }
   }
   ::close(source);

   /* check if at least one source frame was loaded */
   std::list<std::array<char, BUFFER_SIZE> >::iterator next =
      std::begin(lst_frames);
   if(next == std::end(lst_frames)) {
      std::cerr << "failed to load frames" << std::endl;
      std::exit(-4);
   }

   /* setup video for proper format */
   struct v4l2_format fmt;
   int ret;
   fmt.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
   ret = ioctl(sink, VIDIOC_G_FMT, &fmt);
   if( ret < 0 )
      std::exit(ret);
   fmt.fmt.pix.width = WIDTH;
   fmt.fmt.pix.height = HEIGHT;
   fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
   ret = ioctl(sink, VIDIOC_S_FMT, &fmt);
   if( ret < 0 )
      std::exit(ret);
   
   /* start main loop */
   while (!bSignalRecv) {
      if(::write(sink, next->data(), BUFFER_SIZE) != BUFFER_SIZE) {
         break;
      }
      next++;
      if(next == std::end(lst_frames)) {
         next = std::begin(lst_frames);
      }
      ::usleep(1000000 / 30);
   }
   ::close(sink);
   std::cerr << "closed " << argv[1] << std::endl;
   return 0;
}

