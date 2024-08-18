/* ----------------------------------------------------------------------

Routines to update the score on the matrix display
----------------------------------------------------------------------- */

void setMessageFont(void)
{
  matrix.setFont(&TomThumb);
  matrix.setCursor(0, 9);
  matrix.show();
}

void setTextColour(uint16_t p_textColour)
{
  matrix.setTextColor(p_textColour);
  matrix.show();
}

void write_big_title(void)
{
  clearTitleArea();                                   // clear the area of the screen
  setTextColour(PURPLE);                                    // purple
  matrix.setFont(&FreeSansBoldOblique9pt7b);          // oblique font
  matrix.setCursor(-2,12);                            // best position for title
  matrix.write("Electric");                           // print title
  matrix.setCursor(36,26);                            // best position for title
  matrix.write("Tin");                                // print title
  matrix.show();                                      // update the matrix
}

void write_small_title(void)
{
  clearTitleArea();                                   // clear the area of the screen
  setTextColour(PURPLE);                                    // purple
  //matrix.setFont(&FreeMono9pt7b);                     // small font
  matrix.setFont(&FreeSans9pt7b);                     // small font
  matrix.setCursor(-2,12);                            // best position for title
  matrix.write("Electric");                           // print title
  matrix.setCursor(36,26);                            // best position for title
  matrix.write("Tin");                                // print title
  matrix.show();                                      // update the matrix

}

void clearScreen(void)
{
  matrix.fillScreen(0);                               // clear the entire screen
  matrix.setCursor(0, 0);
  matrix.show();
}

void clearUnderTitle(void)
{
  matrix.fillRect(0, UNDERTITLE, 64, 32, BLACK);
  matrix.show();
}

void clearTitleArea(void)
{
  matrix.fillRect(0, 0, 64, 27, BLACK);
  matrix.show();
}

void clearGamesArea(void)
{
  matrix.fillRect(0, 13, 64, 27, BLACK);
  matrix.show();
}

void clearGamesScore(void)
{
  matrix.fillRect(0, GAMESSCORELINE-13, 64, 14, BLACK);
  matrix.show();
}

void writeGamesTitle(void)
{
  uint8_t length;
  uint8_t xpos;

  clearTitleArea();
  matrix.setFont(&FreeSans9pt7b);
  matrix.setTextColor(GREEN);
  length = strlen("Games");
  xpos = (64-(length*11))/2;

  matrix.setCursor(xpos, GAMESLINE);
  matrix.print("Games");  
}

void writeGamesScore(void)
{
  clearGamesScore();
  matrix.setFont(&FreeSans9pt7b);
  matrix.setTextColor(GREEN);

  matrix.setCursor(16, GAMESSCORELINE);
  matrix.print(p1Games);

  matrix.print(" : ");

  matrix.print(p2Games);
  matrix.show();
}

void clearNameLeft(void)
{
  matrix.fillRect(0,27,32,11,BLACK);                   // clear the left half of the name area of the screen
  matrix.show();                                      // top left corner / width and height 
}

void clearNameRight(void)
{
  matrix.fillRect(32,27,32,11,BLACK);                   // clear the right half of the name area of the screen
  matrix.show();
}

void clearScore(void)
{
  matrix.fillRect(0, 38, 64, 16, BLACK);                    // clear the entire score area
  matrix.show();
}

void clearScoreLeft(void)
{
  matrix.fillRect(0, 38, 32, 26, BLACK);                  // clear the lefthand score
  matrix.show();
}

void clearScoreRight(void)
{
  matrix.fillRect(32, 38, 32, 26, BLACK);                  // clear the righthand score
  matrix.show();
}

void writeP1Name(char *p_name1, bool p_left)
{
  uint8_t length;
  uint8_t xpos;

  Serial.print("Func: writeP1Name. P1Name: "); Serial.println(p_name1);
  matrix.setFont(&FreeMax9pt7b);                     // set name font
  matrix.setTextColor(RED);                       // red
  length = strlen(p_name1);
  xpos = (32-(length*5))/2;
  
  if(p_left)
    matrix.setCursor(xpos,NAMELINE);                          // set name position left
  else
    matrix.setCursor(32+xpos,NAMELINE);                          // set name position right

  matrix.print(p_name1);
  matrix.show();
}

void writeP2Name(char *p_name2, bool p_left)
{
  uint8_t length;
  uint8_t xpos;

  Serial.print("Func: writeP2Name. P2Name: "); Serial.println(p_name2);
  matrix.setFont(&FreeMax9pt7b);                     // set name font
  matrix.setTextColor(BLUE);     // blue
  length = strlen(p_name2);
  xpos = (32-(length*5))/2;

  if(p_left)
  {
    clearNameLeft();
    matrix.setCursor(xpos,NAMELINE);                          // set name position left
  }
  else
  { 
    clearNameRight();
    matrix.setCursor(32+xpos,NAMELINE);                          // set name position right
  }
  matrix.print(p_name2);
  matrix.show();
}

