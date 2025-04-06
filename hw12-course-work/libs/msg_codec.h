//
// Created by Liubov Udalova on 05.04.25.
//
#ifndef MSG_CODEC_H
#define MSG_CODEC_H

#include <sys/_types/_ssize_t.h>
#include <sys/_types/_u_int16_t.h>
#include <sys/_types/_u_int8_t.h>
#define MAX_MSG_DATA_LENGTH 0xffff
#define HEADER_LENGTH sizeof(struct MsgHeader)

enum CodecState {
    INIT,
    MSG_HEADER_RX_RUNNING,
    MSG_HEADER_RX_DONE,
    MSG_DATA_RX_RUNNING,
    MSG_DATA_RX_DONE,
    ERROR,
};

struct MsgHeader {
    u_int16_t msg_data_length;
    u_int8_t msg_type;
};

struct CodecData {
    int socket_fd;
    enum CodecState state;
    char rx_data_buf[MAX_MSG_DATA_LENGTH];
    char rx_header_buf[HEADER_LENGTH];
    ssize_t rx_data_len;
    ssize_t rx_header_len;
};

enum CodecState read_socket(struct CodecData *codec_data);

ssize_t write_socket(struct CodecData *codec_data, u_int8_t msg_type, char *msg_data, u_int16_t msg_length);

#endif //MSG_CODEC_H
