#include <fcntl.h>
#include <termios.h>
#include <stdlib.h>
#include <cstring>
#include <unistd.h>
#include <errno.h>

#include "uart_socket.h"

/***********************************************************/
/***********************************************************/

int CUARTSocket::Open() {
   m_nPort = ::open(m_strDevice.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
   if (m_nPort < 0) {
      return errno;
   }

   /* save current port settings */
   tcgetattr(m_nPort, &sPrevPortSettings);
        
   /* struct for the new port settings */              
   struct termios sPortSettings;
   memset(&sPortSettings, 0, sizeof(sPortSettings));

   /* determine speed */
   speed_t eBaud;
   switch(m_unBaud) {
   case 9600:
      eBaud = B9600;
      break;
   case 19200:
      eBaud = B19200;
      break;
   case 38400:
      eBaud = B38400;
      break;
   case 57600:
      eBaud = B57600;
      break;
   case 115200:
      eBaud = B115200;
      break;
   default:
      return -1;
   }

   sPortSettings.c_cflag = eBaud | CS8 | CLOCAL | CREAD;
   sPortSettings.c_iflag = IGNPAR;
   sPortSettings.c_oflag = 0;
        
   /* set input mode to non-canonical */
   sPortSettings.c_lflag = 0;
         
   sPortSettings.c_cc[VTIME] = 0;   /* inter-character timer unused */
   sPortSettings.c_cc[VMIN] = 0;   /* blocking read until 5 chars received */
        
   tcflush(m_nPort, TCIFLUSH);
   tcsetattr(m_nPort, TCSANOW, &sPortSettings);

   return 0;
}

/***********************************************************/
/***********************************************************/

int CUARTSocket::Close() {
   tcsetattr(m_nPort, TCSANOW, &sPrevPortSettings);
   int nRet = ::close(m_nPort);
   m_nPort = -1;
   return nRet;
}

/***********************************************************/
/***********************************************************/

int16_t CUARTSocket::Read(uint8_t* pun_buffer, size_t un_length) const {
   int16_t nReadBytes = ::read(m_nPort, pun_buffer, un_length);
   return (nReadBytes < 0) ? errno : nReadBytes;
}

/***********************************************************/
/***********************************************************/

int16_t CUARTSocket::Write(const uint8_t* pun_buffer, size_t un_length) const {
   int16_t nWrittenBytes = 0;
   int16_t nRes = 0;
   while (nWrittenBytes < un_length) {
      nRes = ::write(m_nPort, pun_buffer + nWrittenBytes, un_length - nWrittenBytes);
      if(nRes < 0) {
         break;
      }
      else {                    
         nWrittenBytes += nRes;
      }
   }
   return (nWrittenBytes < un_length) ? errno : nWrittenBytes;
}