void writeScoreDigit(uint8_t p_score, bool p_left, uint16_t p_colour)
{
  matrix.setTextColor(p_colour);
  matrix.setFont(&FreeSansBoldNarrow18pt7b);

  if(p_left)            // it's the left score
  {
    clearScoreLeft();
    if(p_score < 10)
    {
      matrix.setCursor(DIGIT2, SCORELINE);
      matrix.print(p_score, DEC);
    }
    else
    {
      matrix.setCursor(DIGIT1, SCORELINE);
      matrix.print(p_score/10, DEC);
      matrix.print(p_score % 10, DEC);
    }
  }
  else                  // it's the right score
  {
    clearScoreRight();
    if(p_score < 10)
    {
      matrix.setCursor(DIGIT4, SCORELINE);
      matrix.print(p_score, DEC);
    }
    else
    {
      matrix.setCursor(DIGIT3, SCORELINE);
      matrix.print(p_score/10, DEC);
      matrix.print(p_score % 10, DEC);
    }
  }
  matrix.show();
}

void writeScoreColon(void)
{
  // matrix.setTextColor(matrix.color565(29,0,0));
  // matrix.setCursor(COLON, SCORELINE);
  // matrix.print(":");
}

void flashMessage(void)
{
  uint8_t loopCount;

  clearUnderTitle();

  loopCount = 0;
  while(loopCount < 6)
  {
    Serial.println(loopCount);
    if((loopCount % 2) == 1)
    {
      matrix.fillRect(0, MESSAGELINE-6, strlen(ONLINEMESSAGE)*4, 6, BLACK);
    }
    else
    {
      setTextColour(WHITE);
      setMessageFont();
      matrix.setCursor(0, MESSAGELINE);
      matrix.print(ONLINEMESSAGE);
    }
    matrix.show();
    delay(750);
    loopCount++;
  }
}

void playerWin(void)
{
  char localMessage[30];

  clearScreen();
  matrix.setCursor(0, 12);
  matrix.setFont(&FreeSans9pt7b);
  if(strcmp(messagePayload[currentMessage], "p1") == 0)
  {
    setTextColour(RED);
    sprintf(localMessage, "%s WINS\n%d:%d", p1Name, p1Score, p2Score);
    matrix.print(localMessage);
    Serial.print(localMessage);
  }
  else
  {
    setTextColour(BLUE);
    sprintf(localMessage, "%s WINS\n%d:%d", p2Name, p2Score, p1Score);
    matrix.print(localMessage);
    Serial.print(localMessage);
  }
  matrix.show();
}

void playerMatch(void)
{
  char localMessage[30];

  clearScreen();
  matrix.setCursor(0, 12);
  matrix.setFont(&FreeSans9pt7b);
  if(strcmp(messagePayload[currentMessage], "p1") == 0)
  {
    setTextColour(RED);
    matrix.print(p1Name);
    matrix.setCursor(8, 26);
    matrix.print("WINS");
    matrix.setCursor(1, 40);
    matrix.print("MATCH");
    matrix.setCursor(20, 54);
    sprintf(localMessage, "%d:%d", p1Games, p2Games);
    matrix.print(localMessage);
    sprintf(localMessage, "%s WINS MATCH %d:%d", p1Name, p1Games, p2Games);
    Serial.print(localMessage);
  }
  else
  {
    setTextColour(BLUE);
    matrix.print(p2Name);
    matrix.setCursor(8, 26);
    matrix.print("WINS");
    matrix.setCursor(1, 40);
    matrix.print("MATCH");
    matrix.setCursor(20, 54);
    sprintf(localMessage, "%d:%d", p2Games, p1Games);
    matrix.print(localMessage);
    sprintf(localMessage, "%s WINS MATCH %d:%d", p2Name, p2Games, p1Games);
    Serial.print(localMessage);
  }
  matrix.show();

}

void nextMatch(void)
{
  Serial.println("Next match received***");

  // strcpy(p1Name, "PLAY1");
  // strcpy(p2Name, "PLAY2");
  // p1Games = 0;
  // p2Games = 0;
  // p1Score = 0;
  // p2Score = 0;
  refreshDisplay();
}

void nextGame(void)
{
  Serial.println("Next game received");
  p1Score = 0;
  p2Score = 0;
  refreshDisplay();
}

void refreshDisplay(void)
{
  Serial.println("Refreshing display");
  clearScreen();
  writeGamesTitle();
  writeGamesScore();
  writeP1Name(p1Name, LEFT);
  writeP2Name(p2Name, RIGHT);
  writeScoreDigit(p1Score, LEFT, RED);
  writeScoreDigit(p2Score, RIGHT, BLUE);
  matrix.show();
}
