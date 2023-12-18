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

Stage 4 - Robot Base Movement Programming

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

Stage 5 - Connecting Servo to the Base & Servo Movement for the Window

<img src="https://github.com/whatisyourusername/Performing-Robots/blob/7624ed4fee2550eaf134d5c22e537781540254ac/Images/4.jpg" width = "250" height = "400">

Stage 6 - Building Sides

<img src="https://github.com/whatisyourusername/Performing-Robots/blob/7624ed4fee2550eaf134d5c22e537781540254ac/Images/5.jpg" width = "250" height = "400">

<img src="https://github.com/whatisyourusername/Performing-Robots/blob/7624ed4fee2550eaf134d5c22e537781540254ac/Images/6.jpg" width = "250" height = "400">

<img src="https://github.com/whatisyourusername/Performing-Robots/blob/7624ed4fee2550eaf134d5c22e537781540254ac/Images/7(1).jpg" width = "250" height = "400">

Stage 7 - Attaching NeoPixels & Programming

<img src="https://github.com/whatisyourusername/Performing-Robots/blob/7624ed4fee2550eaf134d5c22e537781540254ac/Images/7.jpg" width = "250" height = "400">

<img src="https://github.com/whatisyourusername/Performing-Robots/blob/7624ed4fee2550eaf134d5c22e537781540254ac/Images/IMG_1484.jpg" width = "250" height = "400">

Stage 8 - Decoration & Adding Details (Sounds editing)

<img src="https://github.com/whatisyourusername/Performing-Robots/blob/7624ed4fee2550eaf134d5c22e537781540254ac/Images/IMG_1840.jpg" width = "250" height = "350">
<img src="https://github.com/whatisyourusername/Performing-Robots/blob/7624ed4fee2550eaf134d5c22e537781540254ac/Images/IMG_1841.jpg" width = "250" height = "350">
<img src="https://github.com/whatisyourusername/Performing-Robots/blob/7624ed4fee2550eaf134d5c22e537781540254ac/Images/IMG_1842.jpg" width = "250" height = "350">
<img src="https://github.com/whatisyourusername/Performing-Robots/blob/7624ed4fee2550eaf134d5c22e537781540254ac/Images/11.jpg" width = "250" height = "200">

<img src="https://github.com/whatisyourusername/Performing-Robots/blob/7624ed4fee2550eaf134d5c22e537781540254ac/Images/12.jpg" width = "250" height = "200">
