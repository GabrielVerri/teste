#include <DHT.h>



class Dado{
public:
  Dado(String valor ){
    this->valor = valor;
    
  }
  
  String valor;

};

class SensorMap {
private:
    int inMax;
    int inMin;
    float outMin;
    float outMax;

public:
    SensorMap(
        int inMax,
        int inMin,
        float outMin,
        float outMax
    ) {
        this->inMax = inMax;
        this->inMin = inMin;
        this->outMin = outMin;
        this->outMax = outMax;
    }

    float mapear(int valor) const {
        return map(valor, inMin, inMax, outMin, outMax);
    }
};

const SensorMap SOIL_MOISTURE_SENSOR(0, 4096, 0, 100);
const SensorMap LDR_SENSOR(0, 4096, 0, 100);


class Isensor{
public:
  virtual  float leitura() = 0;
  virtual Dado* ler() = 0;
};

class Sensor : public Isensor{
private:    
  const int pin;
  
  const SensorMap& mapper;
    
public:
  Sensor(int pin,const SensorMap& mapper) : pin(pin),  mapper(mapper) { pinMode(this->pin, INPUT); }
    
  virtual float leitura(){
    float dado = analogRead(this->pin);
    float leitura = this->mapper.mapear(dado);
    return leitura;
  }
    
  virtual Dado* ler () override {
    Dado* dado; 
    dado->valor = String(this->leitura());
    (dado + 1)->valor = "NULL"; 
    return dado;
  }
};

class SensorDHT : public Isensor{
private:
  DHT dht;
  float temperatura;
  float umidade;

public:
  SensorDHT(int pin) : dht(pin, DHT22) {
    dht.begin();
    
    temperatura;
    umidade;
  }

  float leitura(){
    return 0;
  }

  void verificacaoDHT() {
    umidade = dht.readHumidity();
    temperatura = dht.readTemperature();

    if (isnan(temperatura) || isnan(umidade)) {
      Serial.println("ERRO AO LER O SENSOR DHT");
    }
  }

  virtual Dado* ler() {
    Dado* dado;
     dado->valor =String(lerTemperatura());
     (dado + 1)->valor=String(lerUmidade());
     (dado + 2)->valor="NULL";
    return dado;
  }

  float lerTemperatura() {
    float temperatura = dht.readTemperature();
    return temperatura;
  }

  float lerUmidade() {
    umidade = dht.readHumidity();
    return umidade;
  }
 
};