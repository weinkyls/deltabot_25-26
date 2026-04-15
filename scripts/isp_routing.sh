#!/bin/bash

# links camera nodes if they exist
route_camera() {
  local MEDIA_DEV=$1
  local SOURCE_ENTITY=$2
  local FMT="SRGGB10_1X10/3280x2464"

  # checking if source entity exists
  if sudo media-ctl -d "$MEDIA_DEV" -p | grep -q "$SOURCE_ENTITY"; then
    echo "--- STARTING: $SOURCE_ENTITY on $MEDIA_DEV ---"

    # find the sensor subdev
    [[ "$SOURCE_ENTITY" == *"lvds2"* ]] && BUS="3-0010" || BUS="4-0010"
    SENSOR_SUBDEV=$(grep -l "$BUS" /sys/class/video4linux/v4l-subdev*/name | cut -d/ -f5)

    if [ ! -z "$SENSOR_SUBDEV" ]; then
      SENSOR_NAME=$(cat "/sys/class/video4linux/$SENSOR_SUBDEV/name" | tr -d '\0')
      [[ "$SOURCE_ENTITY" == *"lvds2"* ]] && S_MEDIA="/dev/media0" || S_MEDIA="/dev/media1"

      echo "step 1: sensor"
      sudo media-ctl -d "$S_MEDIA" --set-v4l2 "'$SENSOR_NAME':0 [fmt:$FMT field:none]"

      echo "step 2: bridge"
      sudo media-ctl -d "$S_MEDIA" --set-v4l2 "'rockchip-mipi-csi2':0 [fmt:$FMT field:none]"
    fi

    echo "step 3: receiver"
    sudo media-ctl -d "$MEDIA_DEV" --set-v4l2 "'$SOURCE_ENTITY':0 [fmt:$FMT field:none]"

    echo "step 4: links"
    sudo media-ctl -d "$MEDIA_DEV" -l "'$SOURCE_ENTITY':0 -> 'rkisp-isp-subdev':0 [1]"
    sudo media-ctl -d "$MEDIA_DEV" -l "'rkisp-isp-subdev':2 -> 'rkisp_selfpath':0 [1]"

    echo "step 5: isp input"
    sudo media-ctl -d "$MEDIA_DEV" --set-v4l2 "'rkisp-isp-subdev':0 [fmt:$FMT field:none]"

    echo "step 6: isp output"
    sudo media-ctl -d "$MEDIA_DEV" --set-v4l2 "'rkisp-isp-subdev':2 [fmt:YUYV8_2X8/3280x2464 field:none]"

    echo "--- DONE ---"
  else
    echo "SKIPPING $SOURCE_ENTITY: NOT FOUND"
  fi
}

# run for both paths
route_camera "/dev/media2" "rkcif-mipi-lvds2"
route_camera "/dev/media3" "rkcif-mipi-lvds3"
