#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"
#include "math.h"

#define M_PI 3.14159265358979323846
const int NUM_PIXELS = 30;  // Number of pixels in your LED strip
const int PIN_TX = 0;

struct RGB{
uint8_t r;
uint8_t g;
uint8_t b;
};

static inline void put_pixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
  return ((uint32_t)(r) << 8) |
         ((uint32_t)(g) << 16) |
         (uint32_t)(b);
}

// The brightness is calculated using a sine wave equation
// Max Brightness is 1, Min is 0.2
// https://www.desmos.com/calculator/vfgtcffqq5
double calculate_brigthness(int x){
  float brightness = 0.4 *sin(M_PI * x/80 + M_PI/2) + 0.6;
  return brightness;
}

int get_color(int x){
  static int index = 0;
  //change speed, default is 800 steps
  if(x>0 && x%800==0){
    index = (index + 1) % 6;
  }
  return index;
}

//colors in rgb
const struct RGB color_1 = {165,0,109};
const struct RGB color_2 = {84,3,117};
const struct RGB color_3 = {20,111,210};
const struct RGB color_4 = {68,218,0};
const struct RGB color_5 = {219,224,0};
const struct RGB color_6 = {243,35,35};


int main() {
  stdio_init_all();

  PIO pio = pio0;
  int sm = 0;
  uint offset = pio_add_program(pio, &ws2812_program);
  ws2812_program_init(pio, sm, offset, PIN_TX, 700000, false);
  int step=0;
  struct RGB colors[6] = {color_1,color_2,color_3,color_4,color_5,color_6};
  struct RGB color = colors[get_color(step)];

  uint32_t color_clear = urgb_u32((color.r),(color.g),(color.b));

  int count = 0;
  while(count<NUM_PIXELS){

    for(int in =0;in<=count;in++){
      put_pixel(color_clear);
    }
    sleep_ms(10);
    count++;
    }

  while(true){
    color = colors[get_color(step)];
    // light fading
    double bright = calculate_brigthness(step);
    //printf("brightness is : %f\n", bright);
    uint32_t color_fade = urgb_u32((color.r*bright),(color.g*bright),(color.b*bright));
    int i = 0;
    for (; i < NUM_PIXELS; i++) {
        put_pixel(color_fade);
      }
    sleep_ms(50);
    step++;
      }
    }
