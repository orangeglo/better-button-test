/*
    Better Button Test v4
    GBDK 2020 for Game Boy
*/

#include <stdint.h>
#include <stdio.h>
#include <gb/gb.h>
#include <gb/cgb.h>
#include <gbdk/console.h>
#include <gbdk/font.h>


// Keys
uint8_t previousKeys = 0;
int8_t keys = 0;
#define UPDATE_KEYS() previousKeys = keys; keys = joypad()
#define KEY_PRESSED(K) (keys & (K))
#define KEY_TICKED(K) ((keys & (K)) && !(previousKeys & (K)))
#define KEY_RELEASED(K) (!(keys & (K)) && (previousKeys & (K)))


// Globals
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

uint8_t playChannel = 0;
uint8_t lowIndex = 255;
uint8_t highIndex = 255;
uint16_t scaleLow[] = {710, 854, 986, 1046, 1155, 1253, 1339, 1379}; // G3, A3, B3, C4, D4, E4, F#4, G4
uint16_t scaleHigh[] = {1379, 1452, 1517, 1546, 1602, 1650, 1694, 1714}; // G4, A4, B4, C5, D5, E5, F#5, G5
uint16_t lowTone = 0;
uint16_t highTone = 0;


// Drawing
#define RGB_DARK_PURPLE 0x2405
#define RGB_GOLD 0x331C
font_t ibmFont, minFont, minFontInvert;
const palette_color_t cgb_palette[] = {RGB_WHITE, RGB_BLACK, RGB_GOLD, RGB_DARK_PURPLE};

void setupFonts() {
    font_init();
    font_color(0, 3);
    minFont = font_load(font_min);

    if (_cpu == CGB_TYPE) {
        font_color(2, 3);
    } else {
        font_color(0, 3);
    }
    ibmFont = font_load(font_ibm);

    if (_cpu == CGB_TYPE) {
        font_color(1, 2);
    } else {
        font_color(3, 0);
    }
    minFontInvert = font_load(font_min);
}

