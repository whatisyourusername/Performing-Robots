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
