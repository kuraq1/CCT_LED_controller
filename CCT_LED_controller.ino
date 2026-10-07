const int coldPIN = 33;
const int hotPIN = 32;

void setup() {
  pinMode(coldPIN, OUTPUT);
  pinMode(hotPIN, OUTPUT);
  digitalWrite(coldPIN, LOW);
  digitalWrite(hotPIN, LOW);

}

void loop() {
  digitalWrite(coldPIN, HIGH);
  delay(100);
  digitalWrite(coldPIN, LOW);
  digitalWrite(hotPIN, HIGH);
  delay(100);
  digitalWrite(hotPIN, LOW);
}
