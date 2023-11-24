#include <WiFi.h>
#include <PubSubClient.h>
#include<DHT.h>
#include"pontoH.h"

const char* ssid = "Bolota";
const char* password = "sougayhaha";
const char* mqtt_server = "broker.hivemq.com";

WiFiClient wifiClient;
PubSubClient client(wifiClient); 
char* mqttServer = "broker.hivemq.com";
int mqttPort = 1883;
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];

void setupMQTT() {
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
}

void setup_wifi() {
  delay(10);
    Serial.println();
  Serial.print("Conectando na rede ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.print("Endereço IP : ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Menssagem recebida [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentando conexão MQTT ...");
    String clientId = "GRUPO_007";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("conectado");
      client.subscribe("grupoXYZ/publisher");
    } else {
      Serial.print("falhou, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 5 segundos");
      delay(5000);
    }
  }
}

void NovoPublish(Isensor& sensor,const char* topic){
Dado* dado=sensor.ler();
int i=0;
while((dado+i)->valor!="NULL"){
  client.publish(topic,(dado+i)->valor.c_str());
  i++;
}
}

Sensor sensorSolo(34, SOIL_MOISTURE_SENSOR);
Sensor sensorLDR(35, LDR_SENSOR);
SensorDHT sensorDHT(26);

void setup() {
    Serial.begin(57600);
    setup_wifi();
    client.setServer(mqttServer, mqttPort);
    client.setCallback(callback);
    delay(500);
}

void loop() {
    if (!client.connected())
      reconnect();
    client.loop();
    long now = millis();
    if (now - lastMsg > 60000) {
      
      lastMsg = now;
      delay(10);
    }

    
    //umi_solo = sensorSolo.ler();
    delay(500);
  
    if (sensorSolo.leitura() <= 100 && sensorSolo.leitura() > 70) {
        Serial.print("encharcado");
        snprintf(msg,MSG_BUFFER_SIZE,"Solo encharcado");
        client.publish("grupoXYZ/solo",msg);
    }
    else if (sensorSolo.leitura() <= 70 && sensorSolo.leitura() > 35) {
        Serial.print("ÚMIDO");
        snprintf(msg,MSG_BUFFER_SIZE," Solo úmido");
        client.publish("grupoXYZ/solo",msg);
    }
    else {
        Serial.print("SECO ");
        snprintf(msg,MSG_BUFFER_SIZE,"Solo seco");
        client.publish("grupoXYZ/solo",msg); 
    }

    Serial.print(" Umiddade: ");
    Serial.print(sensorSolo.ler()->valor);
    Serial.println("");


    int ldr = sensorLDR.leitura();
    delay(1500);
    int aux_ldr = sensorLDR.leitura();

    if (aux_ldr; aux_ldr <= 100 && aux_ldr > 60) {
        Serial.println("Muita luz");
        snprintf(msg,MSG_BUFFER_SIZE,"Muita luz");
        client.publish("grupoXYZ/luz",msg); 
    }
    else {
        Serial.println("Pouca luz");
        snprintf(msg,MSG_BUFFER_SIZE,"Pouca luz");
        client.publish("grupoXYZ/luz",msg);
    }
    Serial.println("");


    //sensorDHT.leituraDHT();
    float temp = sensorDHT.lerTemperatura();
    float umid = sensorDHT.lerUmidade();

    Serial.print("Temperatura: ");
    Serial.print(temp);
    Serial.println("°C");
    Serial.print("Umidade do ar: ");
    Serial.print(umid);
    Serial.println("%");
    Serial.println("");

    snprintf(msg,MSG_BUFFER_SIZE,"%.1f",temp);
    client.publish("grupoXYZ/temper",msg);

    snprintf(msg,MSG_BUFFER_SIZE,"%.1f",umid);
    client.publish("grupoXYZ/umid",msg);
    
    delay(1500);


    if (!client.connected()) {
      reconnect();
    }
    client.loop();
}
