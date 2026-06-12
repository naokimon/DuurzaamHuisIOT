#include <DHT.h>
#include <LiquidCrystal_I2C.h>

// ---------- Pin-configuratie ----------
#define DHTPIN_IN   24      // DHT11 Binnen (mogelijk checken)
#define DHTPIN_OUT  25      // DHT11 buiten  (mogelijk nog checken)
#define DHTTYPE     DHT11

const int trig   = 9;
const int echo   = 10;
const int buzzer = 13;

// pinnen voor zonnepanelen en windmolen
const int SOLAR_A_PIN = A5;
const int SOLAR_B_PIN = A4;
const int WIND_PIN    = A6;

// ---------- Objecten ----------
DHT dhtIn(DHTPIN_IN, DHTTYPE);
DHT dhtOut(DHTPIN_OUT, DHTTYPE);
LiquidCrystal_I2C LCD(0x27, 16, 2);

// ---------- Globale waarden ----------
float TempInside  = 0;
float TempOutside = 0;

void setup() {
  Serial.begin(9600);

  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(buzzer, OUTPUT);

  dhtIn.begin();
  dhtOut.begin();

  LCD.init();
  LCD.backlight();
  LCD.setCursor(0, 0);
  LCD.print("Initialiseren...");
  delay(2000);
  LCD.clear();
}

void loop() {
  // Toon temperatuur ~5s, dan zonne-energie ~5s, dan windenergie ~5s.
  // Elke "tick" duurt ~50 ms, 300 ticks ~= 15 seconden.
  for (uint16_t i = 0; i < 300; i++) {
    if (i == 0) {
      LCD.clear();
      DisplayTemperature();
    }
    else if (i == 100) {
      LCD.clear();
      DisplaySolarPower();
    }
    else if (i == 200) {
      LCD.clear();
      DisplayWindPower();
    }

    // Afstand meten en eventueel piepen
    if (ReadUltrasonicSensor() < 20) {
      digitalWrite(buzzer, HIGH);   // buzzer aan
      delay(5);
      digitalWrite(buzzer, LOW);    // buzzer uit
      delay(45);
    }
    else {
      delay(50);
    }
  }
}

float ReadUltrasonicSensor() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  // Echo meten met timeout (30 ms) zodat de loop niet vastloopt
  long duration = pulseIn(echo, HIGH, 30000);

  float distance = duration * 0.0343 / 2.0;

  // Geen echo (timeout) -> behandel als ver weg, niet als 0 cm
  if (distance <= 0) distance = 999;

  return distance;
}

void DisplayTemperature() {
  // Lees binnen- & buitentemperatuur van de DHT11's
  ReadDHT11();

  // Toon op het LCD
  LCD.setCursor(0, 0);
  LCD.print("Temp In: ");
  LCD.print(TempInside, 1);
  LCD.print("c");

  LCD.setCursor(0, 1);
  LCD.print("Temp Out: ");
  LCD.print(TempOutside, 1);
  LCD.print("c");
}

void ReadDHT11() {
  float tIn  = dhtIn.readTemperature();
  float tOut = dhtOut.readTemperature();

  if (!isnan(tIn))  TempInside  = round(tIn  * 10) / 10.0;
  if (!isnan(tOut)) TempOutside = round(tOut * 10) / 10.0;

  Serial.println("Temp In: "  + String(TempInside)  + " C");
  Serial.println("Temp Out: " + String(TempOutside) + " C");
}

void DisplaySolarPower() {
  int solarA = analogRead(SOLAR_A_PIN);
  int solarB = analogRead(SOLAR_B_PIN);

  LCD.setCursor(0, 0);
  LCD.print("Solar A: ");
  LCD.print(solarA);

  LCD.setCursor(0, 1);
  LCD.print("Solar B: ");
  LCD.print(solarB);
}

void DisplayWindPower() {
  int wind = analogRead(WIND_PIN);

  LCD.setCursor(0, 0);
  LCD.print("Windmill: ");
  LCD.print(wind);
}