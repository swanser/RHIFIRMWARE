//Humanoid Robotic Head Controller Firmware
//By Stephen M. Wanser
#include <Servo.h>
char bufferArray[255];
int charPos = 0;

struct command {

  char actionCode;
  char servoCode;
  int targetValue;

};

long unsigned mainTimer;
long unsigned servoTimer;
const int periodIncrement = 250;
const int servoArbInc = 15;

Servo jawServo;
int jawServoPin = 5;
int jawServoSpeed = 50;
int jawCurrentPos = 1500;
int jawTargetPos = 1500;
const int jawMin = 700;
const int jawMax = 2300;

Servo eyesVerticalServo;
int eyesVerticalServoPin = 7;
int eyesVerticalServoSpeed = 50;
int eyesVerticalCurrentPos = 1400;
int eyesVerticalTargetPos = 1400;
const int eyesVerticalMax = 700;
const int eyesVerticalMin = 2300;

Servo eyesHorizontalServo;
int eyesHorizontalServoPin = 6;
int eyesHorizontalServoSpeed = 50;
int eyesHorizontalCurrentPos = 700;
int eyesHorizontalTargetPos = 2300;
const int eyesHorizontalMax = 2300;
const int eyesHorizontalMin = 700;

Servo neckPivotServo;
int neckPivotServoPin = 3;
int neckPivotServoSpeed = 50;
int neckPivotCurrentPos = 1500;
int neckPivotTargetPos = 1500;
const int neckPivotMin = 2300;
const int neckPivotMax = 1500;

command TestCommand;
void setup() {
  
  Serial.begin(9600);
  Serial.println("Robotic Humanoid Head Controller Firmware Initialized!");
  
  mainTimer = millis();
  servoTimer = millis();

}

void loop() {

  //Check to see if it is time to arbitrate servos
  if(millis() - servoTimer > servoArbInc){
    arbitrateServoPositions();
    servoTimer = millis();
  }

  //Check to see if it is time to execute incoming commands
  if(millis() - mainTimer > periodIncrement){
    executeCommands();
    mainTimer = millis();
  }

}

void executeCommands() {
  
  if(Serial.available()){ 
    char readChar;
    readChar = Serial.read();
    bufferArray[charPos] = readChar;
    if(readChar==';'){
      command Command;
      Command = processBufferStream(bufferArray, charPos);
      charPos = 0;
      if(Command.actionCode == 'M'){
        processMovementDirective(Command);
        //Serial.println("Processing a MOVE command");
      }
      if(Command.actionCode == 'P'){
        processSetPinDirective(Command);
      }
      if(Command.actionCode == 'S'){
        processSetSpeedDirective(Command);
      }
      } else {
        charPos++;
      }
    }
}
command processBufferStream(char commandArray[255], int terminatorPos){

  command Command;
  Command.actionCode = commandArray[0];
  Command.servoCode = commandArray[1];

  char targetBuffer[10];
  int targetBufferPos = 0;
  int targetSize = terminatorPos - 2;

  for(int i = 0; i <= terminatorPos; i++){
    if(isDigit(commandArray[i])){
      targetBuffer[targetBufferPos] = commandArray[i];
      targetBufferPos++;
    }
  }

  if(targetSize == 1){
    Command.targetValue = int(targetBuffer[0] - '0');
  } else if(targetSize == 2){
    char tempTwoCharArray[2] = {targetBuffer[0], targetBuffer[1]};
    Command.targetValue = atoi(tempTwoCharArray);
  } else if(targetSize == 3){
    char tempThreeCharArray[3] = {targetBuffer[0], targetBuffer[1], targetBuffer[2]};
    Command.targetValue = atoi(tempThreeCharArray);
  } else if(targetSize == 4){
    char tempFourCharArray[4] = {targetBuffer[0], targetBuffer[1],targetBuffer[2], targetBuffer[3]};
    Command.targetValue = atoi(tempFourCharArray);
  }

  return Command;

}
void processMovementDirective(command Command){

  if(Command.servoCode == 'A'){
     eyesHorizontalTargetPos = Command.targetValue;
  } else if(Command.servoCode == 'B'){
    eyesVerticalTargetPos = Command.targetValue;
  } else if(Command.servoCode == 'C'){
    jawTargetPos = Command.targetValue;
  } else if(Command.servoCode == 'D'){
    neckPivotTargetPos = Command.targetValue;
  } 
}

