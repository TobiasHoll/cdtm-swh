#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Game settings
constexpr int CYCLE_LENGTH = 10000;
constexpr int FLASH_TOTAL  =  3500;
constexpr int FLASH_LENGTH =   500;
constexpr int TICK_USEC    =   750;

// Cup count
constexpr int CUPS = 10;

// Pins
constexpr int IRPINS[CUPS] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}; // IR sensor data pins
constexpr int LEDPIN       = 11;                              // LED strip data pin
constexpr int SEEDPIN      = A0;                              // Unoccupied analog pin used for random seed

// NeoPixel strip and utilities
constexpr int LEDS_PER_CUP = 3;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(CUPS * LEDS_PER_CUP, LEDPIN, NEO_GRB + NEO_KHZ800);

void setCupColor(int cup, int r, int g, int b)
{
  for (int i = (cup - 1) * LEDS_PER_CUP; i < cup * LEDS_PER_CUP; ++i)
    strip.setPixelColor(i, strip.Color(r, g, b));
}
void setAllColors(int r, int g, int b)
{
  for (int i = 0; i < CUPS * LEDS_PER_CUP; ++i)
    strip.setPixelColor(i, strip.Color(r, g, b));
}
void updateStrip()
{
  strip.setBrightness(255);
  strip.show();
}
void disableStrip()
{
  strip.setBrightness(0);
  strip.show();
}
void toggleStrip()
{
  strip.setBrightness(255 - strip.getBrightness());
  strip.show();
}

// Setup
void setup() {
  // Set up serial connection
  Serial.begin(9600);

  // Set up pin mode
  pinMode(LEDPIN, OUTPUT);
  for (int index = 0; index < CUPS; ++index)
    pinMode(IRPINS[index], INPUT);

  // Set up LED strip
  strip.begin();
  disableStrip();
}

// Main loop
void loop()
{
  if (digitalRead(4) || digitalRead(6) || digitalRead(7)) {
    setAllColors(0, 255, 0);
    updateStrip();
    delay(1000);
    setAllColors(0, 0, 0);
    updateStrip();
  } else if (digitalRead(10)) {
    setAllColors(255, 0, 0);
    updateStrip();
    delay(1000);
    setAllColors(0, 0, 0);
    updateStrip();
  } else {
    setAllColors(0, 0, 0);
    setCupColor(4, 0, 255, 255);
    setCupColor(6, 0, 255, 255);
    setCupColor(7, 0, 255, 255);
    updateStrip();
    delay(10);
  }
}
