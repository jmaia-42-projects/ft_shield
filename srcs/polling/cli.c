#include "polling.h"

//TODO: Implements hash function
bool is_password(char *password)
{
	return (strcmp(password, "password") == 0);
}

void treat_command(t_client *client, char *command)
{
	if (!client->authentified)
	{
		if (is_password(command))
		{
			client->authentified = true;
			send_to_client(client, "\n$> ");
		}
		else
		{
			send_to_client(client, "Wrong password\n");
			disconnect(client);
		}
	}
	else
	{
		//TODO: Implements command treatment
	}
}