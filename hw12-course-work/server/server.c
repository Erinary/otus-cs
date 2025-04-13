//
// Created by Liubov Udalova on 13.04.25.
//

#include "server.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/errno.h>
#include <arpa/inet.h>

//TODO logs with timestamp in stderr
void server_run(struct Server *server, int server_fd) {
    server->poll_fds[0] = (struct pollfd){
        .fd = server_fd,
        .events = POLLIN
    };
    struct pollfd *server_socket = &server->poll_fds[0];
    while (1) {
        int clients_number = server_refresh_poll_fds(server);
        int poll_result = poll(server->poll_fds, clients_number + 1, -1);
        if (poll_result < 0) {
            fprintf(stderr, "Failed to poll: %s\n", strerror(errno));
            return;
        }
        if (server_socket->revents & POLLIN) {
            server_accept_connection(server);
        }
        //TODO handle client sockets POLLINs, errors, etc
    }
}

int server_refresh_poll_fds(const struct Server *server) {
    int client_count = 0;
    for (int i = 0; i < MAX_CLIENTS_NUMBER; i++) {
        if (server->clients[i].codec_data.socket_fd != -1) {
            server->poll_fds[client_count + 1] = (struct pollfd){
                .fd = server->clients[i].codec_data.socket_fd,
                .events = POLLIN
            };
            client_count++;
        }
    }
    return client_count;
}

void server_accept_connection(struct Server *server) {
    struct sockaddr_in client_addr;
    socklen_t addr_size = sizeof client_addr;
    fprintf(stderr, "Accepting client connection\n");
    int socket_fd = accept(server->poll_fds[0].fd, (struct sockaddr *) &client_addr, &addr_size);
    if (socket_fd < 0) {
        fprintf(stderr, "Failed to accept connection: %s\n", strerror(errno));
        return;
    }
    for (int i = 0; i < MAX_CLIENTS_NUMBER; i++) {
        if (server->clients[i].codec_data.socket_fd == -1) {
            reset_codec(&server->clients[i].codec_data);
            server->clients[i].codec_data.socket_fd = socket_fd;
            memset(server->clients[i].username, 0, MAX_USERNAME_LEN);
            //TODO handle errors
            fprintf(stderr, "Connection established from: %s:%d\n", inet_ntoa(client_addr.sin_addr),
                    ntohs(client_addr.sin_port));
            return;
        }
    }
    fprintf(stderr, "Failed to accept connection: Reached maximum clients number\n");
    close(socket_fd);
    //TODO send error message to client
}
