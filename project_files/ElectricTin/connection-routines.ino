void serialStart(void)
{
  char      buff[200];
  char      buff1[20];
  uint8_t   serialCount;

  Serial.begin(115200);
  serialCount = 0;
  while ((!Serial) && (serialCount < 5)) 
  {
    serialCount++;
    delay(500);
  }

  if(Serial)
  {
    sprintf(buff, "Program name: %s\nProgram date: %s\nProgram version: %s\n", PROGRAM_NAME, PROGRAM_DATE, PROGRAM_VERSION);
    Serial.println(buff);
  }
}

void wifiStart(void)
{
  char      programNameWithPath[200];
  char      programName[50];
  uint8_t   loopCount;
  uint8_t   start;
  uint8_t   end;
  uint8_t   networkNumber;

  strcpy(programNameWithPath, PROGRAM_NAME);
  loopCount = strlen(programNameWithPath);

  while((loopCount>0) && (programNameWithPath[loopCount]!='\\'))
  {
    loopCount--;
  }
  strncpy(programName, &programNameWithPath[loopCount+1], strlen(programNameWithPath)-loopCount-5);   // remove .cpp from filename
  programName[strlen(programNameWithPath)-loopCount-5] = '\0';                                        // add termination

  matrix.begin();
  matrix.fillScreen(0);
  write_big_title();

  matrix.setTextColor(WHITE);
  setMessageFont();
  matrix.setTextWrap(true);
  matrix.setCursor(0, MESSAGELINE);
  matrix.print("File:\n");
  matrix.print(programName);
  matrix.print("\nVer: ");
  matrix.print(PROGRAM_VERSION);
  matrix.print("\nDate: ");
  matrix.print(PROGRAM_DATE);
  matrix.show();
  delay(5000);

  Serial.println("Scanning available networks...");
  networkNumber = listNetworks();

  #ifdef DEBUG
    Serial.print("\nConnecting to SSID: ");
    Serial.println(SSID_ARRAY[networkNumber]);
  #endif

  Serial.print("Network number: "); Serial.println(networkNumber);

  clearUnderTitle();
  setMessageFont();
  matrix.setCursor(0, MESSAGELINE);
  matrix.setTextColor(WHITE);
  matrix.print("Going online with: \n");

  #ifdef DEBUG
    matrix.print("Local SSID");
  #else
    matrix.print(SSID_ARRAY[networkNumber]);
  #endif
  
  matrix.show();
 
  matrix.setCursor(0, 56);

  // disconnect the WiFi after possible programming
  // any other (re)start probably disconnects the WiFi without having to do this
  WiFi.disconnect();

  loopCount = 0;
  WiFi.begin(SSID_ARRAY[networkNumber], PASSWORD_ARRAY[networkNumber]);
  while(WiFi.status() != WL_CONNECTED)
  {
    // Failed, retry
    Serial.print(WiFi.status());
    Serial.print(".");
    
    matrix.setCursor(0, MESSAGELINE+12);
    matrix.print("Wifi Status: ");
    matrix.print(WiFi.status());
    matrix.show();
    // disconnect the WiFi after board programming - wifi link is not disconnected after programming
    // any other (re)start probably disconnects the WiFi without having to do this
  //  WiFi.disconnect();
    delay(500);
    loopCount++;
    Serial.print("_");Serial.print(loopCount);Serial.print("_");

    matrix.setCursor(0, MESSAGELINE+18);
    matrix.print("Loop count: ");
    matrix.print(loopCount);
    matrix.show();

    #ifdef MPS3                       // seems to be necessary for ESP32
      sslClient.setInsecure();
      sslClient.setBufferSizes(1024 /* rx */, 512 /* tx */);

      #ifdef DEBUG
        sslClient.setDebugLevel(1);
      #else
        sslClient.setDebugLevel(0);
      #endif
      // In case ESP32 WiFiClient, the session timeout should be set,
      // if the TCP session was kept alive because it was unable to detect the server disconnection.
      sslClient.setSessionTimeout(120); // timeout in seconds

      // Assign the basic client
      // Due to the basic_client pointer is assigned, to avoid dangling pointer, basic_client should be existed
      // as long as it was used by ssl_client for transportation.
      sslClient.setClient(&basicClient);
    #endif

  }
}

