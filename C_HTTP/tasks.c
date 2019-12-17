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

void getCredentials(char *cred, JSON_Object * obj){
	int i, count = json_object_get_count(obj);
	const char *name, *value;
	for(i = 0; i < count; i++){
		name = json_object_get_name(obj, i);
		value = json_object_get_string (obj, name);
		if(i == 0){
			strcpy(cred, name);
		} else{
			strcat(cred, "&");
			strcat(cred, name);
		}
		strcat(cred, "=");
		strcat(cred, value);
	}
}

void getCredentialsTask3(char *cred, const char* id){
	strcpy(cred, "raspuns1=omul&raspuns2=numele&id=");
	strcat(cred, id);
}

void getCookies(char *cookies, char *response){
	char *cookie, buf[1000];
	cookie = strstr(response, "Set-Cookie:");
	while(cookie != NULL){
		cookie += strlen("Set-Cookie:");
		strncpy(buf, cookie, strcspn(cookie, ";\n"));
		strncat(cookies, buf, strcspn(cookie, ";\n"));
		strcat(cookies, ";");
		cookie = strstr(cookie, "Set-Cookie:");
	}
}

void get_ip(char* host, char* ip){
	int ret;
	struct addrinfo hints, *result;
	void *ptr;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = PF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	ret = getaddrinfo(host, NULL, &hints, &result);
	if(ret != 0){
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
		exit(-1);
	}
	for(struct addrinfo* addr = result; addr != NULL; addr = addr->ai_next){
		if(addr->ai_family == AF_INET){
			ptr = &((struct sockaddr_in *)addr->ai_addr)->sin_addr;
			inet_ntop(addr->ai_family, ptr, ip, 100);
			break;
		}
	}
	//free allocated data
	if(result){
		freeaddrinfo(result);
	}
}

void task1(char ** response){
	int sockfd;
	char *message;
	sockfd = open_connection(IP_SERVER, PORT_SERVER, AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		perror("open error\n");
	}

	message = compute_get_request(IP_SERVER, "/task1/start", NULL, NULL, NULL);
	send_to_server(sockfd, message);
	*response = receive_from_server(sockfd);

	close_connection(sockfd);
}

void task2(char** response){
	int sockfd;
	char *message, *url, *contentType;
	char credential[1000], cookies[5000];
	JSON_Value * val;
	JSON_Object * obj, * data;

	sockfd = open_connection(IP_SERVER, PORT_SERVER, AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		perror("open error\n");
	}

	memset(cookies, 0, sizeof(cookies));
	getCookies(cookies, *response);
	val = json_parse_string(strchr(*response, '{'));
	obj = json_value_get_object (val);
	url = (char *)json_object_get_string(obj, "url");
	contentType = (char *)json_object_get_string(obj, "type");
	data = json_object_get_object(obj, "data");

	getCredentials(credential, data);
	message = compute_post_request(IP_SERVER, url, NULL, cookies, contentType, credential);
	send_to_server(sockfd, message);
	*response = receive_from_server(sockfd);

	if (val) {
		json_value_free(val);
	}
	close_connection(sockfd);
}

void task3(char** response, char* authorization){
	int sockfd;
	char *message, *id, *token, *url;
	char credential[1000], cookies[5000];
	JSON_Value * val;
	JSON_Object * obj, * data, *queryParams;

	sockfd = open_connection(IP_SERVER, PORT_SERVER, AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		perror("open error\n");
	}

	memset(cookies, 0, sizeof(cookies));
	getCookies(cookies, *response);
	val = json_parse_string(strchr(*response, '{'));
	obj = json_value_get_object (val);
	url = (char *)json_object_get_string(obj, "url");
	data = json_object_get_object(obj, "data");
	queryParams = json_object_get_object(data, "queryParams");


	id = (char *)json_object_get_string (queryParams, "id");
	token = (char*)json_object_get_string (data, "token");
	getCredentialsTask3(credential, id);
	strcpy(authorization, token);
	message = compute_get_request(IP_SERVER, url, credential, authorization, cookies);
	send_to_server(sockfd, message);
	*response = receive_from_server(sockfd);

	if (val) {
		json_value_free(val);
	}
	close_connection(sockfd);
}

void task4(char** response, char* authorization){
	int sockfd;
	char *message, *url;
	char cookies[5000];
	JSON_Value * val;
	JSON_Object * obj;

	sockfd = open_connection(IP_SERVER, PORT_SERVER, AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		perror("open error\n");
	}

	memset(cookies, 0, sizeof(cookies));
	getCookies(cookies, *response);
	val = json_parse_string(strchr(*response, '{'));
	obj = json_value_get_object (val);
	url = (char *)json_object_get_string(obj, "url");

	message = compute_get_request(IP_SERVER, url, NULL, authorization, cookies);
	send_to_server(sockfd, message);
	*response = receive_from_server(sockfd);

	if (val) {
		json_value_free(val);
	}
	close_connection(sockfd);

}

void task5(char** response, char* authorization){
	int sockfd;
	char *message, *url, *weatherWebsite, *contentType;
	char weatherURL[1000], IP[1000], hostName[100];
	char credential[1000], cookies[5000];
	JSON_Value * val;
	JSON_Object * obj, * data, *queryParams;

	sockfd = open_connection(IP_SERVER, PORT_SERVER, AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		perror("open error\n");
	}

	memset(cookies, 0, sizeof(cookies));
	getCookies(cookies, *response);
	val = json_parse_string(strchr(*response, '{'));
	obj = json_value_get_object (val);
	url = (char *)json_object_get_string(obj, "url");
	contentType = (char *)json_object_get_string(obj, "type");
	data = json_object_get_object(obj, "data");
	weatherWebsite = (char *)json_object_get_string(data, "url");
	queryParams = json_object_get_object(data, "queryParams");
	getCredentials(credential, queryParams);

	strncpy(hostName, weatherWebsite, strcspn(weatherWebsite, "/"));
	get_ip(hostName, IP);

	strcpy(weatherURL, strchr(weatherWebsite, '/'));
	strcat(weatherURL, "?");
	strcat(weatherURL, credential);

	sockfd = open_connection(IP, PORT_HTTP, AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		perror("open error\n");
	}
	message = compute_get_request(IP, weatherURL, NULL, NULL, NULL);
	send_to_server(sockfd, message);
	*response = receive_from_server(sockfd);
	printf("%s\n\n\n", *response);
	close_connection(sockfd);

	sockfd = open_connection(IP_SERVER, PORT_SERVER, AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		perror("open error\n");
	}
	message  = compute_post_request(IP_SERVER, url, authorization, cookies, contentType, strchr(*response, '{'));
	send_to_server(sockfd, message);
	*response = receive_from_server(sockfd);
	close_connection(sockfd);
}
