#include <LCD_I2C.h>
#include "Button.h"

// LCD
LCD_I2C lcd(0x27, 16, 2); // Default address of most PCF8574 modules, change according

// LED
const int ledPin = 10;
int light = 0; // store the current light value
int lightValue = 0;
int mappedPourcentageLight = 0;

// Joystick/Button
#define BTN_PIN 2;
#define VRX_PIN  A3 // Broche Arduino connectée à la broche VRX
#define VRY_PIN  A4 // Broche Arduino connectée à la broche VRY
int xValue = 0; // Pour stocker la valeur de l'axe X
int yValue = 0; // Pour stocker la valeur de l'axe Y

int pinButton = 3;
int buttonState = 0;





// Time
unsigned long currentTime = 0;
unsigned long previousTime = 0;
int interval = 1000;
const int timePassed = 5000;
unsigned long start;

void setup() {

  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  lcd.begin();
  lcd.backlight();
  pinMode(pinButton, INPUT_PULLUP);
  start = millis();
} // end of setup

void loop() {
  currentTime = millis();
  static int counter = 0;
  

  xValue = analogRead(VRX_PIN);
  yValue = analogRead(VRY_PIN);

  buttonState = digitalRead(pinButton);
  
  if (millis() == 3000) {

    lcd.setCursor(4, 0);
    lcd.print("Darche "); // You can make spaces using well... spaces
    lcd.setCursor(4, 1); // Or setting the cursor in the desired position.
    lcd.print("60 ");
    


  } // end of if 3s (Darche/60)

  light = map(analogRead(A0), 0, 1023, 0, 100);


  if(light > 50 && (millis() - start) >= timePassed) { // If it is bright...
        Serial.println("It  is quite light!");
        digitalWrite(ledPin, LOW); //turn left LED off
        start = millis();
  } // end of if light

  if(light < 50 && (millis() - start) >= timePassed) {
    Serial.println("It is quite dark!");
    digitalWrite(ledPin, HIGH);
    start = millis();
  } // end of if dark


  int valueButton = digitalRead(pinButton);

  


  if (valueButton == 0) {
      lcd.clear(); // Clear le lcd
      lcd.setCursor(0, 0);
      lcd.print("Pct lum: ");
      lcd.print(light);
      lcd.setCursor(0, 1);
      lcd.print("Phare: ");
      lcd.print("ON");
      
    counter++;
  }
  

  if (counter == 1) {

    Serial.print("x = ");
    Serial.print(xValue);
    Serial.print(" | y = ");
    Serial.println(yValue);

    if (yValue > 498) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Avance!");
    }
    if (yValue < 498) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.println("Recule!");
    }
    if (xValue > 510) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.println("Turn Left");
    }
    if (xValue < 510) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.println("Turn Right");
    }
    counter = 0;
  }
  






    
} // end of loop




