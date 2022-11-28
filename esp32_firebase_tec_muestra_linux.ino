
#include <esp_wpa2.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include "Arduino.h"
#include "addons/RTDBHelper.h"
#include <String.h>

#include "DHT.h"
#define DHTPIN 4
#define DHTTYPE DHT11   // DHT 11

const char* ssid = "Tec"; 
#define EAP_IDENTITY "a01731506@tec.mx"  
#define EAP_PASSWORD "123456789.Santi"   


// Firebase insert auth
#define USER_EMAIL "santosban11@gmail.com"
#define USER_PASSWORD "lololola"




// Insert Firebase project API Key
#define API_KEY "AIzaSyAWJVYq8o_8DJFNpbl5CMhW28S2Ayyd8DM"//"AIzaSyBk5v8CZ1KEz2iCgYGjTzOjgcRKmqiW0FI"//AIzaSyAzXLpt54jZP6Q3yUQoEYq2A-v_0vDQix8

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://proyecto1-a433f-default-rtdb.firebaseio.com"//"https://gta-vi-b658b-default-rtdb.firebaseio.com/"

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int intValue;
float floatValue;

bool signupOK = false;
int count = 0;
DHT dht(DHTPIN, DHTTYPE);
//joystick
int JoyStick_X = 26; // Analog Pin  X
int JoyStick_Y = 27; // // Analog Pin  Y
int JoyStick_button = 25; // IO Pin


//flama     
int Fire_analog = 14;    
int Fire_digital = 12;   


int LEDs[] = {33,32,22,5,18,21,19};    // for ESP32 microcontroller

// 

//se declaran los arreglos que forman los dígitos
int zero[] = {0, 1, 1, 1, 1, 1, 1};   // cero listo
int one[] = {0, 0, 0, 0, 1, 0, 1};   // uno listo
int two[] = {1, 0, 1, 1, 0, 1, 1};   // dos listo
int three[] = {1, 0, 0, 1, 1, 1, 1};   // tres listo
int four[] = {1, 1, 0, 0, 1, 0, 1};   // cuatro listo
int five[] = {1, 1, 0, 1, 1, 1, 0};   // cinco listo
int six[] = {1, 1, 1, 1, 1, 1, 0};   // seis listo
int seven[] = {0, 0, 0, 0, 1, 1, 1};   // siete listo
int eight[] = {1, 1, 1, 1, 1, 1, 1}; // ocho listo
int nine[] = {1, 1, 0, 1, 1, 1, 1};   // nueve listo
int ten[] = {1, 0, 0, 0, 0, 0, 0};   // linea Listo


//se declara contador
unsigned char contador = 0;

//función que despliega el numero
void segment_display(unsigned char valor)
{
    switch(valor)
    {
        case 0:
                    for (int i = 0; i<7; i++) digitalWrite(LEDs[i], zero[i]);
                    break;
        case 1:
                    for (int i = 0; i<7; i++) digitalWrite(LEDs[i], one[i]);
                    break;
        case 2:
                    for (int i = 0; i<7; i++) digitalWrite(LEDs[i], two[i]);
                    break;
        case 3:
                    for (int i = 0; i<7; i++) digitalWrite(LEDs[i], three[i]);
                    break;
        case 4:
                    for (int i = 0; i<7; i++) digitalWrite(LEDs[i], four[i]);
                    break;
        case 5:
                    for (int i = 0; i<7; i++) digitalWrite(LEDs[i], five[i]);
                    break;
        case 6:
                    for (int i = 0; i<7; i++) digitalWrite(LEDs[i], six[i]);
                    break;
        case 7:
                    for (int i = 0; i<7; i++) digitalWrite(LEDs[i], seven[i]);
                    break;
        case 8:
                    for (int i = 0; i<7; i++) digitalWrite(LEDs[i], eight[i]);
                    break;
        case 9:
                    for (int i = 0; i<7; i++) digitalWrite(LEDs[i], nine[i]);
                    break;
        case 10:
                    for (int i = 0; i<7; i++) digitalWrite(LEDs[i], ten[i]);
                    break;

    }
}




