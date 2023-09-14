#include <Arduino.h>

const unsigned int M1_IN_1 = 13;
const unsigned int M1_IN_2 = 12;
const unsigned int M2_IN_1 = 25;
const unsigned int M2_IN_2 = 14;

const unsigned int M1_IN_1_CHANNEL = 0;
const unsigned int M1_IN_2_CHANNEL = 1;
const unsigned int M2_IN_1_CHANNEL = 2;
const unsigned int M2_IN_2_CHANNEL = 3;

const unsigned int M1_I_SENSE = 35;
const unsigned int M2_I_SENSE = 34;

const unsigned int PWM_VALUE = 512; // Do not give max PWM. Robot will move fast

const int freq = 5000;
const int resolution = 10;


void setup() {
  Serial.begin(115200);
  
  ledcSetup(M1_IN_1_CHANNEL, freq, resolution);
  ledcSetup(M1_IN_2_CHANNEL, freq, resolution);
  ledcSetup(M2_IN_1_CHANNEL, freq, resolution);
  ledcSetup(M2_IN_2_CHANNEL, freq, resolution);

  ledcAttachPin(M1_IN_1, M1_IN_1_CHANNEL);
  ledcAttachPin(M1_IN_2, M1_IN_2_CHANNEL);
  ledcAttachPin(M2_IN_1, M2_IN_1_CHANNEL);
  ledcAttachPin(M2_IN_2, M2_IN_2_CHANNEL);

  pinMode(M1_I_SENSE, INPUT);
  pinMode(M2_I_SENSE, INPUT);
}

struct motor{
  const unsigned int in1;
  const unsigned int in2;
};

struct motor motors[] = {{M1_IN_1_CHANNEL, M1_IN_2_CHANNEL}, {M2_IN_1_CHANNEL, M2_IN_2_CHANNEL}};
int num_motors = 2;

int delay_millis = 2000;
void loop() {

  for(int i=0; i<num_motors; i++) {
    Serial.print("Motor[");
    Serial.print(i+1);
    Serial.println("]: Forward");
    ledcWrite(motors[i].in1, 0);
    ledcWrite(motors[i].in2, 512);
    delay(delay_millis);
    Serial.print("Motor[");
    Serial.print(i+1);
    Serial.println("]: Backward");
    ledcWrite(motors[i].in1, 512);
    ledcWrite(motors[i].in2, 0);
    delay(delay_millis);
    ledcWrite(motors[i].in1, 512);
    ledcWrite(motors[i].in2, 512);
    delay(delay_millis);
  }
}
