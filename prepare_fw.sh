#!/bin/bash

firmware="${HOME}/.cache/arduino/futuriumSDNoAp.ino.bin"
# firmware="futuriumSDNoAp.ino.sensebox_mcu.bin"
version=${1}
bikebox=$(grep SENSEBOX_ID= BIKE.CFG |cut -d'=' -f2)
host=$(grep OTA_URL= BIKE.CFG |cut -d'=' -f2)
path="html"

# scp "${firmware}" "pi@${host}:${path}/${bikebox}-V${version}".bin
# scp BIKE.CFG      "pi@${host}:${path}/${bikebox}-V${version}".cfg

cp "${firmware}" "${path}/${bikebox}-V${version}".bin
cp BIKE.CFG      "${path}/${bikebox}-V${version}".cfg

# docker run --name ota -p8080:80 -v `pwd`/html:/usr/share/nginx/html/ota/senseBoxBike:ro  -d nginx
