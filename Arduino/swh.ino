#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Game settings
constexpr int TICK_MSEC    =     1;
constexpr int CYCLE_TICKS  =  4000;
constexpr int FLASH_COUNT  =     3;
constexpr int FLASH_TICKS  =   500;
constexpr int TARGET_CUPS  =     3;

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
  static int currentTargets[TARGET_CUPS] = {0, 0, 0};

  if (tickInCycle == 0)
  {
    // New cycle, select next target
    if (TARGET_CUPS == 1)
    {
      int nextTarget = currentTargets[0];
      while (nextTarget == currentTargets[0]) nextTarget = random(CUPS);
      currentTargets[0] = nextTarget;
    }
    else
    {
      for (int target = 0; target < TARGET_CUPS; ++target)
      {
retry:
        int nextTarget = random(CUPS);
        for (int other = 0; other < target; ++other)
          if (nextTarget == currentTargets[other])
            goto retry;
            
        currentTargets[target] = nextTarget;
      }
    }

    // Update cup colors
    setAllColors(255, 0, 0);
    for (int target = 0; target < TARGET_CUPS; ++target)
      setCupColor(currentTargets[target], 0, 255, 0);
    updateStrip();
  }

  // Check the IR sensors for events
  event evt = event::NONE;
  for (int index = 0; index < CUPS; ++index)
  {
    if (digitalRead(IRPINS[index]))
    {
      for (int target = 0; target < TARGET_CUPS; ++target)
      {
         if (index == currentTargets[target])
         {
           evt = event::FLASH_CORRECT;
           break;
         }
         else evt = event::FLASH_WRONG;
      }
      break;
    }
  }
  
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

