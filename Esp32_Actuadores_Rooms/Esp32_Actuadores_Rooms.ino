// Juan A. Villalpando
// http://kio4.com/arduino/117_Wemos_Firebase.htm

#include <FirebaseESP32.h>
#include <WiFi.h>
FirebaseData firebaseData;

const char* ssid = "ERROR404";
const char* password = "AlanTuring117";

String numero = "";
String texto = "";

unsigned long tiempo_actual = 0;


#define RELAY1_PIN 23  // Pin para el relé 1
#define RELAY2_PIN 22  // Pin para el relé 2
#define RELAY3_PIN 21  // Pin para el relé 3
#define RELAY4_PIN 19  // Pin para el relé 4





void setup() {
    // Configura los pines de los relés como salidas
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);
  pinMode(RELAY3_PIN, OUTPUT);
  pinMode(RELAY4_PIN, OUTPUT);


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
}



void actuadorLuz(bool valor){
  if(valor==true){
    digitalWrite(RELAY1_PIN, HIGH);
  }
   if(valor==false){
    digitalWrite(RELAY1_PIN, LOW);
  }
}

void actuadorVenti(bool valor){
  if(valor==true){
    digitalWrite(RELAY2_PIN, HIGH);
  }
   if(valor==false){
    digitalWrite(RELAY2_PIN, LOW);
  }
}

void loop() {
  bool ValorLuz=false, ValorVenti=false;
  ValorVenti= Firebase.getString(firebaseData, "/Room/Actuadores/Ventilador");
  ValorLuz= Firebase.getString(firebaseData, "/Room/Actuadores/Luz");
  actuadorLuz(ValorLuz);
  actuadorVenti(ValorVenti);

  /*if (Firebase.getString(firebaseData, "/ESP32/numero")) {
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
