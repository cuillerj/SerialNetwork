#!/bin/bash
# send UDP actif
echo "sleep 1mn"
sleep 1m
cd /home/pi/project
echo  $(date) >  log/SerialUdpGateway.log
./SerialUdpGateway /dev/ttyUSB0 19200 8 0 0 1 >> log/SerialUdpGateway.log 
