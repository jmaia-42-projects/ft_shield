#include "polling.h"

t_client clients[MAX_CLIENTS];

void send_to_client(t_client *client, char *message)
{
	send(client->socket, message, strlen(message), 0);
}

int count_connected_clients()
{
	int count = 0;
	for (size_t i = 0; i < MAX_CLIENTS; i++)
	{
		if (clients[i].connected)
			count++;
	}
	return count;
}

void export_pollset(struct pollfd *pollSet, int sockfd)
{
	pollSet[0].fd = sockfd;
	pollSet[0].events = POLLIN;
	pollSet[0].revents = 0;
	int i = 1;
	for (size_t j = 0; j < MAX_CLIENTS; j++)
	{
		if (clients[j].connected)
		{
			pollSet[i].fd = clients[j].socket;
			pollSet[i].events = POLLIN | POLLOUT;
			pollSet[i].revents = 0;
			i++;
		}
	}
}

void accept_client(int serverSocket)
{
	struct sockaddr_in  client_address;
	socklen_t           client_address_length = sizeof(client_address);
	bzero(&client_address, sizeof(client_address));

	int client_socket = accept(serverSocket, (struct sockaddr *)&client_address, &client_address_length);
	if (client_socket == -1)
		return;

	if (count_connected_clients() == MAX_CLIENTS)
	{
		close(client_socket);
		return;
	}

	t_client new_client = {true, client_socket, false, NULL};
	for (size_t i = 0; i < MAX_CLIENTS; i++)
	{
		if (!clients[i].connected)
		{
			clients[i] = new_client;
			break;
		}
	}

	send_to_client(&new_client, "Password: ");
}

void disconnect(t_client *client)
{
	close(client->socket);
	client->connected = false;
}

int get_len_to_separator(char *buffer, char separator)
{
	size_t i = -1;
	while (buffer[++i]) {
		if (buffer[i] == separator)
			return i;
	}
	return -1;
}

char *extract_command(char **buffer)
{
	int separator_pos = get_len_to_separator(*buffer, '\n');
	if (separator_pos == -1)
		return NULL;
	char *command = malloc(separator_pos + 1);
	if (!command)
		return NULL;
	strncpy(command, *buffer, separator_pos);
	command[separator_pos] = '\0';
	
	char *new_buffer = malloc(strlen(*buffer) - separator_pos + 1);
	if (!new_buffer)
		return NULL;
	strcpy(new_buffer, *buffer + separator_pos + 1);
	free(*buffer);
	*buffer = new_buffer;
	return command;
}

void read_client(t_client *client)
{
	char buffer[RECV_BUFFER_SIZE + 1];
	ssize_t read_size = recv(client->socket, buffer, RECV_BUFFER_SIZE, 0);
	if (read_size <= 0)
	{
		disconnect(client);
		return;
	}
	buffer[read_size] = '\0';

	size_t buffer_len = strlen(buffer) + (client->buffer ? strlen(client->buffer) : 0);
	char *new_buffer = malloc(buffer_len + 1);
	if (!new_buffer)
		return;
	if (client->buffer)
	{
		strcpy(new_buffer, client->buffer);
		strcat(new_buffer, buffer);
		free(client->buffer);
	}
	else
		strcpy(new_buffer, buffer);
	client->buffer = new_buffer;

	char *command;
	while ((command = extract_command(&client->buffer)) != NULL)
	{
		treat_command(client, command);
		free(command);
	}
}

void treat_poll(struct pollfd *pollSet, int serverSocket)
{
	if (!(pollSet->revents & POLLIN))
		return;
	if (pollSet->fd == serverSocket)
		accept_client(serverSocket);
	else
	{
		for (size_t i = 0; i < MAX_CLIENTS; i++)
		{
			if (clients[i].connected && clients[i].socket == pollSet->fd)
			{
				read_client(&clients[i]);
				return;
			}
		}
	}
}

void poll_routine(int sockfd)
{
	t_client default_client = {false, -1, false, NULL};
	for (size_t i = 0; i < MAX_CLIENTS; i++)
		clients[i] = default_client;

	while (1)
	{
		size_t in_poll = 1 + count_connected_clients();
		struct pollfd poll_set[in_poll];
		export_pollset(poll_set, sockfd);
		if (poll(poll_set, in_poll, -1) < 0)
		{
			perror("poll failed");
			return;
		}
		for (size_t i = 0; i < in_poll; i++)
		{
			treat_poll(&poll_set[i], sockfd);
		}
	}
}