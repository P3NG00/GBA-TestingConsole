
#include <gba_console.h>
// #include <gba_input.h>
#include <gba_interrupt.h>
#include <gba_video.h>
#include <gba_systemcalls.h>
#include <stdio.h>

// TODO use
// bool button_held(u16 button) {
//     /* and the button register with the button constant we want */
//     u16 pressed = REG_KEYINPUT & button;
//     if (pressed == 0) {
//         return true;
//     } else {
//         return false;
//     }
// }

void vc_print(u8 x, u8 y, const char *text) {
    // ansi escape sequence to set print co-ordinates
    // /x1b[line;columnH
    iprintf("\x1b[%d;%dH%s", x, y, text);
}

enum VCLine {
    LINE_UP     = 0,
    LINE_DOWN  	= 1,
    LINE_RIGHT 	= 2,
    LINE_LEFT  	= 3,
};

void vc_line(enum VCLine direction, u8 n, const char *text) {
    char controlChar = 'A' + (char)direction;
    iprintf("\x1b[%d%c%s", n, controlChar, text);
}

int main(void) {
    // the vblank interrupt must be enabled for VBlankIntrWait() to work
    // since the default dispatcher handles the bios flags no vblank handler
    // is required
    irqInit();
    irqEnable(IRQ_VBLANK);

    consoleInit(0, 4, 0, NULL, 0, 15);

    // background color
    BG_COLORS[0] = RGB8(0b00010000, 0b00010000, 0b00010000);
    // font color
    BG_COLORS[241] = RGB5(0b11111, 0b11111, 0b11111);

    SetMode( MODE_0 | BG0_ON );

    // ansi escape sequence to clear screen and home cursor
    // /x1b[line;columnH (#TODO this seems wrong)
    iprintf("\x1b[2J");

    vc_print(10, 10, "Hello World!");
    vc_line(LINE_UP, 10, "Line 0");
    vc_line(LINE_LEFT, 28, "Column 0");
    vc_line(LINE_DOWN, 19, "Line 19");
    vc_line(LINE_RIGHT, 5, "Column 20");

    while (1) {
        VBlankIntrWait();
    }
}
