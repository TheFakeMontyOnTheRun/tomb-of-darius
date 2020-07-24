#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef CPC_PLATFORM
#include <cpctelera.h>
#endif



void shutdownGraphics();

void clearGraphics();

void writeStr(uint8_t nColumn, uint8_t nLine, char *str, uint8_t fg, uint8_t bg);

void graphicsPut(uint8_t x, uint8_t y);

uint8_t getKey();

void init();

void graphicsFlush();

int8_t cameraX = 33;
int8_t cameraZ = 22;
uint8_t running = 1;

void fill(uint8_t x0, uint8_t y0, uint8_t dx, uint8_t dy, uint8_t colour);


const int8_t map[32][32] = {
        {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 2, 8, 7, 7, 7, 7, 7, 8, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 2, 2, 7, 0, 0, 0, 7, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 2, 7, 7, 7, 7, 7, 9, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 2, 7, 7, 8, 8, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 2, 7, 7, 2, 2, 1, 1, 1, 1, 2, 7, 7, 7, 7, 7, 7, 7, 7, 2, 2, 2, 2, 1, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 2, 7, 7, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 8, 3, 3, 3, 7, 8, 2, 1, 0, 0, 0},
        {1, 2, 2, 2, 2, 2, 2, 7, 7, 2, 9, 2, 8, 2, 2, 2, 2, 2, 2, 1, 1, 2, 7, 7, 7, 7, 2, 2, 1, 0, 0, 0},
        {1, 2, 8, 7, 7, 5, 3, 6, 6, 3, 4, 7, 7, 7, 7, 7, 7, 5, 8, 1, 1, 2, 7, 7, 7, 7, 2, 1, 1, 0, 0, 0},
        {1, 2, 2, 7, 7, 7, 7, 7, 7, 7, 7, 7, 0, 0, 0, 0, 7, 7, 2, 1, 1, 2, 7, 7, 6, 7, 2, 1, 1, 0, 0, 0},
        {1, 2, 9, 7, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2, 1, 1, 2, 7, 7, 7, 7, 2, 2, 1, 1, 1, 0},
        {1, 2, 2, 8, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2, 1, 1, 2, 7, 7, 0, 7, 9, 2, 2, 2, 1, 0},
        {1, 1, 2, 3, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2, 1, 1, 9, 7, 7, 0, 7, 7, 7, 8, 2, 1, 0},
        {0, 1, 2, 3, 6, 7, 0, 0, 0, 0, 7, 7, 7, 0, 0, 0, 0, 7, 2, 1, 1, 7, 7, 7, 0, 0, 0, 7, 2, 2, 1, 0},
        {0, 1, 2, 3, 6, 7, 0, 0, 0, 0, 7, 3, 7, 0, 0, 0, 0, 7, 2, 1, 1, 0, 0, 7, 0, 0, 0, 7, 2, 1, 1, 0},
        {0, 1, 2, 2, 6, 7, 0, 0, 0, 0, 7, 7, 7, 0, 0, 0, 0, 7, 2, 1, 7, 7, 7, 7, 0, 0, 0, 7, 2, 2, 1, 0},
        {0, 1, 1, 2, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2, 1, 2, 2, 8, 7, 0, 7, 7, 7, 9, 2, 1, 0},
        {0, 0, 1, 2, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2, 1, 2, 2, 2, 7, 0, 7, 8, 2, 2, 2, 1, 0},
        {0, 0, 1, 2, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2, 1, 2, 2, 2, 7, 0, 7, 2, 2, 1, 1, 1, 0},
        {0, 0, 1, 2, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2, 1, 2, 2, 2, 7, 0, 7, 2, 2, 1, 0, 0, 0},
        {0, 0, 1, 2, 7, 0, 0, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 9, 1, 2, 2, 9, 7, 7, 7, 9, 2, 1, 0, 0, 0},
        {0, 0, 1, 2, 7, 0, 0, 7, 8, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 7, 7, 7, 8, 2, 2, 1, 0, 0, 0},
        {0, 0, 1, 2, 7, 0, 0, 7, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 7, 0, 7, 2, 2, 1, 1, 0, 0, 0},
        {0, 1, 1, 2, 7, 0, 0, 7, 2, 2, 2, 2, 9, 7, 0, 0, 7, 7, 3, 1, 2, 9, 7, 0, 7, 2, 1, 1, 0, 0, 0, 0},
        {0, 1, 2, 2, 7, 0, 0, 7, 2, 7, 7, 7, 7, 7, 0, 0, 0, 7, 7, 7, 7, 7, 7, 0, 7, 2, 2, 1, 0, 0, 0, 0},
        {0, 1, 2, 9, 7, 7, 7, 7, 2, 7, 7, 7, 7, 7, 0, 0, 0, 7, 7, 7, 7, 7, 7, 7, 7, 9, 2, 1, 0, 0, 0, 0},
        {0, 1, 2, 2, 2, 2, 2, 2, 9, 2, 2, 2, 8, 7, 0, 0, 0, 7, 8, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 7, 0, 0, 7, 7, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 7, 7, 7, 7, 8, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

int8_t max(int8_t x1, int8_t x2) {
    return x1 > x2 ? x1 : x2;
}

int8_t min(int8_t x1, int8_t x2) {
    return x1 < x2 ? x1 : x2;
}


void renderScene() {
   uint8_t x, y;

   for ( y = 0; y < 32; ++y ) {
        for (x = 0; x < 32; ++x ) {
            fill( x * 4, y * 4, 4, 4, map[y][x]);
        }
   }
}

#ifdef CPCTELERA_ALL_H
uint8_t __at(0x7E00) reserveStack[512];
#endif


void tickRenderer() {
    uint8_t prevX;
    uint8_t prevZ;

#ifndef CPCTELERA_ALL_H
    clearGraphics();
#endif
    renderScene();


    graphicsFlush();
#ifdef CPCTELERA_ALL_H
    clearGraphics();
#endif
    prevX = cameraX;
    prevZ = cameraZ;

    waitkey:
    switch (getKey()) {
        case 'q':
            running = 0;
            break;
        case 'l':
            running = 0;
            break;

        case 'a':
            cameraX--;
            break;
        case 'd':
            cameraX++;
            break;


        case 's':
            cameraZ++;
            break;
        case 'w':
            cameraZ--;
            break;

#ifndef XCODE_BUILD
#if !defined(SDLSW) || !defined(AMIGA)
        default:
            goto waitkey;
#endif
#endif
    }

    if (cameraZ >= 32) {
        cameraZ = 31;
    }

    if (cameraX >= 32) {
        cameraX = 31;
    }

    if (cameraZ < 0) {
        cameraZ = 0;
    }

    if (cameraX < 0) {
        cameraX = 0;
    }
}


#ifdef XCODE_BUILD
int demoMain() {
#else

int main(int argc, char **argv) {
#endif


#ifdef CPCTELERA_ALL_H
    cpct_setStackLocation(0x7E00);
#endif
    {
        running = 1;
        cameraX = 5;
        cameraZ = 15;
        init();

#ifndef XCODE_BUILD
        do {
            tickRenderer();
        } while (running);

        shutdownGraphics();
#endif
    }
    return 0;
}
