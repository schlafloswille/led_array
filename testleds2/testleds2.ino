#include <FastSPI_LED.h>

#define NUM_LEDS 169

// Sometimes chipsets wire in a backwards sort of way
struct CRGB { unsigned char g; unsigned char r; unsigned char b; };
// struct CRGB { unsigned char r; unsigned char g; unsigned char b; };
struct CRGB *leds;


#define WIDTH 13
#define PIN 4
#define MODES 5
#define VALS_PER_MODES 255/MODES

uint8_t logo[] = {
0,0,0,0,1,1,1,1,1,0,0,0,0,
0,0,0,1,0,0,0,0,0,1,0,0,0,
0,0,0,1,0,1,1,1,0,1,0,0,0,
0,0,0,1,0,0,1,0,0,1,0,0,0,
0,0,0,0,1,0,0,0,1,0,0,0,0,
0,0,0,0,0,1,0,0,0,0,0,0,0,
0,0,0,0,0,0,1,0,0,0,0,0,0,
0,0,0,0,0,0,0,1,0,0,0,0,0,
0,0,0,0,1,0,0,0,1,0,0,0,0,
0,0,0,1,0,0,1,0,0,1,0,0,0,
0,0,0,1,0,1,1,1,0,1,0,0,0,
0,0,0,1,0,0,0,0,0,1,0,0,0,
0,0,0,0,1,1,1,1,1,0,0,0,0};


uint8_t trace_x[] = {8, 9, 9, 9, 8, 7, 6, 5, 4, 3, 3, 3, 4, 5, 6, 7, 8, 9, 9, 9, 8, 7, 6, 5, 4, 3, 3, 3, 4};
uint8_t trace_y[] = {4, 3, 2, 1, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 12, 12, 12, 12, 11, 10, 9, 8};


void interleave(){
  char temp;
  for(int row=1; row < NUM_LEDS/WIDTH; row += 2) {
    for(int block=0; block < WIDTH/2; block++ ){
      temp = leds[row*WIDTH+block].r;
      leds[row*WIDTH+block].r = leds[row*WIDTH+WIDTH-(block+1)].r;
      leds[row*WIDTH+WIDTH-(block+1)].r = temp;
      temp = leds[row*WIDTH+block].g;
      leds[row*WIDTH+block].g = leds[row*WIDTH+WIDTH-(block+1)].g;
      leds[row*WIDTH+WIDTH-(block+1)].g = temp;
      temp = leds[row*WIDTH+block].b;
      leds[row*WIDTH+block].b = leds[row*WIDTH+WIDTH-(block+1)].b;
      leds[row*WIDTH+WIDTH-(block+1)].b = temp;
    }
  }
  
}
uint8_t dimm = analogRead(1)>>2;
void setup()
{
  FastSPI_LED.setLeds(NUM_LEDS);
  FastSPI_LED.setChipset(CFastSPI_LED::SPI_TM1809);
  FastSPI_LED.setPin(PIN);
  FastSPI_LED.init();
  FastSPI_LED.start();

  leds = (struct CRGB*)FastSPI_LED.getRGBData(); 
  Serial.begin(9600);
  
  
 
}

int toggle=0;

void loop() {
  static uint8_t p = 0;
  static int8_t d = 1;
  static uint8_t first = 0;
  static uint8_t cr = 0;
  static uint8_t cg = 0xff;
  static uint8_t cb = 0;
  static uint8_t *up = &cr;
  static uint8_t *down = &cg;
  static uint8_t *els = &cb;
  
  dimm = analogRead(1)>>2;
  if(!first) {
    for(uint8_t i=0; i < NUM_LEDS; i++) {
      if(logo[i]) {
        leds[i].r = dimm;
        leds[i].g = dimm;
        leds[i].b = dimm;
      }
      else {
        leds[i].r = 0;
        leds[i].g = 0;
        leds[i].b = 0;
      }
    }
    first = 1;
    interleave();
  }
  interleave(); 
  if(*up < 0xff) {
    (*up)++;
    (*down)--;
  }
  else {
    uint8_t *tmp = up;
    up = els;
    els = down;
    down = tmp;
  }
  
  leds[trace_x[p] + WIDTH * trace_y[p]].r = ((uint32_t) cr * dimm) / 0xff;
  leds[trace_x[p] + WIDTH * trace_y[p]].g = ((uint32_t) cg * dimm) / 0xff; 
  leds[trace_x[p] + WIDTH * trace_y[p]].b = ((uint32_t) cb * dimm) / 0xff;
  if(p == sizeof(trace_x) - 1) {
    d = -1;
    p = sizeof(trace_x) - 2;
  }
  else if(p == 0) {
    d = 1;
    p = 1;
  }
  else {
    p += d;
  }
  interleave(); 
  FastSPI_LED.show();
  delay(analogRead(0));
}
