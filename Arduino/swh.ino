#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Pins
#define CUPS 10
constexpr int IRPINS[CUPS] = {2, 1, 4, 3, 6, 7, 5, 8, 9, 10}; // IR sensor data pins

// Sensor layout is OUT IRPIN - GND - VCC 5V  

// Turn off Neopixel
#define LEDPIN 11
#define PIXELS (3 * CUPS)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXELS, LEDPIN, NEO_GRB + NEO_KHZ800);
void setCupColor(int cup, int r, int g, int b)
{
  for (int i = (cup - 1) * 3; i < cup * 3; ++i)
    strip.setPixelColor(i, strip.Color(r, g, b));
}
void setAllColors(int r, int g, int b)
{
  for (int i = 0; i < CUPS * 3; ++i)
    strip.setPixelColor(i, strip.Color(r, g, b));
}
void disableStrip()
{
  strip.setBrightness(0);
  strip.show();
}
void updateStrip()
{
  strip.setBrightness(255);
  strip.show();
}

// Set up input pins, turn off LEDs
void setup() {
  Serial.begin(9600);

  strip.begin();
  strip.setBrightness(0);
  strip.show(); // Initialize all pixels to 'off'

  for (int i = 0; i < CUPS; ++i) pinMode(IRPINS[i], INPUT);
}

// The main loop
int x = 0;
void loop()
{
  // Enable celebration mode if the sensor triggers
  for (int i = 0; i < CUPS; ++i)
  {
    int ir = digitalRead(IRPINS[i]);
    if (ir) {
      Serial.print(x++ );Serial.print(": cup "); Serial.print(i + 1); Serial.print(" at pin "); Serial.println(IRPINS[i]);
      setCupColor(i + 1, 0, 255, 0);
      updateStrip();
      delay(200);
      setAllColors(0, 0, 0);
      updateStrip();
    }
  }
}
