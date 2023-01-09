/*
    Better Button Test v2
    GBDK 2020 for Game Boy
*/

#include <stdint.h>
#include <stdio.h>
#include <gb/gb.h>
#include <gb/cgb.h>
#include <gbdk/console.h>
#include <gbdk/font.h>

#define MIN(x,y) ((x) > (y) ? (y) : (x))
#define MAX(x,y) ((x) < (y) ? (y) : (x))

// Keys
uint8_t previousKeys = 0;
int8_t keys = 0;
#define UPDATE_KEYS() previousKeys = keys; keys = joypad()
#define KEY_PRESSED(K) (keys & (K))
#define KEY_TICKED(K) ((keys & (K)) && !(previousKeys & (K)))

// Global Variables
uint16_t totalCount = 0;
uint16_t startCount = 0;
uint16_t selectCount = 0;
uint16_t aCount = 0;
uint16_t bCount = 0;
uint16_t upCount = 0;
uint16_t downCount = 0;
uint16_t leftCount = 0;
uint16_t rightCount = 0;
uint8_t dpadError = 0;

// Draw Functions
#define RGB_DARK_PURPLE 0x2405
#define RGB_GOLD 0x331C
font_t ibmFont, minFont, minFontInvert;
const palette_color_t cgb_palette[] = {RGB_WHITE, RGB_BLACK, RGB_GOLD, RGB_DARK_PURPLE};

void setup_fonts() {
    font_init();
    font_color(0, 3);
    ibmFont = font_load(font_ibm);
    minFont = font_load(font_min);
    if (_cpu == CGB_TYPE) {
        font_color(1, 2);
    } else {
        font_color(3, 0);
    }
    minFontInvert = font_load(font_min);
}

void print_at_with(char str[], uint8_t x, uint8_t y, font_t font) {
    font_set(font);
    gotoxy(x, y);
    printf(str);
}

void printCountAt(uint8_t x, uint8_t y, uint16_t count) {
    if (count >= 10000) {
        gotoxy(x, y);
    } else if (count >= 1000) {
        gotoxy(x + 1, y);
    } else if (count >= 100) {
        gotoxy(x + 2, y);
    } else if (count >= 10) {
        gotoxy(x + 3, y);
    } else {
        gotoxy(x + 4, y);
    }
    printf("%d", count);
}

font_t pressedFont(uint8_t key) {
    if (KEY_PRESSED(key)) {
        return minFontInvert;
    } else {
        return minFont;
    }
}

void draw_static() {
    print_at_with("Better Button Test", 1, 1, ibmFont);
    print_at_with("v2", 17, 2, ibmFont);
}

#define YO 0 // Y Offset
void draw() {
    print_at_with("A", 1, 4 + YO, pressedFont(J_A));
    if (aCount) printCountAt(14, 4 + YO, aCount);
    
    print_at_with("B", 1, 5 + YO, pressedFont(J_B));
    if (bCount) printCountAt(14, 5 + YO, bCount);

    print_at_with("UP", 1, 6 + YO, pressedFont(J_UP));
    if (upCount) printCountAt(14, 6 + YO, upCount);

    print_at_with("DOWN", 1, 7 + YO, pressedFont(J_DOWN));
    if (downCount) printCountAt(14, 7 + YO, downCount);

    print_at_with("LEFT", 1, 8 + YO, pressedFont(J_LEFT));
    if (leftCount) printCountAt(14, 8 + YO, leftCount);
    
    print_at_with("RIGHT", 1, 9 + YO, pressedFont(J_RIGHT));
    if (rightCount) printCountAt(14, 9 + YO, rightCount);
    
    print_at_with("START", 1, 10 + YO, pressedFont(J_START));
    if (startCount) printCountAt(14, 10 + YO, startCount);

    print_at_with("SELECT", 1, 11 + YO, pressedFont(J_SELECT));
    if (selectCount) printCountAt(14, 11 + YO, selectCount);

    if (dpadError) {
        print_at_with("ILLEGAL DPAD INPUT", 1, 14 + YO, minFontInvert);
        print_at_with("MORE THAN 2 DIRS\n  PRESSED AT ONCE", 2, 15 + YO, minFont);
    } else if (totalCount > 255) {
        print_at_with("YOU REALLY LOVE", 2, 14 + YO, minFontInvert);
        print_at_with("TESTING BUTTONS", 3, 15 + YO, minFontInvert);
    }
}

void update() {
    if (KEY_TICKED(J_START)) { startCount++; totalCount++; }
    if (KEY_TICKED(J_SELECT)) { selectCount++; totalCount++; }
    if (KEY_TICKED(J_A)) { aCount++; totalCount++; }
    if (KEY_TICKED(J_B)) { bCount++; totalCount++; }
    if (KEY_TICKED(J_UP)) { upCount++; totalCount++; }
    if (KEY_TICKED(J_DOWN)) { downCount++; totalCount++; }
    if (KEY_TICKED(J_LEFT)) { leftCount++; totalCount++; }
    if (KEY_TICKED(J_RIGHT)) { rightCount++; totalCount++; }

    if (
        (KEY_PRESSED(J_UP) && KEY_PRESSED(J_DOWN))
        || (KEY_PRESSED(J_LEFT) && KEY_PRESSED(J_RIGHT))
    ) dpadError = 1;
}

void main(void) {
    if (_cpu == CGB_TYPE) set_bkg_palette(0, 1, cgb_palette);

    setup_fonts();
    draw_static();

    while(1) {
        update();
        draw();
        wait_vbl_done();
        UPDATE_KEYS();
    }
}
