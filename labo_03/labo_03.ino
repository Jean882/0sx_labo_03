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



// Time
unsigned long currentTime = 0;
unsigned long serialPrevious = 0;
int serialDelay = 1000;

// State
enum AppState { PAGE1,
                SECOND,
                THIRD,
                FOURTH };
AppState currentState = PAGE1;

void setup() {

  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  lcd.begin();
  lcd.backlight();
  pinMode(pinButton, INPUT_PULLUP);
} // end of setup

void loop() {
  currentTime = millis();

  xValue = analogRead(VRX_PIN);
  yValue = analogRead(VRY_PIN);

  light = analogRead(A0);  // read and save value from PR

  int mappedPourcentageLight = map (lightValue, 0, 1023, 0, 100);

  int valueButton = digitalRead(pinButton);
    
  //Serial.println(light); // print current  light value

  if (millis() == 3000) {

    lcd.setCursor(4, 0);
    lcd.print("Darche "); // You can make spaces using well... spaces
    lcd.setCursor(4, 1); // Or setting the cursor in the desired position.
    lcd.print("60 ");
    


  } // end of if 3s (Darche/60)

  if (valueButton == 0) {
      lcd.clear(); // Clear le lcd
      lcd.setCursor(0, 0);
      lcd.print("Pct lum: ");
      lcd.setCursor(0, 1);
      lcd.print("Phare: ");
      
  }
  else {
    
    valueButton == 1; 
  }
  






    
} // end of loop




