# senseBox:bike 
Mobile Messstation mit der Temperatur, rel. Luftfeuchte, Feinstaub (PM10, PM25), Abstand nach links und rechts sowie die Bodenerschütterung gemessen werden. Die Daten fließen in ein Exponat im [Futurium](https://futurium.de). 


[![senseBox Bike](https://raw.githubusercontent.com/sensebox/senseBox-bike-Arduino/main/senseBox_bike.png "senseBox Bike")](https://raw.githubusercontent.com/sensebox/senseBox-bike-Arduino/main/senseBox_bike.png "senseBox Bike")


# Anleitung 
## Vorbereitungen für Arduino 
 - Installiere das [Board-Support-Package der senseBox](https://docs.sensebox.de/arduino/board-support-package-installieren/ "Board-Support-Package der senseBxo") 
 - Installiere folgende Libraries: 
  - [NewPing](https://www.arduino.cc/reference/en/libraries/newping "NewPing")
  - [SparkFun u-Blox GNSS](https://www.arduino.cc/reference/en/libraries/sparkfun-u-blox-gnss-arduino-library/ "SparkFun u-Blox GNSS")
  - [BMX](https://github.com/sensebox/BMX)
  - [SD](https://www.arduino.cc/en/Reference/SD "SD") For Linux systems
  - [SDConfig](https://github.com/Fuzzer11/SDconfig)
  - [ArduinoOTA](tbd)
  - [ArduinoHttpClient](tbd)
  
## Credentials hinzufügen 
Mit Hilfe der [openSenseMapAPI](https://api.opensensemap.org/) kannst du die Sensor IDs deiner Box abfragen. Der API call lautet `https://api.opensensemap.org/boxes/[:senseBoxID]`.
 - In der Datei `variables/network.h` WiFi Informationen (SSID und Passwort) eingeben 
 - In der Datei `variables/ids.h` senseBox ID und Sensor ID's von der openSenseMap eintragen 

Alternativ kann auf der SDCard eine Datei `BIKE.CFG` (Siehe `BIKE.CFG.example`) angelegt werden, die die Variablen Datenschutzfreundlich initialisiert.

## Upload
- Mit Hilfe der Arduino IDE auf die senseBox hochladen

## Upload via OverTheAir (OTA)

Es besteht die Möglichkeit, die Firmware auch via OTA zu hoch zu laden. Die folgenden Steps und Voraussetzungen sind erforderlich:

1. Diese Firmware hochladen, dass sollte das letzte mal sein, dass die BikeBox aufgeschraubt werden muss.
2. Bei Veränderungen der Firmware in `variables/version.h` die Versionsnummer um eins erhöhen.
3. mit `prepare_fw.sh` neue Firmware in das Verzeichnis `html` erstellen. Das Namensschema hier ist `SENSEBOX_ID-Vn.bin`
4. In der `BIKE.CFG` die WLAN *SSID* und den *PSK*  eintragen und auf die SD Karte der Bikebox schreiben. (Siehe `BIKE.CFG.example`)
5. Die BikeBox mit der neuen Firmware starten
   1. Die BikeBox sucht beim Starten nach dem versorgtem WLAN und versucht eine Firmware in der Version V+1 zu finden und zu laden. (In einer künftigen Version kann auch die `BIKE.CFG` auf der SD Card durch die in `html` hinterlegte `SENSEBOX_ID-Vn.cfg` überschrieben werden.)
   2. Bei Erfolg wir die geladene Firmware in den Flash geschrieben und die BikeBox neu gestartet.
   3. Wenn keine neue Firmware gefunden wird, geht die BikeBox in den Regelbetrieb und die gesammelten Fahrrad-Daten werden hochgeladen und die BikeBox geht in den Standby.



## Entwickeln
 - Damit du das Projekt in der Arduino IDE öffnen kannst, muss der Name des Ordners mit dem der Einstiegsdatei übereinstimmen.
 - Dafür clone das Projekt wie folgt:

   ``` bash
   $ git clone git@github.com:sensebox/senseBox-bike-Arduino.git futuriumSDNoAp
   ```
  - Um das gesamte Projekt zu öffnen, doppelklicke die `futuriumSDNoAp.ino` Datei. Daraufhin sollte sich die Arduino IDE automatisch öffnen und diese und alle anderen Dateien anzeigen (siehe Screenshot).

    ![Arduino IDE Screenshot](arduino-ide-screenshot.png "Arduino IDE Screenshot")

## Debug 

- In `futuriumSDNoAP.ino` das Makro `#define DEBUG_ENABLED` setzen, damit DEBUG Nachrichten in der seriellen Konsole angezeigt werden.
- ⚠️ **ACHTUNG** Im DEBUG Modus startet die Box erst, wenn der *Serielle Monitor* geöffnet wird. Vor dem Betrieb den DEBUG Modus IMMER ausschalten!