void setup() {
    Serial.begin(115200);
    delay(10);
    Serial.println(F("DHTxx test!"));

    dht.begin();

    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    // WPA2 enterprise magic starts here
    WiFi.disconnect(true);
    WiFi.mode(WIFI_STA);   //init wifi mode
  //esp_wifi_set_mac(ESP_IF_WIFI_STA, &masterCustomMac[0]);
  Serial.print("MAC >> ");
  Serial.println(WiFi.macAddress());
  Serial.printf("Connecting to WiFi: %s ", ssid);
  //esp_wifi_sta_wpa2_ent_set_ca_cert((uint8_t *)incommon_ca, strlen(incommon_ca) + 1);
  esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)EAP_IDENTITY, strlen(EAP_IDENTITY));  
  esp_wifi_sta_wpa2_ent_set_username((uint8_t *)EAP_IDENTITY, strlen(EAP_IDENTITY)); 
  esp_wifi_sta_wpa2_ent_set_password((uint8_t *)EAP_PASSWORD, strlen(EAP_PASSWORD)); 
  //esp_wifi_sta_wpa2_ent_enable(&configW);
  esp_wifi_sta_wpa2_ent_enable();
    // WPA2 enterprise magic ends here


    WiFi.begin(ssid); 
  

    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    ///* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  //joysitck
  pinMode(JoyStick_X, INPUT);
  pinMode(JoyStick_Y, INPUT);
  pinMode(JoyStick_button, INPUT_PULLUP);

  //flama  
  pinMode(Fire_digital, INPUT);

  //se inicializan los pines del display como salida
  for (int i = 0; i<7; i++) pinMode(LEDs[i], OUTPUT);
}


void loop() {
  String numero;
  int numero2;


  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)


  float f = dht.readTemperature(true);

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  
  // Clears the trigPin


  #define FORCE_SENSOR_PIN 36
  // Prints the distance in the Serial Monitor

  float analogReading = analogRead(FORCE_SENSOR_PIN);

  Serial.print("The force sensor value = ");
  Serial.print(analogReading); // print the raw analog reading

  if (analogReading < 10)       // from 0 to 9
    Serial.println(" -> no pressure");
  else // from 800 to 1023
    Serial.println(" -> big squeeze");


  delay(1000);

  int x, y, button;
  x = analogRead(JoyStick_X); //  X
  y = analogRead(JoyStick_Y); //  Y
  button = digitalRead(JoyStick_button); // 
  x = map(x, 0, 1023, -512, 512);
  y = map(y, 0, 1023, 512, -512);
    
  Serial.print("X : ");
  Serial.print(x, DEC);
  Serial.print(" / Y : ");
  Serial.print(y, DEC);
  Serial.print(" , B : ");
  Serial.println(button, DEC);
  delay(100);


  //flame 
  int firesensorAnalog = analogRead(Fire_analog);
  int firesensorDigital = digitalRead(Fire_digital);

  Serial.print("Fire Sensor: ");
  Serial.print(firesensorAnalog);
  Serial.print("\t");
  Serial.print("Fire Class: ");
  Serial.print(firesensorDigital);
  Serial.print("\t");
  Serial.print("\t");
  
  

  
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 1 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();
   // Write an Int number on the database path test/int
    if (Firebase.RTDB.setFloat(&fbdo, "test/humedad", h)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    //count++;

    // Write an Float number on the database path test/float
    if (Firebase.RTDB.setFloat(&fbdo, "test/Temperatura C",hic )){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    //////////////
    if (Firebase.RTDB.setFloat(&fbdo, "test/TemperaturaF", hif)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());

    }

    if (Firebase.RTDB.setFloat(&fbdo, "test/Presion", analogReading)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    };

    if (Firebase.RTDB.setFloat(&fbdo, "test/JoyStick_Button", button)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    };

    if (Firebase.RTDB.setFloat(&fbdo, "test/Flama", firesensorDigital)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    };


    if (Firebase.RTDB.getString(&fbdo, "test/numero")){
      numero = fbdo.stringData();
      numero2=numero.toInt();
      if (numero2<=10){
        segment_display(numero2);
      }
      else {
        segment_display(10);
      }
      }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    };

    
  }
};






