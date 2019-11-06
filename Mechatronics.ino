//Header Files
#include <LiquidCrystal.h>
#include <Servo.h>

//Global Variables
String colors[] = {"R", "G", "B", "W"};
int pins[] = {9, 10, 11};
int maxpins[] = {-1, -1, -1};
int intensity[] = {255, 255, 255};
int ldr_pin = A2;
int max_val = 0;
int calibChoice = 0;
int error = 0;
int erfact = 3;
LiquidCrystal lcd(2, 3, 4, 8, 12, 13);
Servo base, arm;

//Generic Math Functions
int findMinimumIndex(int a[], int n) {
  int c, mini, index;
  mini = a[0];
  index = 0;
  for(c = 1; c < n; c++){
    if(a[c] < mini){
       index = c;
       mini = a[c];
    }
  }
  return index;
}


//Calibration Functions
void autocalib(){
  int r_val = 0, g_val = 0, b_val = 0, ldr_val, x, y, i;
  int temparr[] = {0, 0, 0, 0};
  lcd.setCursor(0,0);
  analogWrite(pins[0], 255);
  analogWrite(pins[1], 0);
  analogWrite(pins[2], 0);
  delay(100);
  r_val = analogRead(ldr_pin);
  //Serial.print(r_val);
  lcd.print(colors[0]);
  lcd.print(r_val);
  lcd.print(" ");
  delay(1000);
  analogWrite(pins[0], 0);
  analogWrite(pins[1], 255);
  analogWrite(pins[2], 0);
  //Serial.print('\t');
  delay(100);
  g_val = analogRead(ldr_pin);
  //Serial.print(g_val);
  lcd.print(colors[1]);
  lcd.print(g_val);
  lcd.print(" ");
  delay(1000); 
  analogWrite(pins[0], 0);
  analogWrite(pins[1], 0);
  analogWrite(pins[2], 255);
  //Serial.print('\t');
  delay(100);
  b_val = analogRead(ldr_pin);
  //Serial.println(b_val);
  lcd.print(colors[2]);
  lcd.print(b_val);
  lcd.print(" ");
  delay(1000);

  if(r_val >= b_val){
    if(r_val >= g_val){
      maxpins[0] = 0;
      maxpins[1] = 1;
      maxpins[2] = 2;
      max_val = r_val;
    }
    else{
      maxpins[0] = 1;
      maxpins[1] = 0;
      maxpins[2] = 2;
      max_val = g_val;
    }
  }
  else{
    if(b_val >= g_val){
      maxpins[0] = 2;
      maxpins[1] = 0;
      maxpins[2] = 1;
      max_val = b_val;
    }
    else{
      maxpins[0] = 1;
      maxpins[1] = 0;
      maxpins[2] = 2;
      max_val = g_val;
    }
  }
  //Serial.println(ch0);
  //Serial.println(ch1);
  //Serial.println(ch2);
  //Serial.println(max_val);
  ldr_val=0;
  for(x=255;ldr_val<max_val;x-=2){
    analogWrite(pins[maxpins[0]], 0);
    analogWrite(pins[maxpins[2]], 0);
    analogWrite(pins[maxpins[1]], x);
    delay(100);
    ldr_val = analogRead(ldr_pin);
    //Serial.println(x);
    //Serial.println(ldr_val);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Calibrating...");
    lcd.setCursor(0, 1);
    lcd.print("Inp:");
    lcd.print(x);
    lcd.print(" LDR:");
    lcd.print(ldr_val);
    analogWrite(pins[maxpins[0]], 0);
    analogWrite(pins[maxpins[2]], 0);
    analogWrite(pins[maxpins[1]], 0);
    delay(200);
  }
  for(i=0; i<4; i++){
    analogWrite(pins[maxpins[0]], 0);
    analogWrite(pins[maxpins[2]], 0);
    analogWrite(pins[maxpins[1]], x+(2*(i-1)));
    delay(100);
    temparr[i] = analogRead(ldr_pin);
    temparr[i]=temparr[i]-max_val;
    temparr[i]=abs(temparr[i]);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Fine tuning...");
    lcd.setCursor(0, 1);
    lcd.print("Inp:");
    lcd.print(i);
    lcd.print(" Err:");
    lcd.print(temparr[i]);
    analogWrite(pins[maxpins[0]], 0);
    analogWrite(pins[maxpins[2]], 0);
    analogWrite(pins[maxpins[1]], 0);
    delay(200);
  }
  x += 2*(findMinimumIndex(temparr, 4)-1);
  
  ldr_val=0;
  for(y=255;ldr_val<max_val;y-=2){
    analogWrite(pins[maxpins[2]], y);
    analogWrite(pins[maxpins[0]], 0);
    analogWrite(pins[maxpins[1]], 0);
    delay(100);
    ldr_val = analogRead(ldr_pin);
    //Serial.println(y);
    //Serial.println(ldr_val);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Calibrating...");
    lcd.setCursor(0, 1);
    lcd.print("Inp:");
    lcd.print(y);
    lcd.print(" LDR:");
    lcd.print(ldr_val);
    analogWrite(pins[maxpins[2]], 0);
    analogWrite(pins[maxpins[0]], 0);
    analogWrite(pins[maxpins[1]], 0);
    delay(200);
  }
  for(i=0; i<4; i++){
    analogWrite(pins[maxpins[2]], y+(2*(i-1)));
    analogWrite(pins[maxpins[0]], 0);
    analogWrite(pins[maxpins[1]], 0);
    delay(100);
    temparr[i] = analogRead(ldr_pin);
    temparr[i]=temparr[i]-max_val;
    temparr[i]=abs(temparr[i]);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Fine tuning...");
    lcd.setCursor(0, 1);
    lcd.print("Inp:");
    lcd.print(i);
    lcd.print(" Err:");
    lcd.print(temparr[i]);
    analogWrite(pins[maxpins[2]], 0);
    analogWrite(pins[maxpins[0]], 0);
    analogWrite(pins[maxpins[1]], 0);
    delay(200);
  }
  y += 2*(findMinimumIndex(temparr, 4)-1);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Successful");
  lcd.setCursor(0, 1);

  intensity[maxpins[0]]=255;
  intensity[maxpins[1]]=x;
  intensity[maxpins[2]]=y;
  
  analogWrite(pins[0], intensity[0]);
  analogWrite(pins[1], 0);
  analogWrite(pins[2], 0);
  delay(100);
  //Serial.print(analogRead(ldr_pin));
  temparr[0] = analogRead(ldr_pin);
  lcd.print(colors[0]);
  lcd.print(temparr[0]);
  lcd.print(" ");
  delay(1000);
  analogWrite(pins[0], 0);
  analogWrite(pins[1], intensity[1]);
  analogWrite(pins[2], 0);
  //Serial.print('\t');
  delay(100);
  //Serial.print(analogRead(ldr_pin));
  temparr[1] = analogRead(ldr_pin);
  lcd.print(colors[1]);
  lcd.print(temparr[1]);
  lcd.print(" ");
  delay(1000); 
  analogWrite(pins[0], 0);
  analogWrite(pins[1], 0);
  analogWrite(pins[2], intensity[2]);
  //Serial.print('\t');
  delay(100);
  //Serial.println(analogRead(ldr_pin));
  temparr[2] = analogRead(ldr_pin);
  lcd.print(colors[2]);
  lcd.print(temparr[2]);
  lcd.print(" ");
  error = sqrt(sq(temparr[0]-temparr[1])+sq(temparr[1]-temparr[2])+sq(temparr[0]-temparr[2]));
  error*=erfact;
  //Serial.println(error);
  delay(1000);
}

