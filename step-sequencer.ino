#include <IRremote.h>

const int RECV_PIN = 8;   //  IR pin
IRrecv irrecv(RECV_PIN);  // Initialize IR receiver
decode_results results;

// Declaration of variables
const byte speaker = 13;  //  Speaker pin
const byte pat0 = 4;      //  Led pins
const byte pat1 = 5;
const byte pat2 = 6;
const byte pat3 = 7;

bool debug = false; // Used to print values to Serial Monitor
bool isOn = false;  // Checks if the device should work or not
long counter = 0;   // Used for speed
long tempo;         // Used for reading the speed
int pattern = 0;    // Current potentiometer
int lowFreq = 0;    // The lower frequency boundary
int highFreq = 2500;// The higher frequency boundary
int newValue;       // Used to change freq. boundary more easily
unsigned int note;  // Note that will be played

void switchLEDs(int pattern) {  // Turn on and off the leds for specific patterns
  switch (pattern) {
    case 0:
      digitalWrite(pat3, LOW);
      digitalWrite(pat0, HIGH);
      break;
    case 1:
      digitalWrite(pat0, LOW);
      digitalWrite(pat1, HIGH);
      break;
    case 2:
      digitalWrite(pat1, LOW);
      digitalWrite(pat2, HIGH);
      break;
    case 3:
      digitalWrite(pat2, LOW);
      digitalWrite(pat3, HIGH);
      break;
  }
}

int getNote(int pattern) {  //  Get notes for specific patterns
  if (pattern == 0) {
    note = map(analogRead(A0), 0, 1023, lowFreq, highFreq);
  } else if (pattern == 1) {
    note = map(analogRead(A1), 0, 1023, lowFreq, highFreq);
  } else if (pattern == 2) {
    note = map(analogRead(A2), 0, 1023, lowFreq, highFreq);
  } else if (pattern == 3) {
    note = map(analogRead(A3), 0, 1023, lowFreq, highFreq);
  }
  return note;
}

void configure(long value) {
  switch (value) {
    case 0xFF02FD:  //+         Increment the highFreq boundary
      if (debug) {
        Serial.println("+");
      }
      newValue = highFreq + 100;
      if (newValue <= 5000) {
        highFreq = newValue;
      }
      break;
    case 0xFF9867:  //-         Decrement the highFreq boundary
      if (debug) {
        Serial.println("-");
      }
      newValue = highFreq - 100;
      if (newValue > lowFreq ) {
        highFreq = newValue;
      }
      break;
    case 0xFF906F:  //Forward   Increment the lowFreq boundary
      if (debug) {
        Serial.println("Forward");
      }
      newValue = lowFreq + 100;
      if (newValue < highFreq) {
        lowFreq = newValue;
      }
      break;
    case 0xFFE01F:  //Backward  Decrement the lowFreq boundary
      if (debug) {
        Serial.println("Backward");
      }
      newValue = lowFreq - 100;
      if (newValue >= 0) {
        lowFreq = newValue;
      }
      break;
    case 0xFFC23D:  //Return    Reset the boundaries
      if (debug) {
        Serial.println("Return");
      }
      lowFreq = 0;
      highFreq = 2500;
      break;
    case 0xFFA25D:  //Power     Stop/Start
      if (debug) {
        Serial.println("Power");
      }
      isOn = !isOn;
      break;
  }
}

void setup() {
  if (debug) {
    //  Serial.begin(9600); // Print IR Commands
  }
  irrecv.enableIRIn();

  pinMode(pat0, OUTPUT);
  pinMode(pat1, OUTPUT);
  pinMode(pat2, OUTPUT);
  pinMode(pat3, OUTPUT);
  digitalWrite(pat0, LOW);
  digitalWrite(pat1, LOW);
  digitalWrite(pat2, LOW);
  digitalWrite(pat3, LOW);
}


void loop() {
  if (irrecv.decode(&results)) {
    if (debug) {
      Serial.println("test");
    }
    if (debug) {
      Serial.println(results.value, HEX);
    }
    configure(results.value);
    irrecv.resume();
  }

  if (!isOn) {  // Check power status
    digitalWrite(pat0, LOW);
    digitalWrite(pat1, LOW);
    digitalWrite(pat2, LOW);
    digitalWrite(pat3, LOW);
    noTone(speaker);
    return;
  }

  tempo = map(analogRead(A4), 0, 1023, 4000, 100);
  if (tempo < 3900) { // Enter if speed still acceptajk  ble
    if (counter > tempo) {
      if (debug) {
        Serial.println(pattern);
      }
      noTone(speaker);
      counter = 0;

      switchLEDs(pattern);
      note = getNote(pattern);

      pattern = (pattern + 1) % 4;
      if (note > lowFreq + 10) {
        tone(speaker, note);
      }
    }
    counter++;
  } else {  // Stopped on a specific potentiometer
    switchLEDs(pattern);
    unsigned int note = map(analogRead(pattern), 0, 1023, lowFreq, highFreq);
    
    if (note > lowFreq + 10) {
      tone(speaker, note);
    }else{
      noTone(speaker);
    }
  }
}
