// Pin definitions
// sensors control pins(triggerpins,echopins and irpins) 
int trigPinLeft=7;
int echoPinLeft =6; 
int trigPinFront=5;
int echoPinFront=4;
int trigPinRight=3;
int echoPinRight=2;

//irsensor pins
int irpin=;
int irstate=0;

// Motor control pins(in1,in2,enA,in3,in4,enB)
int motorLeftForward=11;
int motorLeftBackward=12;
int motorRightForward=2;
int motorRightBackward=4;
int enleft=9;
int enright=3;

// Threshold distance for walls (in cm)
int threshold = 7;

// Path memory (to record movements)
char path[100];    // Array to store up to 100 moves
int pathIndex = 0; // Index to track the current move in the path

// Position tracking (basic for loop detection)
int posX = 0, posY = 0; // X, Y coordinates of the robot in the maze
int direction = 0;      // 0: North, 1: East, 2: South, 3: West (relative to starting point)

void setup() {
  Serial.begin(9600);
  // Set ultrasonic(HC-SR04) pins as inputs/outputs
  pinMode(trigPinLeft, OUTPUT);
  pinMode(echoPinLeft, INPUT);
  pinMode(trigPinFront, OUTPUT);
  pinMode(echoPinFront, INPUT);
  pinMode(trigPinRight, OUTPUT);
  pinMode(echoPinRight, INPUT);

  //set input and enable pins as inputs/outputs
  pinMode(motorLeftForward, OUTPUT);
  pinMode(motorLeftBackward, OUTPUT);
  pinMode(motorRightForward, OUTPUT);
  pinMode(motorRightBackward, OUTPUT);
  pinMode(enleft,OUTPUT);
  pinMode(enright,OUTPUT);

  // set irsensors pins as inputs/outputs
  pinMode(irpin,INPUT)
}

void loop() {
   // Read distances from the sensors(in cm)
   //getdistance(parameter1,parameter2):parameter1=triggerpin,parameter2=echopin 
  int distanceLeft = getDistance(trigPinLeft, echoPinLeft);
  int distanceFront = getDistance(trigPinFront, echoPinFront);
  int distanceRight = getDistance(trigPinRight, echoPinRight);
  irstate=digitalRead(irpin);

  // Decision-making based on sensor readings
  
  if (distanceLeft > threshold) { //no wall(obstacle) on left
    turnLeft();
    recordMove('L');  // Record a left turn
  } else if (distanceFront > threshold) { // no wall(obstacle) in the front
    moveForward();
    recordMove('F');  // Record moving forward
  } else if (distanceRight > threshold) { // no wall(obstacle) on the right
    turnRight();
    recordMove('R');  // Record a right turn
  } else { // walls on all the 3 sides(left,front,right) therefore needs to take an u-turn
    turnAround();
    recordMove('B');  // Record an U-turn
  }

  delay(100); // Delay for stability

  // After completing one run, optimize the path (once an exit is found)
  if (irstate==1/* condition for reaching the end */) {
    stop();
    delay(10000);
    optimizePath();  // Optimize the path based on learned data
  }
}

// Function to measure distance using ultrasonic sensors
int getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); //pin turned on to send ultrasonic wave
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW); //pin turned off 
  long duration = pulseIn(echoPin, HIGH); //measures the duration of the ultrasonic waves
  int distance = duration * 0.034 / 2; // caluculates distance

  return distance;
}

// Movement functions
void moveForward() {  // both wheels turning forward
  digitalWrite(motorLeftForward, HIGH);
  digitalWrite(motorLeftBackward, LOW);
  digitalWrite(motorRightForward, HIGH);
  digitalWrite(motorRightBackward, LOW);
  analogWrite(enleft,255);
  analogwrite(enright,255);
  updatePosition(1);  // Move forward in the current direction (each forward movement increments the position by 1)
}

void turnLeft() { // left wheel turning backward and right wheel turning forward
  digitalWrite(motorLeftForward, LOW);
  digitalWrite(motorLeftBackward, HIGH);
  digitalWrite(motorRightForward, HIGH);
  digitalWrite(motorRightBackward, LOW);
  analogWrite(enleft,50);
  analogwrite(enright,100);
  delay(300);
  updateDirection(-1);  // Turn 90 degrees left (each left turn decrements the direction by 1)
}

void turnRight() { // left wheel turning forward and right wheel turning backward
  digitalWrite(motorLeftForward, HIGH);
  digitalWrite(motorLeftBackward, LOW);
  digitalWrite(motorRightForward, LOW);
  digitalWrite(motorRightBackward, HIGH);
  analogWrite(enleft,255);
  analogwrite(enright,255);
  delay(300);
  updateDirection(1);  // Turn 90 degrees right (each right turn increments the direction by 1)
}

void turnAround() {  // both wheels turning backwards
  digitalWrite(motorLeftForward, LOW);
  digitalWrite(motorLeftBackward, HIGH);
  digitalWrite(motorRightForward, HIGH);
  digitalWrite(motorRightBackward, LOW);
  analogWrite(enleft,255);
  analogwrite(enright,255);
  delay(600);  // Longer delay for an U-turn
  updateDirection(2);  // U-turn (each u-turn increments the direction by 2 becuase we are taking 2 90-degree turns so 1+1=2)
} 

void stop(){ // both the wheels stop moving
  digitalWrite(motorLeftForward, LOW);
  digitalWrite(motorLeftBackward, LOW);
  digitalWrite(motorRightForward, LOW);
  digitalWrite(motorRightBackward, LOW);

}

// Function to record each move
void recordMove(char move) {
  if (pathIndex < 100) {  // Avoid overflow
    path[pathIndex++] = move; //storing each move and turn
  }
}

// Function to optimize the path after multiple attempts
void optimizePath() {
  Serial.println("Optimizing path...");
  // Implement path optimization logic here
  // Example: Remove U-turns or redundant turns
  int i = 0;
  while (i < pathIndex - 1) {
    // If the robot makes unnecessary back-and-forth moves (e.g., L, R)
    if ((path[i] == 'L' && path[i+1] == 'R') || (path[i] == 'R' && path[i+1] == 'L')) {
      // Remove both movements
      removePathElement(i); //to eliminate the unnessecary turns to optimize the path
      removePathElement(i);
    } else {
      i++; //updates the value of i
    }
  }
}

// Function to remove an element from the path
void removePathElement(int index) {
  for (int i = index; i < pathIndex - 1; i++) {
    path[i] = path[i + 1];
  }
  pathIndex--;  // Decrease path length
}

// Update the robot's position based on movement
void updatePosition(int step) { //int step=parameter used to indicate how many steps the bot moves forward
  switch (direction) {
    case 0: posY += step; break;  // North(+ Y direction)
    case 1: posX += step; break;  // East(+ X direction)
    case 2: posY -= step; break;  // South(- Y direction)
    case 3: posX -= step; break;  // West(- X direction)
  }
}

// Update the robot's direction after a turn
void updateDirection(int turn) {
  direction = (direction + turn + 4) % 4;  // Ensure direction stays within 0-3
}
