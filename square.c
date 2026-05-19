#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define sleep_ms(ms) usleep((ms) * 1000)

#define WIDTH 80
#define HEIGHT 24

int main() {
  float angle = 0.0;
  // remove the cursor (optional)
  printf("\x1b[?25l");
  char buffer[WIDTH * HEIGHT];
  while (1) {
    memset(buffer, ' ', sizeof(buffer));

    // setup the mathematical cordinates system
    for (float x = -10; x <= 10; x += 0.5) {
      for (float y = -10; y <= 10; y += 0.5) {
        if (fabs(x) > 9.5 || fabs(y) > 9.5) {
          float rotated_x = x * cos(angle) - y * sin(angle);
          float rotated_y = x * sin(angle) + y * cos(angle);

          // center from top-left of the terminal to (0,0) dead center
          // it like calculating the index for framebuffer cordinates system
          // based on math system because they aren't the same and we need to
          // sort of like translate
          int terminal_x = (int)(WIDTH / 2 + rotated_x);
          int terminal_y = (int)(HEIGHT / 2 + rotated_y * 0.5);
          // 0.5 because terminals rows are taller than columns
          if (terminal_x >= 0 && terminal_y >= 0 && terminal_x < WIDTH &&
              terminal_y < HEIGHT) {
            // index = (row * WIDTH) + columns
            buffer[terminal_y * WIDTH + terminal_x] = '#';
          }
        }
      }
    }
    printf("\x1b[H");
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
