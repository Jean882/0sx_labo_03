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
#define BTN_PIN 2
#define VRX_PIN  A1 // Broche Arduino connectée à la broche VRX
#define VRY_PIN  A2 // Broche Arduino connectée à la broche VRY
int xValue = 0; // Pour stocker la valeur de l'axe X
int yValue = 0; // Pour stocker la valeur de l'axe Y


int buttonState = 0;

byte DA[8] = {B11100, B10000, B11100, B10100, B11111, B00101, B00101, B00111};





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
  pinMode(BTN_PIN, INPUT_PULLUP);
  start = millis();
  lcd.createChar(0,DA);

} // end of setup


int estClic(unsigned long ct) {
  static unsigned long lastTime = 0;
  static int lastState = HIGH;
  const int rate = 50;
  int clic = 0;

  if (ct - lastTime < rate) {
    return clic; // Trop rapide
  }

  lastTime = ct;

  int state = digitalRead(BTN_PIN);

  if (state == LOW) {
    if (lastState == HIGH) {
      clic = !clic;
    }
  }

  lastState = state;

  return clic;
}


void loop() {
  currentTime = millis();
  static int counter = 0;
  

  xValue = analogRead(VRX_PIN);
  yValue = analogRead(VRY_PIN);

  buttonState = digitalRead(BTN_PIN);
  
  while (millis() <= 3000) {

    lcd.setCursor(0, 0);
    lcd.print("Darche "); // You can make spaces using well... spaces
    lcd.setCursor(0, 1); // Or setting the cursor in the desired position.
    lcd.write(byte(0));
    delay(3000);
    


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


  int valueButton = digitalRead(BTN_PIN);

  


  if (valueButton == 0) {
      lcd.clear(); // Clear le lcd
      lcd.setCursor(0, 0);
      lcd.print("Pct lum: ");
      lcd.print(light);
      lcd.setCursor(0, 1);
      lcd.print("Phare: ");
      lcd.print("ON");
      
   
  }
  

  if (counter) {

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




