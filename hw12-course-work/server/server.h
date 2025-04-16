//
// Created by Liubov Udalova on 13.04.25.
//

#ifndef SERVER_H
#define SERVER_H

#include <poll.h>
#include "../libs/msg_codec.h"

#define MAX_USERNAME_LEN 20
#define MAX_CLIENTS_NUMBER 10

struct ClientContainer {
    struct CodecData codec_data;
    char username[MAX_USERNAME_LEN];
};

struct Server {
    struct pollfd poll_fds[MAX_CLIENTS_NUMBER + 1];
    struct ClientContainer clients[MAX_CLIENTS_NUMBER];
};

void server_run(struct Server *server, int server_fd);

void server_stop(struct Server *server);

int server_refresh_poll_fds(struct Server *server);

void server_accept_connection(struct Server *server);

void handle_client_message(struct Server *server, struct ClientContainer *client_container);

struct ClientContainer *find_client_container(struct Server *server, int client_fd);

void reset_client_container(struct ClientContainer *client_container);

#endif //SERVER_H
