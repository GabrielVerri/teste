#include"marcela.h"
char* ssid="wifi name";
char* password="Senha do wifi";
char* mqtt_server="mqtt_server";

WiFiClient wifiCliente;
PubSubClient client(wifiCliente);

MQTTConnection conexao(ssid, password, mqtt_server, 1883);

void setup() {
  Serial.begin(57600);
  conexao.begin(client);
  conexao.setCallback(); 
  delay(500);
}
void loop() {
  conexao.loop();
  
}
