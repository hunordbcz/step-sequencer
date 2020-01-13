# step-sequencer

  Step Sequencer with IR Remote Control - Adjustable Low/High Frequency, Volume and Speed - With 4 Potentiometers

## Parts

  4 * 100K Potentiometer<br>
  2 * 10K Potentiometer<br>
  1 Low power consumption speaker ( So it won't require other power source than from the arduino )<br>
  1 IR Receiver Sensor<br>
  4 Leds + Resistors<br>
  
  
## Requirements

  Soldering<br>
  [IRremote library](https://www.arduinolibraries.info/libraries/i-rremote)<br>
  **Important:**<br>
    In order to make the IRReceiver work with the tone() function, you have to change the following 2 lines in boarddefs.h:
    
    From:
    
    ```
    // Arduino Duemilanove, Diecimila, LilyPad, Mini, Fio, Nano, etc
    // ATmega48, ATmega88, ATmega168, ATmega328
    // #define IR_USE_TIMER1   // tx = pin 9
    #define IR_USE_TIMER2     // tx = pin 3
    ```
    
    To:
    
     ```
    // Arduino Duemilanove, Diecimila, LilyPad, Mini, Fio, Nano, etc
    // ATmega48, ATmega88, ATmega168, ATmega328
    #define IR_USE_TIMER1   // tx = pin 9
    //	#define IR_USE_TIMER2     // tx = pin 3
    ```
 
## Inspired from

  https://cassiopeia.hk/sequencer/
