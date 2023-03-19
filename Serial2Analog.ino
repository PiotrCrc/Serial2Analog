#include <ArduinoJson.h>

const int AO_PIN_0 = 3;
const int AO_PIN_1 = 5;
const int AO_PIN_2 = 6;
const int AO_PIN_3 = 9;

void setup() {
  pinMode(AO_PIN_0, OUTPUT);
  analogWrite(AO_PIN_0, 0);
  pinMode(AO_PIN_1, OUTPUT);
  analogWrite(AO_PIN_1, 0);
  pinMode(AO_PIN_2, OUTPUT);
  analogWrite(AO_PIN_2, 0);
  pinMode(AO_PIN_3, OUTPUT);
  analogWrite(AO_PIN_3, 0);

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
      analogWrite(AO_PIN_0, doc["A0"].as<byte>());
      Serial.print("A0: ");
      Serial.println(doc["A0"].as<byte>());

      analogWrite(AO_PIN_1, doc["A1"].as<byte>());    
      Serial.print("A1: ");
      Serial.println(doc["A1"].as<byte>());
  
      analogWrite(AO_PIN_2, doc["A2"].as<byte>());
      Serial.print("A2: ");
      Serial.println(doc["A2"].as<byte>());

      analogWrite(AO_PIN_3, doc["A3"].as<byte>());
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