void processSetPinDirective(command Command){

  if(Command.servoCode == 'A'){
    eyesHorizontalServoPin = Command.targetValue;
    eyesHorizontalServo.detach();
    eyesHorizontalServo.attach(eyesHorizontalServoPin);
    // Serial.print("eyesHorizontalServo set to ");
    // Serial.println(eyesHorizontalServoPin);   
  } else if(Command.servoCode == 'B'){
    eyesVerticalServoPin = Command.targetValue;
    eyesVerticalServo.detach();
    eyesVerticalServo.attach(eyesVerticalServoPin);
    // Serial.print("eyesVerticalServoPin set to ");
    // Serial.println(eyesVerticalServoPin);
  } else if(Command.servoCode == 'C'){
    jawServoPin = Command.targetValue;
    jawServo.detach();
    jawServo.attach(jawServoPin);
    // Serial.print("jawServoPin set to ");
    // Serial.println(jawServoPin);
  } else if(Command.servoCode == 'D'){
    neckPivotServoPin = Command.targetValue;
    neckPivotServo.detach();
    neckPivotServo.attach(neckPivotServoPin);
    // Serial.print("neckPivotServoPin set to ");
    // Serial.println(neckPivotServoPin);
  } 
}

void processSetSpeedDirective(command Command){

  if(Command.servoCode == 'A'){
    eyesHorizontalServoSpeed = Command.targetValue;
    // Serial.println("eyesHorizontalServoSpeed set to ");
    // Serial.println(eyesHorizontalServoSpeed);
  } else if(Command.servoCode == 'B'){
    eyesVerticalServoSpeed = Command.targetValue;
    // Serial.print("eyesVerticalServoSpeed set to ");
    // Serial.println(eyesVerticalServoSpeed);
  } else if(Command.servoCode == 'C'){
    jawServoSpeed = Command.targetValue;
    // Serial.print("jawServoSpeed set to ");
    // Serial.println(jawServoSpeed);
  } else if(Command.servoCode == 'D'){
    neckPivotServoSpeed = Command.targetValue;
    // Serial.print("neckPivotServoSpeed set to ");
    // Serial.println(neckPivotServoSpeed);
  } 
}

void arbitrateServoPositions(){

  if(jawCurrentPos < jawTargetPos){
    jawCurrentPos = jawCurrentPos + jawServoSpeed;
    if(jawCurrentPos > jawTargetPos){
      jawCurrentPos = jawTargetPos;
    }
  } else if(jawCurrentPos > jawTargetPos){
    jawCurrentPos = jawCurrentPos - jawServoSpeed;
    if(jawCurrentPos < jawTargetPos){
      jawCurrentPos = jawTargetPos;
    }
  }

  if(neckPivotCurrentPos < neckPivotTargetPos){
    neckPivotCurrentPos = neckPivotCurrentPos + neckPivotServoSpeed;
    if(neckPivotCurrentPos > neckPivotTargetPos){
      neckPivotCurrentPos = neckPivotTargetPos;
    }
  } else if(neckPivotCurrentPos > neckPivotTargetPos){
    neckPivotCurrentPos = neckPivotCurrentPos - neckPivotServoSpeed;
    if(neckPivotCurrentPos < neckPivotTargetPos){
      neckPivotCurrentPos = neckPivotTargetPos;
    }
  }

  if(eyesHorizontalCurrentPos < eyesHorizontalTargetPos){
    eyesHorizontalCurrentPos = eyesHorizontalCurrentPos + eyesHorizontalServoSpeed;
    if(eyesHorizontalCurrentPos > eyesHorizontalTargetPos){
      eyesHorizontalCurrentPos = eyesHorizontalTargetPos;
    }
  } else if(eyesHorizontalCurrentPos > eyesHorizontalTargetPos){
    eyesHorizontalCurrentPos = eyesHorizontalCurrentPos - eyesHorizontalServoSpeed;
    if(eyesHorizontalCurrentPos < eyesHorizontalTargetPos){
      eyesHorizontalCurrentPos = eyesHorizontalTargetPos;
    }
  }
  if(eyesVerticalCurrentPos < eyesVerticalTargetPos){
    eyesVerticalCurrentPos = eyesVerticalCurrentPos + eyesVerticalServoSpeed;
    if(eyesVerticalCurrentPos > eyesVerticalTargetPos){
      eyesVerticalCurrentPos = eyesVerticalTargetPos;
    }
  } else if(eyesVerticalCurrentPos > eyesVerticalTargetPos){
    eyesVerticalCurrentPos = eyesVerticalCurrentPos - eyesVerticalServoSpeed;
    if(eyesVerticalCurrentPos < eyesVerticalTargetPos){
      eyesVerticalCurrentPos = eyesVerticalTargetPos;
    }
  }

  jawServo.write(jawCurrentPos);
  eyesHorizontalServo.write(eyesHorizontalCurrentPos);
  eyesVerticalServo.write(eyesVerticalCurrentPos);
  neckPivotServo.write(neckPivotCurrentPos);

}
