void setup() {

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.begin(19600);

  int size = 8;
  byte data[] = {0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

  for(int i=0; i<size; i++){
    Serial.write(data[i]);
  }

}
