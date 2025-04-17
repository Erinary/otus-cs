#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <secure/_string.h>
#include <sys/errno.h>
#include <sys/socket.h>
#include <sys/poll.h>

#include "../libs/msg_codec.h"
#include "../libs/msg_types.h"
//
// Created by Liubov Udalova on 05.04.25.
//
int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: ./main <IP> <port> <username>\n");
        return -1;
    }
    const char *ip = argv[1];
    const char *port = argv[2];
    const char *username = argv[3];

    if (strlen(username) > MAX_USERNAME_LEN) {
        fprintf(stderr, "Username is too long: exceeded %d\n symbols", MAX_USERNAME_LEN);
        return -1;
    }

    struct addrinfo hints;
    struct addrinfo *res;
    memset(&hints, 0, sizeof hints);

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

    char client_msg[MAX_MSG_DATA_LENGTH + 1];
    struct CodecData codec_data;
    codec_data.socket_fd = socket_fd;

    struct pollfd poll_fds[2] = {
        {
            .fd = socket_fd,
            .events = POLLIN
        },
        {
            .fd = STDIN_FILENO,
            .events = POLLIN
        }
    };
    struct pollfd *socket_poll = &poll_fds[0];
    struct pollfd *stdin_poll = &poll_fds[1];

    memset(client_msg, 0, sizeof client_msg);
    memcpy(client_msg, username, strlen(username));
    ssize_t sent_login_size = write_socket(&codec_data, MSG_TYPE_LOGIN, client_msg, strlen(client_msg));
    if (sent_login_size < 0) {
        fprintf(stderr, "Failed to send login message: %s\n", strerror(errno));
        return -1;
    }

    while (1) {
        int poll_result = poll(poll_fds, 2, -1);
        if (poll_result < 0) {
            fprintf(stderr, "Failed to poll: %s\n", strerror(errno));
            return -1;
        }
        if (socket_poll->revents & POLLHUP) {
            fprintf(stderr, "Connection closed\n");
            return -1;
        }
        if (socket_poll->revents & POLLERR) {
            fprintf(stderr, "Connection error\n");
            return -1;
        }
        if (socket_poll->revents & POLLIN) {
            enum CodecState state = read_socket(&codec_data);
            if (state == ERROR) {
                fprintf(stderr, "Failed to handle client message\n");
                close(socket_fd);
                return -1;
            }
            if (state == CLOSED_BY_COUNTERPARTY) {
                //Socket is already closed during read_socket method
                fprintf(stderr, "Server has closed connection\n");
                return -1;
            }
            if (state == MSG_DATA_RX_DONE) {
                struct MsgHeader header = *(struct MsgHeader *) codec_data.rx_header_buf;
                if (header.msg_type == MSG_TYPE_USER_TEXT) {
                    fprintf(stdout, "%s\n", codec_data.rx_data_buf);
                } else if (header.msg_type == MSG_TYPE_SYSTEM_TEXT) {
                    fprintf(stderr, "Server: %s\n", codec_data.rx_data_buf);
                } else {
                    fprintf(stderr, "Unknown message type\n");
                }
            }
        }
        if (stdin_poll->revents & POLLIN) {
            memset(client_msg, 0, sizeof client_msg);
            char *input_res = fgets(client_msg, MAX_MSG_DATA_LENGTH, stdin);
            if (input_res == NULL) {
                fprintf(stderr, "Failed to read from stdin: %s\n", strerror(errno));
                return -1;
            }
            ssize_t sent_size = write_socket(&codec_data, MSG_TYPE_USER_TEXT, client_msg, strlen(client_msg));
            if (sent_size < 0) {
                fprintf(stderr, "Failed to send message: %s\n", strerror(errno));
                return -1;
            }
        }
    }
    return 0;
}
