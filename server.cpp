#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <cstring>

#define DEFAULT_PORT 5454
int main()
{
	int client, server, ret;
	bool is_exit = false;
	struct sockaddr_in server_address;
	char buffer[1024];

	client = socket(AF_INET, SOCK_STREAM, 0);
	if (client < 0){
		std::cout << "SERVER: Socket err client" << std::endl;
		exit(0);
	}
	std::cout << "SERVER: Socket client created" << std::endl;

	server_address.sin_port = htons(DEFAULT_PORT);
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htons(INADDR_ANY);

	ret = bind(client, (struct sockaddr *)&server_address, sizeof(server_address));
	if (ret < 0){
		std::cout << "SERVER: Bind err" << std::endl;
		exit(0);
	}
	std::cout << "SERVER: Listening client....." << std::endl;
	listen(client, 1);
	server = accept(client, NULL, NULL);
	if (server < 0){
		std::cout << "SERVER: Socket err server" << std::endl;
		exit(0);
	}
	while (server > 0){
		strcpy(buffer, "=> Server connected!\n");
		send(server, buffer, 1024, 0);
		std::cout << "=> Connected to the client #1" << std::endl;
		std::cout << "Enter # to end the connection\n\n";
		std::cout << "Client: ";
		recv(server, buffer, 1024, 0);
		std::cout << buffer << std::endl;
		if (strcmp(buffer, "#") == 0)
			break;
		while (!is_exit){
			std::cout << "Server: ";
			std::cin.getline(buffer, 1024);
			send(server, buffer, 1024, 0);
			if (strcmp(buffer, "#") == 0)
				break;
			std::cout << "Client: ";
			recv(server, buffer, 1024, 0);
			std::cout << buffer << std::endl;
			if (strcmp(buffer, "#") == 0)
				break;
		}
		std::cout << "\n Goodbye...." << std::endl;
		is_exit = false;
		close(client);
		close(server);
		exit(0);
	}
}