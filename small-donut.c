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

    float x_offset = 0;
    float y_offset = 0;
    float x_direction = 0.26;
    float y_direction = 0.26;

    int frame_count = 0;

    float x_scale = 0.15f;
    float y_scale = 0.25f;

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

                int x = (int)(screen_width / 2 + screen_width * x_scale * D * (l * h * m - t * n)) + x_offset;
                int y = (int)(screen_height / 2 + screen_height * y_scale * D * (l * h * n + t * m)) + y_offset;
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

        frame_count++;

        if (frame_count % 5 == 0) {
            x_offset += x_direction;
            y_offset += y_direction;

            if (x_offset < -screen_width / 4 || x_offset > screen_width / 4) {
                x_direction = -x_direction;
            }
            if (y_offset < -screen_height / 4 || y_offset > screen_height / 4) {
                y_direction = -y_direction;
            }
        }
    }

    return 0;
} 