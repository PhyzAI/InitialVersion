
void setup() {
Serial.begin(9600);
}


void loop() {
analogWrite(9, 70);
analogWrite(10, 0);
analogWrite(5, 90);
analogWrite(6, 0);

Serial.println(analogRead(A2));
if (analogRead(A2) < 400){analogWrite(9, 0);}
if (analogRead(A1) < 400){analogWrite(5, 0);}


}
