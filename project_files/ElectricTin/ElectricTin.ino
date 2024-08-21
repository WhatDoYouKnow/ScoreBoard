/* ----------------------------------------------------------------------

ElectricTin_SSL_h is intended to use MQTT over the Matrix Portal S3 or M4 WiFi
to talk to HiveMQ broker - other brokers are available.

Includes:
ArduinoMqttClient.h + dependencies

  WiFiNINA.h      for MPM4
OR
  WiFi.h          for MPS3
  ESP_SSLCient.h

Adafruit_Protomatter.h + dependencies

passwords.h   // this contains the "secret" information required to get online with MQTT and local WiFi
              // written specifically for HiveMQ with SSL but should be easily ported for other brokers
defines.h     // contains the other #define statements

Extra files:
matrix-routines.ino         routines for displaying data on the LED display
connection-routines.ino     routines for connection to various online services
----------------------------------------------------------------------- */

#define DEBUG            // for most debug serial prints
//#define DEBUG_BUFFER     // serial.print circular buffer top/tail
// uncomment this line for a Matrix Portal S3 / comment if using M4
// #define MPS3             // MatrixPortal ESP32-S3
// uncomment this line for a Matrix Portal M4 / comment if using S3
#define MPM4            // MatrixPortal M4
// #define P64X32           // 2 x 64x32 panels
#define P64X64           // 1 x 64x64 panel

// this #define is required for M4
#ifdef MPM4
  #define USE_AIRLIFT
#endif

// GNU C-compiler macros for filenames and versions
#define PROGRAM_NAME      __BASE_FILE__
#define PROGRAM_DATE      __DATE__
#define PROGRAM_VERSION   "h_0.0.7" // now copes with 64x32 panels

// protomattier include
#include <Adafruit_Protomatter.h>


#ifdef MPM4
  #include <WiFiNINA.h>         // needed for M4 compilation
  #pragma message "Compiling for Matrix Portal M4";
#elif defined MPS3
  #include <WiFi.h>             // needed for S3 compilation
  #include <ESP_SSLClient.h>    // needed for S3 compilation
  #pragma message "Compiling for Matrix Portal S3";
#else
  #error No #define for board - stopping compilation
#endif

#include <ArduinoMqttClient.h>        // mqtt library
#include "passwords.h"                // store your passwords here
#include "defines.h"                  // other definitions

// these initialisations are different depending on the board
#ifdef MPM4
  WiFiSSLClient wifiClient;    // needed for M4 compilation
  // initialise mqtt client
  MqttClient mqttClient(wifiClient);
#elif defined MPS3
  ESP_SSLClient sslClient;    // needed for S3 compilation
  WiFiClient basicClient;
  // initialise mqtt client
  MqttClient mqttClient(sslClient);
#endif

// fonts for Protomatter/Adafruit GFX library
// commented fonts are not used
#include <Fonts/FreeSansBold18pt7b.h> // Large friendly font
//#include <Fonts/FreeSansBoldOblique18pt7b.h>
//#include <Fonts/FreeSansBoldOblique12pt7b.h>
#include <Fonts/FreeSansBoldOblique9pt7b.h>
#include <Fonts/FreeSerifBoldItalic18pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
//#include <Fonts/FreeSansBold12pt7b.h>

// specially designed for this application
#include <Fonts/FreeSansBoldNarrow18pt7b.h>   // big font for points score
//#include <Fonts/FreeSerifBoldItalic12pt7b.h>
//#include <Fonts/FreeSerifBoldItalic9pt7b.h>
//#include <Fonts/FreeSerifBoldItalic8pt7b.h>
//#include <Fonts/FreeMono9pt7b.h>
//#include <Fonts/FreeSerif9pt7b.h>
#include <Fonts/TomThumb.h>         // message font
//#include <Fonts/Picopixel.h>

// specially designed for this application
#include <Fonts/FreeMax9pt7b.h>     // 
//#include <Fonts/Font5x5Fixed.h>
//#include <Fonts/Font5x7Fixed.h>

// specially designed for this application
#include <Fonts/MaxSans9pt7b.h>     // better numbers for games score

/* ----------------------------------------------------------------------
The RGB matrix must be wired to VERY SPECIFIC pins, different for each
microcontroller board. This first section sets that up for a number of
supported boards.
------------------------------------------------------------------------- */
#ifdef MPM4       // MatrixPortal M4
  uint8_t rgbPins[]  = {7, 8, 9, 10, 11, 12};
  uint8_t addrPins[] = {17, 18, 19, 20, 21};
  uint8_t clockPin   = 14;
  uint8_t latchPin   = 15;
  uint8_t oePin      = 16;
