//
// Created by Liubov Udalova on 05.04.25.
//

#include "msg_codec.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/socket.h>

//TODO description
enum CodecState read_socket(struct CodecData *codec_data) {
    enum CodecState current_state = codec_data->state;
    switch (current_state) {
        case INIT:
        case MSG_DATA_RX_DONE: {
            codec_data->rx_header_len = 0;
            memset(codec_data->rx_header_buf, 0, HEADER_LENGTH);
            codec_data->rx_data_len = 0;
            memset(codec_data->rx_data_buf, 0, MAX_MSG_DATA_LENGTH);
            //TODO handle errors
            //Note: intentionally no break, continue reading header
        }
        case MSG_HEADER_RX_RUNNING: {
            ssize_t recv_header_size = recv(codec_data->socket_fd,
                                            codec_data->rx_header_buf + codec_data->rx_header_len,
                                            HEADER_LENGTH - codec_data->rx_header_len, 0);
            if (recv_header_size < 0) {
                fprintf(stderr, "Failed to receive message header: %s\n", strerror(errno));
                current_state = ERROR;
                break;
            }
            if (recv_header_size < HEADER_LENGTH - codec_data->rx_header_len) {
                codec_data->rx_header_len += recv_header_size;
                current_state = MSG_HEADER_RX_RUNNING;
                break;
            }
            current_state = MSG_HEADER_RX_DONE;
            break;
        }
        case MSG_HEADER_RX_DONE:
        case MSG_DATA_RX_RUNNING: {
            struct MsgHeader header = *(struct MsgHeader *) codec_data->rx_header_buf;
            ssize_t recv_data_size = recv(codec_data->socket_fd, codec_data->rx_data_buf + codec_data->rx_data_len,
                                          header.msg_data_length - codec_data->rx_data_len, 0);
            if (recv_data_size < 0) {
                fprintf(stderr, "Failed to receive message header: %s\n", strerror(errno));
                current_state = ERROR;
                break;
            }
            if (recv_data_size < header.msg_data_length - codec_data->rx_data_len) {
                codec_data->rx_data_len += recv_data_size;
                current_state = MSG_DATA_RX_RUNNING;
                break;
            }
            current_state = MSG_DATA_RX_DONE;
            break;
        }
        default: {
            fprintf(stderr, "Unknown codec state: %d\n", current_state);
            current_state = ERROR;
        }
    }
    codec_data->state = current_state;
    return current_state;
}

//TODO description
ssize_t write_socket(struct CodecData *codec_data, u_int8_t msg_type, char *msg_data, u_int16_t msg_length) {
    struct MsgHeader header = {
        .msg_data_length = msg_length,
        .msg_type = msg_type
    };
    ssize_t sent_header_size = send(codec_data->socket_fd, &header, sizeof(header), 0);
    if (sent_header_size < 0) {
        fprintf(stderr, "Failed to send message header: %s\n", strerror(errno));
        return -1;
    }
    ssize_t sent_size = send(codec_data->socket_fd, msg_data, msg_length, 0);
    if (sent_size < 0) {
        fprintf(stderr, "Failed to send message text: %s\n", strerror(errno));
        return -1;
    }
    return sent_size;
}
