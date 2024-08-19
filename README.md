# ScoreBoard
An HTML-controlled LED-matrix scoreboard using Adafruit Matrix Portal boards (either M4 or S3) with MQTT for comms.\
MQTT requires a broker and not wanting to host my own, I used the HiveMQ free offering.\
A TLS-secured connection is required to keep the MQTT private - this took a while to get going on both the Arduino and HTML but seems to work for me.\
\
Specifically a scoreboard for squash (my preferred game) Arduino code to drive LED panel(s), usually 64x32 or 64x64. Smaller matrices are available. Based on PAR to 15.\
\
Code is written in Arduino IDE (currently 2.3.2) - see:\
    &emsp;/project_files folder for Arduino files\
    &emsp;/project_files/Fonts for font files. The current project uses two font files which are better suited to displaying the score information.\
\
Libraries/versions used:\
  &nbsp;&nbsp;&nbsp;&nbsp;Adafruit_Protomatter.h    1.6.1\
  &nbsp;&nbsp;&nbsp;&nbsp;WiFiNINA.h                1.8.14\
  &nbsp;&nbsp;&nbsp;&nbsp;WiFi.h                    1.2.7\
  &nbsp;&nbsp;&nbsp;&nbsp;ESP_SSLClient.h           2.1.11\
  &nbsp;&nbsp;&nbsp;&nbsp;ArduinoMqttClient.h       0.1.8\
  \
To use this project directly, copy the contents of project_files to a folder in your Arduino folder with the same name as the main file (currently ElectricTin_SSL_h). Copy the two font files to the Adafruit_GFX/Fonts folder otherwise the project will not compile.\
Ensure the #defines are correct for the type of board you are using - either MPM4 or MPS3 - as listed in the code and then compile and upload to your board.\





#












# HTML
The HTML code is definitely a work in progress (my first effort) but will publish to the HiveMQ broker on a topic which is then issued to the subscribing Matrix Portal. Again, this is specific to squash scoring - but could be adapted for anything else.\
\
The HTML page runs the Paho javascript engine for MQTT comms and javascript for the control.\
\
There is the ability to increment/decrement the score for a player and the page confirms the game/match win. Based on PAR to 15.
