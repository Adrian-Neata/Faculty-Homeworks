#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"
#include "parson.h"
#include "tasks.h"

int main(int argc, char *argv[])
{
	char *response, authorization[5000];

	task1(&response);
	printf("%s\n\n\n", response);

	task2(&response);
	printf("%s\n\n\n", response);

	task3(&response, authorization);
	printf("%s\n\n\n", response);

	task4(&response, authorization);
	printf("%s\n\n\n", response);

	task5(&response, authorization);
	printf("%s\n\n\n", response);

	return 0;
}
