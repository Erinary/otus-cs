#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/errno.h>
#include <sys/poll.h>

#define msg_buff_len 65
//
// Created by Liubov Udalova on 25.03.25.
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

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int addrinfo_res = getaddrinfo(ip, port, &hints, &res);
    if (addrinfo_res != 0) {
        fprintf(stderr, "Failed to set Internet address info, getaddrinfo: %s\n", gai_strerror(addrinfo_res));
        return -1;
    }

    int socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    int connect_res = connect(socket_fd, res->ai_addr, res->ai_addrlen);
    if (connect_res != 0) {
        fprintf(stderr, "Failed to connect to server: %s\n", strerror(errno));
        return -1;
    }
    fprintf(stderr, "Connection established\n");

    char server_msg[msg_buff_len];
    char client_msg[msg_buff_len];

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
    while (1) {
        int poll_result = poll(poll_fds, 2, -1);
        if (poll_result < 0) {
            fprintf(stderr, "Failed to poll: %s\n", strerror(errno));
            break;
        }
        if (poll_fds[0].revents & POLLHUP) {
            fprintf(stderr, "Connection closed\n");
            break;
        }
        if (poll_fds[1].revents & POLLERR) {
            fprintf(stderr, "Connection error\n");
            break;
        }
        if (poll_fds[0].revents & POLLIN) {
            //Received data in socket
            ssize_t recv_size = recv(socket_fd, server_msg, msg_buff_len - 1, 0);
            if (recv_size < 0) {
                fprintf(stderr, "Failed to receive message: %s\n", strerror(errno));
                close(socket_fd);
                break;
            }
            fprintf(stdout, "\nServer: %s\n", server_msg);
            memset(server_msg, 0, sizeof server_msg);
        } else if (poll_fds[1].revents & POLLIN) {
            //Received data from stdin
            char *input_res = fgets(client_msg, msg_buff_len, stdin);
            if (input_res == NULL) {
                fprintf(stderr, "Failed to read from stdin: %s\n", strerror(errno));
                break;
            }
            ssize_t sent_size = send(socket_fd, client_msg, strlen(client_msg), 0);
            if (sent_size < 0) {
                fprintf(stderr, "Failed to send message: %s\n", strerror(errno));
                break;
            }
            memset(client_msg, 0, sizeof client_msg);
        }
    }
    freeaddrinfo(res);
    close(socket_fd);
    return 0;
}
