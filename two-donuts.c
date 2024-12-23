#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int main() {
    srand(time(NULL));  // Seed the random number generator

    // Donut #1 rotation parameters
    float A1 = 0.0f, B1 = 0.0f;
    // Donut #2 rotation parameters
    float A2 = 0.0f, B2 = 0.0f;

    // We'll draw both donuts in the same screen buffer.
    // They share the same scale but have independent offsets.
    float x_scale = 0.15f;
    float y_scale = 0.25f;

    // Define the default screen size
    // Prepare to read screen size
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    int screen_width = w.ws_col;
    int screen_height = w.ws_row;

    // Offsets (centers) and directions for Donut #1
    float x_offset1 = -screen_width / 4.0f;  // Start on the left side
    float y_offset1 = -screen_height / 4.0f; // Start at the top
    float x_direction1 = 0.4f;
    float y_direction1 = 0.4f;

    // Offsets (centers) and directions for Donut #2
    float x_offset2 = screen_width / 4.0f;   // Start on the right side
    float y_offset2 = screen_height / 4.0f;  // Start at the bottom
    float x_direction2 = 0.6f;
    float y_direction2 = 0.3f;

    // How often to update positions
    int frame_count = 0;

    // "Radius" at which we consider them colliding (approximate bounding diameter)
    float collision_threshold = 15.0f;

    int buffer_size = screen_width * screen_height;

    // Our buffers for ASCII & depth
    char b[buffer_size];
    float z[buffer_size];

    // Clear the screen once
    printf("\x1b[2J");

    // Main animation loop
    for (;;) {
        // Reset buffers
        memset(b, 32, buffer_size);
        memset(z, 0, buffer_size * sizeof(float));

        // ----------------------------------
        // Draw Donut #1
        // ----------------------------------
        for (float j = 0; j < 2 * M_PI; j += 0.02f) {
            for (float i = 0; i < 2 * M_PI; i += 0.02f) {
                float c = sinf(i), d = cosf(j), e = sinf(A1), f = sinf(j);
                float g = cosf(A1), h = d + 2.0f;
                float D = 1.0f / (c * h * e + f * g + 5.0f);
                float l = cosf(i), m = cosf(B1), n = sinf(B1);
                float t = c * h * g - f * e;

                int x = (int)(
                    screen_width / 2 
                    + screen_width * x_scale * D * (l * h * m - t * n)
                ) + (int)x_offset1;

                int y = (int)(
                    screen_height / 2 
                    + screen_height * y_scale * D * (l * h * n + t * m)
                ) + (int)y_offset1;

                int o = x + screen_width * y;

                // Brightness calc (index into the luminance string)
                int N = (int)(8.0f * ((f * e - c * d * g) * m - c * d * e - f * g - l * d * n));

                if (y >= 0 && y < screen_height && x >= 0 && x < screen_width && D > z[o]) {
                    z[o] = D;
                    if (N < 0)  N = 0;
                    if (N > 11) N = 11;
                    b[o] = ".,-~:;=!*#$@"[N];
                }
            }
        }

        // ----------------------------------
        // Draw Donut #2
        // ----------------------------------
        for (float j2 = 0; j2 < 2 * M_PI; j2 += 0.02f) {
            for (float i2 = 0; i2 < 2 * M_PI; i2 += 0.02f) {
                float c = sinf(i2), d = cosf(j2), e = sinf(A2), f = sinf(j2);
                float g = cosf(A2), h = d + 2.0f;
                float D = 1.0f / (c * h * e + f * g + 5.0f);
                float l = cosf(i2), m = cosf(B2), n = sinf(B2);
                float t = c * h * g - f * e;

                int x = (int)(
                    screen_width / 2 
                    + screen_width * x_scale * D * (l * h * m - t * n)
                ) + (int)x_offset2;

                int y = (int)(
                    screen_height / 2
                    + screen_height * y_scale * D * (l * h * n + t * m)
                ) + (int)y_offset2;

                int o = x + screen_width * y;

                // Brightness calc
                int N = (int)(8.0f * ((f * e - c * d * g) * m - c * d * e - f * g - l * d * n));

                if (y >= 0 && y < screen_height && x >= 0 && x < screen_width && D > z[o]) {
                    z[o] = D;
                    if (N < 0)  N = 0;
                    if (N > 11) N = 11;
                    b[o] = ".,-~:;=!*#$@"[N];
                }
            }
        }

        // Move cursor back to the top-left
        printf("\x1b[H");
        // Print the buffer
        for (int k = 0; k < buffer_size; k++) {
            putchar(k % screen_width ? b[k] : '\n');
        }

        // Update rotation parameters
        A1 += 0.004f;
        B1 += 0.002f;
        A2 += 0.003f;
        B2 += 0.001f;

        frame_count++;

        // Update offsets (bounce logic, every few frames to moderate speed)
        if (frame_count % 5 == 0) {
            // ----------------------------
            // Donut #1 boundary checks
            // ----------------------------
            x_offset1 += x_direction1;
            y_offset1 += y_direction1;

            // Horizontal boundary
            if (x_offset1 < -screen_width / 4.0f || x_offset1 > screen_width / 4.0f) {
                x_direction1 = -x_direction1;
                // Optionally randomize speed again
                float speed_factor = 0.5f + (float)rand() / RAND_MAX; 
                x_direction1 *= speed_factor;

                if (x_offset1 < -screen_width / 4.0f) x_offset1 = -screen_width / 4.0f;
                if (x_offset1 >  screen_width / 4.0f) x_offset1 =  screen_width / 4.0f;
            }

            // Vertical boundary
            if (y_offset1 < -screen_height / 4.0f || y_offset1 > screen_height / 4.0f) {
                y_direction1 = -y_direction1;
                float speed_factor = 0.5f + (float)rand() / RAND_MAX; 
                y_direction1 *= speed_factor;

                if (y_offset1 < -screen_height / 4.0f) y_offset1 = -screen_height / 4.0f;
                if (y_offset1 >  screen_height / 4.0f) y_offset1 =  screen_height / 4.0f;
            }

            // ----------------------------
            // Donut #2 boundary checks
            // ----------------------------
            x_offset2 += x_direction2;
            y_offset2 += y_direction2;

            // Horizontal boundary
            if (x_offset2 < -screen_width / 4.0f || x_offset2 > screen_width / 4.0f) {
                x_direction2 = -x_direction2;
                float speed_factor = 0.5f + (float)rand() / RAND_MAX; 
                x_direction2 *= speed_factor;

                if (x_offset2 < -screen_width / 4.0f) x_offset2 = -screen_width / 4.0f;
                if (x_offset2 >  screen_width / 4.0f) x_offset2 =  screen_width / 4.0f;
            }

            // Vertical boundary
            if (y_offset2 < -screen_height / 4.0f || y_offset2 > screen_height / 4.0f) {
                y_direction2 = -y_direction2;
                float speed_factor = 0.5f + (float)rand() / RAND_MAX; 
                y_direction2 *= speed_factor;

                if (y_offset2 < -screen_height / 4.0f) y_offset2 = -screen_height / 4.0f;
                if (y_offset2 >  screen_height / 4.0f) y_offset2 =  screen_height / 4.0f;
            }

            // ----------------------------
            // Check collision between donuts
            // ----------------------------
            float dx = x_offset1 - x_offset2;
            float dy = y_offset1 - y_offset2;
            float dist_sq = dx * dx + dy * dy;

            // If they are "close enough" (distance < collision_threshold) -> swap velocities
            if (dist_sq < (collision_threshold * collision_threshold)) {
                float temp_dx = x_direction1;
                float temp_dy = y_direction1;
                x_direction1 = x_direction2;
                y_direction1 = y_direction2;
                x_direction2 = temp_dx;
                y_direction2 = temp_dy;
            }
        }
    }

    return 0;
} 