void mqttStart(void)
{
  uint8_t     loopCount;
  #ifdef DEBUG
    Serial.print("\nConnecting to MQTT broker: ");
    Serial.println(MQTT_BROKER);
  #endif

  mqttClient.setUsernamePassword(MQTT_USERNAME, MQTT_PASSWORD);   // give the MQTT client the username/password
  
  //clearScreen();
  //write_big_title();
  clearUnderTitle();                                        // clear the area for a message
  setMessageFont();                                         // set font
  matrix.setCursor(0,MESSAGELINE);                          // set cursor
  matrix.setTextColor(RED);                                 // set colour
  matrix.print("Connecting to MQTT broker");                // print the message

  #ifdef DEBUG
    matrix.print(": \n");
    matrix.print(MQTT_BROKER);
  #endif
  matrix.show();                                            // show the message
  
  loopCount = 0;                                            // keep count of the tries
  if(!mqttClient.connect(MQTT_BROKER, MQTT_PORT))           // try to connect
  {
    #ifdef DEBUG
      Serial.print("MQTT connection failure. Error code: ");  
      Serial.println(mqttClient.connectError());
    #endif

    loopCount++;                                            // increment loopCount
    #ifdef DEBUG
      Serial.print(loopCount);                              // show loopCount
    #endif
    delay(500);                                             // wait a bit
    mqttClient.stop();                                      // stop the MQTT client
    delay(500);                                             // wait a bit
  }

  delay(2000);                                              // allow user to read message
  clearUnderTitle();                                        // clear the message area
  matrix.setCursor(0, MESSAGELINE);                         // set the cursor
  #ifdef DEBUG
    Serial.print("Connected to MQTT broker");Serial.println(MQTT_BROKER);Serial.print("\n");
  #endif
  matrix.setTextColor(GREEN);
  matrix.print("Connected to MQTT broker");
  #ifdef DEBUG
    matrix.print(": ");
    matrix.print(MQTT_BROKER);
  #endif
  matrix.show();
  delay(2000);

  // callback setup
  mqttClient.onMessage(onMqttMessage);
  Serial.println("Setup callback function");
  delay(100);
}

void startSubscription(void)
{
  Serial.print("Subscribing to topic: ");Serial.println(MQTT_TOPIC);
  clearNameLeft();
  clearNameRight();
  clearScore();

  setMessageFont();
  setTextColour(WHITE);
  matrix.setCursor(0, MESSAGELINE);
  matrix.print("Topic: ");
  matrix.setCursor(0, 41);
  matrix.print(MQTT_TOPIC);
  matrix.show();

  mqttClient.subscribe(MQTT_TOPIC);
}

uint8_t listNetworks() {
  int     numSsid;
  int     thisNet;
  int     loopCount;
  int     innerLoop;
  bool    matchFound;
  char    ssidList[40][30];

  // scan for nearby networks:
  Serial.println("** Scan Networks **");
  numSsid = WiFi.scanNetworks();
  if (numSsid == -1) {
    Serial.println("Couldn't get a WiFi connection");
    while (true);
  }

  // print the list of networks seen:

  #ifdef DEBUG
    Serial.print("number of available networks: ");
    Serial.println(numSsid);

    // print the network number and name for each network found:
    for (thisNet = 0; thisNet < numSsid; thisNet++) 
    {
      Serial.print(thisNet);
      Serial.print(") ");
      Serial.print(WiFi.SSID(thisNet));
      #ifdef MPS3
        strcpy(ssidList[thisNet], WiFi.SSID(thisNet).c_str());    // copy the ***String*** to a char array
      #endif
      Serial.print("\tSignal: ");
      Serial.print(WiFi.RSSI(thisNet));
      Serial.println(" dBm");
    }
  #endif

  matchFound = false;
  loopCount = 0;

  while(!matchFound)
  {
    while((loopCount < numSsid) && (!matchFound))
    {
      innerLoop = 0;
      while((innerLoop<ARRAY_LENGTH) && (!matchFound))
      {
        Serial.print(innerLoop);Serial.print("\t");Serial.println(SSID_ARRAY[innerLoop]);
        #ifdef MPM4
          if(strcmp(SSID_ARRAY[innerLoop], WiFi.SSID(loopCount)) == 0)
        #else
          if(strcmp(SSID_ARRAY[innerLoop], ssidList[loopCount]) == 0) // commented out for S3 compilation
        #endif
        {                                                            // ...
          matchFound = true;                                         // ...
        }                                                            // ...
        if(!matchFound)
          innerLoop++;
      }
      if(!matchFound)
        loopCount++;
    }
  }

  Serial.print("matchFound value: "); Serial.println(matchFound);
  Serial.print("innerLoop: ");Serial.print(innerLoop);Serial.print("\t");Serial.println(SSID_ARRAY[innerLoop]);

  return(innerLoop);
}
