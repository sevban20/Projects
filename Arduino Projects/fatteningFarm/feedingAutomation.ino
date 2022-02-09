/* 
 *       *****--------------------------------****
 *----------     Pamukkale University            ->-*->
 *--------  Electric Electronic Engineering      ->-*->-*>
 *------         Sevban KOCAEL                   ->-*->-*>-*>
 *-------      Feeding Farm Automation           ->-*->-*>
 *------                                         ->-*->
 *       *****--------------------------------****
 * 
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <virtuabotixRTC.h>
#include "DHT.h"


LiquidCrystal_I2C lcd(0x27, 16, 2);

#define increaseButton 11
#define decreaseButton 10
#define firstMotorOut 13
#define secondMotorOut 12
#define confirmButton 9
#define exitButton 8
#define DHTPin 7
#define fan 6
#define firstMotorSet A0
#define secondMotorSet A6
#define feeding A2
#define mixer A3
#define cleaning A1

DHT dht(DHTPin, DHT22);
virtuabotixRTC RTC(4, 3, 5);

int value = 0;
int workingTime = 0;
int selectMode = 0;
int subMode = 0;
double firstValue;
char menuValue;
int fanStart;
bool onProgress = true;
bool working = true;
String isAutomatic = "OFF";
String menu;
String subMenu;
int fanSituation = 0;
int siloWorked = 0;
int totalFeeding = 0;
int firstSilo;
int secondsilo;

void setup() {
  Serial.begin(9600);//Start Serial Communication
  lcd.init();//Start LCD 
  dht.begin();//Start DHT
  pinMode(firstMotorSet, INPUT);//Define pin modes
  pinMode(secondMotorSet, INPUT);
  pinMode(decreaseButton, INPUT);
  pinMode(increaseButton, INPUT);
  pinMode(confirmButton, INPUT);
  pinMode(exitButton, INPUT);
}
//Setting food amount for selected silo
void setting (char motor, char set) {

  lcd.clear();//Clear LCD data
  while (onProgress) {
    lcd.setCursor(0, 0);
    lcd.print("Setting Value");
    lcd.setCursor(0, 1);
    lcd.print(String(value) + " kg.");

    if (digitalRead(increaseButton) == HIGH) {
      value += 1;
      Serial.println(value);
      delay(500);
    }
    else if (digitalRead(decreaseButton) == HIGH && value > 0) {
      value -= 1;
      Serial.println(value);
      delay(500);
    }
    else if (digitalRead(confirmButton) == HIGH) {
      delay(500);
      lcd.clear();
      Serial.print("Confirmed value is ");
      Serial.print(value);
      Serial.println(" kg.");
      lcd.setCursor(0, 0);
      lcd.print("Confirmed value");
      lcd.setCursor(0, 1);
      lcd.print(String(value) + " kg.Processing..");
      firstValue = analogRead(set) * 0.39;
      totalFeeding += value;
      while (working) {
        analogWrite(motor, 255);
        if ((firstValue - analogRead(set) * 0.39) - value <= 0) {
          delay(value * 1000);
          analogWrite(motor, 0);
          lcd.clear();
          lcd.print("Completed.");
          delay(750);
          siloWorked += 1;
          working = false;
        }
      }
      working = true;
      value = 0;
      onProgress = false;
    }
    else if (digitalRead(exitButton) == HIGH ) {
      return;
    }
  }
  onProgress = true;
  lcd.clear();

}
//Menu 
void menuConfirm () {

  lcd.setCursor(0, 0);
  lcd.backlight();
  lcd.println("Pls Select Opt.");
  lcd.setCursor(0, 1);
  lcd.print(menu);

  if (digitalRead(increaseButton) == HIGH || digitalRead(decreaseButton) == HIGH) {
    delay(500);
    if (selectMode == 0) {
      menu = "1.Silo";
      lcd.clear();
      selectMode = 1;
    }
    else if (selectMode == 1) {
      Serial.println("2.Silo");
      menu = "2.Silo";
      lcd.clear();
      selectMode = 2;
    }
    else if (selectMode == 2) {
      Serial.println("Start Feeding");
      menu = "Start Feeding";
      lcd.clear();
      selectMode = 3;
    }
    else if (selectMode == 3) {
      Serial.println("Automatic Feeder");
      menu = "Automatic Feeder";
      lcd.clear();
      selectMode = 4;
    }
    else if (selectMode == 4) {
      Serial.println("Start Cleaning");
      menu = "Start Cleaning";
      lcd.clear();
      selectMode = 0;
    }
  }
  if (digitalRead(confirmButton) == HIGH && menu == "1.Silo") {
    Serial.println("-----1-----");
    delay(500);
    setting(firstMotorOut, firstMotorSet);

  }
  else if (digitalRead(confirmButton) == HIGH && menu == "2.Silo") {
    Serial.println("-----2-----");
    delay(500);
    setting(secondMotorOut, secondMotorSet);
  }
  else if (digitalRead(confirmButton) == HIGH && menu == "Start Feeding" && siloWorked > 0 ) {//If there is food in mixer, mixing and disturbuting
    Serial.println("Start Feeding");                                                          
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Total " + String(totalFeeding) + " kg.");
    lcd.setCursor(0, 1);
    analogWrite(mixer, 255);
    lcd.print("Mixing...");
    delay(5000);
    analogWrite(mixer, 0);
    lcd.clear();
    totalFeeding = 0;
    analogWrite(feeding, 255);
    lcd.print("feeding...");
    delay(5000);
    siloWorked = 0;
    analogWrite(feeding, 0);
    lcd.clear();



  }
  else if (digitalRead(confirmButton) == HIGH && menu == "Start Feeding" && siloWorked == 0 ) {//If there is no food in mixer, isn't working
    Serial.println("Start Feeding");
    lcd.clear();
    lcd.print("No Food in Mixer!!");
    delay(1000);
    lcd.clear();
  }

  else if (digitalRead(confirmButton) == HIGH && menu == "Automatic Feeder") { //Control for automatic feed unit
    delay(500);
    lcd.clear();
    Serial.println(isAutomatic);
    subMenu = isAutomatic;
    while (working) {
      lcd.setCursor(0, 0);
      lcd.print("Automatic Feeder:");
      lcd.setCursor(0, 1);
      lcd.print(subMenu);
      if (digitalRead(increaseButton) == HIGH || digitalRead(decreaseButton) == HIGH) {
        delay(500);
        if (subMode == 0) {
          lcd.clear();
          subMenu = "ON";
          subMode = 1;
        }
        else if (subMode == 1) {
          lcd.clear();
          subMenu = "OFF";
          subMode = 0;
        }
      }
      else if (digitalRead(confirmButton) == HIGH && subMenu == "ON") {
        delay(500);
        isAutomatic = "ON";
        working = false;
      }
      else if (digitalRead(confirmButton) == HIGH && subMenu == "OFF") {
        delay(500);
        isAutomatic = "OFF";
        working = false;
      }
      else if (digitalRead(exitButton) == HIGH) {
        delay(500);
        working = false;
      }
    }
    working = true;
  }
  else if (digitalRead(confirmButton) == HIGH && menu == "Start Cleaning") {//Manuel cleaning
    lcd.clear();
    lcd.print("Cleaning...");
    analogWrite(cleaning, 255);
    delay(180);
    analogWrite(cleaning, 0);
  }
}

//Control of environmental humidity and temperature.If it's necessary to start fan
void tempAndHumControl() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {//Check DHT is working
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  else if ((t >= 35 || h >= 85) && fanSituation == 0) {//Rub the fan if the temperature is above 35°C
    fanSituation = 255;
    analogWrite(fan, fanSituation);
    RTC.updateTime();
    fanStart = RTC.minutes;
  }
  Serial.println(fanStart);//Write a fan situation to serial port 
}

void fanOff() {//Turn off the fan when time is up
  if ((RTC.minutes - fanStart) == 3 ) {
    fanSituation = 0;
    fanStart = 60;
    analogWrite(fan, fanSituation);
    Serial.println("kapanması gerekiyor");
  }
}

void automaticFeeder (int firstSilo, int secondSilo) {//Automatic feeding 
  lcd.clear();
  Serial.print("Confirmed value is ");
  Serial.print(firstSilo);
  Serial.println(" kg.");
  lcd.setCursor(0, 0);
  lcd.print("Confirmed value");
  lcd.setCursor(0, 1);
  lcd.print(String(firstSilo) + " kg.Processing..");
  totalFeeding += firstSilo;
  while (working) {
    analogWrite(firstMotorOut, 255);
    delay(firstSilo * 1000);
    analogWrite(firstMotorOut, 0);
    lcd.clear();
    lcd.print("Completed.");
    delay(750);
    working = false;

  }
  working = true;
  delay(500);
  lcd.clear();
  Serial.print("Confirmed value is ");
  Serial.print(secondSilo);
  Serial.println(" kg.");
  lcd.setCursor(0, 0);
  lcd.print("Confirmed value");
  lcd.setCursor(0, 1);
  lcd.print(String(secondSilo) + " kg.Processing..");
  totalFeeding += secondSilo;
  while (working) {
    analogWrite(secondMotorOut, 255);
    delay(secondsilo * 1000);
    analogWrite(secondMotorOut, 0);
    lcd.clear();
    lcd.print("Completed.");
    delay(750);
    siloWorked += 1;
    working = false;

  }
  Serial.println("Start Feeding");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Total " + String(totalFeeding) + " kg.");
  lcd.setCursor(0, 1);
  analogWrite(mixer, 255);
  lcd.print("Mixing...");
  delay(5000);
  analogWrite(mixer, 0);
  lcd.clear();
  totalFeeding = 0;
  analogWrite(feeding, 255);
  lcd.print("feeding...");
  delay(5000);
  siloWorked = 0;
  analogWrite(feeding, 0);
  lcd.clear();
  totalFeeding = 0;
  working = true;

}
void loop() {
  RTC.updateTime();//Clock info 
  tempAndHumControl();//Check humidity and temperature
  menuConfirm ();//Access menu
  fanOff(); // turn off fan
  if ((RTC.hours == 9 && RTC.minutes == 00) || (RTC.hours == 21 && RTC.minutes == 00)) {//Cleaning every 12 hours
    analogWrite(cleaning, 255);
    delay(500);
    analogWrite(cleaning, 0);
  }
  else if (isAutomatic == "ON" && (RTC.hours == 8 && RTC.minutes < 1 || RTC.hours == 20 && RTC.minutes < 1)) {//If automatic feeding is open,
    automaticFeeder(1, 1);                                                                                    //it runs every 12 hours
  }
  //delay(500);
}
