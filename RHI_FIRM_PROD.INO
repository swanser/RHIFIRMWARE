//Humanoid Robotic Head Controller Firmware
//By Stephen M. Wanser
#include <Servo.h>

long unsigned mainTimer;
long unsigned servoTimer;
const int periodIncrement = 50;
const int servoArbInc = 40;

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

String command = "";

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
  
  command = "";
  Serial.flush();
  command = Serial.readString();
  
  command.trim();
  if(command.length() == 0){
    return;
  }

  int delimiterStage = 0;
  char charBuffer[(command.length()+1)];
  command.toCharArray(charBuffer, (command.length()+1));
  int strLength = command.length();
  int delimiterPositions[4];

  for(int i = 0; i <= strLength; i++){

    if(charBuffer[i] == ':'){
      delimiterPositions[delimiterStage] = i;
      delimiterStage++;
    }
  
  }
  String directive = command.substring(0, delimiterPositions[0]);
  String servo = command.substring(delimiterPositions[0] + 1, delimiterPositions[1]);
  String detail = command.substring(delimiterPositions[1] + 1, delimiterPositions[2]);
  String referenceID = command.substring(delimiterPositions[2] + 1, delimiterPositions[3]);

   if(directive.equals("MOVE")){
     processMovementDirective(servo, detail, referenceID);
   }
  if(directive.equals("SET")){
    processSetDirective(servo, detail, referenceID);
  }

}

void processMovementDirective(String servo, String detail, String referenceID){

  if(servo.equals("EYESV")){

    if(detail.equals("NUDGEUP")){
      eyesVerticalCurrentPos = eyesVerticalCurrentPos - 50;
      eyesVerticalTargetPos = eyesVerticalTargetPos - 50;
    } else if (detail.equals("NUDGEDOWN")){
      eyesVerticalCurrentPos = eyesVerticalCurrentPos + 50;
      eyesVerticalTargetPos = eyesVerticalTargetPos + 50;
    } else if (detail.equals("TOP")){
      eyesVerticalTargetPos = eyesVerticalMin;
    } else if (detail.equals("BOTTOM")){
      eyesVerticalTargetPos = eyesVerticalMax;
    } else if(isdigit(detail[0])){
      if( (detail.toInt() >= eyesVerticalMin) && (detail.toInt() <= eyesVerticalMax) ){
        eyesVerticalTargetPos = detail.toInt();
      }
    } else {
      String errorMessage = "ER{message:EYESV Movement Directive not recognized, referenceID:" + referenceID + "}";
      Serial.println(errorMessage);
      return;
    }

    Serial.println("CC{" + referenceID + "}");
    return;
  }
  
  if(servo.equals("JAW")){
    if(detail.equals("NUDGEUP")){
      jawCurrentPos = jawCurrentPos - 50;
      jawTargetPos = jawTargetPos - 50;
    } else if (detail.equals("NUDGEDOWN")){
      jawCurrentPos = jawCurrentPos + 50;
      jawTargetPos = jawTargetPos + 50;
    } else if(isdigit(detail[0])){
      if( (detail.toInt() >= jawMin) && (detail.toInt() <= jawMax) ){
        jawTargetPos = detail.toInt();
      }
    } else {
      String errorMessage = "ER{message:JAW Movement Directive not recognized, referenceID:" + referenceID + "}";
      Serial.println(errorMessage);
      return;
    }
    Serial.println("CC{" + referenceID + "}");
    return;
    }
   

  if(servo.equals("EYESH")){
      if(detail.equals("NUDGELEFT")){
        eyesHorizontalCurrentPos = eyesHorizontalCurrentPos - 50;
        eyesHorizontalTargetPos = eyesHorizontalTargetPos - 50;
      } else if (detail.equals("NUDGERIGHT")){
        eyesHorizontalCurrentPos = eyesHorizontalCurrentPos + 50;
        eyesHorizontalTargetPos = eyesHorizontalTargetPos + 50;
      } else if(isdigit(detail[0])){
        if( (detail.toInt() >= eyesHorizontalMin) && (detail.toInt() <= eyesHorizontalMax) ){
          eyesHorizontalTargetPos = detail.toInt();
        }
      } else {String errorMessage = "ER{message:EYESH Movement Directive not recognized, referenceID:" + referenceID + "}";
      Serial.println(errorMessage);
      return;
    }
    Serial.println("CC{" + referenceID + "}");
    return;
  }

  if(servo.equals("NECKPIVOT")){
      if(detail.equals("NUDGELEFT")){
        neckPivotCurrentPos = neckPivotCurrentPos - 50;
        neckPivotTargetPos = neckPivotTargetPos - 50;
      } else if (detail.equals("NUDGERIGHT")){
        neckPivotCurrentPos = neckPivotCurrentPos + 50;
        neckPivotTargetPos = neckPivotTargetPos + 50;
      } else if (detail.equals("LEFT")){
        neckPivotTargetPos = neckPivotMin;
      } else if (detail.equals("RIGHT")){
        neckPivotTargetPos = neckPivotMax;
      } else if(isdigit(detail[0])){
        if( (detail.toInt() >= neckPivotMin) && (detail.toInt() <= neckPivotMax) ){
          neckPivotTargetPos = detail.toInt();
        }
      }else {String errorMessage = "ER{message:NECKPIVOT Movement Directive not recognized, referenceID:" + referenceID + "}";
      Serial.println(errorMessage);
      return;
    }
    Serial.println("CC{" + referenceID + "}");
    return;
  }
}

