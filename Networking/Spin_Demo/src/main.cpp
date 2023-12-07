#include <Arduino.h>
#include <string>
#include <U8x8lib.h>
#include <Encoder.h>
#include <Adafruit_MCP3008.h>
#include <Adafruit_MPU6050.h>
#include <WiFiUdp.h>
#include <WiFi.h>
#include "state_machine.hpp"
#include "state_machine.cpp"
#include "motors.hpp"
#include "passwd.h"

#define OLED_CLOCK 15
#define OLED_DATA 4
#define OLED_RESET 16

#define ADC_1_CS 2
#define ADC_2_CS 17

#define MPU_IMU_RANGE MPU6050_RANGE_8_G
#define MPU_GYR_RANGE MPU6050_RANGE_500_DEG
#define MPU_FILTER MPU6050_BAND_21_HZ

#define M1_ENC_A 39
#define M1_ENC_B 38
#define M2_ENC_A 37
#define M2_ENC_B 36

#define UDP_LOCAL_PORT 2390

#define UDP_BUF_LEN 256

const float METERS_PER_TICK = (3.14159 * 0.032) / 360.0; // The diameter of the wheel is 0.032 not 0.031
const float TURNING_RADIUS_METERS = 4.3 / 100.0; // Wheels are about 4.3 cm from pivot point

Adafruit_MCP3008 adc1;
Adafruit_MCP3008 adc2;

void init_adc() {
  pinMode(ADC_1_CS, OUTPUT);
  pinMode(ADC_2_CS, OUTPUT);

  digitalWrite(ADC_1_CS, HIGH); // Without this the ADC's write
  digitalWrite(ADC_2_CS, HIGH); // to the SPI bus while the nRF24 is!!!!

  adc1.begin(ADC_1_CS);
  adc2.begin(ADC_2_CS);
  
  delay(10);
}

Adafruit_MPU6050 mpu;

void init_mpu() {
  mpu.begin();
  delay(10);

    if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  mpu.setAccelerometerRange(MPU_IMU_RANGE);
  mpu.setGyroRange(MPU_GYR_RANGE);
  mpu.setFilterBandwidth(MPU_FILTER);
}


U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(OLED_CLOCK, OLED_DATA, OLED_RESET);

void init_screen() {
  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.clear();
}

WiFiUDP udp;

void init_wifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifi_ssid.c_str(), pass.c_str());

  u8x8.drawString(0, 0, "Connecting to: ");
  u8x8.drawString(0, 1,  wifi_ssid.c_str());
  u8x8.drawString(0, 2, "Result: ");

  while (WiFi.status() != (int)wl_status_t::WL_CONNECTED) {
    u8x8.drawString(0, 3, std::to_string(WiFi.status()).c_str());
    delay(500);
  }

  udp.begin(UDP_LOCAL_PORT);
}

struct Receive_Packet {
  unsigned int timestamp;
  unsigned int next_state;
};

struct Send_Packet {
  unsigned int current_state;
  float angular_acc_x;
  float angular_acc_y;
  float angular_acc_z;
  char state_log[64];
};

struct Input {
  Encoder& left;
  Encoder& right;
  Adafruit_MPU6050& mpu;
  Receive_Packet& receive_packet;

  Input(Encoder& left, Encoder& right, Adafruit_MPU6050& mpu, Receive_Packet& receive_packet) 
  : left(left), right(right), mpu(mpu), receive_packet(receive_packet) {}
};

struct Output {
  const Motor& left;
  const Motor& right;
  const U8X8_SSD1306_128X64_NONAME_SW_I2C& screen;
  char log[64];

  Output(Motor& left, Motor& right, U8X8_SSD1306_128X64_NONAME_SW_I2C& screen)
  : left(left), right(right), screen(screen) {}
};

class Rest_State : public Abstract_State<Input, Output> {
    public:
        static Rest_State& instance();
        
        
        Rest_State& get_next_state(const Input& input);

    private:
        Rest_State()
        : Abstract_State("Startup") {}
};
class Startup_State : public Abstract_State<Input, Output> {
    public:
        static Startup_State& instance() {
            static Startup_State __instance;
            return __instance;
        }
        
        
        Abstract_State& get_next_state(const Input& input) override {
            return Rest_State::instance();
        }


    private:
        Startup_State()
        : Abstract_State("Startup") {}
};

Rest_State& Rest_State::instance() {
    static Rest_State __instance;
    return __instance;
}
        
        
Rest_State& Rest_State::get_next_state(const Input& input) {
    return Rest_State::instance();
}

Encoder left_enc(M1_ENC_A, M1_ENC_B);
Encoder right_enc(M2_ENC_A, M2_ENC_B);
Motor left_motor(M1_IN_1_CHANNEL, M1_IN_2_CHANNEL);
Motor right_motor(M2_IN_1_CHANNEL, M2_IN_2_CHANNEL);

Receive_Packet receive_packet;
Input input(left_enc, right_enc, mpu, receive_packet);
Output output(left_motor, right_motor, u8x8);

State_Machine<Input, Output> sm(Startup_State::instance(), input, output);


void setup() {
  setup_motors();

  forward(&left_motor, 0);
  forward(&right_motor, 0);

  init_mpu();
  init_adc();
  
  init_wifi();

  u8x8.clear();
}

Send_Packet send_packet;

char packet_buffer[UDP_BUF_LEN];
char reply_buffer[UDP_BUF_LEN];
void loop() {
    int packetSize = udp.parsePacket();
  IPAddress remote_ip = udp.remoteIP();
  //Serial.println(WiFi.localIP());
  //Serial.println(WiFi.getHostname());
  if (packetSize) {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remoteIp = udp.remoteIP();
    Serial.print(remoteIp);
    Serial.print(", port ");
    Serial.println(udp.remotePort());
    // read the packet into packetBufffer
    int len = udp.read(packet_buffer, 255);
    if (len > 0) {
      packet_buffer[len] = 0;
    }
    Serial.println("Contents:");
    Serial.println(packet_buffer);
    // send a reply, to the IP address and port that sent us the packet we received
    udp.beginPacket(udp.remoteIP(), udp.remotePort());
    sm.get_state_name().copy(reply_buffer, UDP_BUF_LEN);
    udp.print(reply_buffer);
    udp.endPacket();
  }
  delay(5000);
  u8x8.clear();
  u8x8.drawString(0, 0, "Network: ");
  u8x8.drawString(0, 1, wifi_ssid.c_str());
  u8x8.drawString(0, 2, "Status: ");
  u8x8.drawString(0, 3, std::to_string(WiFi.status()).c_str());
  u8x8.drawString(0, 4, "Remote IP:");
  u8x8.drawString(0, 5, std::to_string(remote_ip).c_str());
}