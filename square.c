#include <math.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#define sleep_ms(ms) Sleep(ms)
#else
#include <unistd.h>
#define sleep_ms(ms) usleep((ms) * 1000)
#endif

#define WIDTH 80
#define HEIGHT 24

int main() {
  float angle = 0.0;
  // remove the cursor (optional)
  printf("\x1b[?25l");
  char buffer[WIDTH * HEIGHT];
  while (1) {
    // filling the frame buffer with spaces before the next frame
    memset(buffer, ' ', sizeof(buffer));

    // creating a math grid from -10 to 10
    for (float x = -10; x <= 10; x += 0.5) {
      for (float y = -10; y <= 10; y += 0.5) {
        if (fabs(x) > 9.5 || fabs(y) > 9.5) {
          // rotation formula
          float rotated_x = x * cos(angle) - y * sin(angle);
          float rotated_y = x * sin(angle) + y * cos(angle);

          // mapping the math space to terminal space (centering rotated points
          // according to the terminal)

          int terminal_x = (int)(WIDTH / 2 + rotated_x);
          int terminal_y = (int)(HEIGHT / 2 + rotated_y * 0.5);
          // 0.5 because terminals rows are taller than columns
          if (terminal_x >= 0 && terminal_y >= 0 && terminal_x < WIDTH &&
              terminal_y < HEIGHT) {
            // index = (row * WIDTH) + column
            // draw the rotated point into the frame buffer
            buffer[terminal_y * WIDTH + terminal_x] = '#';
          }
        }
      }
    }
    // move the cursor to top-left (the (0,0) for the terminal) for redrawing
    printf("\x1b[H");
    // render the current frame
    for (int i = 0; i < HEIGHT; ++i) {
      for (int j = 0; j < WIDTH; j++) {
        putchar(buffer[i * WIDTH + j]);
      }
      putchar('\n');
    }

    angle += 0.05;
    sleep_ms(50);
  }

  return 0;
}
