#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Game settings
constexpr int TICK_MSEC    =     1;
constexpr int CYCLE_TICKS  =  4000;
constexpr int FLASH_COUNT  =     3;
constexpr int FLASH_TICKS  =   500;

// Cup count
constexpr int CUPS = 10;

// Pins
constexpr int IRPINS[CUPS] = {2, 1, 4, 3, 6, 7, 5, 8, 9, 10}; // IR sensor data pins
constexpr int LEDPIN       = 11;                              // LED strip data pin
constexpr int MOTORPIN     = 12;                              // Motor pin
constexpr int DIRECTIONPIN = 13;                              // Motor direction pin
constexpr int SEEDPIN      = A0;                              // Unoccupied analog pin used for random seed

// Motor settings
constexpr int MOTOR_STEPS        = 1800;
constexpr int MOTOR_DELAY_US     = 300;
constexpr int DIRECTION_OUTWARDS = HIGH;
constexpr int DIRECTION_INWARDS  = LOW;

// NeoPixel strip and utilities
constexpr int LEDS_PER_CUP = 3;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(CUPS * LEDS_PER_CUP, LEDPIN, NEO_GRB + NEO_KHZ800);

void setCupColor(int cup, int r, int g, int b)
{
  for (int i = cup * LEDS_PER_CUP; i < (cup + 1) * LEDS_PER_CUP; ++i)
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

void flashBlock(int r, int g, int b)
{
  // Preserve original colors
  uint32_t originalColors[CUPS * LEDS_PER_CUP];
  for (int i = 0; i < CUPS * LEDS_PER_CUP; ++i)
    originalColors[i] = strip.getPixelColor(i);

  // Update all colors and flash
  for (int i = 0; i < FLASH_COUNT; ++i)
  {
    setAllColors(r, g, b);
    updateStrip();
    delay(TICK_MSEC * FLASH_TICKS);
    setAllColors(0, 0, 0);
    updateStrip();
    delay(TICK_MSEC * FLASH_TICKS);
  }

  // Restore colors
  for (int i = 0; i < CUPS * LEDS_PER_CUP; ++i)
    strip.setPixelColor(i, originalColors[i]);
  updateStrip();
}

void moveDispenser(int dir)
{
  for (int i = 0; i < MOTOR_STEPS; ++i)
  {
    digitalWrite(DIRECTIONPIN, dir);
    digitalWrite(MOTORPIN, HIGH);
    delayMicroseconds(MOTOR_DELAY_US);
    digitalWrite(MOTORPIN, LOW);
    delayMicroseconds(MOTOR_DELAY_US);
  }
}

void dispense()
{
  moveDispenser(DIRECTION_OUTWARDS);
  moveDispenser(DIRECTION_INWARDS);
}

// Data types
enum class event
{
  NONE,
  FLASH_WRONG,
  FLASH_CORRECT,
};

// Setup
void setup() {
  // Set up serial connection
  Serial.begin(9600);

  // Set up pin mode
  pinMode(LEDPIN, OUTPUT);
  pinMode(MOTORPIN, OUTPUT);
  pinMode(DIRECTIONPIN, OUTPUT);
  for (int index = 0; index < CUPS; ++index)
    pinMode(IRPINS[index], INPUT);

  // Set up LED strip
  strip.begin();
  disableStrip();

  // Seed RNG
  randomSeed(analogRead(SEEDPIN));
}

// Main loop
void loop()
{
  static int tickInCycle = 0;
  static int currentTarget = 0;

  if (tickInCycle == 0)
  {
    // New cycle, select next target
    int nextTarget = currentTarget;
    while (nextTarget == currentTarget) nextTarget = random(CUPS);
    currentTarget = nextTarget;

    // Update cup colors
    setAllColors(255, 0, 0);
    setCupColor(currentTarget, 0, 255, 0);
    updateStrip();
  }
  
  event evt = event::NONE;
  for (int index = 0; index < CUPS; ++index)
    if (digitalRead(IRPINS[index]))
      evt = (index == currentTarget) ? event::FLASH_CORRECT : event::FLASH_WRONG;

  switch (evt)
  {
    case event::FLASH_CORRECT:
      flashBlock(0, 255, 0);
      setAllColors(0, 255, 0);
      updateStrip();
      dispense();
      tickInCycle = CYCLE_TICKS - 1;
      break;
    case event::FLASH_WRONG:
      flashBlock(255, 0, 0);
      tickInCycle = CYCLE_TICKS - 1;
      break; 
  }
  
  // Tick.
  tickInCycle = (tickInCycle + 1) % CYCLE_TICKS;
  delay(TICK_MSEC);
}

