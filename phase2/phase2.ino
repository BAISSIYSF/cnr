#include<Servo.h>
#include<ColorPAL.h>
#include<Ultrasonic.h>

Servo myservo1;
Servo myservo2;

ColorPAL sensorPal;

Ultrasonic distance(A4,A5);

int ls = 11;
int l1 = 9;
int l2 = 10;
int r1 = 8;
int r2 = 7;
int rs = 6;
int c1 = 13;
int c2 = 12;
int c3 = 5;
int c4 = 4;
int c5 = 3;
int sensor1 ;
int sensor2 ;
int sensor3 ;
int sensor4 ;
int sensor5 ;
int node=0;
int nodeerr;
int p=0;

bool v1;
bool v2;
bool v3;
bool v4;
bool v5;

//PID constants
const float Kp = 2.5; // Proportional constant
const float Kd = 1.5; // Derivative constant
const float Ki = 1; // Integral constant

//define variables for PID
float error = 0;
float previous_error = 0;
float integral = 0;
float derivative = 0;
float pid_value = 0;

char cmd;

char color;

struct track
{
    /* data */
    char zone ;
    struct track* forward;
    struct track* back;
    struct track* right;
    struct track* left;

};


struct track** get_dirs(struct track* atrack){
    struct track** dirs = new track*[3]{
    atrack->forward,
    atrack->right,
    atrack->left
    };
    return dirs;
}

static int k = 0;
bool get_path_to(struct track* atrack, String path[], char zone){
    
    if(atrack == nullptr){
        return false;
    } else if(atrack->zone == zone){
        return true;
    } 
    struct track** dirs = get_dirs(atrack);
    for(int i=0; i<3; i++){
        if(dirs[i] == nullptr){
            continue;
        } else {
            bool ch = get_path_to(dirs[i], path, zone);
            if(ch){
                if(i==0){
                    path[k++]="forward";
                    delete[] dirs;
                    return true;
                } else if(i==1){
                    path[k++]="right";
                    delete[] dirs;
                    return true;
                } else if(i==2){
                    path[k++]="left";
                    delete[] dirs;
                    return true;
                }
            }
        }
    }
    delete[] dirs;
    return false;
}

  // Objects Creation
    struct track* track1 = new track();
    struct track* track2 = new track();
    struct track* track3 = new track();
    struct track* track4 = new track();
    struct track* track5 = new track();
    struct track* track6 = new track();


char getCmd(){

  char cmd;
  while(true){
  if(Serial.available()){
    cmd = Serial.read();
    return cmd;
  }
  }
}
String path[2];
void setup() {
    // Zone Attribute Assignement
    track1->zone = '\0';
    track2->zone = 'c';
    track3->zone = 'd';
    track4->zone = '\0';
    track5->zone = 'a';
    track6->zone = 'b';

    // ForWard Object assignement
    track1->forward = track4;
    track2->forward = nullptr;
    track3->forward = nullptr;
    track4->forward = nullptr;
    track5->forward = nullptr;
    track6->forward = nullptr;

    // Back Object assignement
    track1->back = nullptr;
    track2->back = track3;
    track3->back = track2;
    track4->back = track1;
    track5->back = track6;
    track6->back = track5;

    // Right Object assignement
    track1->right = track3;
    track2->right = nullptr;
    track3->right = nullptr;
    track4->right = track6;
    track5->right = nullptr;
    track6->right = nullptr;

    // Left Object assignement
    track1->left = track2;
    track2->left = nullptr;
    track3->left = nullptr;
    track4->left = track5;
    track5->left = nullptr;
    track6->left = nullptr;
    Serial.begin(9600);
    pinMode(c1, INPUT);
    pinMode(c2, INPUT);
    pinMode(c3, INPUT);
    pinMode(c4, INPUT);
    pinMode(c5, INPUT);
    pinMode(l1, OUTPUT);
    pinMode(l2, OUTPUT);
    pinMode(ls, OUTPUT);
    pinMode(r1, OUTPUT);
    pinMode(r2, OUTPUT);
    pinMode(rs, OUTPUT);
    digitalWrite(l1, 0);
    digitalWrite(l2, 1);
    digitalWrite(r1, 0);
    digitalWrite(r2, 1);
    analogWrite(rs, 0);
    analogWrite(ls, 0);
    myservo1.attach(A0);
    myservo2.attach(A1);
    myservo1.write(75);
    myservo2.write(50);

    sensorPal.attachPAL(A3);
    
  cmd=getCmd();
  get_path_to(track1,path, cmd);

  int n = (sizeof(path) / sizeof(path[0]));
  for (int i = 0; i < n / 2; i++) {
    String temp = path[i];
    path[i] = path[n - i - 1];
    path[n - i - 1] = temp;
  }

  delay(3000);
}

char t='n';
char x='y';
void readSensors(){
  sensor1 = digitalRead(c1);
  sensor2 = digitalRead(c2);
  sensor3 = digitalRead(c3);
  sensor4 = digitalRead(c4);
  sensor5 = digitalRead(c5);
}

void setBackPath(){
  if(path[0]=="left"||path[1]=="left"){
    path[0]="right";
    path[1]="forward";
  }else{
    path[0]="left";
    path[1]="forward";
  }
}

