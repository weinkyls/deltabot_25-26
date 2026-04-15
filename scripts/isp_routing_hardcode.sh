#!/bin/bash

fmt="fmt:SRGGB10_1X10/3280x2464 field:none"

sudo media-ctl -d /dev/media1 -r
sudo media-ctl -d /dev/media1 -l "'m01_b_imx219 4-0010':0 -> 'rockchip-csi2-dphy2':0 [1]"
sudo media-ctl -d /dev/media1 -l "'rockchip-csi2-dphy2':1 -> 'rockchip-mipi-csi2':0 [1]"
sudo media-ctl -d /dev/media1 -l "'rockchip-mipi-csi2':1 -> 'stream_cif_mipi_id0':0 [1]"

sudo media-ctl -d /dev/media1 --set-v4l2 "'m01_b_imx219 4-0010':0 [$fmt]"
sudo media-ctl -d /dev/media1 --set-v4l2 "'rockchip-csi2-dphy2':0 [$fmt]"
sudo media-ctl -d /dev/media1 --set-v4l2 "'rockchip-mipi-csi2':0 [$fmt]"

sudo media-ctl -d /dev/media3 -r
sudo media-ctl -d /dev/media3 -l "'rkcif-mipi-lvds3':0 -> 'rkisp-isp-subdev':0 [1]"
sudo media-ctl -d /dev/media3 -l "'rkisp-isp-subdev':2 -> 'rkisp_mainpath':0 [1]"

v4l2-ctl -d /dev/video31 --set-fmt-video=width=3280,height=2464,pixelformat=NV12
