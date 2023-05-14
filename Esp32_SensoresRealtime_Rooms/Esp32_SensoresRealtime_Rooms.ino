//---------------------------------------IMPORTS PARA FIREBASE REALTIME
#include <FirebaseESP32.h>
#include <WiFi.h>
FirebaseData firebaseData;
const char* ssid = "ERROR404";
const char* password = "AlanTuring117";
//---------------------------------------> IMPORTS PARA SENSOR HT11
#include <Adafruit_Sensor.h>
#include <DHT.h>
#define DHTPIN 4          // Pin del sensor conectado al ESP32
#define DHTTYPE DHT11     // Tipo de sensor (DHT11 o DHT22)
DHT dht(DHTPIN, DHTTYPE); // Crear objeto DHT
//---------------------------------------> IMPORTS PARA SENSOR bmp
#include <Wire.h>
#include <Adafruit_BMP085.h>
Adafruit_BMP085 bmp;




String numero = "";
String texto = "";
unsigned long tiempo_actual = 0;

//VARIABLES DE LOS SENSORES
float Temperatura=0, Humedad=0, Ppm=0, PresionAtmosferica=0;

//--------------------------------------------------------------MÉTODOS DE LECTURA DE SENSORES
float obtenerTemperatura(){
  float temp = dht.readTemperature(); // Leer la temperatura en grados Celsius
  return temp;
}
double obtenerHumedad(){
  float hum = dht.readHumidity(); // Leer la humedad relativa en porcentaje
  return hum;
}
float obtenerCo2(){
  int sensorValue = analogRead(34); // read the input on analog pin 0:
  float ppm=(sensorValue);
  return ppm;
}
float obtenerPresionAtmosferica(){
  float temperatura = bmp.readTemperature();
  float presion = bmp.readPressure() / 100.0F;
  return presion;

  /*SDA: Pin 21
SCL: Pin 22*/
}

void VerifCo2(float valor){
  if(valor>1000){
    Firebase.setBool(firebaseData, "/Room/Actuadores/Ventilador", true);
  }
  if(valor<=1000){
    Firebase.setBool(firebaseData, "/Room/Actuadores/Ventilador", false);
  }
}



void setup() {
  //---------------------------FIREBASE WIFI
  Serial.begin(115200);
  delay(100);
  Serial.println();
  Serial.print("Connecting with ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("WiFi conected. IP: ");
  Serial.println(WiFi.localIP());
  Firebase.begin("https://pruebas-iot-a58f4-default-rtdb.firebaseio.com/", "AIzaSyBQ1p2_zuAdcTRySh-Yjk-ZxySVhmD8VhU");

  //------------------------SENSORES DHT
  dht.begin();


}


void loop() {
  //Obtener Temperatura y humedad
  Humedad= obtenerHumedad();
  Temperatura= obtenerTemperatura();
  //Obtener presion atm
  PresionAtmosferica= obtenerPresionAtmosferica();
  //obtener ppm
  Ppm= obtenerCo2();
  VerifCo2(Ppm);
  //Guardar datos en firebase
  Firebase.setString(firebaseData, "/Room/Realtime/DHT11_Hum", Humedad);
  Firebase.setString(firebaseData, "/Room/Realtime/DHT11_Temp", Temperatura);
  Firebase.setString(firebaseData, "/Room/Realtime/Ppm", Ppm);
  Firebase.setString(firebaseData, "/Room/Realtime/PresionAt", PresionAtmosferica);
  delay(2000);
/*
Serial.print("\nHum: " + String(Humedad));
Serial.print("\nTemp: " + String(Temperatura));
Serial.print("\nPA: " + String(PresionAtmosferica));
Serial.print("\nPPM: " + String(Ppm));
*/
 
  /*Serial.print("Temperatura: ");
  Serial.print(Temperatura);
  Serial.print("C\tHumedad: ");
  Serial.print(Humedad);
  Serial.println("%");
*/
 /* if (Firebase.getString(firebaseData, "/ESP32/numero")) {
      String numero_fb = firebaseData.stringData();
      if (numero_fb != numero) {
        numero = numero_fb;
        Serial.println(numero);
      }
    }

    if (Firebase.getString(firebaseData, "/ESP32/texto")) {
      String texto_fb = firebaseData.stringData();
      if (texto_fb != texto) {
        texto = texto_fb;
        Serial.println(texto);
      }
    }

// Send a random number every 3 seconds.
    if((millis()-tiempo_actual)>= 3000){
          String alea = (String) random(0,100);
          tiempo_actual = millis();
          Serial.println(alea);
          Firebase.setString(firebaseData, "/ESP32/aleatorio", alea);
    }
  
*/
}
