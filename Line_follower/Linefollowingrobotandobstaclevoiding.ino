/*
  🛠️ Line Following + Obstacle Avoiding Robot
  📌 Author: Lee Curiosity
  📅 Last updated: June 2025

  👉 Features:
    - Follow a black line using 3 IR sensors
    - Stop and navigate around obstacles using an ultrasonic sensor
    - Motor control via L298N
    - Power supplied by 2 x 18650 batteries
*/

// -------------------- Constants --------------------
int stop_distance = 12; // Stop if obstacle is within 12 cm

// Ultrasonic sensor pins
const int trigPin = 11;
const int echoPin = 12;

// Motor A (left)
const int motorA1     = 3;
const int motorA2     = 4;
const int motorAspeed = 5; // PWM speed control

// Motor B (right)
const int motorB1     = 7;
const int motorB2     = 8;
const int motorBspeed = 9; // PWM speed control

// Line sensor pins
const int L_S = 6;   // Left
const int S_S = 2;   // Center
const int R_S = 10;  // Right

// -------------------- Variables --------------------
int left_sensor_state;
int s_sensor_state;
int right_sensor_state;
long duration;
int distance;

// -------------------- Setup --------------------
void setup() {
  pinMode(L_S, INPUT);
  pinMode(S_S, INPUT);
  pinMode(R_S, INPUT);

  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorAspeed, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);
  pinMode(motorBspeed, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(9600);

  // Start motors slowly after setup delay
  analogWrite(motorAspeed, 180);
  analogWrite(motorBspeed, 180);
  delay(3000); // Wait 3 seconds before starting
}

// -------------------- Main Loop --------------------
void loop() {
  // Measure distance using ultrasonic sensor
  digitalWrite(trigPin, LOW); delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  Serial.print("Distance: "); Serial.println(distance);

  // Read IR sensors
  left_sensor_state  = digitalRead(L_S);
  s_sensor_state     = digitalRead(S_S);
  right_sensor_state = digitalRead(R_S);

  // Line following logic
  if ((left_sensor_state == 0) && (s_sensor_state == 1) && (right_sensor_state == 0)) forword();  // Straight
  if ((left_sensor_state == 1) && (s_sensor_state == 1) && (right_sensor_state == 0)) turnLeft(); // Adjust Left
  if ((left_sensor_state == 1) && (s_sensor_state == 0) && (right_sensor_state == 0)) turnLeft(); // Hard Left
  if ((left_sensor_state == 0) && (s_sensor_state == 1) && (right_sensor_state == 1)) turnRight();// Adjust Right
  if ((left_sensor_state == 0) && (s_sensor_state == 0) && (right_sensor_state == 1)) turnRight();// Hard Right
  if ((left_sensor_state == 1) && (s_sensor_state == 1) && (right_sensor_state == 1)) Stop();     // Stop (off track)

  // Obstacle avoidance
  if (distance < stop_distance) {
    // Reverse slightly
    digitalWrite(motorA1, HIGH); digitalWrite(motorA2, LOW);
    digitalWrite(motorB1, LOW);  digitalWrite(motorB2, HIGH);
    analogWrite(motorAspeed, 170);
    analogWrite(motorBspeed, 170);
    delay(300);

    // Stop
    analogWrite(motorAspeed, 0);
    analogWrite(motorBspeed, 0);
    delay(300);

    // Go forward briefly
    digitalWrite(motorA1, HIGH); digitalWrite(motorA2, LOW);
    digitalWrite(motorB1, HIGH); digitalWrite(motorB2, LOW);
    analogWrite(motorAspeed, 150);
    analogWrite(motorBspeed, 150);
    delay(500);

    // Turn left (first phase)
    digitalWrite(motorA1, LOW); digitalWrite(motorA2, HIGH);
    digitalWrite(motorB1, HIGH); digitalWrite(motorB2, LOW);
    analogWrite(motorAspeed, 180);
    analogWrite(motorBspeed, 180);
    delay(700);

    // Left motor only
    digitalWrite(motorB1, LOW); digitalWrite(motorB2, LOW);
    delay(700);

    // Turn left again
    digitalWrite(motorB1, HIGH); digitalWrite(motorB2, LOW);
    delay(500);

    // Pause right motor
    digitalWrite(motorB1, LOW); digitalWrite(motorB2, LOW);
    delay(300);

    // Move forward again
    digitalWrite(motorB1, HIGH); digitalWrite(motorB2, LOW);
    analogWrite(motorAspeed, 150);
    analogWrite(motorBspeed, 150);

    // Wait until line is detected again
    while (digitalRead(L_S) == LOW) {
      Serial.println("Searching for line...");
    }
  }
}

// -------------------- Movement Functions --------------------
void forword() {
  digitalWrite(motorA1, LOW); digitalWrite(motorA2, HIGH);
  digitalWrite(motorB1, HIGH); digitalWrite(motorB2, LOW);
  analogWrite(motorAspeed, 180);
  analogWrite(motorBspeed, 180);
}

void turnRight() {
  digitalWrite(motorA1, LOW); digitalWrite(motorA2, LOW);
  digitalWrite(motorB1, HIGH); digitalWrite(motorB2, LOW);
  analogWrite(motorAspeed, 120);
  analogWrite(motorBspeed, 120);
}

void turnLeft() {
  digitalWrite(motorA1, LOW); digitalWrite(motorA2, HIGH);
  digitalWrite(motorB1, LOW); digitalWrite(motorB2, LOW);
  analogWrite(motorAspeed, 120);
  analogWrite(motorBspeed, 120);
}

void Stop() {
  digitalWrite(motorA1, LOW); digitalWrite(motorA2, LOW);
  digitalWrite(motorB1, LOW); digitalWrite(motorB2, LOW);
  analogWrite(motorAspeed, 0);
  analogWrite(motorBspeed, 0);
}