#elif defined(MPS3)     // MatrixPortal ESP32-S3
  uint8_t rgbPins[]  = {42, 41, 40, 38, 39, 37};
  uint8_t addrPins[] = {45, 36, 48, 35, 21};
  uint8_t clockPin   = 2;
  uint8_t latchPin   = 47;
  uint8_t oePin      = 14;
#endif


/* ----------------------------------------------------------------------
This is the initialisation code for the Protomatter matrix
Depending on the size of the matrices, edit the numbers 
Don't forget that the E-line needs to be soldered across 
  on the Matrix Portal if a 64x64 matrix is used
Also don't forget that the pin on the LED matrix might be 
  8 or 16 - this is important for which pin to solder
------------------------------------------------------------------------- */

#ifdef P64X32
#pragma message "64x32";
Adafruit_Protomatter matrix(
  64,                          // Matrix width in pixels
  3,                           // Bit depth -- 6 here provides maximum color options / 3 may reduce artefacts with reduced colours available
  1, rgbPins,                  // # of matrix chains, array of 6 RGB pins for each
  4, addrPins,                 // # of address pins (height is inferred), array of pins *** this is for 64x32***
  //5, addrPins,               // # of address pins (height is inferred), array of pins ***this is for 64x64***
  clockPin, latchPin, oePin,   // Other matrix control pins
  false,                       // TRUE = DOUBLE-BUFFERING / FALSE = NO BUFFERING. Not used for this program
  2                            // 2 = 2 x 64x32 matrices one above the other, neither rotated
  );                           // -2 = 2 x 64x32 matrices one above the other, one rotated 180 degrees
#elif defined P64X64
#pragma message "64x64";
Adafruit_Protomatter matrix(
  64,                          // Matrix width in pixels
  3,                           // Bit depth -- 6 here provides maximum color options
  1, rgbPins,                  // # of matrix chains, array of 6 RGB pins for each
  //4, addrPins,               // # of address pins (height is inferred), array of pins *** this is for 64x32***
  5, addrPins,                 // # of address pins (height is inferred), array of pins ***this is for 64x64***
  clockPin, latchPin, oePin,   // Other matrix control pins
  false);                      // TRUE = DOUBLE-BUFFERING / FALSE = NO BUFFERING. Not used for this program
#endif
// Sundry globals used for animation ---------------------------------------

int16_t  textX;        // Current text position (X)
int16_t  textY;        // Current text position (Y)
int16_t  textMin;      // Text pos. (X) when scrolled off left edge

uint8_t  p1Score;
bool     p1NameChange;
bool     p1ScoreUpdate;

uint8_t  p2Score;
bool     p2NameChange;
bool     p2ScoreUpdate;

bool     mqttUpdate;

char     p1Name[10] = {"PLAY1"};   // red player
char     p2Name[10] = {"PLAY2"};   // blue player

uint8_t  p1Games;
uint8_t  p2Games;

int8_t    mqttErrorString;
uint8_t   currentMessage = 0;     // current message in circular buffer
uint8_t   receivedMessage = 0;    // messages waiting to be processed in buffer - hard-coded to 10

uint16_t   timerCheck;            // apparently, 60 seconds is the default - needs to be a uint16 rather than uint8

char      message[10][30];        // 10 messaages can be waiting - split into header and payload
char      messageHeader[10][15];  // 10 message headers 
char      messagePayload[10][15]; // 10 message payloads


void setup(void) 
{
  uint8_t serialCount;
  uint8_t loopCount;
  char    buff[200];              // store sprintf messages for serial output

  serialStart();                  // start the serial port
  wifiStart();                    // start the wifi

  delay(2000);                    // wait for 2 seconds
  mqttStart();                    // start the mqtt service

}

