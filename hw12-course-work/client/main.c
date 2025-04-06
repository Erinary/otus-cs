#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <secure/_string.h>
#include <sys/errno.h>
#include <sys/socket.h>

#include "../libs/msg_codec.h"
//
// Created by Liubov Udalova on 05.04.25.
//
int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: ./main <IP> <port>\n");
        return -1;
    }
    const char *ip = argv[1];
    const char *port = argv[2];

    struct addrinfo hints;
    struct addrinfo *res;
    memset(&hints, 0, sizeof hints);
    //TODO handle error

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int addrinfo_res = getaddrinfo(ip, port, &hints, &res);
    if (addrinfo_res != 0) {
        fprintf(stderr, "Failed to set Internet address info, getaddrinfo: %s\n", gai_strerror(addrinfo_res));
        return -1;
    }

    int socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (socket_fd == -1) {
        perror("Failed to create socket");
        return -1;
    }

    int connect_res = connect(socket_fd, res->ai_addr, res->ai_addrlen);
    if (connect_res != 0) {
        fprintf(stderr, "Failed to connect to server: %s\n", strerror(errno));
        return -1;
    }
    fprintf(stderr, "Connection established\n");

    //TODO below is not final version
    struct CodecData codec_data;
    codec_data.socket_fd = socket_fd;

    while (1) {
        enum CodecState new_state = read_socket(&codec_data);
        if (new_state == MSG_DATA_RX_DONE) {
            printf("Got message: %s\n", codec_data.rx_data_buf);
        } else if (new_state == ERROR) {
            fprintf(stderr, "Error reading message\n");
            return -1;
        }
    }
    return 0;
}