void rotate(){
    p=1;
    digitalWrite(l1, 0);
    digitalWrite(l2, 1);
    digitalWrite(r1, 0);
    digitalWrite(r2, 1);
    analogWrite(rs,200);
    analogWrite(ls,100);
    delay(1000);
    digitalWrite(l1, 0);
    digitalWrite(l2, 1);
    digitalWrite(r1, 1);
    digitalWrite(r2, 0);
    analogWrite(rs,200);
    analogWrite(ls,100);
    delay(2700);
    do{
      readSensors();
      digitalWrite(l1, 0);
      digitalWrite(l2, 1);
      digitalWrite(r1, 1);
      digitalWrite(r2, 0);
      analogWrite(rs,200);
      analogWrite(ls,100);
    }while(!(sensor1 && sensor2 && !sensor3 && sensor4 && sensor5));
    digitalWrite(l1, 0);
    digitalWrite(l2, 1);
    digitalWrite(r1, 0);
    digitalWrite(r2, 1);
    analogWrite(rs,150);
    analogWrite(ls,100);
}

char getColor(double r, double g, double b){
//  String ccolor;
  double black=(0.00447789*r)+ (-0.00578627*g) + (0.04360736*b);
  double maron=(-0.19773801*r)+ (-2.02820053*g) + (0.00373707*b);
  double yellow=( 0.00451086*r)+ (1.13107215*g) + ( -0.93245945*b);
  double red=( 0.29122624 *r)+ (-0.52551881*g) + (0.00371039*b);
  double green=(-1.68518845*r)+ ( 3.14986746*g) + (-1.01124192*b);
  double blue=(-0.751246 *r)+ ( -0.28274866*g) + ( 1.21217567*b);
  double num[6]={black,maron,yellow,red,green,blue};
  double maxNum = num[0];
  int maxNumidx = 0;
  for (int i = 1; i < 6; i++) {
    if (num[i] > maxNum) {
      maxNum = num[i];
      maxNumidx = i;
    }
  }
  char colors[6]={'x','y','a','b','c','d'};
  if(colors[maxNumidx]=='a'&&yellow>=20){
    Serial.println(colors[maxNumidx]);
    return colors[maxNumidx];
  }else if(colors[maxNumidx]=='b'&&red>=15){
    Serial.println(colors[maxNumidx]);
    return colors[maxNumidx];
  }else if(colors[maxNumidx]=='c'&&green>=3){
    Serial.println(colors[maxNumidx]);
    return colors[maxNumidx];
  }else if(colors[maxNumidx]=='d'&&blue>=15){
    Serial.println(colors[maxNumidx]);
    return colors[maxNumidx];
  }else{
    return char();
  }
  
}

void takeCube(){
  
  do{
    analogWrite(rs,150);
    analogWrite(rs,150);
  }while(!(distance.read()<3));
  myservo1.write(50);
  delay(700);
  myservo2.write(120);
    digitalWrite(l1, 1);
    digitalWrite(l2, 0);
    digitalWrite(r1, 1);
    digitalWrite(r2, 0);
    analogWrite(rs,150);
    analogWrite(rs,150);
    delay(1700);
  if(distance.read()<6){
    rotate();
    digitalWrite(l1, 0);
    digitalWrite(l2, 1);
    digitalWrite(r1, 0);
    digitalWrite(r2, 1);
    setBackPath();
    x='n';
    node=0;
    return;
  }else{
    digitalWrite(l1, 0);
    digitalWrite(l2, 1);
    digitalWrite(r1, 0);
    digitalWrite(r2, 1);
    takeCube();
  }
  
}

void go(){
  if(p==0){
  color=getColor(sensorPal.redPAL(),sensorPal.greenPAL(),sensorPal.bluePAL());
  if (color==cmd){
    analogWrite(rs, 0);
    analogWrite(ls, 0);
    delay(5000);
    setBackPath();
    rotate();
    node=0;
  }}
  readSensors();
  error = (!sensor1 * -30) + (!sensor2 * -15) + (!sensor3 * 0) + (!sensor4 * 15) + (!sensor5 * 30);//}
      
      //calculate PID
      integral = integral + error;
      derivative = error - previous_error;
      pid_value = (Kp * error) + (Ki * integral) + (Kd * derivative);
    
      //update previous error
      previous_error = error;
    
      if ( error == 0 ) {
        integral = 0 ;
      }
    
      if (pid_value < 0) {
        analogWrite(rs,255);
        analogWrite(ls,(180+pid_value)>=0?(180+pid_value):0);
      } else {
        analogWrite(rs,(255-2*pid_value)>=0?(255-2*pid_value):0);
        analogWrite(ls,180);
      }
    
}

void forward(){
    do{
          go();
          readSensors();
        }while(!(sensor1 && sensor2 && !sensor3 && sensor4 && sensor5));
 }

void left(){
  analogWrite(rs,255);
  analogWrite(ls,0);
  delay(1000);
    do{
       analogWrite(rs,255);
       analogWrite(ls,0);
       readSensors();
    }while(!(sensor1 && sensor2 && !sensor3 && sensor4 && sensor5));
    if(x=='y')
    t='y';
 }

 void right(){
  analogWrite(rs,0);
       analogWrite(ls,200);
  delay(1000);
    do{
       analogWrite(rs,0);
       analogWrite(ls,200);
       readSensors();
   }while(!(sensor1 && sensor2 && !sensor3 && sensor4 && sensor5));
   if(x=='y')
   t='y';
 }
  
void loop() {
//  //read sensor values
  readSensors();
// 
  nodeerr = !sensor1 + !sensor2 + !sensor3 + !sensor4 + !sensor5;
  Serial.println(nodeerr);
  if(nodeerr>=3){
      if(path[node]=="forward"){
        Serial.println("forward1");
          forward();
          node++;
      }else if((path[node]=="left")||(path[node]=="\0"&&node==0&&path[1]=="left")){
        Serial.println("left");
        left();
        node++;
      }else if((path[node]=="right")||(path[node]=="\0"&&node==0&&path[1]=="right")){
        Serial.println("right");
        right();
        node++;
      }
  }else{
    Serial.println("forward2");
      go();
}


}
