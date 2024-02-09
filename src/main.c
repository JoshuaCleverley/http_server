#include "logger.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <unistd.h>

#define PORT 8000

char *reply = 
"HTTP/1.1 200 OK\n"
"Date: Thu, 19 Feb 2009 12:27:04 GMT\n"
"Server: Apache/2.2.3\n"
"Last-Modified: Wed, 18 Jun 2003 16:05:58 GMT\n"
"ETag: \"56d-9989200-1132c580\"\n"
"Content-Type: text/html\n"
"Content-Length: 12\n"
"Accept-Ranges: bytes\n"
"Connection: close\n"
"\n"
"hello world!";

int main()
{
  int sockfd;
  int clientfd;
  int bytes_read;
  char buf[100];
  int struct_size;
  struct sockaddr_in my_addr;
  struct sockaddr_in con_addr;
    
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  log_info("Socket created");
    
  my_addr.sin_family = AF_INET;
  my_addr.sin_port = htons(PORT);
  my_addr.sin_addr.s_addr = INADDR_ANY;
  my_addr.sin_zero[8]='\0';

  bind(sockfd, (struct sockaddr*)&my_addr, sizeof(struct sockaddr));
  log_info("Socket bound to port 8000");

  listen(sockfd,5);
  log_info("Listening for connections");
    
  struct_size = sizeof(con_addr);

  while (true) {
    clientfd = accept(sockfd, (struct sockaddr*)&con_addr, &struct_size);
    log_info("Connection accepted");

    bytes_read = read(clientfd, buf, 100);
    buf[bytes_read] = '\0';
    printf("Message from client:%d is \n %s \n", clientfd, buf);

    send(clientfd, reply, strlen(reply), 0);
  }
  close(sockfd);
  close(clientfd);
}