void loop(void)
{
  uint8_t count_up;
  uint8_t count_down;
  uint8_t loopCount;

  char player1[10];               // player 1 name
  char player2[10];               // player 2 name

  char buff[30];                  // message buffer

  bool lockMQTT = false;          //

  uint8_t tempScore1;             //
  uint8_t tempScore2;             //

  flashMessage();                 // flash an area of the screen

  startSubscription();            // start mqtt subscription

  delay(5000);                    // wait 5 seconds - arbitrary wait

  clearScreen();                  // clear the screen

  writeGamesTitle();              // show the games title
  writeGamesScore();              // show the games score
  writeP1Name(p1Name, LEFT);              // write the player names
  writeP2Name(p2Name, RIGHT);             // write the player names
  writeScoreDigit(p1Score, LEFT, RED);    // write score on the left in RED
  writeScoreDigit(p2Score, RIGHT, BLUE);  // write score on the right in BLUE

  // this section processes messages with a keepalive of slightly less than 60s
  // due to the nature of potentially successive messages, process even when not sending keepalive
  while(1)
  {
    if(timerCheck > 5950)         // given the default 60s timeout, allow some delay processing to ping before 60s is up
    {
      mqttClient.poll();          // refresh connection
      timerCheck = 0;             // reset timerCheck
    }
    else
    {
      timerCheck++;               // timerCheck controls MQTT ping - once a minute
    }

    if(mqttUpdate)                // if we have a message to process
    {
      splitMessage();             // message is spilt on ":" first, then"-" if necessary
      #ifdef DEBUG
        Serial.print("currentMessage: ");Serial.println(currentMessage);
        Serial.print("receivedMessage: ");Serial.println(receivedMessage);
        Serial.print(messageHeader[currentMessage]); Serial.print(":"); Serial.println(messagePayload[currentMessage]); 
      #endif

      if(strcmp(messageHeader[currentMessage], "p1Name") == 0)        // if message is about p1Name
      {
        strcpy(p1Name, messagePayload[currentMessage]);               // get the name
        clearNameLeft();                                              // clear the left display
        writeP1Name(messagePayload[currentMessage], LEFT);            // print new name on the left
        #ifdef DEBUG
          Serial.println("p1Name change");
        #endif
      }
      else
      {
        if(strcmp(messageHeader[currentMessage], "p2Name") == 0)      // if message is about p2Name
        {
          strcpy(p2Name, messagePayload[currentMessage]);             // get the name
          clearNameRight();                                           // clear the right display
          writeP2Name(messagePayload[currentMessage], RIGHT);         // print the new name on the right
          #ifdef DEBUG                    
            Serial.print("p2Name change");
          #endif
        }
        else
        {
          if(strcmp(messageHeader[currentMessage], "Games") == 0)     // if message is about games score
          {
            #ifdef DEBUG
              Serial.println("Games");
            #endif
            getGames();                                               // process the message
            refreshDisplay();                                         // refresh
          }
          else
          {
            if(strcmp(messageHeader[currentMessage], "Score") == 0)   // if the message is about score within the game
            {
              getScores();                                            // precess the message
              #ifdef DEBUG  
                Serial.println("Current game score");
              #endif
              clearScore();                                            // clear the old score
              writeScoreDigit(p1Score, LEFT, RED);                     // print p1Score
              writeScoreColon();                                      
              writeScoreDigit(p2Score, RIGHT, BLUE);                   // print p2Score
            }
            else
            {
              if(strcmp(messageHeader[currentMessage], "Win") == 0)
              {
                playerWin();
              }
              else
              {
                if(strcmp(messageHeader[currentMessage], "Match") == 0)
                {
                  playerMatch();
                }
                else
                {
                  if(strcmp(messageHeader[currentMessage], "Next") == 0)
                  {
                    if(strcmp(messagePayload[currentMessage], "Match") == 0)
                    {
                      nextMatch();  
                    }
                    else
                    {
                      if(strcmp(messagePayload[currentMessage], "Game") == 0)
                      {
                        nextGame();
                      }
                      else
                      {
                        Serial.println("Error in NextGame logic");
                      }
                    }
                  }
                  else
                  {
                    Serial.println("Error in NextGame/Match logic");
                  }
                }
              }
            }
          }
        }
      }

      if(currentMessage < 9)                            // currentMessage/receivedMessage act as head/tail for a circular buffer of 10 messages
        currentMessage++;
      else
        currentMessage = 0;

      if(receivedMessage == currentMessage)             // if no new messages, stop checking
        mqttUpdate = false;
      
      #ifdef DEBUG
        for(count_up=0;count_up<10;count_up++)            // print the message queue - neatly
        {
          sprintf(buff, "Mess%d: %s_%s\n", count_up, messageHeader[count_up], messagePayload[count_up]);
          Serial.print(buff);
        }
      #endif
    }
    #ifdef DEBUG_BUFFER
      Serial.println(currentMessage);
      Serial.println(receivedMessage);
    #endif
  }
  delay(10);
}

