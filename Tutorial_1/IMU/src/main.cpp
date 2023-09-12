#include <Arduino.h>
// Basic demo for accelerometer readings from Adafruit MPU6050

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
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

const unsigned int PWM_VALUE = 450; // Max PWM given 8 bit resolution

const int freq = 5000;
const int resolution = 10;
Adafruit_MPU6050 mpu;

const unsigned int ADC_1_CS = 2;
const unsigned int ADC_2_CS = 17;

float initial_gyr[3];

void setup(void) {
  // Stop the right motor by setting pin 14 low
  // this pin floats high or is pulled
  // high during the bootloader phase for some reason
  pinMode(14, OUTPUT);
  digitalWrite(14, LOW);
  delay(100);
  
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  pinMode(ADC_1_CS, OUTPUT);
  pinMode(ADC_2_CS, OUTPUT);

  digitalWrite(ADC_1_CS, HIGH); // Without this the ADC's write
  digitalWrite(ADC_2_CS, HIGH); // to the SPI bus while the nRF24 is!!!!

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

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


  Serial.println("");
  delay(100);

  // set initial position
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  memcpy(initial_gyr, g.gyro.v, __SIZEOF_FLOAT__*3);

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

// computes v = a*v1 + b*v2
void lin_comb_3vec(float* v, float a, float* v1, float b, float* v2) {
  for(int i=1; i < 3; i++) {
    v[i] = a*v1[i] + b*v2[i];
  }
}

// computes v = v1 + v2
void add_3vec(float* v, float* v1, float* v2) {
  lin_comb_3vec(v, 1, v1, 1, v2);
}

// computes v = v1 - v2
void sub_3vec(float* v, float* v1, float* v2) {
  lin_comb_3vec(v, 1, v1, -1, v2);
}

int last_timestamp = 0;
int delay_timestamp = 0;
float rotations[] = {90, -180};
int num_rotations = 2;
int delay_millis = 2*1000;
int rot_index = 0;
float eps_deg = 2; // acceptable error in degrees for taking 2 angles as the same
float rotation_degrees = 0;
float rotation_target = rotations[0];

void loop() {
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  /* Print out the values */
  /*
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.print(" m/s^2 \t");
  */

  Serial.print("Rotation X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  Serial.print(" rad/s \t");

  Serial.print("dt: ");
  Serial.print(g.timestamp - last_timestamp);
  Serial.print(" ms \t");

  Serial.print("Rotation Number: ");
  if(rot_index >= num_rotations) {
    Serial.print("N/A \t");
  }
  else {
    Serial.print(rot_index);
    Serial.print(" \t");
  }

  Serial.print("Rotation: ");
  Serial.print(rotation_degrees);
  Serial.println(" degrees");

  float gyr[3];
  memcpy(gyr, g.gyro.v, __SIZEOF_FLOAT__*3);
  sub_3vec(gyr, gyr, initial_gyr); // remove any DC offset, will still suffer from slow drift and fast noise
  float delta_t = ((float)(g.timestamp - last_timestamp))/1000; // change in time between measurements
  // assuming IMU reasonably well aligned in axis of motion
  rotation_degrees += delta_t * 57.296 * g.gyro.z; // do simple Riemann sum, convert to degrees first
  // rotations logic
  if(rot_index >= num_rotations) {
    // done with rotations, do nothing
  }
  else if (abs(rotation_target - rotation_degrees) > eps_deg) {
    // have not finished rotation, continue rotating
    if(rotation_target - rotation_degrees > 0) {
      // rotate left
    }
    else {

    }
    delay_timestamp = g.timestamp;
  }
  else if (rot_index < num_rotations - 1) {
    // have completed rotation, and have at least one more to do
    
    if (g.timestamp - delay_timestamp > delay_millis) { //delay before starting next rotation
      rotation_target += rotations[++rot_index]; // increment index and update target rotation
    }
  }
  else {
    // just finished, increase index to mark done
    rot_index++;
  }

  last_timestamp = g.timestamp;
}