    //LEDs
    #include <FastLED.h>
    #define NUM_LEDS    144 
    #define LED_PIN     13
    long unsigned int prevTime = 0;
    CRGB leds[NUM_LEDS];

    //Buttons
    const char UP_PIN = 12;
    const char DOWN_PIN = 11;
    const char LEFT_PIN = 10;
    const char RIGHT_PIN = 9;
    const char PLACE_PIN = 8;

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


    // --#--#--
    // --#--#--
    // ########
    // --#--#--
    // --#--#--
    // ########
    // --#--#--
    // --#--#--

    // LED grid mappings 
    int board[3][3][4] = { {{8, 9, 22, 23}, {38, 39, 40, 41}, {56, 57, 70, 71}},
                    {{11, 12, 19, 20}, {35, 36, 43, 44}, {59, 60, 67, 68}}, 
                    {{14, 15, 16, 17}, {32, 33, 46, 47}, {62, 63, 64, 64}} };

    CRGB oColor = CRGB(255, 0, 0); // red
    CRGB xColor = CRGB(0, 0, 255); // blue
    CRGB wColor = CRGB(255, 255, 255); // white
    CRGB eColor = CRGB(0, 0, 0); // clear/empty
    CRGB gColor = CRGB(0, 255, 0); //green

    void setup() {
        // reset positions
        x_pos = 0;
        y_pos = 0;

        // setup buttons
        pinMode(UP_PIN, INPUT_PULLUP);
        pinMode(DOWN_PIN, INPUT_PULLUP);
        pinMode(LEFT_PIN, INPUT_PULLUP);
        pinMode(RIGHT_PIN, INPUT_PULLUP);
        pinMode(PLACE_PIN, INPUT_PULLUP);

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
        winner = false;
        who = PIECE_X;
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
        if (winner) {
            return;
        }
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

        if (checkForWin(who)) {
            winner = true;
        } else {
            who = opposite(who);
        }
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
    bool checkForWin(char player) {
    // Check if there is a win for player.

        // check row
        for (int i = 0; i < 3; i++) {
            if (checkLine(game[i], player)) {
                return true;
            }
        }
        // check column
        for (int i = 0; i < 3; i++) {
            int col[3];
            for(int j = 0; j < 3; j++) {
                col[j] = game[j][i];
            }
            if (checkLine(col, player)) {
                return true;
            }
        }

        // check diagonal
        int dia_lr[3] = {game[0][0], game[1][1], game[2][2]};
        int dia_rl[3] = {game[2][0], game[1][1], game[0][2]};
        if (checkLine(dia_lr, player)) {
            return true;
        }
        if (checkLine(dia_rl, player)) {
            return true;
        }
        return false;
    }

    void cursor() {
        if (digitalRead(UP_PIN) == LOW) {
            long unsigned int currTime = millis();
            if (currTime - prevTime > 10) {
                x_pos = min(x_pos-1, 0);
            }
        prevTime = currTime;
        }
        else if (digitalRead(DOWN_PIN) == LOW) {
            long unsigned int currTime = millis();
            if (currTime - prevTime > 10) {
                x_pos = max(y_pos+1, 2);
            }
            prevTime = currTime;
        }
        else if (digitalRead(LEFT_PIN) == LOW) {
            long unsigned int currTime = millis();
            if (currTime - prevTime > 10) {
                y_pos = min(y_pos-1, 0);
            }
            prevTime = currTime;
        }
        else if (digitalRead(RIGHT_PIN) == LOW) {
            long unsigned int currTime = millis();
            if (currTime - prevTime > 10) {
                y_pos = max(y_pos+1, 2);
            }
            prevTime = currTime;
        }
        else if (digitalRead(PLACE_PIN) == LOW) {
            long unsigned int currTime = millis();
            if (currTime - prevTime > 10) {
                place();
            }
            prevTime = currTime;
        }
    }
        
    void blink() {
        for (int i = 0; i < 4; i++) {
            leds[board[x_pos][y_pos][i]] = gColor;
        }
        FastLED.show();
        delay(500);
        for (int i = 0; i < 4; i++) {
            leds[board[x_pos][y_pos][i]] = eColor;
        }
        FastLED.show();
        delay(500);
    }
    void loop() {
        // blink at x_pos, y_pos
        cursor();
        // this might cause issues bc of delay idk
        blink(); 
    }
