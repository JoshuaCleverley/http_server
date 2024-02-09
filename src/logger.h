#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>

void timestamp();
void log_with_type(const char* type, const char *msg, va_list args);
void log_error(const char *msg, ...);
void log_info(const char *msg, ...);
