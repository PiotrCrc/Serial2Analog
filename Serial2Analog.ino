#include <ArduinoJson.h>

const int AO_PIN_0 = 3;
const int AO_PIN_1 = 5;
const int AO_PIN_2 = 6;
const int AO_PIN_3 = 9;

volatile byte AO_0_value = 0;
volatile byte AO_1_value = 0;
volatile byte AO_2_value = 0;
volatile byte AO_3_value = 0;

byte AO_0_counter = 0;
byte AO_1_counter = 0;
byte AO_2_counter = 0;
byte AO_3_counter = 0;

void setup() {
  // Enable Timer/Counter1 input capture interrupt
  TIMSK1 |= (1 << TICIE1);
  // Set Timer/Counter1 clock prescaler to 64
  TCCR1B |= (1 << CS11) | (1 << CS10);
  // Set Timer/Counter1 top value to 624 (256Hz)
  OCR1A = 624;
  // Initialize interrupt flag
  TIFR1 |= (1 << ICF1);
  // Enable global interrupts
  sei();
  
  pinMode(AO_PIN_0, OUTPUT);
  digitalWrite(AO_PIN_0, False);
  pinMode(AO_PIN_1, OUTPUT);
  digitalWrite(AO_PIN_1, False);
  pinMode(AO_PIN_2, OUTPUT);
  digitalWrite(AO_PIN_2, False);
  pinMode(AO_PIN_3, OUTPUT);
  digitalWrite(AO_PIN_3, False);

  Serial.begin(9600);
  while (!Serial) continue;
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
      AO_0_value = doc["A0"].as<byte>();
      Serial.print("A0: ");
      Serial.println(doc["A0"].as<byte>());

      AO_1_value = doc["A1"].as<byte>();    
      Serial.print("A1: ");
      Serial.println(doc["A1"].as<byte>());
  
      AO_2_value = doc["A2"].as<byte>();
      Serial.print("A2: ");
      Serial.println(doc["A2"].as<byte>());

      AO_0_value =  doc["A3"].as<byte>();
      Serial.print("A3: ");
      Serial.println(doc["A3"].as<byte>());
    } 
    else 
    {
      // Print error to the "debug" serial port
      Serial.print("deserializeJson() returned ");
      Serial.println(err.c_str());
  
      // Flush all bytes in the "link" serial port buffer
      while (Serial.available() > 0)
        Serial.read();
    }
  }
}

ISR(TIMER1_CAPT_vect) {
  // Timer/Counter1 input capture interrupt service routine
  TIFR1 |= (1 << ICF1); // Clear interrupt flag
  if (AO_0_value > 0) {
    AO_0_counter++;
    if (AO_0_counter > (255 - AO_0_value)) {
      AO_0_counter = 0;
      digitalWrite(AO_PIN_0, !digitalRead(AO_PIN_0));
    }
  }
}