void processSetDirective(String servo, String detail, String referenceID){

  if(servo.equals("EYESV")){
    if(detail.startsWith("SPEED")){
      eyesVerticalServoSpeed = detail.substring(5).toInt();
    } else if (detail.startsWith("PIN")){
      eyesVerticalServoPin = detail.substring(3).toInt();
      eyesVerticalServo.detach();
      eyesVerticalServo.attach(eyesVerticalServoPin);
    } else {
      String errorMessage = "ER{message:EYESV Set Directive not recognized, referenceID:" + referenceID + "}";
      Serial.println(errorMessage);
      return;
    }

    Serial.println("CC{" + referenceID + "}");
    return;
    
  } else if(servo.equals("EYESH")){
    if(detail.startsWith("SPEED")){
      eyesHorizontalServoSpeed = detail.substring(5).toInt();
    } else if (detail.startsWith("PIN")){
      eyesHorizontalServoPin = detail.substring(3).toInt();
      eyesHorizontalServo.detach();
      eyesHorizontalServo.attach(eyesHorizontalServoPin);
    } else {
      String errorMessage = "ER{message:EYESH Set Directive not recognized, referenceID:" + referenceID + "}";
      Serial.println(errorMessage);
      return;
    }

    Serial.println("CC{" + referenceID + "}");
    return;
    }

  if(servo.equals("JAW")){
    if(detail.startsWith("SPEED")){
     jawServoSpeed = detail.substring(5).toInt();
    } else if (detail.startsWith("PIN")){
      jawServoPin = detail.substring(3).toInt();
      jawServo.detach();
      jawServo.attach(jawServoPin);
    } else {
      String errorMessage = "ER{message:JAW Set Directive not recognized, referenceID:" + referenceID + "}";
      Serial.println(errorMessage);
      return;
    }

    Serial.println("CC{" + referenceID + "}");
    return;
    
  } else if(servo.equals("NECKPIVOT")){
    if(detail.startsWith("SPEED")){
      neckPivotServoSpeed = detail.substring(5).toInt();
    } else if (detail.startsWith("PIN")){
      neckPivotServoPin = detail.substring(3).toInt();
      neckPivotServo.detach();
      neckPivotServo.attach(neckPivotServoPin);
    } else {
      String errorMessage = "ER{message:NECKPIVOT Set Directive not recognized, referenceID:" + referenceID + "}";
      Serial.println(errorMessage);
      return;
    }

    Serial.println("CC{" + referenceID + "}");
    return;
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

void setServoPin(String servoName, String commandReferenceID, int pinNumber){
  if(servoName.equals("eyesVerticalServo")){
    eyesVerticalServoPin = pinNumber;
    Serial.println("CC:{" + commandReferenceID + "}");
  }
  if(servoName.equals("eyesHorizontalServo")){
    eyesHorizontalServoPin = pinNumber;
    Serial.println("CC:{" + commandReferenceID + "}");
  }
  if(servoName.equals("jawServo")){
    jawServoPin = pinNumber;
    Serial.println("CC:{" + commandReferenceID + "}");
  }
  if(servoName.equals("neckPivotServo")){
    neckPivotServoPin = pinNumber;
    Serial.println("CC:{" + commandReferenceID + "}");
  }
}

void setServoSpeed(String servoName, String commandReferenceID, int servoSpeed){
  if(servoName.equals("eyesVerticalServo")){
    eyesVerticalServoSpeed = servoSpeed;
    Serial.println("CC:{" + commandReferenceID + "}");
  }
  if(servoName.equals("eyesHorizontalServo")){
    eyesHorizontalServoSpeed = servoSpeed;
    Serial.println("CC:{" + commandReferenceID + "}");
  }
  if(servoName.equals("jawServo")){
    jawServoSpeed = servoSpeed;
    Serial.println("CC:{" + commandReferenceID + "}");
  }
  if(servoName.equals("neckPivotServo")){
    neckPivotServoPin = servoSpeed;
    Serial.println("CC:{" + commandReferenceID + "}");
  }
}
