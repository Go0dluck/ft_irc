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
	int client, ret;
	char buffer[1024];
	struct sockaddr_in server_address;
	client = socket(AF_INET, SOCK_STREAM, 0);
	if (client < 0){
		std::cout << "CLIENT: Socket err client" << std::endl;
		exit(0);
	}
	std::cout << "CLIENT: Socket client created" << std::endl;
	server_address.sin_port = htons(DEFAULT_PORT);
	server_address.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr);
	ret = connect(client, (struct sockaddr *)&server_address, sizeof(server_address));
	if (ret < 0){
		std::cout << "CLIENT: connect err" << std::endl;
		exit(0);
	}
	std::cout << "=> Connected to server " << inet_ntoa(server_address.sin_addr) << " port " << DEFAULT_PORT << std::endl;
	std::cout << "Wait for server info...\n";
	recv(client, buffer, 1024, 0);
	std::cout << "=> Connected success\n";
	while (true){
		std::cout << "Client: ";
		std::cin.getline(buffer, 1024);
		send(client, buffer, 1024, 0);
		if (strcmp(buffer, "#") == 0)
			break;
		std::cout << "Server: ";
		recv(client, buffer, 1024, 0);
		std::cout << buffer << std::endl;
		if (strcmp(buffer, "#") == 0)
			break;
	}
	close(client);
	std::cout << "\n Goodbye...." << std::endl;
}