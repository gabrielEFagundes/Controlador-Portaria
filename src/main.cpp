#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_SSD1306.h>
#include <ESP32Servo.h>
#include <Button.h>
#include <Ultrasonic.h>
#include <Wire.h>

#define SERVO_MOTOR 13
#define BOTAO 25
#define OLED_SDA 21
#define OLED_SCK 6
#define BUZZER 4

#define STEP_I1 19
#define STEP_I2 18
#define STEP_I3 5
#define STEP_I4 17

Button botao(BOTAO);
Servo servoMotor;
Adafruit_SSD1306 oledEntrada(128, 32, &Wire, -1);
Adafruit_SSD1306 oledSaida(128, 32, &Wire, -1);

const char* ssid = "M32";
const char* password = "senha123";
const char* broker = "broker.hivemq.com";

WiFiClient wifi;
PubSubClient client(wifi);

void connect_wifi(){

  delay(10);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED){

    delay(50);
    Serial.println("Wifi Conectado.");
    
    Serial.println("Endereço IP:");
    Serial.println(WiFi.localIP());

  }
}

void reconnect_MQTT(){
  while(!client.connected()){
    Serial.println("Conectando ao broker...");

    if(client.connect("WOKWI_client")){
      Serial.println("Conectado.");

    }else{
      Serial.println("Não foi possível conectar: ");
      Serial.println(client.state());
      Serial.println("Reconectando...");
      delay(5000);
    }
  }
}
/*
void send_data(){
  client.publish("portao", String(value).c_str());
  delay(1000);
}
*/

void setup(){
  Serial.begin(115200);
  connect_wifi();

  pinMode(BOTAO, INPUT);
  servoMotor.attach(SERVO_MOTOR);
  oledEntrada.begin();

  client.setServer(broker, 1883);

}

void loop(){
  client.loop();
  reconnect_MQTT();

  if(BOTAO == HIGH){
    Serial.println("Botão pressionado");
    for(int i = 0; i < 90; i++){
      servoMotor.write(i); // faz o movimento de "abrir" e "fechar"
      delay(15);
    }
  }

  // send_data();
}