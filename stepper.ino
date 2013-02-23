//set the clock pin of the shift register
const int clockPin = 4; 
//set the latch pin of the shift register
const int latchPin = 12;
//set the serial data pin of the shift register
const int dataPin = 8;
//set the clock pin of the shift register
const int enPin = 7;

//set the l293d enable pins for use pwm
const int PWM2A = 11;
const int PWM2B = 3;


//function used to shift
void motorWrite(byte motorPosition){
   digitalWrite(latchPin, LOW);
   
   shiftOut(dataPin , clockPin, MSBFIRST, motorPosition);
   
   digitalWrite(latchPin, HIGH);
}


// class used to define a motor and the pins it uses
class motor{
  byte pin[4];
  public:
  byte count;
  byte position;
  int direction;
  void init(byte M1A_pin, byte M1B_pin, byte M2A_pin, byte M2B_pin){
  pin[0] = M1A_pin;
  pin[1] = M2A_pin;
  pin[2] = M1B_pin;
  pin[3] = M2B_pin;
  
  //set the initial state to 0 with the motor off
  count = 0;
  direction = 2;
  position = 0;
  motorWrite(position);
  }
  
  void next(){
  //set the previous pin that was HIGH to LOW
  bitWrite(position,pin[count],LOW);
 
  switch(direction){
    case 1:
    if(count < 3){
    count ++;
    }else{
    count = 0;}
    bitWrite(position,pin[count],HIGH);
    break;
    case -1:
    if(count > 0){
    count--;
    }else{
    count = 3;}
    bitWrite(position,pin[count],HIGH);
    break;
    case 0:
    default :
    bitWrite(position,pin[count],HIGH);
    break;
    case 2:
    bitWrite(position,pin[count],LOW);
    break;
  }
  //write the next pin that needs to go high
  motorWrite(position);
  }
  
  void forward(){
  direction = 1;
  //Serial.println(direction);
  }
  
  void reverse(){
  direction = -1;
  //Serial.println(direction);
  }
  
  void lock(){
  direction = 0;
  //Serial.println(direction);
  }
  
  void stop(){
  direction = 2;
  //Serial.println(direction);
  }
  
};

int step_interval = 1;

motor motor1;

void setup() {
   pinMode(clockPin, OUTPUT);
   pinMode(latchPin, OUTPUT);
   pinMode(dataPin, OUTPUT);
   pinMode(enPin, OUTPUT);
   pinMode(PWM2A, OUTPUT);
   pinMode(PWM2B, OUTPUT);
   
   digitalWrite(dataPin, LOW);
   digitalWrite(clockPin, LOW);
   digitalWrite(enPin, LOW);
   digitalWrite(PWM2A, HIGH);
   digitalWrite(PWM2B, HIGH);
   
   motor1.init(2,3,1,4);
   
   Serial.begin(9600);
   

}

void loop() {
  
  char cmd[5] = "";
  
  if (Serial.available() > 0) {
  
  Serial.readBytes(cmd, 3);
  
  if(strncmp(cmd,"REV",3) == 0){
  //Serial.println(cmd);
  motor1.reverse();
  }else if(strncmp(cmd,"FWD",3) == 0){
  //Serial.println(cmd);
  motor1.forward();
  }else if(strncmp(cmd,"LCK",3) == 0){
  //Serial.println(cmd);
  motor1.lock();
  }else if(strncmp(cmd,"STP",3) == 0){
  //Serial.println(cmd);
  motor1.stop();
  }
  }
  
  motor1.next();
  //Serial.println(motor1.count, DEC);
  delay(step_interval);

}

