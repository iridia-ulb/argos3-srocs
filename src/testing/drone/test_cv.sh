#!/bin/bash

# TODO sample video can be captured using
# ffmpeg -f v4l2 -framerate 10 -video_size 800x600 -i /dev/video0 -c:v rawvideo -pix_fmt yuyv422 output.yuv

# TODO sample video can be played back using
# ffplay -f rawvideo -pixel_format yuyv422 -video_size 800x600 -framerate 10 output.yuv


#make -C ../argos3/build-hardware || exit 1
#sudo make install -C ../argos3/build-hardware || exit 1
#make -C build || exit 1

# check arguments
if [ ${#} -ne "2" ]; then
   echo "usage: $0 [first device] [number of devices]"
   exit 1
fi

if [ "${1}" -lt "0" ]; then
   echo "[first device] must be a non-negative integer"
   exit 1
fi

if [ "${2}" -lt "1" ]; then
   echo "[number of devices] must be at least 1"
   exit 1
fi

# load loopback module with four devices
echo "creating video devices `seq -s, ${1} $((${1}+${2}-1))`"
sudo modprobe v4l2loopback video_nr=`seq -s, ${1} $((${1}+${2}-1))` || exit 1

tmux new-session -d -s ffmpeg "./v4l2source /dev/video${1} output.yuyv422"
for n in `seq $((${1}+1)) $((${1}+${2}-1))`
do
sleep 0.25
tmux split-window -h "./v4l2source /dev/video${n} output.yuyv422"
done

tmux unbind-key C-c
tmux bind-key -n C-c setw synchronize-panes on\\\; send-keys C-c
tmux select-layout -t ffmpeg even-vertical
tmux attach-session -t ffmpeg

echo "removing video devices `seq -s, ${1} $((${1}+${2}-1))`"
sudo modprobe -r v4l2loopback
