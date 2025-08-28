#!/bin/bash
message=$msg
topic=$tpc
proj_dir="/home/felizr/Arduino/"

sudo arduino-cli sketch new "${proj_dir}project_${topic}"
sudo touch "${proj_dir}project_${topic}/text.ino"
echo $msg > "${proj_dir}project_${topic}/text.ino"
sudo tail -c +2 "${proj_dir}project_${topic}/text.ino" 
	 > "${proj_dir}project_${topic}/project_${topic}.ino"
sudo rm "${proj_dir}project_${topic}/text.ino"
sudo arduino-cli compile --fqbn arduino:mbed_nano:nanorp2040connect 
     "${proj_dir}project_${topic}/project_${topic}.ino"
sleep 2s
if [ ${topic} == "station1" ]; then
  sudo arduino-cli upload -p /dev/ttyACM0 --fqbn arduino:mbed_nano:nanorp2040connect 
  "${proj_dir}project_${topic}"
elif [ ${topic} == "station2" ]; then
  sudo arduino-cli upload -p /dev/ttyACM1 --fqbn arduino:mbed_nano:nanorp2040connect 
  "${proj_dir}project_${topic}"
fi
sudo rm -r "${proj_dir}project_${topic}"

