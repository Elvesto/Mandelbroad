#include "raylib.h"
#include "render.h"

#include <stdlib.h>
#include <stdio.h>

#include <immintrin.h>
#include <stdint.h>
#include <stdio.h>

int Calculate(Color* buffer, int countPix, MandelWrote* view) {
    for (int i = 0; i < countPix; i += 8) {
        float tempX[8] = {};
        float tempY[8] = {};
        for (int j = 0; j < 8; j++) {
            int index = i + j;
            if (index >= countPix) { break; }

            int px = index % view->screenWidth;
            int py = index / view->screenWidth;

            tempX[j] = (float)((px - view->screenWidth / 2.0) / (0.3 * view->zoom * view->screenWidth) + view->offsetX);
            tempY[j] = (float)((py - view->screenHeight / 2.0) / (0.3 * view->zoom * view->screenHeight) + view->offsetY);
        }

        __m256 x0 = _mm256_loadu_ps(tempX);
        __m256 y0 = _mm256_loadu_ps(tempY);
        __m256 x = _mm256_set1_ps(0);
        __m256 y = _mm256_set1_ps(0);
        __m256 four = _mm256_set1_ps(4);
        __m256i iters = _mm256_setzero_si256();
        __m256i one = _mm256_set1_epi32(1);

        for (int iteration = 0; iteration < view->maxIterations; iteration++) {
            __m256 x2 = _mm256_mul_ps(x, x);
            __m256 y2 = _mm256_mul_ps(y, y);

            __m256 mask = _mm256_cmp_ps(_mm256_add_ps(x2, y2), four, _CMP_LE_OQ);

            if (_mm256_movemask_ps(mask) == 0) {
                break;
            }

            iters = _mm256_add_epi32(iters, _mm256_and_si256(one, _mm256_castps_si256(mask)));

            __m256 xy = _mm256_mul_ps(x, y);
            y = _mm256_add_ps(_mm256_add_ps(xy, xy), y0);

            x = _mm256_add_ps(_mm256_sub_ps(x2, y2), x0);
        }

        int final[8] = {};
        _mm256_storeu_si256((__m256i*)final, iters);

        for (int j = 0; j < 8; j++) {
            if (i + j >= countPix) { 
                break; 
            }

            int iteration = final[j];
            if (iteration < view->maxIterations) {
                unsigned char val = (unsigned char)(255 * iteration / view->maxIterations);
                buffer[i + j] = (Color){ val, 0, val, 255 };
            } else {
                buffer[i + j] = PURPLE;
            }
        }
    }

    return 0;
}