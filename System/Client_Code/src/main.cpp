#include <Arduino.h>
#include <string>
#include <U8x8lib.h>
// #include <Encoder.h>
#include <Adafruit_MCP3008.h>
#include <Adafruit_MPU6050.h>
#include <WiFiUdp.h>
#include <WiFi.h>
#include "state_machine.hpp"
#include "state_machine.cpp"
#include "motors.hpp"
#include "passwd.h"
#include "udp_packets.hpp"
#include "sd_state_machine.hpp"

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
#define PACKET_DECIMATION_RATE 10 // only send a packet every 10th cycle by default


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







// Encoder left_enc(M1_ENC_A, M1_ENC_B);
// Encoder right_enc(M2_ENC_A, M2_ENC_B);

Encoder left_enc;
Encoder right_enc;
Motor left_motor(M1_IN_1_CHANNEL, M1_IN_2_CHANNEL);
Motor right_motor(M2_IN_1_CHANNEL, M2_IN_2_CHANNEL);

Receive_Packet receive_packet;
Send_Packet send_packet;
Input input(left_enc, right_enc, receive_packet);
Output output(left_motor, right_motor, u8x8, send_packet);

class Bot_State_Machine : public State_Machine<Input, Output> {
  public:
    Bot_State_Machine(Abstract_State<Input, Output>& initial_state, 
        Input& input, Output& output, Adafruit_MPU6050& mpu, WiFiUDP& udp)
    : State_Machine(initial_state, input, output) , mpu(mpu), udp(udp) {}

  private:
    Adafruit_MPU6050& mpu;
    WiFiUDP& udp;

    char receive_buffer[UDP_BUF_LEN];
    char reply_buffer[UDP_BUF_LEN];

    bool remote_ip_init = false;
    IPAddress remote_ip;
    uint16_t remote_port;
    unsigned int counter = 0; // used to rate limit sending UDP packets

  private:
    void generate_send_packet() {
      this->output.send_packet.current_state = this->get_state_code();
      this->output.send_packet.angular_acc_x = this->input.gyro.gyro.x;
      this->output.send_packet.angular_acc_y = this->input.gyro.gyro.y;
      this->output.send_packet.angular_acc_z = this->input.gyro.gyro.z;
      this->output.send_packet.timestamp = this->input.gyro.timestamp;
    }

    void receive_UDP_packet(Receive_Packet& receive_packet) {
      int packetSize = this->udp.parsePacket();
      if (packetSize) {
        // logging for debug
        Serial.print("Received packet of size ");
        Serial.println(packetSize);
        Serial.print("From ");
        Serial.print(this->udp.remoteIP());
        Serial.print(", port ");
        Serial.println(this->udp.remotePort());

        // read the packet into packetBufffer
        int len = this->udp.read(receive_buffer, 255);
        if (len > 0) {
          this->receive_buffer[len] = 0;
        }

        // logging for debug
        Serial.println("Contents:");
        Serial.println(receive_buffer);
        
        // save ip if first receive
        if(!remote_ip_init) {
          remote_ip = this->udp.remoteIP();
          remote_port = this->udp.remotePort();
          remote_ip_init = true;
        }
        deserialize_receive_packet(receive_packet, receive_buffer);
      }
      else {
        // no new packet, whatever packet is in the buffer has already been read
        receive_packet.stale = true;
      }

    }
  
  public:
    void fetch_input() override{
      mpu.getEvent(&this->input.accel, &this->input.gyro, &this->input.temp);
      receive_UDP_packet(this->input.receive_packet);
    }

    void post_iterate() override {
      if(this->remote_ip_init && this->counter++ % PACKET_DECIMATION_RATE == 0) {
        generate_send_packet();
        serialize_send_packet(this->output.send_packet, this->reply_buffer, UDP_BUF_LEN);

        udp.beginPacket(this->remote_ip, this->remote_port);
        udp.print(this->reply_buffer);
        udp.endPacket();
      }
  }
};

Bot_State_Machine sm(Startup_State::instance(), input, output, mpu, udp);


void setup() {
  Serial.begin(115200);
  Serial.println("Setup Motors");
  //init_adc();
  
  left_motor.init(M1_IN_1, M1_IN_2, M1_I_SENSE);
  right_motor.init(M2_IN_1, M2_IN_2, M2_I_SENSE);
  Serial.println("Stop Motors");
  left_motor.stop();
  right_motor.stop();

  init_mpu();
  Serial.println("Setup Screen");
  init_screen();
  
  init_wifi();
}

void loop() {
  sm.fetch_input();
  sm.iterate();
  sm.post_iterate();
}