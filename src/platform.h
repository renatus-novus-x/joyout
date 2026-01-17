#ifndef PLATFORM_H
#define PLATFORM_H

#include <stdint.h>



#if X68K

#include <x68k/iocs.h>

#undef  CLOCKS_PER_SEC
#define CLOCKS_PER_SEC 100
typedef uint32_t clock_t;

static int usp_;

/*
  Set this to 1 if your _iocs_bitsns() returns 0 when a key is pressed.
  (Some documents describe the key-matrix as active-low.)
*/
#ifndef PLATFORM_BITSNS_ACTIVE_LOW
#define PLATFORM_BITSNS_ACTIVE_LOW 0
#endif

/* forward declarations */
static inline void platform_joyout_init(void);
static inline void platform_joyout_write4(uint8_t v);

static inline uint8_t platform_bitsns_raw(int group){
  return (uint8_t)_iocs_bitsns(group);
}

static inline uint8_t platform_bitsns_bit(int group, int bit){
  uint8_t v = (platform_bitsns_raw(group) >> bit) & 1;
#if PLATFORM_BITSNS_ACTIVE_LOW
  v = (uint8_t)(v ^ 1);
#endif
  return v;
}

static inline uint32_t trap_ontime_cs(void){
  uint32_t cs;
  __asm__ volatile(
    "moveq  #0x7F,%%d0 \n\t"  /* _ONTIME */
    "trap   #15        \n\t"  /* IOCS    */
    "move.l %%d0,%0    \n\t"
    : "=d"(cs)
    :
    : "d0","d1","a0","cc","memory"
  );
  return cs;
}

static inline void platform_init(void){
  usp_ = _iocs_b_super(0);
  /* Configure i8255 and make outputs released by default. */
  platform_joyout_init();
}

static inline void platform_exit(void){
  _iocs_b_super(usp_);
}

static inline clock_t platform_clock(void){
  return (clock_t)trap_ontime_cs();
}

static inline uint32_t platform_seconds_to_ticks(uint32_t sec){
  return sec * (uint32_t)CLOCKS_PER_SEC;
}

static inline uint32_t platform_elapsed_ticks(clock_t start, clock_t end){
  return (uint32_t)((uint32_t)end - (uint32_t)start);
}

static inline float platform_elapsed_sec(clock_t start, clock_t end){
  return (float)platform_elapsed_ticks(start, end) / (float)CLOCKS_PER_SEC;
}

static inline void platform_wait_next_tick(clock_t* now){
  clock_t base = *now;
  while ((uint32_t)(trap_ontime_cs() - (uint32_t)base) == 0) { /* busy-wait 1/100 */ }
  *now = base + 1;
}

static inline unsigned char platform_gttrig(int no){
  if (no == 0) { /* keyboard: SPACE (group6 bit5) */
    return platform_bitsns_bit(6, 5);
  } else {      /* joystick: A button */
    no = no - 1;
    return !(_iocs_joyget(no) & 0x20);
  }
}

/*
  Keyboard helpers for this project.

  Key mapping is based on the BITSNS group/bit table:
  group 0: bit0=ESC, bit1='1', bit2='2', bit3='3', bit4='4'
*/
static inline uint8_t platform_kbd_esc_down(void){
  return platform_bitsns_bit(0, 0);
}

static inline uint8_t platform_kbd_1234_mask(void){
  uint8_t g0 = platform_bitsns_raw(0);
#if PLATFORM_BITSNS_ACTIVE_LOW
  g0 = (uint8_t)(g0 ^ 0xFF);
#endif
  /* pack [1][2][3][4] into 4-bit: bit0=1, bit1=2, bit2=3, bit3=4 */
  return (uint8_t)(((g0 >> 1) & 1) |
                   (((g0 >> 2) & 1) << 1) |
                   (((g0 >> 3) & 1) << 2) |
                   (((g0 >> 4) & 1) << 3));
}

/*
  JoyPort output control (i8255 Port C bits).

  Target pins (ATARI port output) you requested:
    JoyPort1 pin6 = PC6 (inverted output)
    JoyPort1 pin7 = PC7 (inverted output)
    JoyPort1 pin8 = PC4 (normal output)
    JoyPort2 pin8 = PC5 (normal output)

  Notes:
    - The 'inverted output' pins become LOW when the port-C bit is set to 1.
    - The 'normal output' pins become LOW when the port-C bit is 0.
*/
#define I8255_PORTA 0xE9A001u
#define I8255_PORTB 0xE9A003u
#define I8255_PORTC 0xE9A005u
#define I8255_CTRL  0xE9A007u

static inline void i8255_out8(uint32_t addr, uint8_t v){
  *((volatile uint8_t*)addr) = v;
}

static inline void i8255_pc_bsr(int bit, int set){
  /* BSR: D7=0, D3..1=bit number, D0=set(1) / reset(0) */
  i8255_out8(I8255_CTRL, (uint8_t)((bit << 1) | (set ? 1 : 0)));
}

static inline void platform_joyout_init(void){
  /*
    8255 mode-set:
      0x93 = 1001 0011b
        PA: input, PB: input
        PC upper(4-7): output
        PC lower(0-3): input (keeps ADPCM-related lines untouched, if any)
  */
  i8255_out8(I8255_CTRL, 0x93);
  /* release all outputs */
  platform_joyout_write4(0);
}

static inline void platform_joyout_write4(uint8_t v){
  /* bit0: JoyPort1 pin6 (PC6 inverted) */
  i8255_pc_bsr(6, (v & 0x1) ? 1 : 0);

  /* bit1: JoyPort1 pin7 (PC7 inverted) */
  i8255_pc_bsr(7, (v & 0x2) ? 1 : 0);

  /* bit2: JoyPort1 pin8 (PC4 normal, ON=LOW) */
  i8255_pc_bsr(4, (v & 0x4) ? 0 : 1);

  /* bit3: JoyPort2 pin8 (PC5 normal, ON=LOW) */
  i8255_pc_bsr(5, (v & 0x8) ? 0 : 1);
}

#endif /* X68K */

#endif /* PLATFORM_H */
