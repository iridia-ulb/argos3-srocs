# Useful commands

```bash 
ffmpeg -f v4l2 -framerate 10 -video_size 800x600 -i /dev/video0 -c:v rawvideo -pix_fmt yuyv422 output.yuv
```

```bash
ffplay -f rawvideo -pixel_format yuyv422 -video_size 800x600 -framerate 10 output.yuv
```