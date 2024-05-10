#ifndef POLLING_H
# define POLLING_H

# include <poll.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <string.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>

# include "sha256.h"

# define PORT 4242
# define LISTEN_LIMIT 10
# define MAX_CLIENTS 3
# define RECV_BUFFER_SIZE 1024

# define SHA256_PASS "5e884898da28047151d0e56f8dc6292773603d0d6aabbdd62a11ef721d1542d8"

typedef struct s_client
{
	bool connected;
	int	socket;
	bool authentified;
}	t_client;

int	prepare_socket();
void poll_routine(int sockfd);
void treat_command(t_client *client, char *command);
void send_to_client(t_client *client, char *message);
void disconnect(t_client *client);

#endif