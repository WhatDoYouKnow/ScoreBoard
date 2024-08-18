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

  //clearScreen();
  //write_big_title();

  // //******************************************************************
  // Serial.println("Going online with TP-Link_IoT_7981");
  // WiFi.disconnect();
  // Serial.print("WiFi status after disconnection: ");Serial.println(WiFi.status());

  // loopCount = 0;

  // WiFi.begin("TP-Link_IoT_7981", "helloworld");
  // while(WiFi.status() != WL_CONNECTED)
  // {
  //   // Failed, retry
  //   Serial.print(WiFi.status());
  //   Serial.print(".");

  // }
    
  // //   // disconnect the WiFi after board programming - wifi link is not disconnected after programming
  // //   // any other (re)start probably disconnects the WiFi without having to do this
  // //   WiFi.disconnect();
  // //   delay(500);
  // //   WiFi.begin("TP-Link_IoT_7981", "helloworld");
  // //   loopCount++;
  // //   Serial.print("_");Serial.print(loopCount);Serial.print("_");
  // // }

  // while(1)
  // {
  //   Serial.println("Waiting...");
  //   delay(1000);
  // }

//***********************************************************************











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
  Serial.print("\nConnecting to MQTT broker: ");
  Serial.println(MQTT_BROKER);

  mqttClient.setUsernamePassword(MQTT_USERNAME, MQTT_PASSWORD);
  
  //clearScreen();
  //write_big_title();
  clearUnderTitle();
  setMessageFont();
  matrix.setCursor(0,MESSAGELINE);
  matrix.setTextColor(RED);
  matrix.print("Connecting to MQTT broker");

  #ifdef DEBUG
    matrix.print(": \n");
    matrix.print(MQTT_BROKER);
  #endif
  matrix.show();
  
  loopCount = 0;
  if(!mqttClient.connect(MQTT_BROKER, MQTT_PORT))
  {
    Serial.print("MQTT connection failure. Error code: ");
    Serial.println(mqttClient.connectError());
    loopCount++;
    Serial.print(loopCount);
    delay(500);
    //mqttClient.stop();
    mqttClient.stop();
    delay(500);
  }

  delay(2000);
  //clearScreen();
  clearUnderTitle();
  matrix.setCursor(0, MESSAGELINE);
  Serial.print("Connected to MQTT broker");Serial.println(MQTT_BROKER);Serial.print("\n");
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
      strcpy(ssidList[thisNet], WiFi.SSID(thisNet).c_str());    // copy the ***String*** to a char array
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
        if(strcmp(SSID_ARRAY[innerLoop], ssidList[loopCount]) == 0) // commented out for S3 compilation
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
