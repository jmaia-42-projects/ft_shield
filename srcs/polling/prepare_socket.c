#include "polling.h"

static struct sockaddr_in	get_addr()
{
	struct sockaddr_in	addr;
	bzero(&addr, sizeof(struct sockaddr_in));

	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	return (addr);
}

int	prepare_socket() {
	int	sockfd;
	struct sockaddr_in servaddr = get_addr();
	int	unused = 1;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		perror("socket creation failed");
		return -1;
	}
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &unused, sizeof(int));
	if ((bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0) {
		perror("socket bind failed");
		return -1;
	}
	if ((listen(sockfd, LISTEN_LIMIT)) != 0) {
		perror("Listen failed");
		return -1;
	}
	return (sockfd);
}