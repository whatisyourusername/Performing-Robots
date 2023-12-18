# Performing-Robots

#Yunho & Akhat Robot [Cody] Journal

##Stage 1 - Basic Robot Design

<img src="https://github.com/whatisyourusername/Performing-Robots/blob/7624ed4fee2550eaf134d5c22e537781540254ac/Images/BMO.png" width = "270" height = "250">
**Draft of the outlook of the robot**

##Stage 2 - Planning Robot Functionalities

<img src="https://github.com/whatisyourusername/Performing-Robots/blob/7624ed4fee2550eaf134d5c22e537781540254ac/Images/2.jpg" width = "250" height = "300">

A robot that has a window that can showoff the inner fancy design. Slightly arrogant, but also highly related to his job as a youtuber.

<img src="https://github.com/whatisyourusername/Performing-Robots/blob/7624ed4fee2550eaf134d5c22e537781540254ac/Images/1.jpg" width = "250" height = "300">

Possible way of displaying face using LEDs as pixels. Later implemented into Neopixels.

##Stage 3 - Robot Base Build

<img src="https://github.com/whatisyourusername/Performing-Robots/blob/7624ed4fee2550eaf134d5c22e537781540254ac/Images/bottom.jpg" width = "300" height = "250">
<img src="https://github.com/whatisyourusername/Performing-Robots/blob/7624ed4fee2550eaf134d5c22e537781540254ac/Images/top.jpg" width = "300" height = "250">

We concentrated on the balance of the wheels throughout the whole robot base. The front and back wheel has different height so that they gain better movements when the robot is moving on a bumpy land. However, the difference of height is minimize to make sure the robot does not fall down by the momentum of the movement after it is fully built.

<img src="https://github.com/whatisyourusername/Performing-Robots/blob/7624ed4fee2550eaf134d5c22e537781540254ac/Images/Soldering.jpg" width = "250" height = "400">
**Image of a soldering done to connect motor wires.**

##Stage 4 - Robot Base Movement Programming

