#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "maps.h"

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
uint8_t room = 0;
enum ECellType playerState = kHero;

void fill(uint8_t x0, uint8_t y0, uint8_t dx, uint8_t dy, uint8_t colour);


enum ECellType applyState(enum ECellType otherState) {
    switch (otherState) {
        case kKey1:
        case kKey2:
        case kKey3:
        case kKey4:
        case kPrize:
            playerState = otherState;
            return kNothing;
        case kEnemy:
            playerState = kGameOver;
            puts("GAME OVER!");
            return otherState;
        case kAltar:
            if (playerState == kPrize) {
                puts("VICTORY!");
                playerState = kVictory;
            }

        case kWall:
        case kNothing:
        case kHero:
        default:
            return otherState;
    }
}


int block(enum ECellType otherState) {
    switch (otherState) {



        case kWall:
            return 1;
        case kAltar:
        case kKey1:
        case kKey2:
        case kKey3:
        case kKey4:
        case kPrize:
        case kEnemy:
        case kNothing:
        case kHero:
        default:
            return 0;
    }
}

int8_t max(int8_t x1, int8_t x2) {
    return x1 > x2 ? x1 : x2;
}

int8_t min(int8_t x1, int8_t x2) {
    return x1 < x2 ? x1 : x2;
}

int blink = 0;

void renderScene() {
   uint8_t x, y;

   for ( y = 0; y < 32; ++y ) {
        for (x = 0; x < 32; ++x ) {
            fill( x * 5, y * 5, 5, 5, map[room][y][x]);
        }
   }

   blink++;

   fill(cameraX * 5, cameraZ * 5, 5, 5, playerState);
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
        case 's':
            cameraX++;
            break;


        case 'z':
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

    map[room][cameraZ][cameraX] = applyState(map[room][cameraZ][cameraX]);

        if (block(map[room][cameraZ][cameraX])){
            cameraX = prevX;
            cameraZ = prevZ;
        }




        if (cameraZ >= 32) {
            cameraZ = 0;
            room += 8;
        }

        if (cameraX >= 32) {
            cameraX = 0;
            room++;
        }

        if (cameraZ < 0) {
            cameraZ = 31;
            room -= 8;
        }

        if (cameraX < 0) {
            cameraX = 31;
            --room;
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
        cameraX = 15;
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
