#!/bin/bash
# script for when working camera is at cam0 port
# cam 0 (Media 2 -> /dev/video23)
echo "configuring Media 2..."
# working camera
sudo media-ctl -d /dev/media2 -l '"rkcif-mipi-lvds2":0 -> "rkisp-isp-subdev":0 [1]'
sudo media-ctl -d /dev/media2 -l '"rkisp-isp-subdev":2 -> "rkisp_selfpath":0 [1]'
sudo media-ctl -d /dev/media2 --set-v4l2 '"rkisp-isp-subdev":0 [fmt:SRGGB10_1X10/3280x2464]'
sudo media-ctl -d /dev/media2 --set-v4l2 '"rkisp-isp-subdev":2 [fmt:YUYV8_2X8/3280x2464]'

# cam 1 (Media 3 -> /dev/video32)
echo "configuring Media 3..."

sudo media-ctl -d /dev/media3 -l '"rkcif-mipi-lvds3":0 -> "rkisp-isp-subdev":0 [1]'
sudo media-ctl -d /dev/media3 -l '"rkisp-isp-subdev":2 -> "rkisp_selfpath":0 [1]'
sudo media-ctl -d /dev/media3 --set-v4l2 '"rkisp-isp-subdev":0 [fmt:SRGGB10_1X10/3280x2464]'
sudo media-ctl -d /dev/media3 --set-v4l2 '"rkisp-isp-subdev":2 [fmt:YUYV8_2X8/3280x2464]'

echo "isp pipeline config attempt finished."
