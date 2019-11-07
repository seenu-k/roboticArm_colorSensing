//Header Files
#include <LiquidCrystal.h>
#include <Servo.h>

//Global Variables
String colors[] = {"R", "G", "B", "W"};
int pins[] = {9, 10, 11};
int maxpins[] = {-1, -1, -1};
int intensity[] = {255, 255, 255};
int tempr[] = {0, 0, 0};
int coLoc[] = {0, 0, 0};
int ldr_pin = A2;
int max_val = 0;
int calibChoice = 0;
int error = 0;
int erfact = 2.5;
int offset = 20;
int pick = -1;
int drop = -1;
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
int findMinimumIndexLong(long int a[], int n) {
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
  int colRead[29];
  int clustUniv[15][2];
  int clustSort[3][2];
  int score[3][3];
  int pos, i, j, k, count, maxNum, maxRow;
  long int devia = 0;
  int col = 3;
  long int readval[] = {0, 0, 0};
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
      col = findMinimumIndexLong(readval, 3);
      //if(coLoc[col]==0){
      //  coLoc[col]=base.read();
      //}
    }
    else{
      col = 3;
    }
    colRead[29-((pos/5)-7)]=col;
    //Serial.println(col);
    lcd.setCursor(0, 1);
    //Serial.println(colors[col]);
    lcd.print(colors[col]);
    delay(500);
  }
  for(i=0; i<15; i++){
    for(j=0; j<2; j++){
      clustUniv[i][j]=-1;
    }
  }
  count = 0;
  for(i=0; i<28; i++){
    if((colRead[i]==3&&colRead[i+1]==3) || (colRead[i]!=3&&colRead[i+1]!=3)){
      continue;
    }
    if(colRead[i]==3){
      clustUniv[count][0] = i+1;
    }
    else{
      clustUniv[count][1] = i;
      count++;
    }
  }
  if(colRead[28]!=3){
    clustUniv[count][1]=28;
  }
  for(j = 0; j<3; j++){
    maxNum = -1;
    maxRow = -1;
    for(i=0; i<15; i++){
      if(clustUniv[i][1]-clustUniv[i][0]+1>maxNum){
        maxNum = clustUniv[i][1]-clustUniv[i][0]+1;
        maxRow = i;
      }
    }
    clustSort[j][0]=clustUniv[maxRow][0];
    clustSort[j][1]=clustUniv[maxRow][1];
    clustUniv[maxRow][0]=-1;
    clustUniv[maxRow][1]=-1;
  }
  for(count = 0; count<3; count++){
    i = 0;
    j = 0;
    k = 0;
    for(maxNum = clustSort[count][0]; maxNum<clustSort[count][1]+1; maxNum++){
      if(colRead[maxNum]==0){
        i++;
      }
      if(colRead[maxNum]==1){
        j++;
      }
      if(colRead[maxNum]==2){
        k++;
      }
    }
    score[0][count] = (1000*i)/(clustSort[count][1]-clustSort[count][0]+1);
    score[1][count] = (1000*j)/(clustSort[count][1]-clustSort[count][0]+1);
    score[2][count] = (1000*k)/(clustSort[count][1]-clustSort[count][0]+1);
  }
  for(k=0; k<3; k++){
    maxNum = -1;
    maxRow = -1;
    count = -1;
    for(i=0; i<3; i++){
      for(j=0; j<3; j++){
        if(score[i][j]>maxNum){
          maxNum = score[i][j];
          maxRow = i;
          count = j;
        }
      }
    }
    coLoc[maxRow] = 5*(36-((clustSort[count][0]+clustSort[count][1])/2));
    for(i=0; i<3; i++){
      for(j=0; j<3; j++){
        if(i==maxRow || j==count){
          score[i][j]=-1;
        }
      }
    }
  }
}

void pickAndDrop(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(coLoc[0]);
  lcd.print(" ");
  lcd.print(coLoc[1]);
  lcd.setCursor(0, 1);
  lcd.print(coLoc[2]);
  delay(2000);
  base.attach(5);
  base.write(180);
  delay(1000);
  base.write(coLoc[pick]-offset);
  delay(1000);
  base.detach();
  delay(200);
  arm.attach(A3);
  arm.write(90);
  delay(1000);
  while(digitalRead(A1)==false){
    delay(200);
  }
  
//  while(pos<=130){
//    arm.write(pos);
//    delay(500);
//    if(digitalRead(A1)==true){
//      break;
//    }
//    pos++;
//  }
  digitalWrite(7, HIGH);
  delay(5000);
  arm.write(0);
  delay(1000);
  arm.detach();
  base.attach(5);
  base.write(coLoc[drop]-offset);
  delay(1000);
  base.detach();
  delay(200);
  arm.attach(A3);
//  pos=0;
//  while(pos<=130){
//    arm.write(pos);
//    delay(500);
//    if(digitalRead(A1)==true){
//      break;
//    }
//    pos++;
//  }
  arm.write(90);
  delay(1000);
  digitalWrite(7, LOW);
  delay(1000);
  arm.write(0);
  delay(1000);
  arm.detach();
  delay(200);
  base.attach(5);
  base.write(180);
  delay(1000);
  base.detach();
  delay(200);
}

