int count = 0;
int i = 0;
void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

  count = random(1, 50);
  i = 0;

  while (i < count){

    tone(10, random(30, 4978), 250);
    delay(random(20, 100));
    i = i + 1;

  }
  
  delay(random(0, 15000));

}
