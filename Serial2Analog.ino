#include <ArduinoJson.h>

const int AO_PIN_0 = 3;
const int AO_PIN_1 = 5;
const int AO_PIN_2 = 6;
const int AO_PIN_3 = 9;

volatile int AO_0_value = 0;
volatile int AO_1_value = 0;
volatile int AO_2_value = 0;
volatile int AO_3_value = 0;

int AO_0_counter = 0;
int AO_1_counter = 0;
int AO_2_counter = 0;
int AO_3_counter = 0;

void setup() {
// TIMER 1 for interrupt frequency 256 Hz:
cli(); // stop interrupts
TCCR1A = 0; // set entire TCCR1A register to 0
TCCR1B = 0; // same for TCCR1B
TCNT1  = 0; // initialize counter value to 0
// set compare match register for 256 Hz increments
OCR1A = 161; // = 16000000 / (1 * 256) - 1 (must be <65536)
// turn on CTC mode
TCCR1B |= (1 << WGM12);
// Set CS12, CS11 and CS10 bits for 1 prescaler
TCCR1B |= (0 << CS12) | (0 << CS11) | (1 << CS10);
// enable timer compare interrupt
TIMSK1 |= (1 << OCIE1A);
sei(); // allow interrupts

  pinMode(AO_PIN_0, OUTPUT);
  digitalWrite(AO_PIN_0, false);
  pinMode(AO_PIN_1, OUTPUT);
  digitalWrite(AO_PIN_1, false);
  pinMode(AO_PIN_2, OUTPUT);
  digitalWrite(AO_PIN_2, false);
  pinMode(AO_PIN_3, OUTPUT);
  digitalWrite(AO_PIN_3, false);

  Serial.begin(9600);
  Serial.println("<Ready>");
}

void loop() {
  if (Serial.available()) 
  {
    // Allocate the JSON document
    StaticJsonDocument<50> doc;

    // Read the JSON document from the serial port
    DeserializationError err = deserializeJson(doc, Serial);

    if (err == DeserializationError::Ok) 
    {
      // Print the values
      // (we must use as<T>() to resolve the ambiguity)
      AO_0_value = doc["0"].as<byte>();
      Serial.print("A0: ");
      Serial.println(doc["0"].as<int>());

      AO_1_value = doc["1"].as<int>();
      Serial.print("A1: ");
      Serial.println(doc["1"].as<int>());
  
      AO_2_value = doc["2"].as<int>();
      Serial.print("A2: ");
      Serial.println(doc["2"].as<int>());

      AO_3_value =  doc["3"].as<int>();
      Serial.print("A3: ");
      Serial.println(doc["3"].as<byte>());
    } 
    else 
    {
      // Print error to the "debug" serial port
      Serial.print("deserializeJson() returned ");
      Serial.println(err.c_str());
  
      // Flush all bytes in the "link" serial port buffer
      while (Serial.available() > 0) {
        Serial.read();
      }
    }
  }
}

ISR(TIMER1_COMPA_vect){
  if (AO_0_value > 0) {
    AO_0_counter++;
    if (AO_0_counter > AO_0_value) {
      AO_0_counter = 0;
      digitalWrite(AO_PIN_0, !digitalRead(AO_PIN_0));
    }
  }
  if (AO_1_value > 0) {
    AO_1_counter++;
    if (AO_1_counter > AO_1_value) {
      AO_1_counter = 0;
      digitalWrite(AO_PIN_1, !digitalRead(AO_PIN_1));
    }
  }
  if (AO_2_value > 0) {
    AO_2_counter++;
    if (AO_2_counter > AO_2_value) {
      AO_2_counter = 0;
      digitalWrite(AO_PIN_2, !digitalRead(AO_PIN_2));
    }
  }
  if (AO_3_value > 0) {
    AO_3_counter++;
    if (AO_3_counter > AO_3_value) {
      AO_3_counter = 0;
      digitalWrite(AO_PIN_3, !digitalRead(AO_PIN_3));
    }
  }
}
