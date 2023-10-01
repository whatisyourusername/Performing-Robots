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
#define Motor12 5
#define Motor21 9
#define Motor22 6

#define RC_CH1_PIN  8
#define RC_CH2_PIN  7
#define RC_CH3_PIN  4
#define RC_CH4_PIN  2

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

  Serial.print("CH1:"); Serial.print(rc_values[RC_CH1]); Serial.print("\t");
  // left wheel control
  if (rc_values[RC_CH1] > 1700){
    digitalWrite(Motor11, HIGH);
    digitalWrite(Motor12, LOW);
  }
  else if (rc_values[RC_CH1] < 1300){
    digitalWrite(Motor11, LOW);
    digitalWrite(Motor12, HIGH);
  }
  else {
    digitalWrite(Motor11, LOW);
    digitalWrite(Motor12, LOW);
  }

  Serial.print("CH3:"); Serial.print(rc_values[RC_CH3]); Serial.print("\t");
  // right wheel control
  if (rc_values[RC_CH3] > 1700){
    digitalWrite(Motor21, HIGH);
    digitalWrite(Motor22, LOW);
  }
  else if (rc_values[RC_CH3] < 1300){
    digitalWrite(Motor21, LOW);
    digitalWrite(Motor22, HIGH);
  }
  else {
    digitalWrite(Motor21, LOW);
    digitalWrite(Motor22, LOW);
  }
  
  Serial.print("CH2:"); Serial.print(rc_values[RC_CH2]); Serial.print("\t");
  // forward and backward
  if (rc_values[RC_CH2] > 1700){
    digitalWrite(Motor11, HIGH);
    digitalWrite(Motor12, LOW);
    digitalWrite(Motor21, HIGH);
    digitalWrite(Motor22, LOW);
  }
  else if (rc_values[RC_CH2] < 1300){
    digitalWrite(Motor11, LOW);
    digitalWrite(Motor12, HIGH);
    digitalWrite(Motor21, LOW);
    digitalWrite(Motor22, HIGH);
  }/*
  else {
    digitalWrite(Motor11, LOW);
    digitalWrite(Motor12, LOW);
    digitalWrite(Motor21, LOW);
    digitalWrite(Motor22, LOW);
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