void manualcalib(){
  int temparr[] = {0, 0, 0};
  intensity[0]=255;
  intensity[1]=255;
  intensity[2]=255;
  while(true){
    analogWrite(pins[0], intensity[0]);
    analogWrite(pins[1], 0);
    analogWrite(pins[2], 0);
    delay(100);
    temparr[0] = analogRead(ldr_pin);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("R");
    lcd.print(temparr[0]);
    lcd.print(" ");
    //Serial.print(analogRead(LDR_PIN));
    delay(1000);
    analogWrite(pins[0], 0);
    analogWrite(pins[1], intensity[1]);
    analogWrite(pins[2], 0);
    //Serial.print('\t');
    delay(100);
    temparr[1] = analogRead(ldr_pin);
    lcd.print("G");
    lcd.print(temparr[1]);
    lcd.print(" ");
    //Serial.print(analogRead(LDR_PIN));
    delay(1000); 
    analogWrite(pins[0], 0);
    analogWrite(pins[1], 0);
    analogWrite(pins[2], intensity[2]);
    //Serial.print('\t');
    delay(100);
    temparr[2] = analogRead(ldr_pin);
    lcd.print("B");
    lcd.print(temparr[2]);
    //Serial.println(analogRead(LDR_PIN));
    error = sqrt(sq(temparr[0]-temparr[1])+sq(temparr[1]-temparr[2])+sq(temparr[0]-temparr[2]));
    error*=erfact;
    delay(1000);
  }
}