/*
* onMqttMessage is the callback for receiving an MQTT message
* int p_messaageSize is the length of the message
* 
*/

void onMqttMessage(int p_messageSize)
{
  uint8_t parseMessage;           // count charactersin message

  // got a message of size p_messageSize
  #ifdef DEBUG
    Serial.print("Received a message with topic: '");
    Serial.print(mqttClient.messageTopic());
    Serial.print("', Length: ");Serial.print(p_messageSize);Serial.println(" bytes:");
  #endif

  // apparently, we have to get the message char by char
  // use the Stream interface to print the contents

  parseMessage = 0;
  while(mqttClient.available())
  {
    message[receivedMessage][parseMessage] = mqttClient.read();
    #ifdef DEBUG
      Serial.print(message[receivedMessage][parseMessage]);
    #endif
    parseMessage++;
  }
  message[receivedMessage][parseMessage] = '\0';     // terminate string
  #ifdef DEBUG
    Serial.print("\n\n");
  #endif

  if(receivedMessage < 9)                 // circular buffer management
    receivedMessage++;                    // ...
  else                                    // ...
    receivedMessage = 0;                  // ...

  mqttUpdate = true;                      // show there is a message
}

/* void splitMessage(void)
* Splits the message based on ':'
* stores the split message in two global char arrays:
* messageHeader[]
* messagePayload[]
*
*/
void splitMessage(void)
{
  char localMessage[30];        // get a copy of the message - strtok destroys the char array
  char * localMessageHeader;    // used to hold the message header
  char * localMessagePayload;   // used t hold the message payload

  strcpy(localMessage, message[currentMessage]);              // get a copy
  localMessageHeader = strtok(localMessage, ":");             // split on ":"
  strcpy(messageHeader[currentMessage], localMessageHeader);  // copy to header queue
  localMessagePayload = strtok(NULL, ":");                    // parse the rest of the message on "-"
  strcpy(messagePayload[currentMessage], localMessagePayload);// copy to payload queue
  
  #ifdef DEBUG
    Serial.print("message[currentMessage]: ");Serial.println(message[currentMessage]);
    Serial.print("localMessage (splitMessage): "); Serial.println(localMessage);
    Serial.print("Header: "); Serial.println(messageHeader[currentMessage]);
    Serial.print("Payload: "); Serial.println(messagePayload[currentMessage]);
  #endif
}


/* void getScores(void)
* Splits the message based on '-'
* stores the original message in localP1Score[] and localP2Score[]
* converts the scores into global uint8_t p1Score and p2Score
*
*/
void getScores(void)
{
  char * localP1Score;      // local char array to store P1 score as chars
  char * localP2Score;      // local char array to store P2 score as chars
  char localScore[15];      // copy of message to prevent strtok destroying the message

  strcpy(localScore, messagePayload[currentMessage]);   // create the copy
  localP1Score = strtok(localScore, "-");               // get first score in chars
  localP2Score = strtok(NULL, "-");                     // get second score in chars

  p1Score = atoi(localP1Score);                         // convert to global uint8_t
  p2Score = atoi(localP2Score);                         // convert to global uint8_t

  #ifdef DEBUG                                               // if DEBUG set
    Serial.print("Score P1: "); Serial.println(p1Score);  // print the score
    Serial.print("Score P2: "); Serial.println(p2Score);  // print the score
  #endif
}

/* void getGames(void)
* Splits the message based on '-'
* stores the original message in localScore[]
* converts the scores into global uint8_t p1Score and p2Score
*
*/

void getGames(void)
{
  char * localP1Games;                // char array of P1Games
  char * localP2Games;                // char array of P2Games
  char localGames[15];                // copy of original message to prevent destruction

  strcpy(localGames, messagePayload[currentMessage]);   // create the copy
  localP1Games = strtok(localGames, "-");               // get first games in chars
  localP2Games = strtok(NULL, "-");                     // get second games in chars

  p1Games = atoi(localP1Games);                         // convert to global uint8_t
  p2Games = atoi(localP2Games);                         // convert to global uint8_t

  #ifdef DEBUG                                             // if DEBUG set
    Serial.print("Score P1: "); Serial.println(p1Games);// print the games
    Serial.print("Score P2: "); Serial.println(p2Games);// print the games
  #endif
}

