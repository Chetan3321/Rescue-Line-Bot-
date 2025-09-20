// --- Pin Definitions ---
// Motor Pins
#define ENA 9  // Left Motor Speed Control (PWM)
#define ENB 3  // Right Motor Speed Control (PWM)
#define IN1 8  // Left Motor Direction A
#define IN2 7  // Left Motor Direction B
#define IN3 5  // Right Motor Direction A
#define IN4 4  // Right Motor Direction B

// Sensor Pins
const int sensorPins[5] = {A0, A1, A2, A3, A4};
const int numSensors = 5;

// --- PID Constants ---
// Tune these values for your robot
double Kp = 0.5;
double Ki = 0.001;
double Kd = 0.2;

// --- Global Variables ---
int baseSpeed = 100; // Base speed for motors (0-255)
double integral = 0;
double lastError = 0;

void setup() {
  // Initialize motor pins as outputs
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Initialize sensor pins as inputs
  for (int i = 0; i < numSensors; i++) {
    pinMode(sensorPins[i], INPUT);
  }

  // Set initial motor direction to forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  Serial.begin(9600);
}

void loop() {
  int position = readSensors();

  // PID Calculation
  double error = 0 - position;
  integral += error;
  double derivative = error - lastError;

  double pTerm = Kp * error;
  double iTerm = Ki * integral;
  double dTerm = Kd * derivative;

  double correction = pTerm + iTerm + dTerm;

  // Calculate motor speeds and clamp to a valid range (0-255)
  int leftMotorSpeed = constrain(baseSpeed + correction, 0, 255);
  int rightMotorSpeed = constrain(baseSpeed - correction, 0, 255);

  // Set motor speeds
  analogWrite(ENA, leftMotorSpeed);
  analogWrite(ENB, rightMotorSpeed);

  // Update lastError for the next loop
  lastError = error;
}

int readSensors() {
  int sensorValues[numSensors];
  int totalSensorValue = 0;
  int totalWeight = 0;
  int position;

  // Read each analog sensor and calculate a weighted average
  for (int i = 0; i < numSensors; i++) {
    sensorValues[i] = analogRead(sensorPins[i]); // Using analogRead for analog sensors
    
    // Normalize values to 0 or 1 for easy calculation
    // Assuming a threshold, you may need to adjust this value
    if (sensorValues[i] < 500) { // Assuming black line gives a low reading
      totalSensorValue += (i * 100);
      totalWeight += 100;
    }
  }

  // Calculate weighted average position
  if (totalWeight == 0) {
    // If no sensor sees the line, a large value can make the robot turn
    return lastError > 0 ? 400 : -400; 
  }
  
  position = totalSensorValue / totalWeight;
  position = position - 200; // Offset to center the error around 0

  return position;
}