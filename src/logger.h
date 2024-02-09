#pragma once
#include <stdio.h>

void timestamp();
void log_with_type(const char* type, const char *msg);
void log_error(const char *msg);
void log_info(const char *msg);
