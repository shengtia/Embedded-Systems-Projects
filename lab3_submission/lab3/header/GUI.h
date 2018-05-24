#include <stdbool.h>
#define SCREEN_WIDTH	64
#define SCREEN_HEIGHT	32
#define FONTSET_SIZE	80
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 320

// method to draw a single pixel on the screen
void draw_square(float x_coord, float y_coord);

// idling function for animation, called by glutIdleFunc
void idle();

// initializes GL 2D mode and other settings
void initGL();

// function to handle user keyboard input (when pressed) 
void your_key_press_handler(unsigned char key, int x, int y);

// function to handle key release
void your_key_release_handler(unsigned char key, int x, int y);

// change key status
void updateKey(unsigned char key, bool b);
