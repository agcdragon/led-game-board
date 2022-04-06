  //LEDs
  #include <FastLED.h>
  #include <string.h>
  #define NUM_LEDS    72
  #define LED_PIN     13
  long unsigned int prevTime = 0;
  int resetDelay = 250;
  CRGB leds[NUM_LEDS];
  
  //Buttons
  const char UP_PIN = 12;
  const char DOWN_PIN = 11;
  const char LEFT_PIN = 10;
  const char RIGHT_PIN = 9;
  const char PLACE_PIN = 8;
  const char RESET_PIN = 7;
  
  // Directions
  #define DIR_UP    0
  #define DIR_DOWN  1
  #define DIR_LEFT  2
  #define DIR_RIGHT 3
  
  // Positions
  int x_pos = 0;
  int y_pos = 0;
  
  // Gameboard
  const char PIECE_X = 'x';
  const char PIECE_O = 'o';
  const char PIECE_EMPTY = '-';
  int game[3][3];
  bool winner;
  char who;
  

  int charToNum(char piece) {
      if (piece == PIECE_X){
          return -1;
      }
      return 1; //if piece == PIECE_O, return 
  }

  char numToChar(int piece) {
      if (piece == -1){
          return PIECE_X;
      }
      return PIECE_O; //if piece == 1
  }
  
  // --#--#--
  // --#--#--
  // ########
  // --#--#--
  // --#--#--
  // ########
  // --#--#--
  // --#--#--
  
  // LED grid mappings 
  int board[3][3][4] = { {{14, 15, 16, 17}, {32, 33, 46, 47}, {62, 63, 64, 65}},
                  {{11, 12, 19, 20}, {35, 36, 43, 44}, {59, 60, 67, 68}}, 
                  {{8, 9, 22, 23}, {38, 39, 40, 41}, {56, 57, 70, 71}} };
  
  CRGB oColor = CRGB(255, 0, 0); // red
  CRGB xColor = CRGB(0, 0, 255); // blue
  CRGB wColor = CRGB(255, 255, 255); // white
  CRGB eColor = CRGB(0, 0, 0); // clear/empty
  CRGB gColor = CRGB(0, 255, 0); //green
  
  void setup() {
      Serial.begin(9600);
      Serial.write(4500);
      // reset positions
      x_pos = 0;
      y_pos = 0;
      winner = false;
      who = PIECE_X;
  
      // setup buttons
      pinMode(UP_PIN, INPUT_PULLUP);
      pinMode(DOWN_PIN, INPUT_PULLUP);
      pinMode(LEFT_PIN, INPUT_PULLUP);
      pinMode(RIGHT_PIN, INPUT_PULLUP);
      pinMode(PLACE_PIN, INPUT_PULLUP);
      pinMode(RESET_PIN, INPUT_PULLUP);
  
      // setup ws2812b leds
      FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
      
      // setup grid lines 
      int grid[28] = {10, 13, 18, 21, 24, 25, 26, 27, 28, 29, 30, 31, 34, 37, 42, 45, 48, 49, 50, 51, 52, 53, 54, 55, 58, 61, 66, 69};
      for (int i = 0; i < 28; i++) {
          leds[grid[i]] = wColor;
          FastLED.show();
      }
  
      // reset led board to be empty
      for (int i = 0; i < 3; i++) {
          for (int j = 0; j < 3; j++) {
              for (int k = 0; k < 4; k++) {
                  leds[board[i][j][k]] = eColor;
              }
          }
      }
  
      // reset game to be empty
      for (int i = 0; i < 3; i++) {
          for (int j = 0; j < 3; j++) {
              game[i][j] = PIECE_EMPTY;
          }
      }
  }
  
  char opposite(char piece) {
  // Return opposite of PIECE
  
      if (piece == PIECE_X) {
          return PIECE_O;
      }
      return PIECE_X;
  }
  
  void place() {
  // Place WHO at X_POS, Y_POS
      Serial.print(x_pos);
      Serial.print(y_pos);
      if (game[x_pos][y_pos] != PIECE_EMPTY) {
          return;
      }
      game[x_pos][y_pos] = who;
      for (int i = 0; i < 4; i++) {
          if (who == PIECE_X) {
              leds[board[x_pos][y_pos][i]] = xColor;
          } else {
              leds[board[x_pos][y_pos][i]] = oColor;
          }
      }
      FastLED.show();
  
      if (checkForWin(game, who)) {
          winner = true;
      } else {
          who = opposite(who);
      }

      return;
  }
  
  bool checkLine(int line[], char player) {
  // Check if line is completed by one player
      for(int i = 0; i < 3; i++) {
          if (line[i] != player) {
              return false;
          }
      }
      return true;
  }
  
  // might need to debug row, cols
  // might need to debug row, cols
  bool checkForWin(int temp_board[3][3], char player) {
  // Check if there is a win for player.
  
      // check row
      for (int i = 0; i < 3; i++) {
          if (checkLine(temp_board[i], player)) {
              return true;
          }
      }
      // check column
      for (int i = 0; i < 3; i++) {
          int col[3];
          for(int j = 0; j < 3; j++) {
              col[j] = temp_board[j][i];
          }
          if (checkLine(col, player)) {
              return true;
          }
      }
  
      // check diagonal
      int dia_lr[3] = {temp_board[0][0], temp_board[1][1], temp_board[2][2]};
      int dia_rl[3] = {temp_board[2][0], temp_board[1][1], temp_board[0][2]};
      if (checkLine(dia_lr, player)) {
          return true;
      }
      if (checkLine(dia_rl, player)) {
          return true;
      }
      return false;
  }


  void bluecursor() { //person playing 1st
      bool placed = false;
      Serial.write("bluecursor");
      while (!placed) {
        blueblink();
        if (digitalRead(UP_PIN) == LOW) {
            Serial.write("up");
            long unsigned int currTime = millis();
            if (currTime - prevTime > 10) {
                x_pos = min(x_pos+1, 2);
            }
            prevTime = currTime;
        }
        else if (digitalRead(DOWN_PIN) == LOW) {
            Serial.write("down");
            long unsigned int currTime = millis();
            if (currTime - prevTime > 10) {
                x_pos = max(x_pos-1, 0);
            }
            prevTime = currTime;
        }
        else if (digitalRead(LEFT_PIN) == LOW) {
            Serial.write("left");
            long unsigned int currTime = millis();
            if (currTime - prevTime > 10) {
                y_pos = max(y_pos-1, 0);
            }
            prevTime = currTime;
        }
        else if (digitalRead(RIGHT_PIN) == LOW) {
            Serial.write("right");
            long unsigned int currTime = millis();
            if (currTime - prevTime > 10) {
                y_pos = min(y_pos+1, 2);
            }
            prevTime = currTime;
        }
        else if (digitalRead(PLACE_PIN) == LOW) {
            
            long unsigned int currTime = millis();
            if (currTime - prevTime > 10) {
                placed = true;
                Serial.write("place");
                place();
            }
            prevTime = currTime;
        }
    }
    return;
  }



  void redcursor() { //ai playing 2nd
    
    int best_i = -1;
    int best_j = -1;
    int score = -2;
    int gamecopy[3][3];
    
    for(int i = 0; i < 3; i++) {//For all moves,
        for(int j = 0; j < 3; j++) {//For all moves,
            if(game[i][j] == PIECE_EMPTY) {
                for (int k = 0; k < 3; k++) {
                  for (int l = 0; l < 3; l++) {
                    gamecopy[k][l] = game[k][l];
                  }
                }
                gamecopy[i][j] = PIECE_O;
                int tempScore = -minimax(gamecopy, PIECE_X);
                if(tempScore > score) {
                    score = tempScore;
                    best_i = i;
                    best_j = j;
                }
            }
        }
    }
    //returns a score based on minimax tree at a given node.
    x_pos = best_i;
    y_pos = best_j;
    Serial.write("x");
    Serial.println(x_pos);
    Serial.write("y");
    Serial.println(y_pos);
    place();

    return;
  }

  //minimax function
  int minimax(int tempboard[3][3], char player) {
    //How is the position like for player (their turn) on board?
    bool winner = checkForWin(tempboard, player);
    if(winner) { //if someone won
        if (player == PIECE_X){ //if person won
            return -1;
        } else { //if player is piece_o (if ai won)
            return 1;
        }
    }

    int move = -1;
    int score = -2;//Losing moves are preferred to no move
    for(int i = 0; i < 3; i++) {//For all moves,
        for(int j = 0; j < 3; j++) {//For all moves,
            if(tempboard[i][j] == PIECE_EMPTY) {//If legal,
                tempboard[i][j] = player;//Try the move
                int thisScore = -minimax(tempboard, opposite(player));
                if(thisScore > score) {
                    score = thisScore;
                    move = i;
                }//Pick the one thats worst for the opponent
                tempboard[i][j] = PIECE_EMPTY;//Reset board after try
            }
        }
    }
    if(move == -1) return 0;
    return score;
  } 


  void redblink() {
      if (game[x_pos][y_pos] == PIECE_EMPTY) {
        for (int i = 0; i < 4; i++) {
            leds[board[x_pos][y_pos][i]] = gColor;
        }
        FastLED.show();
        delay(500);
        for (int i = 0; i < 4; i++) {
            leds[board[x_pos][y_pos][i]] = eColor;
        }
        FastLED.show();
        delay(100);
      }
      return;
  }

  

  void blueblink() {
      if (game[x_pos][y_pos] == PIECE_EMPTY) {
        for (int i = 0; i < 4; i++) {
            leds[board[x_pos][y_pos][i]] = gColor;
        }
        FastLED.show();
        delay(500);
        for (int i = 0; i < 4; i++) {
            leds[board[x_pos][y_pos][i]] = eColor;
        }
        FastLED.show();
        delay(100);
      }
      return;
  }

  void winnerblink() {
    //modify to blink only winning squares later
    if (winner) {
      for (int i = 8; i < 72; i++) {
        if (who == PIECE_X) {
          leds[i] = xColor;
        }
        else {
          leds[i] = oColor;
        }
      }
      FastLED.show();
    }
    return;
  }
  
  void loop() {
      if (!winner) {
        Serial.write("blue");
        bluecursor();
        if (winner) {
            delay(2000);
            winnerblink();
            delay(3000);
            reset();
            bluecursor();
        }
        Serial.write("red");
        redcursor();
        //place();
        if (winner) {
            delay(2000);
            winnerblink();
            delay(3000);
            reset();
        }
      }   
  }

  void reset() {
    x_pos = 0;
      y_pos = 0;
      winner = false;
      who = PIECE_X;
      
      // setup grid lines 
      int grid[28] = {10, 13, 18, 21, 24, 25, 26, 27, 28, 29, 30, 31, 34, 37, 42, 45, 48, 49, 50, 51, 52, 53, 54, 55, 58, 61, 66, 69};

      for (int i = 0; i < 28; i++) {
          leds[grid[i]] = wColor;
          FastLED.show();
      }
  
      // reset led board to be empty
      for (int i = 0; i < 3; i++) {
          for (int j = 0; j < 3; j++) {
              for (int k = 0; k < 4; k++) {
                  leds[board[i][j][k]] = eColor;
              }
          }
      }
  
      // reset game to be empty
      for (int i = 0; i < 3; i++) {
          for (int j = 0; j < 3; j++) {
              game[i][j] = PIECE_EMPTY;
          }
      }
  }
