//
// Created by Liubov Udalova on 13.04.25.
//

#include "server.h"
#include "../libs/msg_types.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/errno.h>
#include <arpa/inet.h>

//TODO logs with timestamp in stderr
void server_run(struct Server *server, int server_fd) {
    fprintf(stderr, "Starting server...\n");
    server->poll_fds[0] = (struct pollfd){
        .fd = server_fd,
        .events = POLLIN
    };
    for (int i = 0; i < MAX_CLIENTS_NUMBER; i++) {
        reset_client_container(&server->clients[i]);
    }
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
        for (int i = 1; i <= clients_number; i++) {
            struct pollfd *client_pollfd = &server->poll_fds[i];
            struct ClientContainer *client_container = find_client_container(server, client_pollfd->fd);
            if (client_container == NULL) {
                fprintf(stderr, "Client not found in container list, poll file descriptor: %d\n", client_pollfd->fd);
                close(client_pollfd->fd);
                return;
            }
            if (client_pollfd->revents & POLLIN) {
                handle_client_message(server, client_container);
            } else if (client_pollfd->revents & POLLHUP) {
                fprintf(stderr, "Connection closed\n");
                reset_client_container(client_container);
            } else if (client_pollfd->revents & POLLERR) {
                fprintf(stderr, "Connection error\n");
                reset_client_container(client_container);
                close(client_pollfd->fd);
            }
        }
    }
}

void server_stop(struct Server *server) {
    fprintf(stderr, "Stopping server...\n");
    for (int i = 0; i < MAX_CLIENTS_NUMBER; i++) {
        int socket_fd = server->clients[i].codec_data.socket_fd;
        if (socket_fd != -1) {
            close(socket_fd);
        }
    }
}

int server_refresh_poll_fds(struct Server *server) {
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
            reset_client_container(&server->clients[i]);
            server->clients[i].codec_data.socket_fd = socket_fd;
            fprintf(stderr, "Connection established from: %s:%d\n", inet_ntoa(client_addr.sin_addr),
                    ntohs(client_addr.sin_port));
            return;
        }
    }
    fprintf(stderr, "Failed to accept connection: Reached maximum clients number\n");
    close(socket_fd);
    //TODO send error message to client
}

void handle_client_message(struct Server *server, struct ClientContainer *client_container) {
    int client_fd = client_container->codec_data.socket_fd;
    enum CodecState state = read_socket(&client_container->codec_data);
    if (state == ERROR) {
        fprintf(stderr, "Failed to handle client message\n");
        close(client_fd);
        reset_client_container(client_container);
    } else if (state == MSG_DATA_RX_DONE) {
        struct MsgHeader header = *(struct MsgHeader *) client_container->codec_data.rx_header_buf;
        struct CodecData *codec = &client_container->codec_data;
        if (strlen(client_container->username) == 0) {
            if (header.msg_type == MSG_TYPE_LOGIN) {
                if (codec->rx_data_len > MAX_USERNAME_LEN) {
                    fprintf(stderr, "Username too long\n");
                    //TODO send error to client
                    return;
                }
                memcpy(client_container->username, codec->rx_data_buf, codec->rx_data_len);
                //TODO handle error
            } else {
                fprintf(stderr, "User is not logged in\n");
                //TODO send error to client
            }
        } else {
            if (header.msg_type == MSG_TYPE_CLIENT_TEXT) {
                //TODO notify all clients
                fprintf(stderr, "%s: %s\n", client_container->username, codec->rx_data_buf);
            } else {
                fprintf(stderr, "%s: Unknown message type\n", client_container->username);
            }
        }
    }
}

struct ClientContainer *find_client_container(struct Server *server, int client_fd) {
    for (int i = 0; i < MAX_CLIENTS_NUMBER; i++) {
        if (server->clients[i].codec_data.socket_fd == client_fd) {
            return &server->clients[i];
        }
    }
    return NULL;
}

void reset_client_container(struct ClientContainer *client_container) {
    reset_codec(&client_container->codec_data);
    memset(client_container->username, 0, MAX_USERNAME_LEN);
    //TODO handle errors
}
