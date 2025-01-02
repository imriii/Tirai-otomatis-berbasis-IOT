#define BLYNK_TEMPLATE_ID "TMPL6VU2tAxIT"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "2O1irE72r609ApqmeYZSUWRul79DbIle"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Encoder.h>

char ssid[] = "ALHAMDULILLAH";
char pass[] = "alhamdulillah010101";

const int motorPin1 = D1;
const int motorPin2 = D2;
const int enablePin = D3;
const int encoderPinA = D5;
const int encoderPinB = D6;

Encoder motorEncoder(encoderPinA, encoderPinB);
const int MOTOR_SPEED = 240;  // Maintained at 200 for high torque
const unsigned long DESIRED_MOVEMENT_TIME = 4000; // 4 seconds in milliseconds
const int TARGET_ENCODER_VALUE = 19350; // Calculated: (1013 * (4/0.36))

void setup() {
  Serial.begin(115200);
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(enablePin, OUTPUT);
  analogWriteRange(255);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  Blynk.run();
}

BLYNK_WRITE(V0) {
  int value = param.asInt();
  if (value == 1) {
    openCurtain();
  } else {
    closeCurtain();
  }
}

void openCurtain() {
  Serial.println("Membuka tirai...");
  motorEncoder.write(0);
  unsigned long startTime = millis();
  
  while (motorEncoder.read() < TARGET_ENCODER_VALUE) {
    analogWrite(enablePin, MOTOR_SPEED);
    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, LOW);
    
    // Print debug info every 250ms
    if ((millis() - startTime) % 250 == 0) {
      Serial.print("Time: ");
      Serial.print((millis() - startTime) / 1000.0);
      Serial.print("s, Encoder: ");
      Serial.println(motorEncoder.read());
    }
    
    yield();
  }
  
  stopMotor();
  unsigned long duration = millis() - startTime;
  Serial.print("Total time: ");
  Serial.print(duration / 1000.0);
  Serial.println(" seconds");
  Serial.print("Final encoder value: ");
  Serial.println(motorEncoder.read());
  Serial.println("Tirai dibuka!");
}

void closeCurtain() {
  Serial.println("Menutup tirai...");
  motorEncoder.write(0);
  unsigned long startTime = millis();
  
  while (motorEncoder.read() > -TARGET_ENCODER_VALUE) {
    analogWrite(enablePin, MOTOR_SPEED);
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, HIGH);
    
    // Print debug info every 250ms
    if ((millis() - startTime) % 250 == 0) {
      Serial.print("Time: ");
      Serial.print((millis() - startTime) / 1000.0);
      Serial.print("s, Encoder: ");
      Serial.println(motorEncoder.read());
    }
    
    yield();
  }
  
  stopMotor();
  unsigned long duration = millis() - startTime;
  Serial.print("Total time: ");
  Serial.print(duration / 1000.0);
  Serial.println(" seconds");
  Serial.print("Final encoder value: ");
  Serial.println(motorEncoder.read());
  Serial.println("Tirai ditutup!");
}

void stopMotor() {
  analogWrite(enablePin, 0);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
}