void detectColor(){
  int pos;
  int devia = 0;
  int col = 3;
  int readval[] = {0, 0, 0};
  base.attach(5);
  base.write(180);
  delay(1000);
  base.detach();
  for(pos=180; pos>=40; pos-=5){
    lcd.clear();
    lcd.setCursor(0, 0);
    base.attach(5);
    base.write(pos);
    delay(500);
    base.detach();
    delay(20);
    analogWrite(pins[0], intensity[0]);
    analogWrite(pins[1], 0);
    analogWrite(pins[2], 0);
    delay(100);
    readval[0] = analogRead(ldr_pin);
    //Serial.print(analogRead(ldr_pin));
    lcd.print(colors[0]);
    lcd.print(readval[0]);
    lcd.print(" ");
    delay(500);
    analogWrite(pins[0], 0);
    analogWrite(pins[1], intensity[1]);
    analogWrite(pins[2], 0);
    //Serial.print('\t');
    delay(100);
    readval[1] = analogRead(ldr_pin);
    //Serial.print(analogRead(ldr_pin));
    lcd.print(colors[1]);
    lcd.print(readval[1]);
    lcd.print(" ");
    delay(500); 
    analogWrite(pins[0], 0);
    analogWrite(pins[1], 0);
    analogWrite(pins[2], intensity[2]);
    //Serial.print('\t');
    delay(100);
    //Serial.println(analogRead(ldr_pin));
    readval[2] = analogRead(ldr_pin);
    lcd.print(colors[2]);
    lcd.print(readval[2]);
    lcd.print(" ");
    devia = sqrt(sq(readval[0]-readval[1])+sq(readval[1]-readval[2])+sq(readval[0]-readval[2]));
    //Serial.println(devia);
    if(devia>=error){
      col = findMinimumIndex(readval, 3);
    }
    else{
      col = 3;
    }
    //Serial.println(col);
    lcd.setCursor(0, 1);
    //Serial.println(colors[col]);
    lcd.print(colors[col]);
    delay(500);
  }
}

void setup() {
  //Attach LDR, LED
  pinMode(ldr_pin, INPUT);
  pinMode(pins[0], OUTPUT);
  pinMode(pins[1], OUTPUT);
  pinMode(pins[2], OUTPUT);
  //Attach Serial Monitor
  //Serial.begin(9600);
  //Attach LCD
  lcd.begin(16, 2);
  //Positioning Base Servo
  base.attach(5);
  base.write(180);
  delay(1000);
  base.detach();
}

void loop() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("EDITH. Welcome.");
  delay(1000);
  lcd.clear();
  lcd.print("Callibrate?");
  delay(1000);
  lcd.clear();
  lcd.print("Hold 8 tochoose");
  delay(1000);
  lcd.clear();
  lcd.print("Manual Calib");
  delay(1000);
  lcd.clear();
  pinMode(8, INPUT_PULLUP);
  delay(2000);
  if(digitalRead(8)==LOW){
    delay(1000);
    pinMode(8, INPUT);
    digitalWrite(8, LOW);
    pinMode(8, OUTPUT);
    //Serial.println("Manual");
    calibChoice=1;
    manualcalib();
  }
  if(calibChoice!=1){
    delay(1000);
    pinMode(8, INPUT);
    digitalWrite(8, LOW);
    pinMode(8, OUTPUT);
    lcd.print("Automatic Calib");
    delay(1000);
    lcd.clear();
    pinMode(8, INPUT_PULLUP);
    delay(2000);
    if(digitalRead(8)==LOW){
      pinMode(8, INPUT);
      digitalWrite(8, LOW);
      pinMode(8, OUTPUT);
      //Serial.println("Auto");
      calibChoice=2;
      autocalib();
    }
  }
  delay(1000);
  pinMode(8, INPUT);
  digitalWrite(8, LOW);
  pinMode(8, OUTPUT);
  lcd.print("No Choice Chosen");

  detectColor();
  
  delay(100000);
}
