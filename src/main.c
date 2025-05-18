/*
    Better Button Test v5
    GBDK 2020 for Game Boy
*/

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <gb/gb.h>
#include <gb/cgb.h>
#include <gbdk/console.h>
#include <gbdk/font.h>

#include "border.h"


// RAM function stuff
#define objectDistance(a, b) ((void *)&(b) - (void *)&(a))
unsigned char __at _ram_func ramBuffer[]; // compiler will set this
typedef void (*ram_function_ptr)(void); // setup function pointers
ram_function_ptr saveFlashViaMem = (ram_function_ptr) ramBuffer;


// Keys
uint8_t previousKeys = 0;
uint8_t keys = 0;
#define UPDATE_KEYS() previousKeys = keys; keys = joypad()
#define KEY_PRESSED(K) (keys & (K))
#define KEY_TICKED(K) ((keys & (K)) && !(previousKeys & (K)))
#define KEY_RELEASED(K) (!(keys & (K)) && (previousKeys & (K)))
#define NO_KEYS_PRESSED() keys == 0


// Globals
uint8_t inMenu = 0;
uint8_t menuKeyLock = 0;
uint8_t themeIndex = 0;

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
uint16_t lowTone = 0;
uint16_t highTone = 0;

const uint16_t scaleLowG[] = {710, 854, 986, 1046, 1155, 1253, 1339, 1379}; // G3, A3, B3, C4, D4, E4, F#4, G4
const uint16_t scaleHighG[] = {1379, 1452, 1517, 1546, 1602, 1650, 1694, 1714}; // G4, A4, B4, C5, D5, E5, F#5, G5


// Drawing & Themes
font_t ibmFont, minFont, minFontInvert;
#define RGB_GOLD 0x331C
#define RGB_DARK_PURPLE 0x2405
#define RGB_DARK_GREEN 0x0923
#define RGB_DARK_BLUE 0x3860
#define RGB_DARK_ORANGE 0x012E
#define RGB_DARK_RED 0x080E
#define RGB_CERULEAN 0x5CC2
#define RGB_ICE_BLUE 0x5E89
#define RGB_GREY 0x3DEF
#define RGB_MUTED_PINK 0x5573
#define RGB_VIOLET 0x584C
#define RGB_POCKET_WHITE 0x5BDB
#define RGB_POCKET_LIGHT_GREY 0x4B15
#define RGB_POCKET_DARK_GREY 0x3E4F
#define RGB_POCKET_BLACK 0x2D89
#define RGB_EXTREME_RED 0x0CB7
#define RGB_LEAF_GREEN 0x2240
#define RGB_DUSK_GREEN 0x2392
#define RGB_DUSK_TEAL 0x4608
#define RGB_DUSK_PURPLE 0x28A6
#define RGB_SUNRISE_PURPLE 0x2CEF
#define RGB_SUNRISE_ORANGE 0x3ABF
#define RGB_SUNRISE_RED 0x35BD
#define RGB_SUNRISE_YELLOW 0x6BDF

                                    // NORMAL TEXT, HIGHLIGHT TEXT, HIGHLIGHT/TITLE, BG
const palette_color_t palettePurple[] = {RGB_WHITE, RGB_BLACK, RGB_GOLD, RGB_DARK_PURPLE};
const palette_color_t paletteBlue[] = {RGB_WHITE, RGB_BLACK, RGB_GOLD, RGB_DARK_BLUE};
const palette_color_t paletteGreen[] = {RGB_WHITE, RGB_BLACK, RGB_GOLD, RGB_DARK_GREEN};
const palette_color_t paletteOrange[] = {RGB_WHITE, RGB_BLACK, RGB_GOLD, RGB_DARK_ORANGE};
const palette_color_t paletteRed[] = {RGB_WHITE, RGB_BLACK, RGB_GOLD, RGB_DARK_RED};
const palette_color_t paletteBlack[] = {RGB_WHITE, RGB_BLACK, RGB_GOLD, RGB_BLACK};
const palette_color_t paletteSuperBlue[] = {RGB_WHITE, RGB_WHITE, RGB_ICE_BLUE, RGB_CERULEAN};
const palette_color_t paletteVeryGreen[] = {RGB_WHITE, RGB_GREEN, RGB_DARK_GREEN, RGB_LEAF_GREEN};
const palette_color_t paletteReallyPink[] = {RGB_WHITE, RGB_BLACK, RGB_VIOLET, RGB_MUTED_PINK};
const palette_color_t paletteExtremeRed[] = {RGB_WHITE, RGB_RED, RGB_BLACK, RGB_EXTREME_RED};
const palette_color_t paletteTerminal[] = {RGB_GREEN, RGB_WHITE, RGB_GREY, RGB_BLACK};
const palette_color_t paletteDotMatrix[] = {RGB_POCKET_WHITE, RGB_POCKET_LIGHT_GREY, RGB_POCKET_DARK_GREY, RGB_POCKET_BLACK};
const palette_color_t paletteDusk[] = {RGB_WHITE, RGB_DUSK_GREEN, RGB_DUSK_TEAL, RGB_DUSK_PURPLE};
const palette_color_t paletteSunrise[] = {RGB_SUNRISE_PURPLE, RGB_SUNRISE_ORANGE, RGB_SUNRISE_RED, RGB_SUNRISE_YELLOW};
const palette_color_t paletteWhiteHC[] = {RGB_BLACK, RGB_WHITE, RGB_BLACK, RGB_WHITE};
const palette_color_t paletteBlackHC[] = {RGB_WHITE, RGB_BLACK, RGB_WHITE, RGB_BLACK};

