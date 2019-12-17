#include <iostream>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include<unistd.h>

using namespace std;

#define MAX_MESSAGE_LEN 1600

int main(int argc, char* argv[]) {
	if(argc != 4){
		fprintf(stderr, "Usage: %s <ID_Client> <IP_Server> <Port_Server>\n", argv[0]);
		exit(-1);
	}
	
	int sockfd_TCP, port_NO;
	struct sockaddr_in serv_addr;
	fd_set read, tmp_read;
	int ret;
	char buff[80], message[MAX_MESSAGE_LEN];

	if(strlen(argv[1]) > 10){
		perror("Client ID is more than 10 characters long.\n");
		exit(-1);
	}

	port_NO = atoi(argv[3]);
	if(port_NO <= 0){
		perror("Invalid port number.\n");
		exit(-1);
	}

	sockfd_TCP = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd_TCP < 0){
		perror("Could not create TCP socket.\n");
		exit(-1);
	}

	memset((char *) &serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port_NO);
	ret = inet_aton(argv[2], &serv_addr.sin_addr);
	if(ret < 0) {
		perror("Could not get IP address.\n");
		exit(-1);
	}

	ret = connect(sockfd_TCP, (const struct sockaddr*) (&serv_addr), sizeof(struct sockaddr));
	if(ret < 0){
		perror("Could not connect to TCP server.\n");
		exit(-1);
	}

	memset(buff, 0, sizeof(buff));
	sprintf(buff, "%s", argv[1]);
	ret = send(sockfd_TCP, buff, sizeof(buff), 0);
	if(ret < 0){
		perror("Message could not be sent.\n");
		exit(-1);
	}

	FD_ZERO(&read);
	FD_SET(STDIN_FILENO, &read);
	FD_SET(sockfd_TCP, &read);

	while(1){
		tmp_read = read;
		ret = select(sockfd_TCP + 1, &tmp_read, NULL, NULL, NULL);
		if(ret < 0){
			perror("Select failed.\n");
			exit(-1);
		}
		if(FD_ISSET(STDIN_FILENO, &tmp_read)){
			memset(buff, 0, sizeof(buff));
			fgets(buff, sizeof(buff) - 1, stdin);
			if (strncmp(buff, "exit\n", 5) == 0) {
				break;
			}
			ret = send(sockfd_TCP, buff, sizeof(buff), 0);
			if(ret < 0){
				perror("Message could not be sent.\n");
				exit(-1);
			}
		} else{
			memset(message, 0, sizeof(message));
			ret = recv(sockfd_TCP, message, sizeof(message), 0);
			if(ret < 0){
				perror("Message could not be received.\n");
				exit(-1);
			} else if(ret == 0) {
				printf("Server closed.\n");
				break;
			} else {
				printf("%s\n", message);
			}
		}
	}
	return 0;
}
