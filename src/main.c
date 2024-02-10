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

void send_text(int clientfd, char* text) {
  send(clientfd, text, strlen(text), 0);
}

void send_file(int clientfd, char* path) {
  if (path == NULL) {
    log_error("No path provided");
    return;
  }

  if (strcmp(path, "/") == 0) {
    path = "/index.html";
  }

  char* location = "public";
  char full_path[100];
  strcpy(full_path, location);
  strcat(full_path, path);
  
  int fd = open(full_path, O_RDONLY);

  if (fd == -1) {
    log_error("Error opening file '%s': %s", full_path, strerror(errno));
    send_file(clientfd, "/404.html");
    return;
  }

  char buffer[1000];
  int bytes_read;
  bytes_read = read(fd, buffer, 1000);
  buffer[bytes_read] = '\0';

  char *reply = 
  "HTTP/1.1 200 OK\n"
  "Date: Thu, 19 Feb 2009 12:27:04 GMT\n"
  "Server: Apache/2.2.3\n"
  "Content-Type: text/html\n"
  "Accept-Ranges: bytes\n"
  "Connection: close\n";

  // Add the content length to the reply
  char res[1000];
  char content_length[100];
  sprintf(content_length, "Content-Length: %d\n", bytes_read);
  strcpy(res, reply);
  strcat(res, content_length);
  strcat(res, "\n");
  strcat(res, buffer);

  send_text(clientfd, res);

  close(fd);
}

void get(int clientfd, char* request) {
  // Get the path from the request
  char* path = strtok(request, " ");
  path = strtok(NULL, " ");
  log_info("Get request for: %s", path);
  send_file(clientfd, path);
}

void parse_request(int clientfd, char* request) {  // Check if request is a GET request  
  if (strncmp(request, "GET", 3) == 0) {
    get(clientfd, request);
  } else {
    log_error("Invalid or unimplemented request received");  
  }
}

int main() {
  int sockfd;
  int clientfd;
  int bytes_read;
  char buf[100];
  int struct_size;
  struct sockaddr_in my_addr;
  struct sockaddr_in con_addr;
    
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    log_error("Error creating socket %d", errno);
    return 1;
  }
  log_info("Socket created");
    
  my_addr.sin_family = AF_INET;
  my_addr.sin_port = htons(PORT);
  my_addr.sin_addr.s_addr = INADDR_ANY;
  my_addr.sin_zero[8]='\0';

  if (bind(sockfd, (struct sockaddr*)&my_addr, sizeof(struct sockaddr)) == -1) {
    log_error("Error binding socket");
    return 1;
  }
  log_info("Socket bound to port 8000");

  if (listen(sockfd, 5) == -1) {
    log_error("Error listening on socket");
    return 1;
  }

  log_info("Listening for connections");
    
  struct_size = sizeof(con_addr);

  while (true) {
    clientfd = accept(sockfd, (struct sockaddr*)&con_addr, &struct_size);
    if (clientfd == -1) {
      log_error("Error accepting connection");
      return 1;
    }
    //log_info("Connection accepted");

    bytes_read = read(clientfd, buf, 100);
    if (bytes_read == -1) {
      log_error("Error reading from client");
      return 1;
    }
    buf[bytes_read] = '\0';
    //printf("Message from client:%d is \n %s \n", clientfd, buf);

    parse_request(clientfd, buf);
  }
  close(sockfd);
  close(clientfd);
}