const char* themeNames[] = {
    "     Purple", "       Blue", "      Green", "     Orange",
    "        Red", "      Black", " Super Blue", " Very Green",
    "Really Pink", "Extreme Red", "   Terminal", " Dot Matrix", 
    "       Dusk", "    Sunrise", "   White HC", "   Black HC"
};
palette_color_t* palettes[] = {
    palettePurple, paletteBlue, paletteGreen, paletteOrange,
    paletteRed, paletteBlack, paletteSuperBlue, paletteVeryGreen,
    paletteReallyPink, paletteExtremeRed, paletteTerminal, paletteDotMatrix,
    paletteDusk, paletteSunrise, paletteWhiteHC, paletteBlackHC
};
const uint8_t themeCount = sizeof palettes / sizeof palettes[0];

void setupFonts(void) {
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

void drawBorder(void) {
    set_bkg_data(border_TILE_ORIGIN, border_TILE_COUNT, border_tiles);
    set_bkg_tiles(0, 0, 20u, 18u, border_map);
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

void clearMessageArea(void) {
    printAtWith("                  ", 1, 14, minFont);
    printAtWith("                  ", 1, 15, minFont);
    printAtWith("                  ", 1, 16, minFont);
}

void whiteScreen(void) {
    font_set(ibmFont);
    set_bkg_palette(0, 1, paletteWhiteHC);
    for (uint8_t i = 0; i < 18; i++) {
        gotoxy(0, i);
        printf("                    ");
    }
}


// Sound
void initSound(void) {
    NR52_REG = 0x80; // Turn on sound hardware
    NR11_REG = 0b10111111; // 2 bits for duty, 6 for length
    NR21_REG = 0b10111111; // 2 bits for duty, 6 for length
    NR50_REG = 0b01110111; // Channel Volume
    NR51_REG = 0b00010010; // Mix in Channel 1 & 2
}

void playTone(uint8_t scaleIndex) {
    if (playChannel) { // Channel 1 (Low)
        playChannel = 0;
        lowIndex = scaleIndex;
        lowTone = scaleLowG[lowIndex];
        if (highIndex == 0 && lowIndex == 7) lowTone = scaleLowG[0]; // prevent the same pitch from being played together
        
        NR12_REG = 0b10001111; // 4 bits for initial vol, 1 for env direction, 3 for sweep count
        NR13_REG = 0xFF & lowTone;
        NR14_REG = 0b10000000 | ((lowTone >> 8) & 0b00000111);
    } else { // Channel 2 (High)
        playChannel = 1;
        highIndex = scaleIndex;
        highTone = scaleHighG[highIndex];
        if (lowIndex == 7 && highIndex == 0) highTone = scaleHighG[7]; // prevent the same pitch from being played together
        
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

void stopAllTones(void) {
    for (uint8_t i = 0; i < 8; i++) maybeStopTone(i);
}


// Saving
#define RAM_ID 0xB4
uint8_t usingFlashSave = 0;

uint8_t volatile * const ramId = (uint8_t *) 0xA000;
uint8_t volatile * const ramTheme = (uint8_t *) 0xA001;

uint8_t volatile * const flashSectorAddr = (uint8_t *) 0x7000;
uint8_t volatile * const flashSaveByte = (uint8_t *) 0x7FFF;
uint8_t volatile * const fives = (uint8_t *) 0x5555;
uint8_t volatile * const two_a = (uint8_t *) 0x2AAA;

void saveFlash(void) {
    *fives = 0xAA;
    *two_a = 0x55;
    *fives = 0xA0;
    *flashSaveByte = themeIndex & 0xF;
    delay(100);
}
void saveFlashEnd(void) {}

void wipeFlash(void) {
    // wipe 0x7000 - 7FFF
    usingFlashSave = 1;
    *fives = 0xAA;
    *two_a = 0x55;
    *fives = 0x80;
    *fives = 0xAA;
    *two_a = 0x55;
    *flashSectorAddr = 0x30;
    delay(100);
}

void saveSettings(void) {
    *ramId = RAM_ID;
    *ramTheme = themeIndex;
    if (*ramId != RAM_ID && (usingFlashSave || themeIndex > 0)) {
        saveFlashViaMem();
    }
}

void loadSettings(void) {
    if (*flashSaveByte != 0xFF) {
        themeIndex = *flashSaveByte & 0xF;
        usingFlashSave = 1;
    }

    if (*ramId == RAM_ID) {
        themeIndex = *ramTheme;
        usingFlashSave = 0;
    }
}


// Konami code detection
uint8_t konamiStep = 0;
uint8_t konamiKeyLock = 0;
uint8_t konamiKeyLast = 0;
const uint8_t konamiSequence[] = {J_UP, J_UP, J_DOWN, J_DOWN, J_LEFT, J_RIGHT, J_LEFT, J_RIGHT, J_B, J_A};

uint8_t konamiCodeEntered(void) {
    if (_cpu != CGB_TYPE) return 0;
    if (NO_KEYS_PRESSED() || (konamiKeyLock && KEY_RELEASED(konamiKeyLast))) konamiKeyLock = 0;
    if (konamiKeyLock) return 0;

    if (KEY_PRESSED(konamiSequence[konamiStep])) {
        konamiKeyLast = konamiSequence[konamiStep];
        konamiKeyLock = 1;
        konamiStep++;
    } else if (keys > 0 && (konamiStep > 2 || !KEY_PRESSED(J_UP))) {
        // reset sequence if wrong key is pressed
        // EXCEPT if it's additional UP inputs at the beginning
        konamiStep = 0;
    }

    if (konamiStep == 10) {
        konamiStep = 0;
        return 1;
    }

    return 0;
}


// Main Loop
void draw(void) {
    if (inMenu) {
        font_set(ibmFont);
        gotoxy(1,1);
        printf("Theme: %s", (char*)themeNames[themeIndex]);

        if (usingFlashSave) {
            printAtWith("USING FLASH CONFIG", 1, 14, minFontInvert);
            printAtWith("HIT START TO CLEAR", 1, 15, minFont);
            printAtWith("THEN RESET GAMEBOY", 1, 16, minFont);
        } else {
            printAtWith("NICE KONAMI CODE", 2, 14, minFontInvert);
            printAtWith("SECRET MENU OPEN", 2, 15, minFontInvert);
        }
    } else {
        printAtWith("Better Button Test", 1, 1, ibmFont);

        if (!menuKeyLock) {
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
        }

        if (dpadError) {
            printAtWith("ILLEGAL DPAD INPUT", 1, 14, minFontInvert);
            printAtWith("MORE THAN 2 DIRS\n  PRESSED AT ONCE", 2, 15, minFont);
        } else if (totalCount > 255) {
            printAtWith("YOU REALLY LOVE", 2, 14, minFontInvert);
            printAtWith("TESTING BUTTONS", 3, 15, minFontInvert);
        }
    }
}

void update(void) {
    if (inMenu) {
        if (!usingFlashSave) {
            if (KEY_TICKED(J_RIGHT)) { 
                themeIndex = (themeIndex + 1) % themeCount;
            } else if (KEY_TICKED(J_LEFT)) {
                themeIndex = themeIndex - 1;
                if (themeIndex == 255) themeIndex = themeCount - 1;
            }

            if (_cpu == CGB_TYPE) set_bkg_palette(0, 1, palettes[themeIndex]);
        }

        if ((!usingFlashSave && KEY_TICKED(J_START)) || KEY_RELEASED(J_START) || KEY_TICKED(J_SELECT) || KEY_TICKED(J_A) || KEY_TICKED(J_B)) {
            inMenu = 0;
            menuKeyLock = 1;
            saveSettings();
            clearMessageArea();
        } else if (usingFlashSave && KEY_TICKED(J_START)) {
            whiteScreen();
            wipeFlash();
        }

    } else {
        if (!menuKeyLock) {
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

        if (NO_KEYS_PRESSED()) menuKeyLock = 0;

        if (konamiCodeEntered()) {
            stopAllTones();
            loadSettings();
            clearMessageArea();
            inMenu = 1;
        }
    }
}

void main(void) {
    ENABLE_RAM;

    memcpy(&ramBuffer, (void *) &saveFlash, (uint16_t) objectDistance(saveFlash, saveFlashEnd));

    loadSettings();
    initSound();
    setupFonts();
    drawBorder();

    if (_cpu == CGB_TYPE) set_bkg_palette(0, 1, palettes[themeIndex]);

    while(1) {
        UPDATE_KEYS();
        update();
        draw();

        wait_vbl_done();
    }
}
