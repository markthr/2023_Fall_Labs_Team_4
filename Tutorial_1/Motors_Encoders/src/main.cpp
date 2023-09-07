#include <Arduino.h>
#include <Encoder.h>

const unsigned int M1_ENC_A = 39;
const unsigned int M1_ENC_B = 38;
const unsigned int M2_ENC_A = 37;
const unsigned int M2_ENC_B = 36;

const unsigned int M1_IN_1 = 13;
const unsigned int M1_IN_2 = 12;
const unsigned int M2_IN_1 = 25;
const unsigned int M2_IN_2 = 14;

const unsigned int M1_IN_1_CHANNEL = 8;
const unsigned int M1_IN_2_CHANNEL = 9;
const unsigned int M2_IN_1_CHANNEL = 10;
const unsigned int M2_IN_2_CHANNEL = 11;

const unsigned int M1_I_SENSE = 35;
const unsigned int M2_I_SENSE = 34;

const unsigned int PWM_VALUE = 512; // Max PWM given 8 bit resolution

const int freq = 5000;
const int resolution = 10;

void setup() {
  // Stop the right motor by setting pin 14 low
  // this pin floats high or is pulled
  // high during the bootloader phase for some reason
  pinMode(14, OUTPUT);
  digitalWrite(14, LOW);
  delay(100);

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
  Encoder enc;
};

void forward(struct motor* m, uint32_t duty){
  ledcWrite(m->in1, 0);
  ledcWrite(m->in2, duty);
}

void backward(struct motor* m, uint32_t duty){
  ledcWrite(m->in1, duty);
  ledcWrite(m->in2, 0);
}

void brake(struct motor* m) {
  ledcWrite(m->in1, 1023);
  ledcWrite(m->in2, 1023);
}

int counts_per_rot = 358;

int rotations[] = {counts_per_rot, -counts_per_rot};
int num_rots = 2;
void loop() {
  // Create the encoder objects after the motor has
  // stopped, else some sort exception is triggered
  struct motor motors[] = {
    {M1_IN_1_CHANNEL, M1_IN_2_CHANNEL, Encoder(M1_ENC_A, M1_ENC_B)},
    {M2_IN_1_CHANNEL, M2_IN_2_CHANNEL, Encoder(M2_ENC_A, M2_ENC_B)}};
  int num_motors = 2;

  int ref_counts[] = {motors[0].enc.read(), motors[1].enc.read()};
  int rot_index = 0;
  int eps_count = 2; // within 2 counts is okay
  bool rot_complete[] = {false, false};
  int duty = 450;
  while(true) {
    // loop over motors
    Serial.print("Rotation[");
    Serial.print(rot_index+1);
    Serial.print("]: ");
    if(rot_index < num_rots) {
      Serial.print(rotations[rot_index]);
    }
    else {
      Serial.print("N/A");
    }
    Serial.print(" \t");
    for(int i=0; i<2; i++) {
      // flip sign for M2, remove offset
      int enc_reading = motors[i].enc.read() * (i==1 ? -1 : 1) - ref_counts[i];

      Serial.print("Motor[");
      Serial.print(i+1);
      Serial.print("]: ");
      Serial.print(enc_reading);
      Serial.print(" \t");
      
      if(rot_index >= num_rots) {
        // done, do nothing, just write 0
        forward(&motors[i], 0);
        rot_complete[i] = false;
      }
      else if(abs(enc_reading  - rotations[rot_index]) > eps_count) {
        // need to keep rotating
        rot_complete[i] = false;

        if(enc_reading - rotations[rot_index] > 0) {
          // need to go backwards
          backward(&motors[i], duty);
        }
        else {
          forward(&motors[i], duty);
        }
      }
      else {
        // rotation completed
        rot_complete[i] = true;
        forward(&motors[i], 0); // turn off motor
      }
    }
    Serial.println();
    if(rot_complete[0] && rot_complete[1]) {
      brake(&motors[0]);
      brake(&motors[1]);
      delay(100);
      forward(&motors[0], 0);
      forward(&motors[1], 0);
      delay(4900);
      rot_index++;
    }
  }
}
