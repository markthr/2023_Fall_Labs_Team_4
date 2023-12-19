#ifndef __UDP_PACKETS_H__
#define __UDP_PACKETS_H__

#define STATE_LOG_LEN 64
#define SEND_PACKET_FIELDS 7
#define RECEIVE_PACKET_FIELDS 2

struct Receive_Packet {
    bool stale; // this flag is always set to true during deserialization and false in the next cycle
    bool valid = false; // this flag indicates if the incoming packet was ill-formed
    unsigned long timestamp;
    unsigned int next_state;
    bool ball_detected;
    float ball_offset_x;
    float ball_offset_y;
};

struct Send_Packet {
    unsigned long timestamp; // this is ignored during serialization, generated there
    bool packet_overflow; // this is ignored during serialization, generated there
    unsigned int current_state;
    float angular_acc_x;
    float angular_acc_y;
    float angular_acc_z;
    char state_log[STATE_LOG_LEN];
};

/**
 * Success: the operation completed normally
 * Buf_Len: The buffer was too short to write the full struct to or did not contain enough data to write to every field of the struct
 * Bad_Format: Serialization or deserialization was unable to be completed due to formatting issues
*/
enum class Serialization_Result {Success, Buf_Len, Bad_Format};

/**
 * Read the contents of buf into a Receive_Packet struct
 * 
 * buf MUST be NULL terminated
*/
Serialization_Result deserialize_receive_packet(Receive_Packet& receive_packet, const char* buf);

/**
 * Write the contents of a Send_Packet struct into a char* buff
*/
Serialization_Result serialize_send_packet(const Send_Packet& send_packet, char* buf, int buf_len);

#endif