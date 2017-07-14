# Interactive Beerpong Table (Single Player)

An interactive beerpong game that allows to play beerpong in an arcade game style.

## Description
The interactive beerpong table allows a player to play beerpong (without actual beer) in a normal beerpong set-up. The game is for single players (or players taking turns), because only one side of the beerpong table is built.
A green light within the cups indicates where the player needs to hit. If he succeeds, an alcohol shot is released automatically from the dispenser. Different game logics can be programmed in order to have different levels of difficulties.

## Gameplay

3 cups are lit up green and 7 are lit up red. The cups colours change every 4 secods to make it more difficult. If you hit a green cup, a shot is dispensed. If not you can throw again (3 balls normally)

<p align="center"><a href="https://www.youtube.com/watch?v=cWcqSVTLIn0">
  <img src="https://img.youtube.com/vi/cWcqSVTLIn0/0.jpg" title="Beerpong-machine" width="640" />
</a></p>

## Components and where to get them
### Beerpong Box
* Hardware
    * [Arduino](https://store.arduino.cc/arduino-leonardo-with-headers)
    * [Additional 5V Power source](https://www.sparkfun.com/products/12977)
    * [Breadboard](https://www.sparkfun.com/products/12615)
    * [LEDs to light up the cups](http://www.ebay.com/itm/10-1000pcs-WS2812B-5050-RGB-LED-no-PCB-Board-1-LED-Module-Pixel-Light-5V-/221989107317?var=&hash=item33af94f675:m:m_MtWTkA9QfpJUMVl3_-mZQ)
    * [Infra-red sensor for ball detection](https://www.ebay.com/p/10pcs-IR-Infrared-Obstacle-Avoidance-Sensor-Module-for-Arduino-Smart-Car-Robot/1149951524?iid=322371790311)
    * [A lot of wires](https://www.conrad.de/de/raspberry-pi-verbindungskabel-rb-cb1-25-1192229.html?gclid=Cj0KEQjwkZfLBRCzg-69tJy84N8BEiQAffAwqhcOIRJZI9cYf5_AMAyMzOXRfxqoOpha5KWjcE8c7AQaAp4F8P8HAQ&insert_kz=VQ&ef_id=V31VEgAAAPxQoMuU:20170712123835:s)
* Material
    * Wooden panels (2x 45cm*40cm, 1x 40cm*40c)
    * Additional Power Source for the Stepper (depends on the stepper, we used an old ac-adapter)
    
* Material
    * Wooden panels (2x 45cm\*40cm, 1x 40cm\*40cm, 1x 39cm\*40cm)
    * Wooden blocks (4x 5cm\*5cm\*15cm)
    * Lego Technics (check out the video)

## Assembly of the Beerpong-box

Check out the [Youtube video](https://www.youtube.com/watch?v=cWcqSVTLIn0) to see how it's built. This should provide you with enough details, so you can reproduce it.

### Cups (repeat this 10 times) 
* Wire up 3 LEDS per cup
* Remove the bottom of the cup and make 3 holes for the LEDs into it (We used a hot needle)
* Attach the LEDs with hot glue
* Remove the head and the bottom from one bottle
* Make a hole into the side of the bottle (for the sensor)
* Attach the bottle to the bottom of the cup with hot glue
* Attach the IR-Sensor to hole in the bottle (again with hot glue)
In the end it should look like this:

### Box

Our box is 45cm\*40cm\*40cm big, which means you need at least 4 pieces of wood. 

* Build a wooden box with only 4 sides (top and three walls), so you can access the cups easier
* Make holes for the cups into the top of the box. We used a holesaw for that.
* For the door simply attach a wooden panel (slightly smaller than the sides of the box) with a hinge to the box

### Wiring it up
* Put all the cups into the holes in the box
* Connect all the LED-wires, so they are in a row (ends can hang loose)
* Connect the beginning of the LED-wire-row to the Arduino
* Connect all the Power and Ground wires from the IR-Sensors with each other so they're also all in a row
* Attach the Power and Ground beginnings of the Sensor Wiring to your additional power source (we used an USB-cable for that)
* Connect the IR-Data Pin with the Arduino (we used the breadboard for this)
* Connect the ground of the additional power source with the ground of the arduino
* Attach the arduino to the wooden box (together with the breadboard)

Important! Secure the connections of all the wires and attach them to the box if possible.

## Assembly of the Dispenser

### Dispenser
1. Bottle container
* Build  wooden tower which has inner dimensions of the bottles you want to dispense
* A the bottom of the towerm cut out some wood (height of one bottle) on two sides
* Attach the tower to a wooden plate

### Bottle-pushing Mechanism
This one is a bit tricky and really depends on the stepper motor you are using, the height of your bottle and the thinkness of the wood you're using. So here are important aspects to make it work as smooth as possible:
* The bottle-pusher should have the height of one bottle
* The rack needs to be long enough, so the pusher can move forward until the end of the tower and back to the beginning
* The rack should run as smooth as possible, which really depends on the height of the pusher
* The connection of the stepper and the Lego parts is done with a standard [Lego piece (hole in the middle)](http://img.brickowl.com/files/image_cache/larger/lego-yellow-technic-brick-1-x-2-with-hole-3700-30-64503-93.jpg) and some pressure and hot glue 

### Wiring it up

We used an EasyDriver v4.4 to control the stepper motor. To provide enough power to the motor, we wired up a separate power source to the EasyDriver's PWR IN ports. Then, connect the motor to the board and the three STEP pins to the Arduino - make sure that you have enough cable to connect the EasyDriver (which will be part of your dispenser) to the Arduino (which is mounted in the wooden box alongside the cups).

Here, you can see an overview of the wiring. Make sure that the pins for the IR sensors align with the order in which the cups' LEDs are connected - otherwise, adapt the `IRPINS` array in `swh.ino` so that `IRPINS[0]` refers to the pin used for the first cup and so on:

![Wiring diagram](https://raw.githubusercontent.com/cdtm-swh/cdtm-swh/master/wiring.png)

## Software
You just need the [Arduino IDE](https://www.arduino.cc/en/main/software) and the [Adafruit Neopixel Library](https://github.com/adafruit/Adafruit_NeoPixel). After that, just load the `swh.ino` sketch and you're good to go.

## Authors

Tobi, Lisa, Maxim, and Flo; see also the [list of contributors](https://github.com/cdtm-swh/cdtm-swh/graphs/contributors) who participated in this project.

