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
int speedActual = 0;
int speedActualX = 0;
int speedY = 0;
int speedX = 0;
int maxSpeed = 120;
int minSpeed = -25;
int leftDirection = -90;
int rightDirection = 90;


// DA student
byte DA[8] = {B11100, B10000, B11100, B10100, B11111, B00101, B00101, B00111};

// time
unsigned long currentTime = 0;
unsigned long previousTime = 0;
int interval = 100;
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

  

  if(currentTime - previousTime >= interval){
      previousTime = currentTime;

    Serial.print("etd:2206160");
    Serial.print(",x:");
    Serial.print(xValue);
    Serial.print(",y:");
    Serial.print(yValue);
    Serial.print(",sys:");
    Serial.println(light);
  } // end of 100s etd
  
  while (millis() <= 3000) {

    lcd.setCursor(0, 0);
    lcd.print("Darche "); // You can make spaces using well... spaces
    lcd.setCursor(0, 1); // Or setting the cursor in the desired position.
    lcd.write(byte(0));
    lcd.print((char)223);
    delay(3000);

  } // end of while (Darche/60)

  light = map(analogRead(A0), 0, 1023, 0, 100);
  speedActual = 1023 - analogRead(VRY_PIN); 

  if (estClic(currentTime)) {

    driveTask();
  } else {

    lightTask(currentTime);
  }

    
} // end of main loop

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
  lcd.print("%");
  lcd.setCursor(0, 1);
  lcd.print("Phares: ");

  if(light > 50 && (millis() - start) >= timePassed) { // If it is bright...
   
    ledState = LOW;
    digitalWrite(ledPin, ledState); //turn left LED off
    start = millis();
  } // end of if light

  if(light < 50 && (millis() - start) >= timePassed) {
  
    ledState = HIGH;
    digitalWrite(ledPin, ledState);
    start = millis();
  } // end of if dark

  if (!ledState) {
    
    lcd.print("OFF      ");
  } else {
    
    lcd.print("ON       ");
  }
} // end of lightTask

void driveTask() {
  
  xValue = analogRead(VRX_PIN);
  yValue = analogRead(VRY_PIN);
  

  if (speedActual > speedStart) {

    speedY = map(speedActual, speedStart, 1023, 0, maxSpeed);

  } else if(speedActual < speedStart) {

    speedY = map(speedActual, 0, speedStart, minSpeed, 0);

  } else if (speedActual == speedStart) {

    speedY = map(speedActual, speedStart, 0, 0, 0);
  }

  speedX = map(xValue, 0, 1023, leftDirection, rightDirection);


  
  
  lcd.setCursor(0, 0);
  lcd.print("Vitesse:            ");
  lcd.setCursor(0, 1);
  lcd.print("Direction:          ");

  if (speedY > 0) {
    lcd.setCursor(9, 0);
    lcd.print(speedY);
    lcd.print("km/h");
    lcd.setCursor(11, 1);
    

  } else if (speedY < 0) {
    lcd.setCursor(9, 0);
    lcd.print(speedY);
    lcd.print("km/h");
    lcd.setCursor(11, 1);
    

  } else if (speedY == 0) {
    lcd.setCursor(9, 0);
    lcd.print(speedY);
    lcd.print("km/h");
    lcd.setCursor(11, 1);
    lcd.print("none       ");
  }
  
  if (speedX < -5) {
    
    lcd.setCursor(11, 1);
    lcd.print("left        ");
  } else if (speedX > 5) {
    
    lcd.setCursor(11, 1);
    lcd.print("right       ");
  } else {

    lcd.setCursor(11, 1);
    lcd.print("none       ");
  }

  
} // end of driveTask




