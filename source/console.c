
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

void vc_print(u8 x, u8 y, char text[]) {
	// ansi escape sequence to set print co-ordinates
	// /x1b[line;columnH
	iprintf("\x1b[%d;%dH%s", x, y, text);
}

int main(void) {
	// the vblank interrupt must be enabled for VBlankIntrWait() to work
	// since the default dispatcher handles the bios flags no vblank handler
	// is required
	irqInit();
	irqEnable(IRQ_VBLANK);

	consoleInit(0, 4, 0, NULL, 0, 15);

	BG_COLORS[0] = RGB8(0b00010000, 0b00010000, 0b00010000);  // background color
	BG_COLORS[241] = RGB5(0b11111, 0b11111, 0b11111);  // font color

	SetMode( MODE_0 | BG0_ON );

	// ansi escape sequence to clear screen and home cursor
	// /x1b[line;columnH (TODO this seems wrong)
	iprintf("\x1b[2J");

	vc_print(10, 10, "Hello World!");

	// ansi escape sequence to move cursor up
	// /x1b[linesA
	iprintf("\x1b[10ALine 0");

	// ansi escape sequence to move cursor left
	// /x1b[columnsD
	iprintf("\x1b[28DColumn 0");

	// ansi escape sequence to move cursor down
	// /x1b[linesB
	iprintf("\x1b[19BLine 19");

	// ansi escape sequence to move cursor right
	// /x1b[columnsC
	iprintf("\x1b[5CColumn 20");

	while (1) {
		VBlankIntrWait();
	}
}
