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
  
  int mode=analogRead(1)>>2;
  memset(leds,0,NUM_LEDS);
dimm = analogRead(2)>>2;
  
  if(mode<VALS_PER_MODES){
	 memset(leds,0,13*13*3);
      
       
         if(toggle){
           for(int k=0 ; k< NUM_LEDS/2+1; k++){
           leds[k].r=dimm;
           leds[k].g=0;
           leds[k+NUM_LEDS/2].g=dimm;
           leds[k+NUM_LEDS/2].r=0;
           toggle=0;
           }
         }else{
           for(int k=0 ; k< NUM_LEDS/2+1; k++){
           leds[k].g=dimm;
           leds[k].r=0;
           leds[k+NUM_LEDS/2].r=dimm;
           leds[k+NUM_LEDS/2].g=0;
           toggle=1;
         }
         }
      
         
  }else if(mode>=VALS_PER_MODES && mode <VALS_PER_MODES*2){
	 memset(leds,0,13*13*3);
    if(toggle){
           for(int k=0 ; k< NUM_LEDS; k++){
           if(k&1){
             leds[k].r=dimm;
             leds[k].g=0;
           }else{
             leds[k].g=dimm;
             leds[k].r=0;
           }
           toggle=0;
           }
         }else{
           for(int k=0 ; k< NUM_LEDS; k++){
             if(k&1){
               leds[k].g=dimm;
               leds[k].r=0;
             }else{
               leds[k].r=dimm;
               leds[k].g=0;
             }
           toggle=1;
         }
         }
  }else if(mode>=VALS_PER_MODES*2 && mode <VALS_PER_MODES*3){
	 memset(leds,0,13*13*3);
    if(toggle){
           for(int k=0 ; k< NUM_LEDS; k++){
           if(k&1){
             leds[k].r=0;
             leds[k].g=0;
           }else{
             leds[k].g=0;
             leds[k].r=dimm;
           }
           toggle=0;
           }
         }else{
           for(int k=0 ; k< NUM_LEDS; k++){
             if(k&1){
               leds[k].g=dimm;
               leds[k].r=0;
             }else{
               leds[k].r=0;
               leds[k].g=0;
             }
           toggle=1;
         }
         }
  }else if(mode>=VALS_PER_MODES*3 && mode <VALS_PER_MODES*4){
	 memset(leds,0,13*13*3);
    if(toggle){
      for(int k=0 ; k< NUM_LEDS; k++){
             
               leds[k].b=dimm;
               leds[k].r=0;
    }    
          
      toggle=0;
    }else{
    for(int k=0 ; k< NUM_LEDS; k++){
             
               leds[k].r=dimm;
               leds[k].b=0;
    }    
           toggle=1;
    }
  }else if(mode>=VALS_PER_MODES*4 && mode<VALS_PER_MODES*5 ){
  	 static int8_t uline=4, bar=5;
		
	 memset(leds,0,13*13*3);
	 for(uint8_t i=uline;i<uline+bar;i++){
		if(uline%4){
			leds[uline*WIDTH+i].r=dimm;
			leds[i*WIDTH+uline].r=dimm;
            if(uline!=6){
				leds[(uline+bar-1)*WIDTH+i].r=dimm;
				leds[i*WIDTH +uline+bar-1].r=dimm;
						  
				leds[(uline+1)*WIDTH+i].r=dimm;
				leds[i*WIDTH+uline+1].r=dimm;
				leds[(uline+bar-2)*WIDTH+i].r=dimm;
				leds[i*WIDTH +uline+bar-2].r=dimm;
            }
		}else{
			leds[uline*WIDTH+i].g=dimm;
			leds[i*WIDTH+uline].g=dimm;
			leds[(uline+bar-1)*WIDTH+i].g=dimm;
			leds[i*WIDTH +uline+bar-1].g=dimm;
                      
			leds[(uline+1)*WIDTH+i].g=dimm;
			leds[i*WIDTH+uline+1].g=dimm;
			leds[(uline+bar-2)*WIDTH+i].g=dimm;
			leds[i*WIDTH +uline+bar-2].g=dimm;
                       
		}
	 }
	bar+=4;
	uline-=2;
	if(uline<0){
	 	 uline=6;
	 	 bar=1;
	}
  }else if(mode>=VALS_PER_MODES*5){
		
  }

  
  FastSPI_LED.show();
  delay(analogRead(0));
  
 
}
