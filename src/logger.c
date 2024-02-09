#include "logger.h"

void timestamp(char* buffer) {
  time_t current_time;
  struct tm *time_info;

  time(&current_time);
  time_info = localtime(&current_time);
  // TODO: Get seconds working ?!
  strftime(buffer, sizeof(buffer), "%H:%M", time_info);
}

void log_with_type(const char* type, const char* message, va_list args) {
  time_t now;     
  time(&now);     
  char * date = ctime(&now);   
  date[strlen(date) - 1] = '\0';  
  printf("[%s] [%s] ", date, type);  
  vprintf(message, args);     
  printf("\n");
  //char buffer[100];
  //timestamp(buffer);
  //printf("[%s] %s: %s\n", buffer, type, message);
}

void log_info(const char* message, ...) {
  va_list args;   
  va_start(args, message); 
  
  log_with_type("INFO", message, args);

  va_end(args);
}

void log_error(const char* message, ...) {
  va_list args;
  va_start(args, message);

  log_with_type("ERROR", message, args); 
  
  va_end(args);
}

