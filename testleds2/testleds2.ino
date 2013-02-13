#include <FastSPI_LED.h>

#define NUM_LEDS 169

// Sometimes chipsets wire in a backwards sort of way
struct CRGB { unsigned char g; unsigned char r; unsigned char b; };
// struct CRGB { unsigned char r; unsigned char g; unsigned char b; };
struct CRGB *leds;
struct CRGB leds_a[13][13];


#define WIDTH 13
#define PIN 4
#define MODES 5
#define VALS_PER_MODES 255/MODES

uint8_t logo[] = {0,0,0,0,1,1,1,1,1,0,0,0,0,
0,0,0,1,0,0,0,0,0,1,0,0,0,
0,0,0,1,0,1,1,1,0,1,0,0,0,
0,0,0,1,0,0,1,0,0,1,0,0,0,
0,0,0,0,1,0,0,0,1,0,0,0,0,
0,0,0,0,0,1,0,0,0,0,0,0,0,
0,0,0,0,0,0,1,0,0,0,0,0,0,
0,0,0,0,1,0,0,1,0,0,0,0,0,
0,0,0,1,0,0,1,0,0,1,0,0,0,
0,0,0,1,0,1,1,1,0,1,0,0,0,
0,0,0,1,0,0,0,0,0,1,0,0,0,
0,0,0,0,1,1,1,1,1,0,0,0,0};

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
void setup()
{
  FastSPI_LED.setLeds(NUM_LEDS);
  //FastSPI_LED.setChipset(CFastSPI_LED::SPI_SM16716);
  FastSPI_LED.setChipset(CFastSPI_LED::SPI_TM1809);
  //FastSPI_LED.setChipset(CFastSPI_LED::SPI_LPD6803);
  //FastSPI_LED.setChipset(CFastSPI_LED::SPI_HL1606);
  //FastSPI_LED.setChipset(CFastSPI_LED::SPI_595);
  //FastSPI_LED.setChipset(CFastSPI_LED::SPI_WS2801);

  FastSPI_LED.setPin(PIN);
  
  FastSPI_LED.init();
  FastSPI_LED.start();

  leds = (struct CRGB*)FastSPI_LED.getRGBData(); 
  Serial.begin(9600);
}

int toggle=0;
uint8_t dimm = analogRead(2)>>2;
void loop() { 
  
  //int mode=analogRead(1)>>2;
  //memset(leds,0,NUM_LEDS);
  //dimm = analogRead(2)>>2;
  for(uint8_t i=0; i < NUM_LEDS; i++) {
    if(logo[i]) {
      leds[i].r = 0xff;
      leds[i].g = 0xff;
      leds[i].b = 0xff;
    }
    else {
      leds[i].r = 0;
      leds[i].g = 0;
      leds[i].b = 0;
    }
  }
 
 interleave(); 
  FastSPI_LED.show();
  delay(analogRead(0));
  
 
}
