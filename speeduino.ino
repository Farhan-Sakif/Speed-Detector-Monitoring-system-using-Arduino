#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // 16x2 LCD at I2C address 0x27

const int ledPin = 9;   // LED indicator
byte irPinA = 7;        // First IR sensor pin
byte irPinB = 4;        // Second IR sensor pin

byte irValA;
byte irValB;

unsigned long timeFirst = 0;   // Time when first sensor is triggered
unsigned long timeSecond = 0;  // Time when second sensor is triggered
float diff;                    // Time difference
float spd;                     // Speed result

// Adjust this constant according to your sensor spacing!
// Formula: (distance_mm * 3600) / 1000
float speedConst = 270;  // Example: 75 mm spacing → 270

void setup() {
  // LCD setup
  lcd.init();
  lcd.backlight();

  // Pin setup
  pinMode(ledPin, OUTPUT);
  pinMode(5, OUTPUT);   // powering sensor A
  pinMode(6, OUTPUT);   // powering sensor B
  pinMode(irPinA, INPUT);
  pinMode(irPinB, INPUT);
  pinMode(10,OUTPUT);

  // Start with LED ON (ready state)
  digitalWrite(ledPin, HIGH);

  // Startup message
  lcd.setCursor(0,0);
  lcd.print("Speed Gun");
  delay(2000);
  lcd.clear();

  // Serial monitor for debugging
  Serial.begin(9600);
}

void loop() {
  // Power sensors 
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);

  // Read sensors
  irValA = digitalRead(irPinA);
  irValB = digitalRead(irPinB);

  // Detect first sensor
  if (irValA == LOW && timeFirst == 0) {
    timeFirst = millis();
    digitalWrite(ledPin, LOW); // turn LED off while measuring
    Serial.println("First sensor triggered");
  }

  // Detect second sensor, only if first already triggered
  if (irValB == LOW && timeFirst > 0 && timeSecond == 0) {
    timeSecond = millis();
    diff = timeSecond - timeFirst;

    if (diff > 0) {
      spd = speedConst / diff; // calculate speed

      // Display on LCD
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Speed:");
      lcd.print(spd);
      lcd.print("km/h");

      // Debug to Serial
      Serial.print("timeFirst = ");
      Serial.println(timeFirst);
      Serial.print("timeSecond = ");
      Serial.println(timeSecond);
      Serial.print("diff = ");
      Serial.println(diff);
      Serial.print("Speed = ");
      Serial.println(spd);
      if(spd > 8){ //set off a buzzer if speed is over 8km/h(can be any speed you want)
        digitalWrite(10,HIGH);
        lcd.setCursor(0,1);
        lcd.print("Overspeeding!");
        delay(3000);
        digitalWrite(10,LOW);
        


      }
    }

    // Reset for next object
    timeFirst = 0;
    timeSecond = 0;

    digitalWrite(ledPin, HIGH); // LED back on
    
  }
}
