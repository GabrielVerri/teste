#include "Client.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>



class MQTTConnection {
public:
  PubSubClient client;
  MQTTConnection(const char* ssid, const char* password, const char* mqttServer, int mqttPort)
    : ssid(ssid), password(password), mqttServer(mqttServer), mqttPort(mqttPort) {}


  void begin(PubSubClient client) {
    this->client=client;


    setupWiFi();
    setupMQTT();
  }

  void connect() {
    while (!client.connected()) {
      Serial.print("Tentando conexão MQTT ...");
      String clientId = "GRUPO_007";
      clientId += String(random(0xffff), HEX);
      if (client.connect(clientId.c_str())) {
        Serial.println("conectado");
      } else {
        Serial.print("Falhou, rc=");
        Serial.print(client.state());
        Serial.println(" tentando novamente em 5 segundos");
        delay(5000);
      }
    }
  }

  void loop() {
    if (!this->client.connected()) {
      connect();
    }
    this->client.loop();
  }

  void callback(char* topic, byte* conteudo, unsigned int length) {
    Serial.print("Mensagem recebida [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++) {
      Serial.print((char)conteudo[i]);
    }
    Serial.println();
  }

  void setCallback() {
    this->client.setCallback([this](char* topic, byte* conteudo, unsigned int length) {
      callback(topic, conteudo, length);
    });
  }

private:
  const char* ssid;
  const char* password;
  const char* mqttServer;
  int mqttPort;

  void setupWiFi() {
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

  void setupMQTT() {
    this->client.setServer(mqttServer, mqttPort);
  }
};


