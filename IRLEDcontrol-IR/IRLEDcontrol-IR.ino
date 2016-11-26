#include <IRremote.h>
#include <EEPROM.h>

#define STATE_ON          1
#define STATE_OFF         0

#define ADDR_COLOR_IS_STORED  0
#define ADDR_COLOR_ID         1
#define COLOR_IS_STORED       1

//                                Pin setup
int RECV_PIN =         2; 
int OUTPUT_RED =       9;
int OUTPUT_GREEN =     10;
int OUTPUT_BLUE =      11;

//                                IR codes
unsigned long CODE_RED =         551505585;
unsigned long CODE_GREEN =       551521905;
unsigned long CODE_YELLOW =      551536185;
unsigned long CODE_BLUE =        551519865;
unsigned long CODE_LONG_PRESS =  4294967295;

//                                Button association
#define BTN_TOGGLE            CODE_RED
#define BTN_COLOR_CHANGE      CODE_GREEN
#define BTN_BRIGHTNESS_CHANGE CODE_BLUE

//                                Brightness level definitions
unsigned char brightnessLevels[] = {
  20, // 0
  50, // 1
  80  // 2
};

#define MIN_BRIGHTNESS    0
#define MAX_BRIGHTNESS    2

/*
unsigned long CODE_RED    =     551505585;
unsigned long CODE_GREEN  =     551521905;
unsigned long CODE_YELLOW =     551536185;
unsigned long CODE_BLUE   =     551519865;
unsigned long CODE_LONG_PRESS = 4294967295;

unsigned long BTN_TOGGLE = CODE_RED;
unsigned long BTN_COLOR_CHANGE = CODE_GREEN;
unsigned long BTN_BRIGHTNESS_CHANGE = CODE_GREEN;
/**/

unsigned short state = STATE_OFF;

struct struct_color { 
    unsigned char r;
    unsigned char g;
    unsigned char b;
};
typedef struct struct_color color;


struct_color* colorArray;

#define COLOR_ID_WHITE    0

#define COLOR_COUNT     7

unsigned short current_color_id = 0;
color current_color = colorArray[current_color_id];

char currentBrightnessID = MAX_BRIGHTNESS;

IRrecv irrecv(RECV_PIN);
decode_results results;


void changeColor(unsigned short* id){
  (*id)++;
  if(*id >= COLOR_COUNT){
    (*id) = 0;
  }
  EEPROM.write(ADDR_COLOR_IS_STORED, COLOR_IS_STORED);
  EEPROM.write(ADDR_COLOR_ID, *id); 
}


void setColor(unsigned short* id) {
  current_color = colorArray[(*id)];
}


char changeBrightness() {
  currentBrightnessID++;
  if (currentBrightnessID > MAX_BRIGHTNESS) {
    currentBrightnessID = MIN_BRIGHTNESS;
  }
  // apply changed brighness
  setOutputs();
}


unsigned short getValueWithBrightness(int input) {
  int value = input * brightnessLevels[currentBrightnessID];
  value = (value / 100);
  return (value & 0xff);
}


void setOutputs() {
  if(STATE_OFF == state) {
              // Turn OFF
    digitalWrite(OUTPUT_RED,   LOW);  
    digitalWrite(OUTPUT_GREEN, LOW);  
    digitalWrite(OUTPUT_BLUE,  LOW); 
  } else {    // Turn ON
    setColor(&current_color_id);
    /*
    analogWrite(OUTPUT_RED,   getValueWithBrightness(current_color.r));
    analogWrite(OUTPUT_GREEN, getValueWithBrightness(current_color.g));
    analogWrite(OUTPUT_BLUE,  getValueWithBrightness(current_color.b));
    /**/
    analogWrite(OUTPUT_RED,   current_color.r);
    analogWrite(OUTPUT_GREEN, current_color.g);
    analogWrite(OUTPUT_BLUE,  current_color.b);
    /**/
  }
}



void setup(){

  (colorArray) = malloc(COLOR_COUNT*sizeof(color));
  colorArray[0] = {.r=255, .g=255, .b=255};  // 0 white
  colorArray[1] = {.r=255, .g=0  , .b=0  };  // 1 red
  colorArray[2] = {.r=255, .g=30 , .b=0  };  // 2 orange
  colorArray[3] = {.r=255, .g=70 , .b=0  };  // 3 yellow
  colorArray[4] = {.r=255, .g=0  , .b=255};  // 4 pink
  colorArray[5] = {.r=145, .g=0  , .b=255};  // 5 purple
  colorArray[6] = {.r=0  , .g=0  , .b=255};  // 6 blue
  
  irrecv.enableIRIn(); // start the receiver

  Serial.begin(9600);

  // set pinModes
  pinMode(13,   OUTPUT);
  pinMode(OUTPUT_RED,   OUTPUT);
  pinMode(OUTPUT_GREEN, OUTPUT);
  pinMode(OUTPUT_BLUE,  OUTPUT);

  // init start state
  if(EEPROM.read(ADDR_COLOR_IS_STORED) == COLOR_IS_STORED){
    current_color_id = EEPROM.read(ADDR_COLOR_ID);
  }else{
    current_color_id = COLOR_ID_WHITE;  
  }
  setColor(&current_color_id);
  currentBrightnessID = MAX_BRIGHTNESS;
  state = STATE_OFF; 

}



void loop() {
  if (irrecv.decode(&results)) {
    Serial.print("ColorID: ");
    Serial.print(current_color_id);
    Serial.print(" ");

    if (results.value == BTN_TOGGLE) { // need to turn ON or OFF
      results.value = 0;
      if (state == STATE_OFF) {
        state = STATE_ON;
      } else {
        state = STATE_OFF;
      }
      setOutputs();
    } 

    else

    if (results.value == BTN_COLOR_CHANGE) {
      //results.value = 0;
      if(state == STATE_ON){ // we need to change color only in ON state
        changeColor(&current_color_id);
        setColor(&current_color_id);
        setOutputs();
      }
    }

    else

    if (results.value == BTN_BRIGHTNESS_CHANGE) {
      results.value = 0;
      if(state == STATE_ON){ // we need to change brightness only in ON state
        changeBrightness();
      }
    }
    
     
    irrecv.resume();
  }

}
//este
