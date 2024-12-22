#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys/ioctl.h>
#include <unistd.h>

int main() {
    float A = 0, B = 0;
    float i, j;

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    int screen_width = w.ws_col;
    int screen_height = w.ws_row;
    int buffer_size = screen_width * screen_height;

    char b[buffer_size];
    float z[buffer_size];
    int k;

    printf("\x1b[2J");
    for (;;) {
        memset(b, 32, buffer_size);
        memset(z, 0, buffer_size * sizeof(float));

        for (j = 0; j < 2 * M_PI; j += 0.02) {
            for (i = 0; i < 2 * M_PI; i += 0.02) {
                float c = sinf(i);
                float d = cosf(j);
                float e = sinf(A);
                float f = sinf(j);
                float g = cosf(A);
                float h = d + 2;
                float D = 1 / (c * h * e + f * g + 5);
                float l = cosf(i);
                float m = cosf(B);
                float n = sinf(B);
                float t = c * h * g - f * e;

                int x = (int)(screen_width / 2 + screen_width * 0.3 * D * (l * h * m - t * n));
                int y = (int)(screen_height / 2 + screen_height * 0.5 * D * (l * h * n + t * m));
                int o = x + screen_width * y;

                int N = (int)(8 * ((f * e - c * d * g) * m - c * d * e - f * g - l * d * n));

                if (y >= 0 && y < screen_height && x >= 0 && x < screen_width && D > z[o]) {
                    z[o] = D;
                    b[o] = ".,-~:;=!*#$@"[(N > 0) ? N : 0];
                }
            }
        }

        printf("\x1b[H");
        for (k = 0; k < buffer_size; k++) {
            putchar(k % screen_width ? b[k] : '\n');
        }

        A += 0.004f;
        B += 0.002f;
    }

    return 0;
} 