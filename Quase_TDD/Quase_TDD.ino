#include "interface_sensor.h"


 Sensor sensor(3,Umidade,SOIL_MOISTURE_SENSOR);



SensorDHT sensorDHT(4);

  


 




void setup() {
  Serial.begin(9600);
  Serial.println("#Sensor generico teste#");
  Serial.println("funcao leitura:");
  Serial.print(sensor.leitura());
  Serial.println("leitura do dado da funcao ler:");
  Serial.println("valor:");
  Serial.print(sensor.ler().valor);
  Serial.println("unidade:");
  Serial.print(sensor.ler().unidade);

  Serial.println("");

  Serial.println("#Sensor DHT teste#");
  Serial.println("funcao leitura:");
  Serial.print(sensorDHT.leitura());
  Serial.println("leitura do dado da funcao ler:");
  Serial.println("valor:");
  Serial.print(sensorDHT.ler().valor);
  Serial.println("unidade:");
  Serial.print(sensorDHT.ler().unidade);
}

void loop() {
  // put your main code here, to run repeatedly:

}