[Motor Movement Test Video](https://github.com/whatisyourusername/Performing-Robots/blob/7624ed4fee2550eaf134d5c22e537781540254ac/Videos/MotorMovementTest.mp4)

[Motor Movement with Scenario](https://drive.google.com/file/d/1jXIbXLM76nJtqLb-dmAJM6ExaX0xkn9a/view?usp=sharing)

[Motor Movement RC Demo](https://drive.google.com/file/d/1oGkHQ82NQxOIcYpWesVlAkgKT5NSXC0F/view?usp=sharing)

Code for the RC_Demo

```
// install this library from the library manager
// by Mike "GreyGnome" Schwager
#include <EnableInterrupt.h>

#define SERIAL_PORT_SPEED 9600
#define RC_NUM_CHANNELS  4

#define RC_CH1  0
#define RC_CH2  1
#define RC_CH3  2
#define RC_CH4  3

#define Motor11 3
#define Motor12 6
#define Motor21 9
#define Motor22 10

#define RC_CH1_PIN  8
#define RC_CH2_PIN  7
#define RC_CH3_PIN  4
#define RC_CH4_PIN  2

int power = 255;

uint16_t rc_values[RC_NUM_CHANNELS];
uint32_t rc_start[RC_NUM_CHANNELS];
volatile uint16_t rc_shared[RC_NUM_CHANNELS];

void rc_read_values() {
  noInterrupts();
  memcpy(rc_values, (const void *)rc_shared, sizeof(rc_shared));
  interrupts();
}

void calc_input(uint8_t channel, uint8_t input_pin) {
  if (digitalRead(input_pin) == HIGH) {
    rc_start[channel] = micros();
  } else {
    uint16_t rc_compare = (uint16_t)(micros() - rc_start[channel]);
    rc_shared[channel] = rc_compare;
  }
}

void calc_ch1() {
  calc_input(RC_CH1, RC_CH1_PIN);
}
void calc_ch2() {
  calc_input(RC_CH2, RC_CH2_PIN);
}
void calc_ch3() {
  calc_input(RC_CH3, RC_CH3_PIN);
}
void calc_ch4() {
  calc_input(RC_CH4, RC_CH4_PIN);
}

void setup() {
  Serial.begin(SERIAL_PORT_SPEED);

  pinMode(RC_CH1_PIN, INPUT);
  pinMode(RC_CH2_PIN, INPUT);
  pinMode(RC_CH3_PIN, INPUT);
  pinMode(RC_CH4_PIN, INPUT);
  
  pinMode(Motor11, OUTPUT);
  pinMode(Motor12, OUTPUT);
  pinMode(Motor21, OUTPUT);
  pinMode(Motor22, OUTPUT);

  enableInterrupt(RC_CH1_PIN, calc_ch1, CHANGE);
  enableInterrupt(RC_CH2_PIN, calc_ch2, CHANGE);
  enableInterrupt(RC_CH3_PIN, calc_ch3, CHANGE);
  enableInterrupt(RC_CH4_PIN, calc_ch4, CHANGE);
}

void loop() {
  rc_read_values();
  
  Serial.print("CH3:"); Serial.print(rc_values[RC_CH3]); Serial.print("\t");
  power = (rc_values[RC_CH3] - 1000)/4.24;
  if (power < 70) {
    power = 0;
  }

  Serial.print("CH1:"); Serial.print(rc_values[RC_CH1]); Serial.print("\t");
  // left wheel control
  if (rc_values[RC_CH1] > 1700){
    analogWrite(Motor11, power);
    analogWrite(Motor12, LOW);
    analogWrite(Motor21, LOW);
    analogWrite(Motor22, power);
  }
  else if (rc_values[RC_CH1] < 1300){
    analogWrite(Motor11, LOW);
    analogWrite(Motor12, power);
    analogWrite(Motor21, power);
    analogWrite(Motor22, LOW);
  }
  else {
    analogWrite(Motor11, LOW);
    analogWrite(Motor12, LOW);
    analogWrite(Motor21, LOW);
    analogWrite(Motor22, LOW);
  }

  
  Serial.print("CH2:"); Serial.print(rc_values[RC_CH2]); Serial.print("\t");
  // forward and backward
  if (rc_values[RC_CH2] > 1700){
    analogWrite(Motor11, power);
    analogWrite(Motor12, LOW);
    analogWrite(Motor21, power);
    analogWrite(Motor22, LOW);
  }
  else if (rc_values[RC_CH2] < 1300){
    analogWrite(Motor11, LOW);
    analogWrite(Motor12, power);
    analogWrite(Motor21, LOW);
    analogWrite(Motor22, power);
  }/*
  else {
    analogWrite(Motor11, LOW);
    analogWrite(Motor12, LOW);
    analogWrite(Motor21, LOW);
    analogWrite(Motor22, LOW);
  }*/
  
  Serial.print("CH4:"); Serial.println(rc_values[RC_CH4]);
  // brake
  
  if (rc_values[RC_CH4] > 2000){
    digitalWrite(Motor11, LOW);
    digitalWrite(Motor12, LOW);
    digitalWrite(Motor21, LOW);
    digitalWrite(Motor22, LOW);
  }
  
  delay(200);
}
```

##Stage 5 - Connecting Servo to the Base

<img src="https://github.com/whatisyourusername/Performing-Robots/blob/7624ed4fee2550eaf134d5c22e537781540254ac/Images/4.jpg" width = "250" height = "400">

##Stage 6 - Building Sides

<img src="https://github.com/whatisyourusername/Performing-Robots/blob/7624ed4fee2550eaf134d5c22e537781540254ac/Images/7(3).jpg" width = "250" height = "400">
Used Several L-Shape Brackets to gain a rigit connection between three sides and the base.

<img src="https://github.com/whatisyourusername/Performing-Robots/blob/7624ed4fee2550eaf134d5c22e537781540254ac/Images/5.jpg" width = "250" height = "400">
Hole in the front wood for the windows.

<img src="https://github.com/whatisyourusername/Performing-Robots/blob/7624ed4fee2550eaf134d5c22e537781540254ac/Images/6.jpg" width = "250" height = "400">

<img src="https://github.com/whatisyourusername/Performing-Robots/blob/7624ed4fee2550eaf134d5c22e537781540254ac/Images/7(1).jpg" width = "250" height = "400">

##Stage 7 - Attaching NeoPixels & Servo and Neopixels Programming

<img src="https://github.com/whatisyourusername/Performing-Robots/blob/7624ed4fee2550eaf134d5c22e537781540254ac/Images/7.jpg" width = "250" height = "400">

<img src="https://github.com/whatisyourusername/Performing-Robots/blob/7624ed4fee2550eaf134d5c22e537781540254ac/Images/IMG_1484.jpg" width = "250" height = "400">

###Code for Servo & Neopixels Demo
```
// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// Released under the GPLv3 license to match the rest of the
// Adafruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>
#include <Servo.h>

#define ServoPIN 20

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        21 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 64 // Popular NeoPixel ring size

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 100 // Time (in milliseconds) to pause between pixels

// These are the pins used for the breakout example
#define BREAKOUT_RESET  9      // VS1053 reset pin (output)
#define BREAKOUT_CS     10     // VS1053 chip select pin (output)
#define BREAKOUT_DCS    8      // VS1053 Data/command select pin (output)
// These are the pins used for the music maker shield
#define SHIELD_RESET  -1      // VS1053 reset pin (unused!)
#define SHIELD_CS     7      // VS1053 chip select pin (output)
#define SHIELD_DCS    6      // VS1053 Data/command select pin (output)

// These are common pins between breakout and shield
#define CARDCS 4     // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ 3       // VS1053 Data request, ideally an Interrupt pin

Adafruit_VS1053_FilePlayer musicPlayer =
  // create breakout-example object!
  //Adafruit_VS1053_FilePlayer(BREAKOUT_RESET, BREAKOUT_CS, BREAKOUT_DCS, DREQ, CARDCS);
  // create shield-example object!
  Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);

// Servo Variable
Servo myServo;


void setup() {
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)

  Serial.begin(9600);
  Serial.println("Adafruit VS1053 Simple Test");

  if (! musicPlayer.begin()) { // initialise the music player
    Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
    while (1);
  }
  Serial.println(F("VS1053 found"));

  if (!SD.begin(CARDCS)) {
    Serial.println(F("SD failed, or not present"));
    while (1);  // don't do anything more
  }

  // list files
  printDirectory(SD.open("/"), 0);

  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(20, 20);

  // Timer interrupts are not suggested, better to use DREQ interrupt!
  //musicPlayer.useInterrupt(VS1053_FILEPLAYER_TIMER0_INT); // timer int

  // If DREQ is on an interrupt pin (on uno, #2 or #3) we can do background
  // audio playing
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int

  // ServoSetting
  myServo.attach(ServoPIN);
}

void loop() {
  // First Face
  pixels.clear(); // Set all pixel colors to 'off'
  // smile
  // eyebrows
  pixels.setPixelColor(0, pixels.Color(0, 150, 0));
  pixels.setPixelColor(1, pixels.Color(0, 150, 0));
  pixels.setPixelColor(10, pixels.Color(0, 150, 0));
  pixels.setPixelColor(6, pixels.Color(0, 150, 0));
  pixels.setPixelColor(7, pixels.Color(0, 150, 0));
  pixels.setPixelColor(13, pixels.Color(0, 150, 0));
  // eyes
  pixels.setPixelColor(17, pixels.Color(0, 150, 0));
  pixels.setPixelColor(18, pixels.Color(0, 150, 0));
  pixels.setPixelColor(21, pixels.Color(0, 150, 0));
  pixels.setPixelColor(22, pixels.Color(0, 150, 0));
  pixels.setPixelColor(25, pixels.Color(0, 150, 0));
  pixels.setPixelColor(26, pixels.Color(0, 150, 0));
  pixels.setPixelColor(29, pixels.Color(0, 150, 0));
  pixels.setPixelColor(30, pixels.Color(0, 150, 0));
  // mouth
  pixels.setPixelColor(41, pixels.Color(0, 150, 0));
  pixels.setPixelColor(50, pixels.Color(0, 150, 0));
  pixels.setPixelColor(51, pixels.Color(0, 150, 0));
  pixels.setPixelColor(52, pixels.Color(0, 150, 0));
  pixels.setPixelColor(53, pixels.Color(0, 150, 0));
  pixels.setPixelColor(46, pixels.Color(0, 150, 0));

  pixels.show();
  Serial.println(F("Playing track 002"));
  musicPlayer.playFullFile("/track002.mp3");

  myServo.write(90);

  delay(2000);
  // Second Face
  // SUS
  pixels.clear(); // Set all pixel colors to 'off'
  // eyebrows
  pixels.setPixelColor(0, pixels.Color(50, 50, 0));
  pixels.setPixelColor(1, pixels.Color(50, 50, 0));
  pixels.setPixelColor(6, pixels.Color(50, 50, 0));
  pixels.setPixelColor(7, pixels.Color(50, 50, 0));
  pixels.setPixelColor(10, pixels.Color(50, 50, 0));
  pixels.setPixelColor(13, pixels.Color(50, 50, 0));
  // eyes
  pixels.setPixelColor(24, pixels.Color(50, 50, 0));
  pixels.setPixelColor(31, pixels.Color(50, 50, 0));
  pixels.setPixelColor(25, pixels.Color(50, 50, 0));
  pixels.setPixelColor(26, pixels.Color(50, 50, 0));
  pixels.setPixelColor(29, pixels.Color(50, 50, 0));
  pixels.setPixelColor(30, pixels.Color(50, 50, 0));
  // mouth
  pixels.setPixelColor(40, pixels.Color(50, 50, 0));
  pixels.setPixelColor(41, pixels.Color(50, 50, 0));
  pixels.setPixelColor(42, pixels.Color(50, 50, 0));
  pixels.setPixelColor(43, pixels.Color(50, 50, 0));
  pixels.setPixelColor(44, pixels.Color(50, 50, 0));
  pixels.setPixelColor(45, pixels.Color(50, 50, 0));
  pixels.setPixelColor(46, pixels.Color(50, 50, 0));
  pixels.setPixelColor(47, pixels.Color(50, 50, 0));
  pixels.setPixelColor(49, pixels.Color(50, 50, 0));
  pixels.setPixelColor(54, pixels.Color(50, 50, 0));
  pixels.setPixelColor(58, pixels.Color(50, 50, 0));
  pixels.setPixelColor(59, pixels.Color(50, 50, 0));
  pixels.setPixelColor(60, pixels.Color(50, 50, 0));
  pixels.setPixelColor(61, pixels.Color(50, 50, 0));

  pixels.show();
  Serial.println(F("Playing track 001"));
  musicPlayer.playFullFile("/track001.mp3");

  myServo.write(50);

  delay(2000);

  // Third Face
  pixels.clear(); // Set all pixel colors to 'off'

  pixels.setPixelColor(0, pixels.Color(150, 0, 0));
  pixels.setPixelColor(1, pixels.Color(150, 0, 0));
  pixels.setPixelColor(6, pixels.Color(150, 0, 0));
  pixels.setPixelColor(7, pixels.Color(150, 0, 0));
  pixels.setPixelColor(10, pixels.Color(150, 0, 0));
  pixels.setPixelColor(13, pixels.Color(150, 0, 0));
  pixels.setPixelColor(17, pixels.Color(150, 0, 0));
  pixels.setPixelColor(18, pixels.Color(150, 0, 0));
  pixels.setPixelColor(21, pixels.Color(150, 0, 0));
  pixels.setPixelColor(22, pixels.Color(150, 0, 0));
  pixels.setPixelColor(25, pixels.Color(150, 0, 0));
  pixels.setPixelColor(26, pixels.Color(150, 0, 0));
  pixels.setPixelColor(29, pixels.Color(150, 0, 0));
  pixels.setPixelColor(30, pixels.Color(150, 0, 0));

  pixels.setPixelColor(40, pixels.Color(150, 0, 0));
  pixels.setPixelColor(47, pixels.Color(150, 0, 0));
  pixels.setPixelColor(49, pixels.Color(150, 0, 0));
  pixels.setPixelColor(50, pixels.Color(150, 0, 0));
  pixels.setPixelColor(51, pixels.Color(150, 0, 0));
  pixels.setPixelColor(52, pixels.Color(150, 0, 0));
  pixels.setPixelColor(53, pixels.Color(150, 0, 0));
  pixels.setPixelColor(54, pixels.Color(150, 0, 0));

  pixels.setPixelColor(58, pixels.Color(150, 0, 0));
  pixels.setPixelColor(59, pixels.Color(150, 0, 0));
  pixels.setPixelColor(60, pixels.Color(150, 0, 0));
  pixels.setPixelColor(61, pixels.Color(150, 0, 0));

  pixels.show(); // Pause before next pass through loop

  Serial.println(F("Playing track 003"));
  musicPlayer.playFullFile("/track003.mp3");

  myServo.write(90);

  delay(2000);
  pixels.clear();
  pixels.show();
  while(true){
    // do nothing
  }
}

/// File listing helper
void printDirectory(File dir, int numTabs) {
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      //Serial.println("**nomorefiles**");
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}
```

###Code for NeoPixel & Servo + Sound States for the Final Show Can be Found
[here](https://github.com/akhatsuleimenov/Performing-Robots-Fall-2023/tree/2af7961d39b709f7926e11102dbb450ea78b6f01/code)


Stage 8 - Decoration & Adding Details (Sounds editing)

<img src="https://github.com/whatisyourusername/Performing-Robots/blob/7624ed4fee2550eaf134d5c22e537781540254ac/Images/IMG_1840.jpg" width = "250" height = "350">
<img src="https://github.com/whatisyourusername/Performing-Robots/blob/7624ed4fee2550eaf134d5c22e537781540254ac/Images/IMG_1841.jpg" width = "250" height = "350">
<img src="https://github.com/whatisyourusername/Performing-Robots/blob/7624ed4fee2550eaf134d5c22e537781540254ac/Images/IMG_1842.jpg" width = "250" height = "350">
<img src="https://github.com/whatisyourusername/Performing-Robots/blob/7624ed4fee2550eaf134d5c22e537781540254ac/Images/11.jpg" width = "250" height = "200">

<img src="https://github.com/whatisyourusername/Performing-Robots/blob/7624ed4fee2550eaf134d5c22e537781540254ac/Images/12.jpg" width = "250" height = "200">
