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

// --#--#--
// --#--#--
// ########
// --#--#--
// --#--#--
// ########
// --#--#--
// --#--#--

//grid 
int board[3][3] = { {new G(8, 9, 22, 23), new G(38, 39, 40, 41), new G(56, 57, 70, 71)}, {new G(11, 12, 19, 20), new G(35, 36, 43, 44), new G(59, 60, 67, 68)}, 
{new G(14, 15, 16, 17), new G(32, 33, 46, 47), new G(62, 63, 64, 64)} };

class G {
    private:
        int p[4];
    public:
        G(int a, int b, int c, int d) {
            p[0] = a;
            p[1] = b;
            p[2] = c;
            p[3] = d;
        }
        int[] getG() {
            return p;
        }
};

CRGB oColor = CRGB(255, 0, 0);
CRGB xColor = CRGB(0, 0, 255);
CRGB wColor = CRGB(255, 255, 255);
CRGB eColor = CRGB(0, 0, 0);

void setup() {
    // reset positions
    x_pos = 0;
    y_pos = 0;

    // setup buttons
    pinMode(UP_PIN, INPUT_PULLUP);
    pinMode(DOWN_PIN, INPUT_PULLUP);
    pinMode(LEFT_PIN, INPUT_PULLUP);
    pinMode(RIGHT_PIN, INPUT_PULLUP);
    pinMode(PAUSE_PIN, INPUT_PULLUP);

    // setup ws2812b leds
    FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
    
    // setup grid lines 
    int grid[28] = {10, 13, 18, 21, 24, 25, 26, 27, 28, 29, 30, 31, 34, 37, 42, 45, 48, 49, 50, 51, 52, 53, 54, 55, 58, 61, 66, 69};
    for (int i = 0; i < 28; i++) {
        leds[grid[i]] = wColor;
        FastLED.show();
    }

    // reset board to be empty
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 4; k++) {
                leds[board[i][j].getG()[k]] = eColor;
            }
        }
    }
}


void loop() {

}