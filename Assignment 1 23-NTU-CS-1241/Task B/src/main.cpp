// Assignment 1 -HomeTask 
// Task B — Coding: Use a single button with press-type detection (display the event on the OLED): 
// Embedded IoT System Fall-2025 
 
// Name: Abdullah Ahmad                  
// Reg#: 23-NTU-CS-1241
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int LED_PIN = 25;
const int BUZZER_PIN = 33;
const int BUTTON_PIN = 14;

bool ledState = false;
bool buttonPressed = false;
unsigned long pressStartTime = 0;
const unsigned long longPressDuration = 1500; // 1.5 seconds

void displayMessage(String msg) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 20);
  display.println(msg);
  display.display();
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 20);
  display.println("System Ready");
  display.display();
  delay(1000);
}

void loop() {
  int buttonState = digitalRead(BUTTON_PIN);

  // When button is pressed
  if (buttonState == LOW && !buttonPressed) {
    buttonPressed = true;
    pressStartTime = millis();
  }

  // When button is released
  if (buttonState == HIGH && buttonPressed) {
    unsigned long pressDuration = millis() - pressStartTime;
    buttonPressed = false;

    if (pressDuration > longPressDuration) {
      // Long press action
      displayMessage("Long Press");
      tone(BUZZER_PIN, 1000, 400);
    } else {
      // Short press action
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState);
      displayMessage("Short Press");
    }
  }
}