void printAtWith(char str[], uint8_t x, uint8_t y, font_t font) {
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


// Sound
void initSound() {
    NR52_REG = 0x80; // Turn on sound hardware
    NR11_REG = 0b10111111; // 2 bits for duty, 6 for length
    NR21_REG = 0b10111111; // 2 bits for duty, 6 for length
    NR50_REG = 0b01110111; // Channel Volume
    NR51_REG = 0b00110011; // Mix in Channel 1 & 2
}

void playTone(uint8_t scaleIndex) {
    if (playChannel) { // Channel 1 (Low)
        playChannel = 0;
        lowIndex = scaleIndex;
        lowTone = scaleLow[lowIndex];
        if (highIndex == 0 && lowIndex == 7) lowTone = scaleLow[0]; // prevent the same pitch from being played together
        
        NR12_REG = 0b10001111; // 4 bits for initial vol, 1 for env direction, 3 for sweep count
        NR13_REG = 0xFF & lowTone;
        NR14_REG = 0b10000000 | ((lowTone >> 8) & 0b00000111);
    } else { // Channel 2 (High)
        playChannel = 1;
        highIndex = scaleIndex;
        highTone = scaleHigh[highIndex];
        if (lowIndex == 7 && highIndex == 0) highTone = scaleHigh[7]; // prevent the same pitch from being played together
        
        NR22_REG = 0b10001111; // 4 bits for initial vol, 1 for env direction, 3 for sweep count
        NR23_REG = 0xFF & highTone;
        NR24_REG = 0b10000000 | ((highTone >> 8) & 0b00000111);
    }
}

void maybeStopTone(uint8_t scaleIndex) {
    if (lowIndex == scaleIndex) {
        lowIndex = 255;
        NR12_REG = 0b10000111; // 4 bits for initial vol, 1 for env direction, 3 for sweep count
        NR14_REG = 0b01000000 | ((lowTone >> 8) & 0b00000111);
    } else if (highIndex == scaleIndex) {
        highIndex = 255;
        NR22_REG = 0b10000111; // 4 bits for initial vol, 1 for env direction, 3 for sweep count
        NR24_REG = 0b01000000 | ((highTone >> 8) & 0b00000111);
    }
}

void drawStatic() {
    printAtWith("Better Button Test", 1, 1, ibmFont);
}

void draw() {
    printAtWith("A", 1, 4, pressedFont(J_A));
    if (aCount) printCountAt(14, 4, aCount);
    
    printAtWith("B", 1, 5, pressedFont(J_B));
    if (bCount) printCountAt(14, 5, bCount);

    printAtWith("UP", 1, 6, pressedFont(J_UP));
    if (upCount) printCountAt(14, 6, upCount);

    printAtWith("DOWN", 1, 7, pressedFont(J_DOWN));
    if (downCount) printCountAt(14, 7, downCount);

    printAtWith("LEFT", 1, 8, pressedFont(J_LEFT));
    if (leftCount) printCountAt(14, 8, leftCount);
    
    printAtWith("RIGHT", 1, 9, pressedFont(J_RIGHT));
    if (rightCount) printCountAt(14, 9, rightCount);
    
    printAtWith("START", 1, 10, pressedFont(J_START));
    if (startCount) printCountAt(14, 10, startCount);

    printAtWith("SELECT", 1, 11, pressedFont(J_SELECT));
    if (selectCount) printCountAt(14, 11, selectCount);


    if (dpadError) {
        printAtWith("ILLEGAL DPAD INPUT", 1, 14, minFontInvert);
        printAtWith("MORE THAN 2 DIRS\n  PRESSED AT ONCE", 2, 15, minFont);
    } else if (totalCount > 255) {
        printAtWith("YOU REALLY LOVE", 2, 14, minFontInvert);
        printAtWith("TESTING BUTTONS", 3, 15, minFontInvert);
    }
}

void update() {
    if (KEY_TICKED(J_A)) { playTone(0); aCount++; totalCount++; } else if (KEY_RELEASED(J_A)) maybeStopTone(0);
    if (KEY_TICKED(J_B)) { playTone(1); bCount++; totalCount++; } else if (KEY_RELEASED(J_B)) maybeStopTone(1);
    if (KEY_TICKED(J_UP)) { playTone(2); upCount++; totalCount++; } else if (KEY_RELEASED(J_UP)) maybeStopTone(2);
    if (KEY_TICKED(J_DOWN)) { playTone(3); downCount++; totalCount++; } else if (KEY_RELEASED(J_DOWN)) maybeStopTone(3);
    if (KEY_TICKED(J_LEFT)) { playTone(4); leftCount++; totalCount++; } else if (KEY_RELEASED(J_LEFT)) maybeStopTone(4);
    if (KEY_TICKED(J_RIGHT)) { playTone(5); rightCount++; totalCount++; } else if (KEY_RELEASED(J_RIGHT)) maybeStopTone(5);
    if (KEY_TICKED(J_START)) { playTone(6); startCount++; totalCount++; } else if (KEY_RELEASED(J_START)) maybeStopTone(6);
    if (KEY_TICKED(J_SELECT)) { playTone(7); selectCount++; totalCount++; } else if (KEY_RELEASED(J_SELECT)) maybeStopTone(7);

    if (
        (KEY_PRESSED(J_UP) && KEY_PRESSED(J_DOWN))
        || (KEY_PRESSED(J_LEFT) && KEY_PRESSED(J_RIGHT))
    ) dpadError = 1;
}

void main(void) {
    if (_cpu == CGB_TYPE) set_bkg_palette(0, 1, cgb_palette);

    initSound();
    setupFonts();
    drawStatic();

    while(1) {
        update();
        draw();
        wait_vbl_done();
        UPDATE_KEYS();
    }
}
