const int  trigPin = 9;
const int echoPin = 10;
const int buzzerPin = 13;
float duration, distance;


void setup() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.0343)/2;
  if (distance < 20) {
    digitalWrite(buzzerPin, 1000);
    delay(100);
    noTone(buzzerPin);
    delay(900);
  } else if (distance < 7) {
    digitalWrite(buzzerPin, 1000);
    delay(500);
    noTone(buzzerPin);
    delay(500);
  } else {
    noTone(buzzerPin);
  }
  Serial.print("Distance: ");  
	Serial.println(distance);
	delay(100);
}
