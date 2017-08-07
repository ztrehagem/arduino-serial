#define DATA_LEN 3

byte data[DATA_LEN];

void setup() {
  Serial.begin(9600);

  int i;
  for (i = 0; i < DATA_LEN; i++) {
    data[i] = 0x61 + i;
  }
}

void loop() {
  Serial.write(data, DATA_LEN);
  int i, rd;

  if (Serial.available() >= 3) {
    rd = Serial.read();
    data[0] = rd & 0xff;
  }

//  Serial.println("Hello World!!!");
  delay(1000);

}