void setup() {
  //Attach LDR, LED, EM, IR
  pinMode(ldr_pin, INPUT);
  pinMode(pins[0], OUTPUT);
  pinMode(pins[1], OUTPUT);
  pinMode(pins[2], OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(A1, INPUT);
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
  delay(1000);
  lcd.clear();
  if(error==0){
    lcd.setCursor(0, 0);
    lcd.print("Calc Error");
    delay(500);
    lcd.setCursor(0, 1);
    analogWrite(pins[0], intensity[0]);
    analogWrite(pins[1], 0);
    analogWrite(pins[2], 0);
    delay(100);
    //Serial.print(analogRead(ldr_pin));
    tempr[0] = analogRead(ldr_pin);
    lcd.print(colors[0]);
    lcd.print(tempr[0]);
    lcd.print(" ");
    delay(1000);
    analogWrite(pins[0], 0);
    analogWrite(pins[1], intensity[1]);
    analogWrite(pins[2], 0);
    //Serial.print('\t');
    delay(100);
    //Serial.print(analogRead(ldr_pin));
    tempr[1] = analogRead(ldr_pin);
    lcd.print(colors[1]);
    lcd.print(tempr[1]);
    lcd.print(" ");
    delay(1000); 
    analogWrite(pins[0], 0);
    analogWrite(pins[1], 0);
    analogWrite(pins[2], intensity[2]);
    //Serial.print('\t');
    delay(100);
    //Serial.println(analogRead(ldr_pin));
    tempr[2] = analogRead(ldr_pin);
    lcd.print(colors[2]);
    lcd.print(tempr[2]);
    lcd.print(" ");
    error = sqrt(sq(tempr[0]-tempr[1])+sq(tempr[1]-tempr[2])+sq(tempr[0]-tempr[2]));
    error*=erfact;
    //Serial.println(error);
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 0);
  }


  lcd.clear();
  lcd.print("Pick Color?");
  delay(1000);
  lcd.clear();
  lcd.print("Hold 8 tochoose");
  delay(1000);
  lcd.clear();
  lcd.print("Red");
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
    pick=0;
  }
  if(pick!=0){
    delay(1000);
    pinMode(8, INPUT);
    digitalWrite(8, LOW);
    pinMode(8, OUTPUT);
    lcd.print("Green");
    delay(1000);
    lcd.clear();
    pinMode(8, INPUT_PULLUP);
    delay(2000);
    if(digitalRead(8)==LOW){
      pinMode(8, INPUT);
      digitalWrite(8, LOW);
      pinMode(8, OUTPUT);
      //Serial.println("Auto");
      pick = 1;
    }
    if(pick!=1){
      delay(1000);
      pinMode(8, INPUT);
      digitalWrite(8, LOW);
      pinMode(8, OUTPUT);
      lcd.print("Blue");
      delay(1000);
      lcd.clear();
      pinMode(8, INPUT_PULLUP);
      delay(2000);
      if(digitalRead(8)==LOW){
        pinMode(8, INPUT);
        digitalWrite(8, LOW);
        pinMode(8, OUTPUT);
        //Serial.println("Auto");
        pick = 2;
      }
    }
  }
  delay(1000);
  pinMode(8, INPUT);
  digitalWrite(8, LOW);
  pinMode(8, OUTPUT);
  lcd.print("No Choice Chosen");
  delay(1000);

  
  lcd.clear();
  lcd.print("Drop Color?");
  delay(1000);
  lcd.clear();
  lcd.print("Hold 8 tochoose");
  delay(1000);
  lcd.clear();
  lcd.print("Red");
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
    drop=0;
  }
  if(drop!=0){
    delay(1000);
    pinMode(8, INPUT);
    digitalWrite(8, LOW);
    pinMode(8, OUTPUT);
    lcd.print("Green");
    delay(1000);
    lcd.clear();
    pinMode(8, INPUT_PULLUP);
    delay(2000);
    if(digitalRead(8)==LOW){
      pinMode(8, INPUT);
      digitalWrite(8, LOW);
      pinMode(8, OUTPUT);
      //Serial.println("Auto");
      drop = 1;
    }
    if(drop!=1){
      delay(1000);
      pinMode(8, INPUT);
      digitalWrite(8, LOW);
      pinMode(8, OUTPUT);
      lcd.print("Blue");
      delay(1000);
      lcd.clear();
      pinMode(8, INPUT_PULLUP);
      delay(2000);
      if(digitalRead(8)==LOW){
        pinMode(8, INPUT);
        digitalWrite(8, LOW);
        pinMode(8, OUTPUT);
        //Serial.println("Auto");
        drop = 2;
      }
    }
  }
  delay(1000);
  pinMode(8, INPUT);
  digitalWrite(8, LOW);
  pinMode(8, OUTPUT);
  lcd.print("No Choice Chosen");
  delay(1000);
  if(!(pick==-1 || drop==-1)){
    detectColor();
    pickAndDrop();
  }
  max_val = 0;
  calibChoice = 0;
  pick = -1;
  drop = -1;
  tempr[0]=0;
  tempr[1]=0;
  tempr[2]=0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Thank you");
  delay(4000);
}
