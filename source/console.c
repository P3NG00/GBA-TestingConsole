
#include <gba_console.h>
#include <gba_input.h>
#include <gba_interrupt.h>
#include <gba_video.h>
#include <gba_systemcalls.h>
#include <stdio.h>


#define CTRL "\x1b["
#define INVALID_CHAR (u8)-1

#define keyHeld(key) _keysHeld & key
#define keyDown(key) _keysDown & key


enum VCLine {
    LINE_UP     = (u8)'A',
    LINE_DOWN  	= (u8)'B',
    LINE_RIGHT 	= (u8)'C',
    LINE_LEFT  	= (u8)'D',
};


void vc_line(enum VCLine direction, u8 n) {
    iprintf(CTRL"%d%c", n, direction);
}
void vc_pos(u8 x, u8 y) {
    iprintf(CTRL"%d;%dH", y, x);
}
void vc_clear() {
    iprintf(CTRL"2J");
}
void vc_print(const char *text) {
    iprintf(text);
}
void vc_printc(char c) {
    iprintf("%c", c);
}


u16 _keysHeld, _keysDown;

char getCharacterFromHeld() {

    u8 c = INVALID_CHAR;

    if (keyHeld(KEY_RIGHT)) {
        c = 2;    // 2
        if (keyHeld(KEY_UP)) {
            c--;  // 1
        } else if (keyHeld(KEY_DOWN)) {
            c++;  // 3
        }
    } else if (keyHeld(KEY_LEFT)) {
        c = 6;    // 6
        if (keyHeld(KEY_UP)) {
            c++;  // 7
        } else if (keyHeld(KEY_DOWN)) {
            c--;  // 5
        }
    } else if (keyHeld(KEY_UP)) {
        c = 0;    // 0
    } else if (keyHeld(KEY_DOWN)) {
        c = 4;    // 4
    }

    // check if valid letter
    if (c != INVALID_CHAR) {
        // check modifiers for other letters
        if (keyHeld(KEY_L)) {
            c += 8;
        }
        if (keyHeld(KEY_R)) {
            c += 16;
        }
        // TODO uppercase for a-z
        if (c < 26) {
            return c + 'a';
        }
    }

    return '.';  // TODO change to space
}

void _handleInput_typing() {

    // check typing key
    if (keyDown(KEY_A)) {
        char c = getCharacterFromHeld();
        vc_printc(c);
    } else if (keyDown(KEY_B)) {
        // TODO backspace
    }
}

void handleInput() {

    scanKeys();
    _keysHeld = keysHeld();
    _keysDown = keysDown();

    // start clears screen and returns
    if (keyDown(KEY_START)) {
        vc_clear();
        return;
    }

    _handleInput_typing();
}


int main() {

    /* the vblank interrupt must be enabled for VBlankIntrWait() to work since
     * the default dispatcher handles the bios flags no vblank handler is required */
    irqInit();
    irqEnable(IRQ_VBLANK);

    consoleInit(0, 4, 0, NULL, 0, 15);

    // background color
    BG_COLORS[0] = RGB8(0b00010000, 0b00010000, 0b00010000);
    // font color
    BG_COLORS[241] = RGB5(0b11111, 0b11111, 0b11111);

    SetMode( MODE_0 | BG0_ON );

    vc_print("\nwork in progress\n\nuse A to confirm letter\nuse Start to clear screen\nuse DPAD+LR combinations for different letters\n\n");  // TODO edit

    while (1) {

        handleInput();

        VBlankIntrWait();
    }
}
