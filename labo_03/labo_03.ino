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
int speedStart = 0;
int speedActual =0;
int speedY = 0;
int speedX = 0;


// DA student
byte DA[8] = {B11100, B10000, B11100, B10100, B11111, B00101, B00101, B00111};

// time
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
  speedStart = 1023 - analogRead(VRY_PIN);

} // end of setup

void loop() {
  currentTime = millis();  

  
  
  while (millis() <= 3000) {

    lcd.setCursor(0, 0);
    lcd.print("Darche "); // You can make spaces using well... spaces
    lcd.setCursor(0, 1); // Or setting the cursor in the desired position.
    lcd.write(byte(0));
    delay(3000);

  } // (Darche/60)

  light = map(analogRead(A0), 0, 1023, 0, 100);
  speedActual = 1023 - analogRead(VRY_PIN);

  

  if (estClic(currentTime)) {

    driveTask();
  } else {

    lightTask(currentTime);
  }

    
} // end of loop

int estClic(unsigned long ct) {
  static unsigned long lastTime = 0;
  static int lastState = HIGH;
  const int rate = 50;
  static int clic = 0;

  if (ct - lastTime < rate) {
    return clic; // Trop rapide
  }

  lastTime = ct;

  int state = digitalRead(BTN_PIN);

  if (state == LOW) {
    if (state != lastState) {
      clic = !clic;
    }
  }

  lastState = state;

  return clic;
} // end of estClic

void lightTask(unsigned long ct) {

  static int ledState = LOW;
  
  lcd.setCursor(0, 0);
  lcd.print("Pct lum: ");
  lcd.print(light);
  lcd.setCursor(0, 1);
  lcd.print("Phare: ");

  if(light > 50 && (millis() - start) >= timePassed) { // If it is bright...
    Serial.println("It  is quite light!");
    ledState = LOW;
    digitalWrite(ledPin, ledState); //turn left LED off
    start = millis();
  } // end of if light

  if(light < 50 && (millis() - start) >= timePassed) {
    Serial.println("It is quite dark!");
    ledState = HIGH;
    digitalWrite(ledPin, ledState);
    start = millis();
  } // end of if dark

  if (!ledState) {
    
    lcd.print("OFF");
  } else {
    
    lcd.print("ON ");
  }
} // end of lightTask

void driveTask() {

  xValue = analogRead(VRX_PIN);
  yValue = analogRead(VRY_PIN);
  

  if (speedActual > speedStart) {

    speedY = map(speedActual, speedStart, 1023, 0, 120);

  } else if(speedActual < speedStart) {

    speedY = map(speedActual, 0, speedStart, -25, 0);

  } else if (speedActual == speedStart) {

    speedY = map(speedActual, speedStart, 0, 0, 0);
  }



  Serial.print("x = ");
  Serial.print(xValue);
  Serial.print(" | y = ");
  Serial.println(yValue);
  
  lcd.setCursor(0, 0);
  lcd.print("Vitesse:            ");
  lcd.setCursor(0, 1);
  lcd.print("Direction:          ");

  if (speedY > 0) {
    lcd.setCursor(9, 0);
    lcd.print(speedY);

  } else if (speedY < 0) {
    lcd.setCursor(9, 0);
    lcd.print(speedY); 

  } else if (speedY == 0) {
    lcd.setCursor(9, 0);
    lcd.print(speedY);
    
  }
  
  if (xValue > 510) {
    
    
    
  }
  if (xValue < 510) {
    
  }
} // end of driveTask




