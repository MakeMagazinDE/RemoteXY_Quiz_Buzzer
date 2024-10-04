/*
   -- ESP-Quiz --
   
   This source code of graphical user interface 
   has been generated automatically by RemoteXY editor.
   To compile this code using RemoteXY library 3.1.10 or later version 
   download by link http://remotexy.com/en/library/
   To connect using RemoteXY mobile app by link http://remotexy.com/en/download/                   
     - for ANDROID 4.13.11 or later version;
     - for iOS 1.10.3 or later version;
    
   This source code is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.    
*/

//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// you can enable debug logging to Serial at 115200
//#define REMOTEXY__DEBUGLOG    

// RemoteXY select connection mode and include library 
#define REMOTEXY_MODE__ESP32CORE_BLE

#include <BLEDevice.h>

// RemoteXY connection settings 
#define REMOTEXY_BLUETOOTH_NAME "TheQuiz"

// GPIO Input settings
#define BUTTON_PIN_PLAYER1 D1
#define BUTTON_PIN_PLAYER2 D2
#define BUTTON_PIN_PLAYER3 D3

#include <RemoteXY.h>

// RemoteXY GUI configuration  
#pragma pack(push, 1)  
uint8_t RemoteXY_CONF[] =   // 258 bytes
  { 255,5,0,15,0,251,0,17,0,0,0,156,1,106,200,2,1,0,19,0,
  130,56,8,47,16,0,4,130,5,8,30,16,0,4,130,4,160,99,31,0,
  4,1,8,164,24,24,7,1,0,78,79,75,0,1,76,164,24,24,7,135,
  0,79,75,0,1,6,9,26,14,7,4,27,80,108,97,121,0,70,5,5,
  98,2,19,26,121,5,130,4,37,99,113,0,4,69,74,7,20,20,1,66,
  6,60,94,6,128,2,26,129,7,47,53,12,27,83,112,105,101,108,101,114,
  32,49,58,32,0,129,7,85,53,12,27,83,112,105,101,108,101,114,32,50,
  58,32,0,129,7,124,53,12,27,83,112,105,101,108,101,114,32,51,58,32,
  0,65,81,40,18,18,112,65,81,78,18,18,112,65,82,117,18,18,112,66,
  6,98,94,6,128,2,26,66,7,137,94,6,128,2,26,131,57,9,45,14,
  7,4,2,27,67,111,110,102,105,103,0,6,3,0,131,59,11,40,14,3,
  4,2,27,66,97,99,107,0,9,7,62,46,40,10,52,2,26,2,129,3,
  47,56,8,17,78,117,109,32,81,117,101,115,116,105,111,110,115,0 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  uint8_t button_WrongAnswer; // =1 if button pressed, else =0
  uint8_t button_CorrectAnswer; // =1 if button pressed, else =0
  uint8_t button_newGame; // =1 if button pressed, else =0
  int16_t edit_NoQuestions; // -32768 .. +32767

    // output variables
  uint8_t led_Running; // led state, =0 turn off
  int16_t sound_01; // =0 no sound, else ID of sound, =1001 for example, look sound list in app
  int8_t level_Player1; // from 0 to 100
  uint8_t led_01_r; // =0..255 LED Red brightness
  uint8_t led_01_g; // =0..255 LED Green brightness
  uint8_t led_01_b; // =0..255 LED Green brightness
  uint8_t led_02_r; // =0..255 LED Red brightness
  uint8_t led_02_g; // =0..255 LED Green brightness
  uint8_t led_02_b; // =0..255 LED Green brightness
  uint8_t led_03_r; // =0..255 LED Red brightness
  uint8_t led_03_g; // =0..255 LED Green brightness
  uint8_t led_03_b; // =0..255 LED Green brightness
  int8_t level_Player2; // from 0 to 100
  int8_t level_Player3; // from 0 to 100

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;   
#pragma pack(pop)
 

int playerScores[3] = {0, 0, 0};
bool gameRunning = false;
int targetScore = 5; // Default number of correct answers needed to win

 
/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////



void setup() 
{
  RemoteXY_Init (); 
  pinMode(BUTTON_PIN_PLAYER1, INPUT_PULLUP);
  pinMode(BUTTON_PIN_PLAYER2, INPUT_PULLUP);
  pinMode(BUTTON_PIN_PLAYER3, INPUT_PULLUP);
  LEDGameRunning(gameRunning);
  
    
}

void loop() 
{ 
  RemoteXY_Handler ();
  if (RemoteXY.edit_NoQuestions > 0) {
      targetScore = RemoteXY.edit_NoQuestions; // player with most correct answers out of given number wins
  }

  LEDGameRunning(gameRunning);
    // Check if new game button is pressed
  if (RemoteXY.button_newGame == 1) {
    resetGame();
    gameRunning = true;
  }


  if (gameRunning) {
    // Check if any player has pressed the buzzer
    if (digitalRead(BUTTON_PIN_PLAYER1) == LOW) {
      handleBuzzerPress(0);
    } else if (digitalRead(BUTTON_PIN_PLAYER2) == LOW) {
      handleBuzzerPress(1);
    } else if (digitalRead(BUTTON_PIN_PLAYER3) == LOW) {
      handleBuzzerPress(2);
    }
  }
  
}

void resetGame() {
  for (int i = 0; i < 3; i++) {
    playerScores[i] = 0;
  RemoteXY.led_01_r = 0;
  RemoteXY.led_02_r = 0;
  RemoteXY.led_03_r = 0;
  RemoteXY.led_01_g = 0;
  RemoteXY.led_02_g = 0;
  RemoteXY.led_03_g = 0;
  LEDGameRunning(gameRunning);
  RemoteXY.level_Player1 = 0;
  RemoteXY.level_Player2 = 0;
  RemoteXY.level_Player3 = 0;
  }
}

void handleBuzzerPress(int playerIndex) {
  // Update RemoteXY LED colors
  LEDGameRunning(false);
  RemoteXY.led_01_r = 255;
  RemoteXY.led_02_r = 255;
  RemoteXY.led_03_r = 255;
  RemoteXY.led_01_g = 0;
  RemoteXY.led_02_g = 0;
  RemoteXY.led_03_g = 0;
  
  switch (playerIndex) {
    case 0:
      RemoteXY.led_01_g = 255;
      RemoteXY.led_01_r = 0;
      break;
    case 1:
      RemoteXY.led_02_g = 255;
      RemoteXY.led_02_r = 0;
      break;
    case 2:
      RemoteXY.led_03_g = 255;
      RemoteXY.led_03_r = 0;
      break;
  }
  RemoteXY.sound_01 = 2008;
  RemoteXY_delay(1000);
  RemoteXY.sound_01 = 0;
  // Wait for the player to answer
  while (RemoteXY.button_WrongAnswer == 0 && RemoteXY.button_CorrectAnswer == 0) {
    RemoteXY_delay(50);
    // Do nothing, wait for player to answer
  }
  // Switch off LEDs
  RemoteXY.led_01_r = 0;
  RemoteXY.led_02_r = 0;
  RemoteXY.led_03_r = 0;
  RemoteXY.led_01_g = 0;
  RemoteXY.led_02_g = 0;
  RemoteXY.led_03_g = 0;

  // Handle correct or wrong answer
  if (RemoteXY.button_CorrectAnswer == 1) {
    playerScores[playerIndex]++;
    RemoteXY.sound_01 = 1031;
    RemoteXY_delay(1100);
    RemoteXY.sound_01 = 0;
  } else if (RemoteXY.button_WrongAnswer == 1) {
    RemoteXY.sound_01 = 1035;
    RemoteXY_delay(1100);
    RemoteXY.sound_01 = 0;
    playerScores[playerIndex]--;
  }

  // Update progress bar after modifying the scores
  updateProgress(playerIndex);

  // Reset button states
  //RemoteXY.button_CorrectAnswer = 0;
  //RemoteXY.button_WrongAnswer = 0;
  RemoteXY_delay(100);
  // Update scores and check if someone has won
  updateScoresAndCheckWinner();
}

void updateProgress(int playerIndex) {
  int scaledScore = max(0, min(100, playerScores[playerIndex] * 100 / targetScore)); // Scale score to progress bar

  switch (playerIndex) {
    case 0:
      RemoteXY.level_Player1 = scaledScore;
      break;
    case 1:
      RemoteXY.level_Player2 = scaledScore;
      break;
    case 2:
      RemoteXY.level_Player3 = scaledScore;
      break;
  }
}

void updateScoresAndCheckWinner() {
  // Determine win conditions based on RemoteXY settings
 
  for (int i = 0; i < 3; i++) {
    if (playerScores[i] >= targetScore) {
      // Player i has won
      gameRunning = false;
      RemoteXY.sound_01 = 3009; // Sound for winning
      RemoteXY_delay(2000);
      RemoteXY.sound_01 = 0;
      LEDGameRunning(false);
      break;
    }
  }
}


void LEDGameRunning(bool gamerunning){
  if (gamerunning == true){
    RemoteXY.led_Running = true;
  } else {
    RemoteXY.led_Running = false;
  }
}