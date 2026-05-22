#include <DHT.h>

#define DHTPIN 24        
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

const int trig   = 9;
const int echo   = 10;
const int buzzer = 13;

float Temperature = 0;
float Humidity    = 0;
float HeatIndex   = 0;

void setup() {
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long afstand = pulseIn(echo, HIGH) * 0.0343 / 2;
  Serial.println(afstand);

  if (afstand < 7) {
    digitalWrite(buzzer, HIGH);
    delay(500);
    digitalWrite(buzzer, LOW);
    delay(500);
  }
  else if (afstand < 20) {
    digitalWrite(buzzer, HIGH);
    delay(100);
    digitalWrite(buzzer, LOW);
    delay(900);
  }
  else {
    digitalWrite(buzzer, LOW);
  }

  delay(10000);
  ReadDHT11(); 
}

void ReadDHT11() {
  float temperature = dht.readTemperature();
  float humidity    = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("DHT11 sensor error");
    return;
  }

  float heatIndex = dht.computeHeatIndex(temperature, humidity, false);

  Temperature = round(temperature * 10) / 10.0;
  Humidity    = round(humidity * 10) / 10.0;
  HeatIndex   = round(heatIndex * 10) / 10.0;

  Serial.println("Temp: " + String(Temperature) + " C");
  Serial.println("Humidity: " + String(Humidity) + " %");
  Serial.println("HeatIndex: " + String(HeatIndex) + "\n");
}