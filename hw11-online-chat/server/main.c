#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>

#define msg_buff_len 65

//
// Created by Liubov Udalova on 25.03.25.
//
int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 3) {
        printf("Usage: ./main <port> [<IP>]\n");
        return -1;
    }
    const char *port = argv[1];
    const char *ip = argc == 3 ? argv[2] : NULL;

    struct addrinfo hints;
    struct addrinfo *res;
    memset(&hints, 0, sizeof hints);

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if (ip == NULL) {
        hints.ai_flags = AI_PASSIVE;
    }
    int addrinfo_res = getaddrinfo(ip, port, &hints, &res);
    if (addrinfo_res != 0) {
        fprintf(stderr, "Failed to set Internet address info, getaddrinfo: %s\n", gai_strerror(addrinfo_res));
        return -1;
    }

    int socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    int bind_result = bind(socket_fd, res->ai_addr, res->ai_addrlen);
    if (bind_result < 0) {
        fprintf(stderr, "Failed to bind to socket: %s\n", strerror(errno));
        return -1;
    }
    int listen_result = listen(socket_fd, 2);
    if (listen_result < 0) {
        fprintf(stderr, "Failed to listen on socket: %s\n", strerror(errno));
        return -1;
    }

    struct sockaddr_in client_addr;
    while (1) {
        socklen_t addr_size = sizeof client_addr;
        fprintf(stderr, "Waiting for a client connection\n");
        int connection_fd = accept(socket_fd, (struct sockaddr *) &client_addr, &addr_size);
        if (connection_fd < 0) {
            fprintf(stderr, "Failed to accept connection: %s\n", strerror(errno));
            continue;
        }
        fprintf(stderr, "Connection established from: %s:%d\n", inet_ntoa(client_addr.sin_addr),
                ntohs(client_addr.sin_port));

        char client_msg[msg_buff_len];
        char server_msg[msg_buff_len];

        struct pollfd poll_fds[2] = {
            {
                .fd = connection_fd,
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
            if (poll_fds[0].revents & POLLERR) {
                fprintf(stderr, "Connection error\n");
                break;
            }
            if (poll_fds[0].revents & POLLIN) {
                //Received data in socket
                ssize_t recv_size = recv(connection_fd, client_msg, msg_buff_len - 1, 0);
                if (recv_size < 0) {
                    fprintf(stderr, "Failed to receive message: %s\n", strerror(errno));
                    close(connection_fd);
                    continue;
                }
                fprintf(stdout, "\nClient: %s\n", client_msg);
                memset(client_msg, 0, sizeof client_msg);
            } else if (poll_fds[1].revents & POLLIN) {
                //Received data from stdin
                char *input_res = fgets(server_msg, msg_buff_len, stdin);
                if (input_res == NULL) {
                    fprintf(stderr, "Failed to read from stdin: %s\n", strerror(errno));
                    break;
                }
                ssize_t sent_size = send(connection_fd, server_msg, strlen(server_msg), 0);
                if (sent_size < 0) {
                    fprintf(stderr, "Failed to send message: %s\n", strerror(errno));
                    break;
                }
                memset(server_msg, 0, sizeof server_msg);
            }
        }
        close(connection_fd);
    }
    freeaddrinfo(res);
    close(socket_fd);
    return 0;
}
