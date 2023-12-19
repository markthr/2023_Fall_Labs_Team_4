#include <stdio.h>
#include <Arduino.h>
#include <stdio.h>
#include "udp_packets.hpp"

// used for serialization
#define TIME_FORMAT_LEN 12 // HH:MM:SS:MMM is 12 characters
#define FLOAT_FORMAT_LEN  8 //allow 6 significant digits, decimal, and sign
#define STATE_LEN 2 // 2 digits for displaying the state
#define FLAG_LEN 1 // just use 1 and 0
#define GROUP_DELIM_LEN 2 // { and }

// SEND_PACKET_FIELDS covers the delimeters and the 
const int RECEIVE_PACKET_LEN = TIME_FORMAT_LEN + FLOAT_FORMAT_LEN*3 + STATE_LEN + GROUP_DELIM_LEN + SEND_PACKET_FIELDS + 1;

Serialization_Result deserialize_receive_packet(Receive_Packet& receive_packet, const char* buf) {
    int assignments = sscanf(buf, "%u, %d, %d, %f, %f",
        &receive_packet.timestamp, &receive_packet.next_state, &receive_packet.ball_detected,
        &receive_packet.ball_offset_x, &receive_packet.ball_offset_y);
    receive_packet.valid = assignments < RECEIVE_PACKET_FIELDS;
    receive_packet.stale = false;

    if(receive_packet.valid) {
        return Serialization_Result::Bad_Format;
    }
    else {
        return Serialization_Result::Success;
    }
}

Serialization_Result serialize_send_packet(const Send_Packet& send_packet, char* buf, int buf_len) {
    bool overflow = buf_len < RECEIVE_PACKET_LEN;

    // get HH:MM:SS:MMM from millis
    long milliseconds = send_packet.timestamp;
    int seconds = milliseconds/1000;
    milliseconds %= 1000;
    int minutes = seconds/60;
    seconds %= 60;
    int hours = minutes/60;
    minutes %= 60;
    hours %= 100;

    snprintf(buf, buf_len, "%02d:%02d:%02d:%03d,%d,%d,%.4f,%.4f,%.4f,%.64s;",
        hours, minutes, seconds, milliseconds, overflow, send_packet.current_state, 
        send_packet.angular_acc_x, send_packet.angular_acc_y, send_packet.angular_acc_z,
        send_packet.state_log);
    
    if(overflow) {
        return Serialization_Result::Buf_Len;
    }
    else {
        return Serialization_Result::Success;
    }
}