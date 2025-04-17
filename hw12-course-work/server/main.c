#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <secure/_string.h>
#include <sys/errno.h>
#include <sys/signal.h>
#include <sys/socket.h>

#include "server.h"
//
// Created by Liubov Udalova on 05.04.25.
//
struct Server server;
int server_fd = -1;
struct addrinfo *res;

void handle_sigint(int sig) {
    if (server_fd >= 0) {
        close(server_fd);
    }
    server_stop(&server);
    freeaddrinfo(res);
    exit(0);
}

int main(int argc, char *argv[]) {
    signal(SIGINT, handle_sigint);
    signal(SIGTERM, handle_sigint);

    if (argc < 2 || argc > 3) {
        printf("Usage: ./main <port> [<IP>]\n");
        return -1;
    }
    const char *port = argv[1];
    const char *ip = argc == 3 ? argv[2] : NULL;

    struct addrinfo hints;
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

    server_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    int bind_result = bind(server_fd, res->ai_addr, res->ai_addrlen);
    if (bind_result < 0) {
        fprintf(stderr, "Failed to bind to socket: %s\n", strerror(errno));
        return -1;
    }
    int listen_result = listen(server_fd, 2);
    if (listen_result < 0) {
        fprintf(stderr, "Failed to listen on socket: %s\n", strerror(errno));
        return -1;
    }
    server_run(&server, server_fd);
    return 0;
}
