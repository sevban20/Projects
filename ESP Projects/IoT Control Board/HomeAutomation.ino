#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include <Wire.h>
#include "DHT.h"

// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

#define DHTPIN 5
#define WIFI_SSID "WIFI_SSID"
#define WIFI_PASSWORD "WIFI_PASSWORD"

// Insert Firebase project API Key
#define API_KEY "FIREBASE_API_KEY"

// Insert Authorized Email and Corresponding Password
#define USER_EMAIL "FIREBASE_EMAIL"
#define USER_PASSWORD "FIREBASE_PASSWORD"

// Insert RTDB URL define the RTDB URL
#define DATABASE_URL "FIREBASE_RTDB_URL"

DHT dht(DHTPIN,DHT22);
// Define Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

int out1 = 13;
int out2 = 4;
int out3 = 14;
int out4 = 12;
int ldr = A0;

// Variable to save USER UID
String uid;

// Variables to save database paths
String databasePath;
String out1Path;
String out2Path;
String out3Path;
String out4Path;
String ldrPath;
String humPath;
String tempPath;
//Sensor Values
float temperature;
float humidity;
int ldrValue;
//Relays values
int out1Value;
int out2Value;
int out3Value;
int out4Value;

// Initialize WiFi
void initWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  Serial.println();
}

// Write float values to the database
void sendFloat(String path, float value){
  if (Firebase.RTDB.setFloat(&fbdo, path.c_str(), value)){
    Serial.print("Writing value: ");
    Serial.print (value);
    Serial.print(" on the following path: ");
    Serial.println(path);
    Serial.println("PASSED");
    Serial.println("PATH: " + fbdo.dataPath());
    Serial.println("TYPE: " + fbdo.dataType());
  }
  else {
    Serial.println("FAILED");
    Serial.println("REASON: " + fbdo.errorReason());
  }
}

//Get Int value from database
int getInt(String Path){
  if (Firebase.ready()){
      if(Firebase.RTDB.getInt(&fbdo, Path)){
        if(fbdo.dataType() == "int"){
          Serial.print("Read Value: ");
          Serial.println(fbdo.intData());
          return fbdo.intData();
        }
    }
}
}
//Set Relay status with Firebase Database
void relayStatus(){
  out1Value = getInt(out1Path);
  out2Value = getInt(out2Path);
  out3Value = getInt(out3Path);
  out4Value = getInt(out4Path);
  if(out1Value == 1){
    digitalWrite(out1,HIGH);
  }
  else{
    digitalWrite(out1,LOW);
  }
  if(out2Value == 1){
    digitalWrite(out2,HIGH);
  }
  else{
    digitalWrite(out2,LOW);
  }
  if(out3Value == 1){
    digitalWrite(out3,HIGH);
  }
  else{
    digitalWrite(out3,LOW);
  }
  if(out4Value == 1){
    digitalWrite(out4,HIGH);
  }
  else{
    digitalWrite(out4,LOW);
  }
  
}

void setup(){
  Serial.begin(115200);
  pinMode(out1, OUTPUT);
  pinMode(out2, OUTPUT);
  pinMode(out3, OUTPUT);
  pinMode(out4, OUTPUT);
  pinMode(ldr, INPUT);
  dht.begin();
  initWiFi();

  // Assign the api key (required)
  config.api_key = API_KEY;

  // Assign the user sign in credentials
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // Assign the RTDB URL (required)
  config.database_url = DATABASE_URL;

  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(4096);

  // Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  // Assign the maximum retry of token generation
  config.max_token_generation_retry = 5;

  // Initialize the library with the Firebase authen and config
  Firebase.begin(&config, &auth);

  // Getting the user UID might take a few seconds
  Serial.println("Getting User UID");
  while ((auth.token.uid) == "") {
    Serial.print('.');
    delay(1000);
  }
  // Print user UID
  uid = auth.token.uid.c_str();
  Serial.print("User UID: ");
  Serial.println(uid);

  // Update database path
  databasePath = "/UsersData/" + uid;
  out1Path = databasePath + "/out1";
  out2Path = databasePath + "/out2";
  out3Path = databasePath + "/out3";
  out4Path = databasePath + "/out4";
  humPath = databasePath + "/humidity";
  tempPath = databasePath + "/temperature";
  ldrPath = databasePath + "/ldr";
  relayStatus();

}
//For operational executions
void operational(){
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

  if (isnan(temperature) || isnan(humidity)){
    Serial.println("Failed to read from dht sensor !!!"); 
    return;
  }
    // Send readings to database:
  else if ( !isnan(temperature) && !isnan(humidity)){
    sendFloat(humPath, humidity);
    sendFloat(tempPath, temperature);
  }
  relayStatus();
  ldrValue = analogRead(ldr); //Read LDR value
  sendFloat(ldrPath,ldrValue); // Write LDR value to firebase
    delay(1000);
  
}
void loop(){
  if (WiFi.status() == WL_CONNECTED) {//If WiFi connected continue proccess
    operational();
  }
  else{//If WiFi is not connect try to connect WiFi
    initWiFi();
  }
}