#define bot1 13
#define bot2 12
#define bot3 11
#define led1 8
#define led2 9
#define led3 10
int aled;
int a,b;
int score;
const int buz = 7;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(buz, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(bot1, INPUT_PULLUP);
  pinMode(led2, OUTPUT);
  pinMode(bot2, INPUT_PULLUP);
  pinMode(led3, OUTPUT);
  pinMode(bot3, INPUT_PULLUP);
  score = 50;

}

void loop() {
  // put your main code here, to run repeatedly:
  if((score >1) && (score<100)){
    Serial.print("easy");
    a = 10000;
    b = 1000;
  }
  else if(score == 100){
    Serial.print("next level");
    digitalWrite(buz, HIGH); // poner el Pin en HIGH
    delay(5000); // esperar 1 segundos
    digitalWrite(buz, LOW); // poner el Pin en LOW
    delay(1000);
    Serial.print("score: ");
    Serial.println(score);
  }
  else if((101<score) && (score<300)){
    Serial.print("medium");
    a = 5000;
    b = 500;
  }
  else if(score == 300){
    Serial.print("next level");
    digitalWrite(buz, HIGH); // poner el Pin en HIGH
    delay(5000); // esperar 1 segundos
    digitalWrite(buz, LOW); // poner el Pin en LOW
    delay(1000);
    Serial.print("score: ");
    Serial.println(score);
  }
  else if(301<score){
    Serial.print("hard");
    a = 2500;
    b = 250;
  }
  Serial.print("led");
  unsigned long startime = millis();
  aled = random(1, 4);
  if(aled == 1){
    Serial.println('1');
    digitalWrite(led1, HIGH);
    digitalWrite(buz, HIGH); // poner el Pin en HIGH
    delay(1000); // esperar 1 segundos
    digitalWrite(buz, LOW); // poner el Pin en LOW
    delay(1000);
    while ((millis() - startime)<a){
      int val1 = digitalRead(bot1);
      if(val1 == 0){
        digitalWrite(led1, LOW);
        score = score +15;
        break;
      }
      
    }
    digitalWrite(led1, LOW);
    digitalWrite(buz, HIGH); // poner el Pin en HIGH
    delay(500); // esperar 1 segundos
    digitalWrite(buz, LOW); // poner el Pin en LOW
    delay(500);
    score = score -5;
    Serial.print("score: ");
    Serial.println(score);
  }
  else if(aled == 2){
    Serial.println('2');
    digitalWrite(led2, HIGH);
    digitalWrite(buz, HIGH); // poner el Pin en HIGH
    delay(1000); // esperar 1 segundos
    digitalWrite(buz, LOW); // poner el Pin en LOW
    delay(1000);
    while ((millis() - startime)<a){
      int val2 = digitalRead(bot2);
      if(val2 == 0){
        digitalWrite(led2, LOW);
        score = score +15;
        break;
      }
      
    }
    digitalWrite(led2, LOW);
    digitalWrite(buz, HIGH); // poner el Pin en HIGH
    delay(500); // esperar 1 segundos
    digitalWrite(buz, LOW); // poner el Pin en LOW
    delay(500);
    score = score -5;
    Serial.print("score: ");
    Serial.println(score);
  }
  else if(aled == 3){
    Serial.println('3');
    digitalWrite(led3, HIGH);
    digitalWrite(buz, HIGH); // poner el Pin en HIGH
    delay(1000); // esperar 1 segundos
    digitalWrite(buz, LOW); // poner el Pin en LOW
    delay(1000);
    while ((millis() - startime)<a){
      int val3 = digitalRead(bot3);
      if(val3 == 0){
        digitalWrite(led3, LOW);
        score = score +15;
        break;
      }
      
    }
    digitalWrite(led3, LOW);
    digitalWrite(buz, HIGH); // poner el Pin en HIGH
    delay(500); // esperar 1 segundos
    digitalWrite(buz, LOW); // poner el Pin en LOW
    delay(500);
    score = score -5;
    Serial.print("score: ");
    Serial.println(score);
  }
  delay(b);
  if (score<=0){
    Serial.print("you lose");
    digitalWrite(buz, HIGH); // poner el Pin en HIGH
    delay(5000); // esperar 1 segundos
    digitalWrite(buz, LOW); // poner el Pin en LOW
    delay(1000);
    Serial.print("score: ");
    Serial.println(score, DEC);
    while(true){
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
    }
  }

}
