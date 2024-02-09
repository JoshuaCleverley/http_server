#include <time.h>

#include "logger.h"

void timestamp(char* buffer) {
  time_t current_time;
  struct tm *time_info;

  time(&current_time);
  time_info = localtime(&current_time);
  // TODO: Get seconds working ?!
  strftime(buffer, sizeof(buffer), "%H:%M", time_info);
}

void log_with_type(const char* type, const char* message) {
  char buffer[100];
  timestamp(buffer);
  printf("[%s] %s: %s\n", buffer, type, message);
}

void log_info(const char* message) {
  log_with_type("INFO", message);
}

void log_error(const char* message) {
  log_with_type("ERROR", message); 
}

