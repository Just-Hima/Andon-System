#include <LiquidCrystal.h>

// Initialize the LCD with the pin numbers
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);  // RS, E, D4, D5, D6, D7

const int buttonAttentionPin = 2;  // Button for Attention Needed
const int buttonCriticalPin = 3;   // Button for Critical Error
const int buzzerPin = 4;           // Buzzer connected to digital pin 4

const int ledNormalPin = 13;       // LED for Normal status
const int ledAttentionPin = 5;     // LED for Attention Needed status
const int ledCriticalPin = 6;      // LED for Critical Error status

// State variables
bool attentionState = false;
bool criticalState = false;
unsigned long lastBeepTime = 0;
bool beepOn = false;

void setup() {
  Serial.begin(9600); // Start serial communication at 9600 baud
  pinMode(buttonAttentionPin, INPUT_PULLUP);
  pinMode(buttonCriticalPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  
  pinMode(ledNormalPin, OUTPUT);
  pinMode(ledAttentionPin, OUTPUT);
  pinMode(ledCriticalPin, OUTPUT);

  lcd.begin(16, 2);  // Initialize the LCD with 16 columns and 2 rows
  lcd.setCursor(0, 0);
  lcd.print("  System Status  ");
  lcd.setCursor(0, 1);
  lcd.print("     Normal     ");

  noTone(buzzerPin); // Ensure buzzer is off initially
  digitalWrite(ledNormalPin, HIGH); // Turn on Normal LED
  digitalWrite(ledAttentionPin, LOW); // Turn off Attention LED
  digitalWrite(ledCriticalPin, LOW); // Turn off Critical LED
}

void loop() {
  static bool lastAttentionButtonState = HIGH;
  static bool lastCriticalButtonState = HIGH;

  bool currentAttentionButtonState = digitalRead(buttonAttentionPin);
  bool currentCriticalButtonState = digitalRead(buttonCriticalPin);

  // Handle Attention Needed Button
  if (currentAttentionButtonState == LOW && lastAttentionButtonState == HIGH) {
    // Button was pressed
    if (!attentionState) {
      attentionState = true;
      criticalState = false;
      lcd.setCursor(0, 0);
      lcd.print("Attention Needed");
      lcd.setCursor(0, 1);
      lcd.print(" Press button 2 ");

      Serial.println("ATTENTION");
      digitalWrite(ledNormalPin, LOW); // Turn off Normal LED
      digitalWrite(ledAttentionPin, HIGH); // Turn on Attention LED
      digitalWrite(ledCriticalPin, LOW); // Turn off Critical LED
    } else {
      attentionState = false;
      lcd.setCursor(0, 0);
      lcd.clear();
      lcd.print("  System Status  ");
      lcd.setCursor(0, 1);
      lcd.print("     Normal     ");

      Serial.println("NORMAL");
      noTone(buzzerPin); // Turn off buzzer
      digitalWrite(ledNormalPin, HIGH); // Turn on Normal LED
      digitalWrite(ledAttentionPin, LOW); // Turn off Attention LED
      digitalWrite(ledCriticalPin, LOW); // Turn off Critical LED
    }
    delay(500); // Debounce delay
  }

  // Handle Critical Error Button
  if (currentCriticalButtonState == LOW && lastCriticalButtonState == HIGH) {
    // Button was pressed
    if (!criticalState) {
      criticalState = true;
      attentionState = false;
      lcd.setCursor(0, 0);
      lcd.clear();
      lcd.print(" Critical Error ");
      lcd.setCursor(0, 1);
      lcd.print(" Press Button 1 ");

      Serial.println("CRITICAL");
      digitalWrite(ledNormalPin, LOW); // Turn off Normal LED
      digitalWrite(ledAttentionPin, LOW); // Turn off Attention LED
      digitalWrite(ledCriticalPin, HIGH); // Turn on Critical LED
    } else {
      criticalState = false;
      lcd.setCursor(0, 0);
      lcd.clear();
      lcd.print("  System Status  ");
      lcd.setCursor(0, 1);
      lcd.print("     Normal     ");

      Serial.println("NORMAL");
      noTone(buzzerPin); // Turn off buzzer
      digitalWrite(ledNormalPin, HIGH); // Turn on Normal LED
      digitalWrite(ledAttentionPin, LOW); // Turn off Attention LED
      digitalWrite(ledCriticalPin, LOW); // Turn off Critical LED
    }
    delay(500); // Debounce delay
  }

  lastAttentionButtonState = currentAttentionButtonState;
  lastCriticalButtonState = currentCriticalButtonState;

  // Handle Buzzer for Attention and Critical States
  if (attentionState || criticalState) {
    unsigned long currentTime = millis();
    if (currentTime - lastBeepTime >= 1000) { // Change interval for beep
      lastBeepTime = currentTime;
      beepOn = !beepOn;
      if (beepOn) {
        tone(buzzerPin, attentionState ? 1000 : 2000); // Lower frequency for Attention, higher for Critical
      } else {
        noTone(buzzerPin);
      }
    }
  } else {
    noTone(buzzerPin);
  }
}