#include <DHT.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN 24        
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

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

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Initialiseren...");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Ultrasoon meting
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
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
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sensor fout!");
    return;
  }

  float heatIndex = dht.computeHeatIndex(temperature, humidity, false);

  Temperature = round(temperature * 10) / 10.0;
  Humidity    = round(humidity * 10) / 10.0;
  HeatIndex   = round(heatIndex * 10) / 10.0;

  Serial.println("Temp: " + String(Temperature) + " C");
  Serial.println("Humidity: " + String(Humidity) + " %");
  Serial.println("HeatIndex: " + String(HeatIndex) + "\n");

  // LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(Temperature, 1);
  lcd.print(" C");

  lcd.setCursor(0, 1);
  lcd.print("Vocht: ");
  lcd.print(Humidity, 1);
  lcd.print(" %");
}