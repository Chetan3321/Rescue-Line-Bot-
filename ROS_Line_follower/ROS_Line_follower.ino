#include <ros.h>
#include <std_msgs/String.h>

// ROS NodeHandle
ros::NodeHandle nh;
std_msgs::String str_msg;
ros::Publisher chatter("line_follower", &str_msg);

// -------- Motor Pin Connections --------
// Motor A (Left)
int enA = 9;
int in1 = 8;
int in2 = 7;

// Motor B (Right)
int enB = 3;
int in3 = 5;
int in4 = 4;

// -------- Sensor Pins --------
int sensor1 = A0; // extreme left
int sensor2 = A2; // left
int sensor3 = A3; // middle
int sensor4 = A4; // right
int sensor5 = A5; // extreme right


const int onblack= 0;  // reading for black line from sensor is less than this
const int onwhite= 1; // reading for white line is greater than this

// -------- Setup --------
void setup() {
  nh.initNode();
  nh.advertise(chatter);

  // Motor pins
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  // Sensor pins
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  pinMode(sensor3, INPUT);
  pinMode(sensor4, INPUT);
  pinMode(sensor5, INPUT);

  // Stop motors initially
  Stop();
}

// -------- Main Loop --------
void loop() {
  int s1 = digitalRead(sensor1);
  int s2 = digitalRead(sensor2);
  int s3 = digitalRead(sensor3);
  int s4 = digitalRead(sensor4);
  int s5 = digitalRead(sensor5);

  analogWrite(enA, 180);
  analogWrite(enB, 180);

  String state = "";

  if (s1 == onblack) {
    Left();
    state = "LEFT (S1)";
  } 
  else if (s3 == onblack && s1 == onwhite) {
    Forward();
    state = "FORWARD (S3)";
  } 
  else if (s1 == onwhite && s3 == onwhite && s5 == onblack) {
    Right();
    state = "RIGHT (S5)";
  } 
  else if (s2 == onblack) {
    Left();
    state = "LEFT (S2)";
  } 
  else if (s4 == onblack) {
    Right();
    state = "RIGHT (S4)";
  } 
  else if (s1 == onwhite && s2 == onwhite && s3 == onwhite && s4 == onwhite && s5 == onwhite) {
    Left(); // Gap handling
    state = "GAP - ROTATE LEFT";
  } 
  else {
    Stop();
    state = "STOP";
  }

  // Publish state to ROS
  char buffer[50];
  state.toCharArray(buffer, 50);
  str_msg.data = buffer;
  chatter.publish(&str_msg);

  nh.spinOnce();
  delay(100);
}


// -------- Motor Functions --------

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
