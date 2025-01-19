
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

enum VCLine {
    LINE_UP     = (u8)'A',
    LINE_DOWN  	= (u8)'B',
    LINE_RIGHT 	= (u8)'C',
    LINE_LEFT  	= (u8)'D',
};

void vc_line(enum VCLine direction, u8 n) {
    iprintf("\x1b[%d%c", n, direction);
}

void vc_pos(u8 x, u8 y) {
    iprintf("\x1b[%d;%dH", x, y);
}

void vc_clear() {
    iprintf("\x1b[2J");
}

void vc_print(const char *text) {
    iprintf(text);
}

int main(void) {
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

    vc_clear();

    // TODO remove testing
    vc_print("Hello, World!");
    vc_clear();
    vc_print("\ntesting :)");
    vc_pos(0, 0);
    vc_print("whoah");

    while (1) {
        VBlankIntrWait();
    }
}
