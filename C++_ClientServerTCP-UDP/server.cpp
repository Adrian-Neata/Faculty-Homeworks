#include <iostream>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include<unistd.h>
#include<map>
#include<string>
#include<list>
#include <fcntl.h>
#include<fstream>

using namespace std;

#define MAX_CLIENTS 1000

int checkTopic(const char *line, char *topic){
	//checks if message has the same topic as the one received
	char *startOfTopic = strstr((char *)line, " - ") + 3;
	if(strncmp(startOfTopic, topic, strlen(topic)) == 0){
		return 1;
	} else {
		return 0;
	}
}
int getTCP_message(char *message, char* buff, struct sockaddr_in* cli_addr){
	//creates the message the TCP client must print on the screen
	char topic_name[55], line[1550];
	memset(topic_name, 0, sizeof(topic_name));
	memcpy(topic_name, buff, 50);
	sprintf(line, "%s:%d", inet_ntoa(cli_addr->sin_addr), ntohs(cli_addr->sin_port));
	strcat(message, line);
	strcat(message, " - ");
	sprintf(line, "%s", topic_name);
	strcat(message, line);
	strcat(message, " - ");
	int x = 0;
	double y = 0;
	short z = 0;
	switch(buff[50]){
		case 0: //INT
			if(buff[51] == 1){
				strcat(message, "-");
			}
			memcpy((char *)&x, buff + 52, 4);
			x = ntohl(x);
			sprintf(line, "%d", x);
			strcat(message, line);
			break;
		case 1: //SHORT_REAL
			memcpy((char *)&z, buff + 51, 2);
			y = ntohs(z);
			y = y / 100.0;
			sprintf(line, "%.2f", y);
			strcat(message, line);
			break;
		case 2: //FLOAT
			if(buff[51] == 1){
				strcat(message, "-");
			}
			memcpy((char *)&x, buff + 52, 4);
			y = ntohl(x);
			while(buff[56] != 0){
				y = y / 10.0;
				buff[56]--;
			}
			sprintf(line, "%f", y);
			strcat(message, line);
			break;
		case 3: //STRING
			sprintf(line, "%s", buff + 51);
			strncat(message, line, 1450);
			break;
		default:
			break;
	}
	return 0;
}

