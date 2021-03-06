#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cpctelera.h>

uint8_t frame = 0;

#define MOVING_POINTERS

void shutdownGraphics() {
}

void clearGraphics();


void graphicsPut(uint8_t nColumn, uint8_t nLine);

void writeStr(uint8_t nColumn, uint8_t nLine, char *str, uint8_t fg, uint8_t bg);

uint8_t getKey() {

    cpct_scanKeyboard_f();

    if (cpct_isKeyPressed(Key_Q))
        return 'q';


    if (cpct_isKeyPressed(Key_CursorLeft) || cpct_isKeyPressed(Key_A))
        return 'a';

    if (cpct_isKeyPressed(Key_CursorRight) || cpct_isKeyPressed(Key_S))
        return 's';

    if (cpct_isKeyPressed(Key_CursorUp) || cpct_isKeyPressed(Key_W))
        return 'w';

    if (cpct_isKeyPressed(Key_CursorDown) || cpct_isKeyPressed(Key_Z))
        return 'z';

    if (cpct_isKeyPressed(Key_P) || cpct_isKeyPressed(Key_X))
        return 'x';

    if (cpct_isKeyPressed(Key_O) || cpct_isKeyPressed(Key_D))
        return 'd';


    return '.';
}

void fill(uint8_t x0, uint8_t y0, uint8_t dx, uint8_t dy, uint8_t colour) {
    uint8_t *pScreen = (uint8_t *) (frame) ? 0x8000 : 0xC000;

    unsigned char *pS;
    unsigned char *base;

    base = pScreen + (x0 >> 1);


    pS = ((unsigned char *) base + ((y0 >> 3) * 80) + ((y0 & 7) << 11));
    cpct_drawSolidBox(pS, cpct_px2byteM0(colour, colour), dx, dy);
}

void init() {
    cpct_disableFirmware();

    cpct_setVideoMode(0);

    cpct_memset_f64(0xC000, 0x0000, 16 * 1024);
    cpct_memset_f64(0x8000, 0x0000, 16 * 1024);

#ifndef MOVING_POINTERS
    frame = 1;
    cpct_setVideoMemoryPage(cpct_pageC0);
#else
    frame = 0;
    cpct_setVideoMemoryPage(cpct_page80);
#endif
}

void graphicsFlush() {
    cpct_waitVSYNC();
#ifdef MOVING_POINTERS
    if (frame) {
        cpct_setVideoMemoryPage(cpct_page80);
        frame = 0;

    } else {
        cpct_setVideoMemoryPage(cpct_pageC0);
        frame = 1;

    }
#else
    memcpy(0xC000, 0x8000, 80 * 200);
#endif
}

void fix_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {

    if (x0 == x1) {

        uint8_t _y0 = y0;
        uint8_t _y1 = y1;

        if (y0 > y1) {
            _y0 = y1;
            _y1 = y0;
        }


        for (uint8_t y = _y0; y <= _y1; ++y) {
            if (y >= 128) {
                continue;
            }

            graphicsPut(x0, y);
        }
        return;
    }

    if (y0 == y1) {
        uint8_t _x0 = x0;
        uint8_t _x1 = x1;

        if (x0 > x1) {
            _x0 = x1;
            _x1 = x0;
        }

        for (uint8_t x = _x0; x <= _x1; ++x) {
            if (y0 >= 128) {
                continue;
            }

            graphicsPut(x, y0);
        }
        return;
    }

    //switching x0 with x1
    if (x0 > x1) {
        x0 = x0 + x1;
        x1 = x0 - x1;
        x0 = x0 - x1;

        y0 = y0 + y1;
        y1 = y0 - y1;
        y0 = y0 - y1;
    }
    {
        //https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm

        int8_t dx = abs(x1 - x0);
        int8_t sx = x0 < x1 ? 1 : -1;
        int8_t dy = -abs(y1 - y0);
        int8_t sy = y0 < y1 ? 1 : -1;
        int8_t err = dx + dy;  /* error value e_xy */
        int8_t e2;
        while (1) {
            graphicsPut(x0, y0);
            /* loop */
            if (x0 == x1 && y0 == y1) return;
            e2 = 2 * err;

            if (e2 >= dy) {
                err += dy; /* e_xy+e_x > 0 */
                x0 += sx;
            }

            if (e2 <= dx) {
                /* e_xy+e_y < 0 */
                err += dx;
                y0 += sy;
            }
        }
    }
}

void hLine(uint8_t x0, uint8_t x1, uint8_t y) {

    uint8_t *pScreen = (uint8_t *) (frame) ? 0x8000 : 0xC000;
    unsigned char *pS;
    unsigned char *base;
    uint8_t nLine = y;
    uint8_t nColumn = 0;
    uint8_t rest;
    uint8_t bytes;
    uint8_t dx = (x1 - x0);
    bytes = dx >> 1;
    base = ((unsigned char *) pScreen + ((nLine & 248) * 10) + ((nLine & 7) << 11)) + (x0 >> 1);
//write whole bytes first, then the remainder with masks

    if (x0 & 1) {
        graphicsPut(x0, nLine);
        base++;
        dx--;
        bytes--;
        x0++;
    }

    memset(base, 0xFF, bytes);

    base += bytes;
    dx -= (bytes << 1);

    for (nColumn = 0; nColumn <= dx; nColumn++) {

        unsigned char nByte;
        pS = base + (nColumn >> 1);
        nByte = *pS;

        if (nColumn & 1) {
            nByte &= 170;
            nByte |= 64;
        } else {
            nByte &= 85;
            nByte |= 128;
        }

        *pS = nByte;
    }
}

void vLine(uint8_t x0, uint8_t y0, uint8_t y1) {

    uint8_t *pScreen = (uint8_t *) (frame) ? 0x8000 : 0xC000;

    unsigned char *pS;
    unsigned char *base;

    base = pScreen + (x0 >> 1);

    if (y0 > y1) {
        uint8_t tmp = y0;
        y0 = y1;
        y1 = tmp;
    }

    pS = ((unsigned char *) base + ((y0 >> 3) * 80) + ((y0 & 7) << 11));
    cpct_drawSolidBox(pS, 1, 1, y1 - y0);
}

void writeStr(uint8_t nColumn, uint8_t nLine, char *str, uint8_t fg, uint8_t bg) {
    unsigned char *pS = (uint8_t *) (frame) ? 0x8000 : 0xC000;
    unsigned char nPixel = 0;

    if (nColumn >= 128 || nLine >= 200) {
        return;
    }

    nPixel = nColumn & 1;

    pS = ((unsigned char *) pS + ((nLine & 248) * 10) + ((nLine & 7) << 11)) + (nColumn >> 1);

    cpct_drawStringM0(str, pS);
}

inline void graphicsPut(uint8_t nColumn, uint8_t nLine) {
    uint8_t *pScreen = (uint8_t *) (frame) ? 0x8000 : 0xC000;

    unsigned char *pS;
    unsigned char nByte = 0;

    pS = ((unsigned char *) pScreen + ((nLine & 248 ) * 10) + ((nLine & 7) << 11)) + (nColumn >> 1);
    nByte = *pS;

    if (nColumn & 1) {
        nByte &= 170;
        nByte |= 64;
    } else {
        nByte &= 85;
        nByte |= 128;
    }

    *pS = nByte;
}

void clearGraphics() {
    memset((frame) ? 0x8000 : 0xC000, 0, 16 * 1024);
}


//just to ensure nothing will leak into the back buffer
uint8_t __at(0x8000) reserve[16 * 1024];

uint8_t __at(0xC000) reserve2[ 16 * 1024];
#ifdef MOVING_POINTERS

#endif
