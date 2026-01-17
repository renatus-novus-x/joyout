#include <stdio.h>
#include <stdint.h>
#include "platform.h"

static void print_bits4(uint8_t v){
  putchar((v & 0x8) ? '1' : '0');
  putchar((v & 0x4) ? '1' : '0');
  putchar((v & 0x2) ? '1' : '0');
  putchar((v & 0x1) ? '1' : '0');
}

int main(int argc, char* argv[]) {
  (void)argc;
  (void)argv;

  platform_init();

  printf("JOYOUT: map keyboard 1-4 to ATARI 9-pin digital outputs (4-bit)\n");
  printf("  bit0: JoyPort1 pin6 (PC6, inverted)\n");
  printf("  bit1: JoyPort1 pin7 (PC7, inverted)\n");
  printf("  bit2: JoyPort1 pin8 (PC4, normal)\n");
  printf("  bit3: JoyPort2 pin8 (PC5, normal)\n");
  printf("Hold keys [1][2][3][4] to drive outputs. ESC to exit.\n\n");

  uint8_t last = 0xFF;
  clock_t now = platform_clock();

  platform_joyout_write4(0);

  while (1) {
    if (platform_kbd_esc_down()) {
      break;
    }

    uint8_t v = platform_kbd_1234_mask();
    platform_joyout_write4(v);

    if (v != last) {
      last = v;
      printf("\rOUT=" );
      print_bits4(v);
      printf(" (hex:%X)   ", (unsigned)v);
      fflush(stdout);
    }

    platform_wait_next_tick(&now);
  }

  platform_joyout_write4(0);
  printf("\nDone.\n");
  platform_exit();
  return 0;
}
