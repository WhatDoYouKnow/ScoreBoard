# ScoreBoard
Specifically a squash Scoreboard using Adafruit Matrix Portal boards to drive LED panel(s), usually 64x32 or 64x64. Smaller matrices are available.

Code is written in Arduino IDE (currently 2.3.2) - see 
  /project_files folder for Arduino files
  /project_files/Fonts for font files. The current project uses two font files which are better suited to displaying the score information. 
  
To use this project directly, copy the contents of project_files to a folder in your Arduino folder with the same name as the main file. Copy the two font files to the Adafruit_GFX/Fonts folder otherwise the project will not compile.
Ensure the #defines are correct for the type of board you are using - either MPM4 or MPS3 - as listed in the code



The HTML code is definitely a work in progress but will publish to the HiveMQ broker on a topic which is then issued to the subscribing Matrix Portal. Again, this is specific to squash scoring.
There is the ability to increment/decrement the score for a player and the page confirms the game/match win. Based on PAR to 15.