int main(int argc, char* argv[]) {
	if(argc != 2){
		fprintf(stderr, "Usage: %s <PORT_DORIT>\n", argv[0]);
		exit(-1);
	}

	int sockfd_UDP, sockfd_TCP, port_NO;
	struct sockaddr_in serv_addr, cli_addr;
	socklen_t clilen;
	fd_set read, tmp_read, TCP_clients;
	int ret, one = 1, fd_max, client_socket, i, fd;
	char buff[1551], message[1650], id[12], *token, *instr, *SF;
	char topic[55];
	map<int, string> fds_ID; 
	//map to remember each clients file descriptor and ID
	map<int, int> client_file;
	//map to remember each clients file descriptor and their file's file descriptor
	map<string, map<int, int>> topics;
	//map to remember each topic and the clients and their SFs subscribed
	map<string, map<int, int>> unsub_topics;
	//map to remember each topic and the clients who unsubscribed but had their SF 1
	string line;

	port_NO = atoi(argv[1]);
	if(port_NO <= 0){
		perror("Invalid port number.\n");
		exit(-1);
	}

	sockfd_UDP = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd_UDP < 0){
		perror("Could not create UDP socket.\n");
		exit(-1);
	}

	sockfd_TCP = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd_TCP < 0){
		perror("Could not create TCP socket.\n");
		exit(-1);
	}

	ret = setsockopt(sockfd_TCP, SOL_TCP, TCP_NODELAY, &one, sizeof(one));
	if(ret < 0){
		perror("Could not deactivate Nagle's algorithm.\n");
		exit(-1);
	}

	memset((char *) &serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port_NO);
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	ret = bind(sockfd_UDP, (const struct sockaddr*) (&serv_addr), sizeof(struct sockaddr));
	if(ret < 0){
		perror("Could not bind UPD socket.\n");
		exit(-1);
	}

	ret = bind(sockfd_TCP, (const struct sockaddr*) (&serv_addr), sizeof(struct sockaddr));
	if(ret < 0){
		perror("Could not bind TCP socket.\n");
		exit(-1);
	}

	ret = listen(sockfd_TCP, MAX_CLIENTS);
	if(ret < 0){
		perror("Could not listen.\n");
		exit(-1);
	}

	FD_ZERO(&read);
	FD_ZERO(&TCP_clients);
	FD_SET(STDIN_FILENO, &read);
	FD_SET(sockfd_TCP, &read);
	FD_SET(sockfd_UDP, &read);
	
	if(sockfd_TCP > sockfd_UDP){
		fd_max = sockfd_TCP;
	} else{
		fd_max = sockfd_UDP;
	}

	while(1){
		tmp_read = read;
		ret = select(fd_max + 1, &tmp_read, NULL, NULL, NULL);
		if(ret < 0){
			perror("Select failed.\n");
			exit(-1);
		}

		if(FD_ISSET(STDIN_FILENO, &tmp_read)){
			memset(buff, 0, sizeof(buff));
			fgets(buff, sizeof(buff) - 1, stdin);
			if (strncmp(buff, "exit\n", 5) == 0) {
				for(i = 1; i <= fd_max; i++) {
					if(FD_ISSET(i, &read)){
						close(i);
					}
					if(client_file.count(i)){
						close(client_file[i]);
					}
				}
				return 0;
			}
		}
		//checks if server must be closed


		if(FD_ISSET(sockfd_TCP, &tmp_read)){

			clilen = sizeof(cli_addr);
			client_socket = accept(sockfd_TCP, (struct sockaddr *) &cli_addr, &clilen);
			if(client_socket < 0){
				perror("Could not accept a new TCP client.\n");
				exit(-1);
			}

			if (client_socket > fd_max) { 
				fd_max = client_socket;
			}
			FD_SET(client_socket, &read);
			FD_SET(client_socket, &TCP_clients);

			memset(buff, 0, sizeof(buff));
			ret = recv(client_socket, id, sizeof(id), 0);
			//waits to get the client's ID
			if(ret < 0){
				perror("ID_CLIENT could not be received.\n");
				exit(-1);
			}

			fds_ID[client_socket] = id;
			client_file[client_socket] = open(id, O_RDWR | O_CREAT | O_TRUNC, 0777);
			if(client_file[client_socket] < 0){
				perror("Could not open new file for client.\n");
				exit(-1);
			}
			printf("New client (%s) connected from %s:%d.\n", id,
			inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
		}
		//accepts all requests for connexion from TCP clients


		if(FD_ISSET(sockfd_UDP, &tmp_read)){
			clilen = sizeof(cli_addr);
			memset(buff, 0, sizeof(buff));
			ret = recvfrom(sockfd_UDP, buff, sizeof(buff), 0, (struct sockaddr *) &cli_addr, &clilen);
			if(ret < 0){
				perror("Message from UDP client could not be received.\n");
				exit(-1);
			}
			memset(message, 0, sizeof(message));
			getTCP_message(message, buff, &cli_addr);
			memset(topic, 0, sizeof(topic));
			memcpy(topic, buff, 50);
			if(topics.count(topic) == 0){
				topics[topic];
			}
			//if the topic doesn't exist yet than add a new entry

			for(i = 0; i <= fd_max; i++){
				if(topics[topic].count(i)){
					ret = send(i, message, sizeof(message), 0);
					if(ret < 0){
						perror("Message could not be sent.\n");
						exit(-1);
					}
				}
				//if a client is subscribed to that topic than a message is sent to them

				if(unsub_topics[topic].count(i)){
					ret = write(client_file[i], message, strlen(message));
					if(ret <= 0){
						perror("Could not store message for client.\n");
						exit(-1);
					}
					ret = write(client_file[i], "\n", 1);
					if(ret <= 0){
						perror("Could not store message for client.\n");
						exit(-1);
					}
				}
				//if a client is unsubscribed but has store and forward active than their message is saved in their personal file

			}
		}
		for(i = 0; i <= fd_max; i++){
			if(FD_ISSET(i, &tmp_read) && FD_ISSET(i, &TCP_clients)){
				memset(buff, 0, sizeof(buff));
				ret = recv(i, buff, sizeof(buff), 0);
				if(ret < 0){
					perror("Message from TCP client could not be received.\n");
					exit(-1);
				} else if(ret == 0){
					printf("Client (%s) disconnected.\n", fds_ID[i].c_str());
					close(i);

					FD_CLR(i, &read);
					FD_CLR(i, &TCP_clients);
					fds_ID.erase(i);
					close(client_file[i]);
					client_file.erase(i);
					for(auto it = topics.begin(); it != topics.end(); ++it){
						topics[it->first].erase(i);
					}
					for(auto it = unsub_topics.begin(); it != unsub_topics.end(); ++it){
						unsub_topics[it->first].erase(i);
					}
					//erase all data from the client that disconnected

				} else{
					instr = buff;
					token = strtok(instr, " \n");
					if(token != NULL){
						memset(message, 0, sizeof(message));
						if(strcmp(token, "subscribe") == 0){
							token = strtok(NULL, " \n");
							if(token == NULL){
								sprintf(message, "Usage: subscribe topic SF.");
							} else if(strlen(token) > 50){
								sprintf(message, "Topic must be at most 50 characters long.");
							} else{
								SF = strtok(NULL, " \n");
								if(SF == NULL){
									sprintf(message, "Usage: subscribe topic SF.");
								} else if(strlen(SF) != 1 || !(SF[0] == '0' || SF[0] == '1')){
										sprintf(message, "SF must be 0 or 1.");
								} else {
									SF[0] -= '0';
									topics[token][i] = SF[0];
									sprintf(message, "Subscribed topic.");
									if(unsub_topics[token].count(i) == 1){
										fd = open("temporary_file", O_RDWR | O_CREAT | O_TRUNC, 0777);
										if(fd <= 0){
											perror("Could not open temporary file.\n");
											exit(-1);
										}
										//creates new file to store the messages

										ret = lseek(client_file[i], 0, SEEK_SET);
										if(ret < 0){
											perror("Could not go to the beginning of the file.\n");
											exit(-1);
										}
										//goes back to the start of the client's file
										ifstream file (fds_ID[i]);
										while(getline(file, line)){
											line += "\n";
											if(checkTopic(line.c_str(), token)){
												ret = send(i, line.c_str(), strlen(line.c_str()), 0);
												if(ret < 0){
													perror("Message could not be sent.\n");
													exit(-1);
												}
												//sends message to the client if the topic is the same
											} else{
												ret = write(fd, line.c_str(), strlen(line.c_str()));
												if(ret < 0){
													perror("Could not store message for client.\n");
													exit(-1);
												}
												//saves message to the new file if the topic differs
											}
										}

										close(client_file[i]);
										remove(fds_ID[i].c_str());
										client_file[i] = fd;
										unsub_topics[token].erase(i);
										ret = rename("temporary_file", fds_ID[i].c_str());
										if(ret != 0){
											perror("Could not rename file.\n");
											exit(-1);
										}
										//changes client's file from the old one to the newly created one
									}
								}
							}
						} else if(strcmp(token, "unsubscribe") == 0){
							token = strtok(NULL, " \n");
							if(token == NULL){
								sprintf(message, "Usage: unsubscribe topic.");
							} else if(strlen(token) > 50){
								sprintf(message, "Topic must be at most 50 characters long.");
							} else{
								if(topics.count(token) == 0) {
									sprintf(message, "Topic does not exist.");
								} else{
									if(topics[token][i] == 1){
										unsub_topics[token][i] = 1;
									}
									//if SF was active than add an entry to unsub_topics
									topics[token].erase(i);
									sprintf(message, "Unsubscribed topic.");
								}
							}
						} else{
							sprintf(message, "Invalid instruction.");
						}
						ret = send(i, message, sizeof(message), 0);
						if(ret < 0){
							perror("Message could not be sent.\n");
							exit(-1);
						}
					}
				}
			}
		}
	}
	return 0;
}
