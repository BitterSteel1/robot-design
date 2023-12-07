// A - left 
// B - right 
// check this one 

// ir array pins and globals
uint8_t const IR_pins[]={A8,A9,A10,A11,A12,A13,A14,A15};
uint16_t sensorValues[8];
uint16_t const threshold=160 ;
uint8_t IR_digital_array[8];

//motor driver pins- ENTC
uint8_t const INA_1=2;
uint8_t const INB_1=23;
uint8_t const INA_2=3;
uint8_t const INB_2=22;

uint8_t const left_ultra_trig=;
uint8_t const left_ultra_echo=;
uint8_t const right_ultra_trig=;
unit8_t const right_ultra_echo=;

//PID & errors
float const kp = 0.0060;//5-54-60
float  const kd = 0.042;//0.018// 28//35// 
float const ki = 0.0005;
uint16_t const Basespeed=60; 

int previouserror=0;
int error;//temporary use for checking the ir array
int control_signal_1;//for temp use of tunning and debuging- can be made local in pid func
int control_signal_2;//""
int count=0;//""
int total_error=0;//""

int integral = 0;

//line follower logic

uint8_t checkpoint = 0;

void setup() {
  
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);

  //Motor Driver Pins
  pinMode(INA_1, OUTPUT);
  pinMode(INB_1, OUTPUT);
  pinMode(INA_2, OUTPUT);
  pinMode(INB_2, OUTPUT);

  // Configure IR array pins
  for (uint8_t i = 0;i<8;i++){pinMode(IR_pins[i],INPUT);}
  
  //start indicator
  for (uint8_t i = 0; i<10;i++){
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
    }
}


void loop() {


pid_line_following();
delay(30);
//junction_find();

}


/////////////////************///////////////////

void Rotate_Wheels(int speedA, int speedB) {

//shock();

//speedA = constrain(speedA,-150,150);
  
  
  if (speedA > 0) {
  
  digitalWrite(INB_1, LOW);
  analogWrite(INA_1, speedA); 
  }else{

  digitalWrite(INB_1, HIGH);
  analogWrite(INA_1, -speedA);   
  }
  if (speedB > 0) {
  digitalWrite(INB_2, LOW);
  analogWrite(INA_2, speedB);
   
  }else{
  digitalWrite(INB_2, HIGH);
  analogWrite(INA_2, -speedB);  
    
  }
}

void Rotate_Wheels_walls(int speedA, int speedB) {

//shock();
// Right side check 
// get_distance(right_ultra_trig, right_ultra_echo);
//speedA = constrain(speedA,-150,150);
  
  
  if (speedA > 0) {
  
  digitalWrite(INB_1, LOW);
  analogWrite(INA_1, speedA); 
  }else{

  digitalWrite(INB_1, HIGH);
  analogWrite(INA_1, -speedA);   
  }
  if (speedB > 0) {
  digitalWrite(INB_2, LOW);
  analogWrite(INA_2, speedB);
   
  }else{
  digitalWrite(INB_2, HIGH);
  analogWrite(INA_2, -speedB);  
    
  }
}

// Function to get distance from ultrasonic sensors
int get_distance(uint8_t trigPin, uint8_t echoPin)
{
  /
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH);
    int distance = (duration * 0.034) / 2;

    return distance;
}


void shock(){
  Rotate_Wheels(100,100);
  delay(40);
  Rotate_Wheels(0,0);
  }
  
uint8_t digital_read(uint8_t pin){
  int IR_analog_value=analogRead(pin);
  if (IR_analog_value<threshold){return 1;}//the only place to change when switch from black line to white?
  else{return 0;}
}

//int error_v(){ 
//    
//  int error_v; //return value
//  for (uint8_t i=0;i<8;i++){
//    IR_digital-*_array[i]=digital_read(IR_pins[i]); // see globals 
//    sensorValues[i]=analogRead(IR_pins[i]); // see globals
//  }
//  //error_v  = IR_digital_array[0]800+IR_digital_array[1]*500+IR_digital_array[2]*200+IR_digital_array[5]-200+IR_digital_array[6]-500+IR_digital_array[7]-800;
//  error_v  = 
//  return error_v;
//}

int error_v(){ 
  total_error=0;
  count =0 ;   
  int error_v; //return value
  for (uint8_t i=0;i<8;i++){
    IR_digital_array[i]=digital_read(IR_pins[i]); // see globals 
    if (IR_digital_array[i] == 1){
      count+=1;
      total_error =total_error+ 1000*i;
      sensorValues[i]=analogRead(IR_pins[i]); // see globals
      }
      
  sensorValues[i]=analogRead(IR_pins[i]);
  }
  
  if (count!=0){
    error_v= total_error/count-3500;
  }else{
    
    
    }
  return error_v;
}

void pid_line_following()
{
  
  int currenterror=error_v();
  error = currenterror;// global
  integral += currenterror;// global
  int delta=currenterror-previouserror;
  float CT = currenterror*kp + delta*kd + integral*ki;
  previouserror=currenterror;//global previouserror
  control_signal_2=  Basespeed+round(constrain(CT,-255,255));//basespeed global
  control_signal_1=  Basespeed-round(constrain(CT,-255,255));
 // motor_control(1,control_signal_1);motor_control(2,control_signal_2);

  // Check distance from the right side
  int right_distance = get_distance(right_ultra_trig, right_ultra_echo);
  if (right_distance < 10){
    
  }
 Rotate_Wheels(control_signal_1,control_signal_2);
  
  delay(4);

  PID_debug();//comment out in normal operation
}


void junction_find(){
  //if 6 or more ir sensors detected, identify if it is 90 bend or T junct, wait..
  //..(either a certain distance or a time) and  
  uint16_t turn_speed = 100;
  uint16_t turn_time  = 150;
  
  if (count>=5){
    bool left_turn = IR_digital_array[0]==1;//store the current line data
    bool right_turn = IR_digital_array[7]==1;
    
    //keep running pid line follow for around half a second or let go straight 
//    for (uint8_t i = 0;i<14;i++){
//      pid_line_following();
//      delay(34);
//    }
    
    delay(500);//wait
    pid_line_following();//update line data
    
    // line data has changed and "pid_line_following" has updated the values
    if (left_turn && right_turn){
      if (count == 0){
      // T junction encountered
        // turn left/ right with priority 
        Rotate_Wheels(0,turn_speed);
        delay (turn_time);   
        }
      else{
      // check point
        checkpoint++;
        
        return;  
        }  
      }
    
    else if (left_turn){
      
      if (count == 0){
        Rotate_Wheels(turn_speed,0);
        delay (turn_time);
      }
      else{
        // T junc side
        return ; 
        
      }
    }
    else if (right_turn){
      
      if (count == 0){
        Rotate_Wheels(0,turn_speed);
        delay (turn_time);
      }
      else{return;}
    }
    
  }
  else if(count == 0){
    //dead-end //turn back
    Rotate_Wheels(-turn_speed,turn_speed);
    delay (turn_time);
    }
}


void PID_debug(){
  for (uint8_t i=0;i<8;i++){
    Serial.print(sensorValues[i]);
    Serial.print('\t');
    }
  Serial.println('\t');
  for (uint8_t i=0;i<8;i++){
    Serial.print(IR_digital_array[i]);
    Serial.print('\t');
  }
  Serial.print(control_signal_1);
  Serial.print('\t');
  Serial.print(control_signal_2);
  Serial.print('\t');
  Serial.print(error);
  Serial.print('\t');
  Serial.print(count);
  Serial.print('\t');
  Serial.println(total_error);
  
}