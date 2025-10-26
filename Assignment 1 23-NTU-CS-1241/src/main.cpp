#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// --- Pin Definitions ---
const int LED1 = 25;
const int LED2 = 26;
const int LED3 = 27;
const int BUZZER = 33;
const int BTN_MODE = 14;
const int BTN_RESET = 12;

int mode = 0; // 0=OFF, 1=ALT BLINK, 2=BOTH ON, 3=PWM
int lastButtonState1 = HIGH;
int lastButtonState2 = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 200;

// --- PWM Variables ---
int pwmValue = 0;
int fadeAmount = 5;

void updateOLED() {
  display.clearDisplay();
  display.setCursor(0, 20);
  display.setTextSize(1);
  display.println("Current Mode:");
  display.setTextSize(2);
  switch (mode) {
    case 0: display.println("OFF"); break;
    case 1: display.println("ALT"); break;
    case 2: display.println("ON"); break;
    case 3: display.println("PWM"); break;
  }
  display.display();
}
void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(BTN_MODE, INPUT_PULLUP);
  pinMode(BTN_RESET, INPUT_PULLUP);

  // OLED setup
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    while (true);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 30);
  display.println("System Ready");
  display.display();
  delay(1000);
}

void loop() {
  // --- Button for mode cycle ---
  int reading1 = digitalRead(BTN_MODE);
  if (reading1 == LOW && lastButtonState1 == HIGH && millis() - lastDebounceTime > debounceDelay) {
    mode = (mode + 1) % 4;
    tone(BUZZER, 1000, 100);
    updateOLED();
    lastDebounceTime = millis();
  }
  lastButtonState1 = reading1;

  // --- Button for reset ---
  int reading2 = digitalRead(BTN_RESET);
  if (reading2 == LOW && lastButtonState2 == HIGH && millis() - lastDebounceTime > debounceDelay) {
    mode = 0;
    tone(BUZZER, 800, 100);
    updateOLED();
    lastDebounceTime = millis();
  }
  lastButtonState2 = reading2;

  // --- LED Behaviors ---
  switch (mode) {
    case 0: // Both OFF
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      analogWrite(LED3, 0);
      break;

    case 1: // Alternate Blink
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, LOW);
      delay(300);
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, HIGH);
      delay(300);
      break;

    case 2: // Both ON
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, LOW);
      break;

    case 3: // PWM Fade
      analogWrite(LED3, pwmValue);
      pwmValue += fadeAmount;
      if (pwmValue <= 0 || pwmValue >= 255) fadeAmount = -fadeAmount;
      delay(30);
      break;
  }
}

