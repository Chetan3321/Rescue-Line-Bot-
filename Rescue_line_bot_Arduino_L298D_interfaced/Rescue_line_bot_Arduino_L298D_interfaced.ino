// Motor A connections
int enA = 9;
int in1 = 8;
int in2 = 7;
// Motor B connections
int enB = 3;
int in3 = 5;
int in4 = 4;

const int onblack= 1;  // reading for black line from sensor is less than this
const int onWhite= 0; // reading for white line is greater than this

void setup() 
{
  // Set all the motor control pins to outputs

  
  Serial.begin(9600);

  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

    // pinMode(A0,INPUT);  
  pinMode(A0,INPUT);  //extreme left sensor
  pinMode(A2,INPUT);  //left
  pinMode(A3,INPUT);  // middle sensor
  pinMode(A4,INPUT);   // right
  pinMode(A5,INPUT);  // extreme right sensor

  // Turn off motors - Initial state
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
   digitalWrite(enA, HIGH);
  digitalWrite(enB, HIGH);
}

void loop()
{
 int sensor1=digitalRead(A0);//sensor1  Extreme Left
int sensor2=digitalRead(A2);//sensor2 
int sensor3=digitalRead(A3);//sensor3  middle
int sensor4=digitalRead(A4);//sensor4  
int sensor5=digitalRead(A5);//sensor5   Extreme right

Serial.print("extreme Left: ");
Serial.print(sensor1);
Serial.print("\t lower left: ");
Serial.print(sensor2);
Serial.print("\t middle: ");
Serial.print(sensor3);
Serial.print("\t lowerRight: ");
Serial.print(sensor4);
Serial.print("\t Extreme Right: ");
Serial.println(sensor5);


if( isOnBlack(sensor1)){
  Left();
}else{

  if(isOnBlack(sensor3)&&isOnWhite(sensor1)){
    Forward();
  }else

  if( isOnWhite(sensor1)&& isOnWhite(sensor3)&& isOnBlack(sensor5)  ){
    Right();
  }else
  
  if(isOnBlack(sensor2)){
    Left();
  }else
  if(isOnBlack(sensor4)){
    Right();
  }else
  if( isOnWhite(sensor1) && isOnWhite(sensor2) && isOnWhite(sensor3) && isOnWhite(sensor4) && isOnWhite(sensor5) ){
    Left();   // replace with left if gap is present
    Left();   // replace with left if gap is present
    Left();   // replace with left if gap is present
    Left();   // replace with left if gap is present
  }


}
  
  
  


  

}

bool isOnBlack(int sensor){

if( sensor <= onblack)
return true;
else
return false;
}


bool isOnWhite(int sensor){
  if(sensor >= onWhite)
  return true;
  else
  return false;
}


void Forward()
{
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void Backward()
{
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void Stop()
{
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void Left()
{
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void Right()
{
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}
// This function lets you control spinning direction of motors
void directionControl() {
  // Set motors to maximum speed
  digitalWrite(enA, HIGH);
  digitalWrite(enB, HIGH);

  // Turn on motor A & B
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(2000);

  // Now change motor directions
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  delay(2000);

  // Turn off motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

// This function lets you control speed of the motors
void speedControl() {
  // Turn on motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

  // Accelerate from zero to maximum speed
  for (int i = 0; i < 256; i++) {
    analogWrite(enA, i);
    analogWrite(enB, i);
    delay(20);
  }

  // Decelerate from maximum speed to zero
  for (int i = 255; i >= 0; --i) {
    analogWrite(enA, i);
    analogWrite(enB, i);
    delay(20);
  }

  // Now turn off motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}