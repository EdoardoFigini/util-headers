#ifndef LOGGING_H
#define LOGGING_H

#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#define LOG_SEP "\\"
#else
#define LOG_SEP "/"
#endif

#define __FILENAME__ strrchr(LOG_SEP __FILE__, LOG_SEP[0]) + 1

#define LOG_LVL_FATAL 4
#define LOG_LVL_ERROR 3
#define LOG_LVL_WARN  2
#define LOG_LVL_DEBUG 1
#define LOG_LVL_INFO  0

#define LOG_ICON_FATAL "-"
#define LOG_ICON_ERROR "-"
#define LOG_ICON_WARN  "!"
#define LOG_ICON_DEBUG "*"
#define LOG_ICON_INFO  "i"

#define LOG_COLOR_RESET "\033[0m"
#define LOG_COLOR_FATAL "\033[41m"
#define LOG_COLOR_ERROR "\033[31m"
#define LOG_COLOR_WARN  "\033[33m"
#define LOG_COLOR_DEBUG "\033[37m"
#define LOG_COLOR_INFO  "\033[36m"

#ifdef LOG_FILE_LOC
#define LOG(lvl, fmt, ...) fprintf(LOG_LVL_ ## lvl < LOG_LVL_DEBUG ? stdout : stderr, LOG_COLOR_ ## lvl "[" LOG_ICON_ ## lvl "] " fmt "\n    at %s:%d" LOG_COLOR_RESET "\n", ##__VA_ARGS__, __FILENAME__, __LINE__);
#else
#define LOG(lvl, fmt, ...) fprintf(LOG_LVL_ ## lvl < LOG_LVL_DEBUG ? stdout : stderr, LOG_COLOR_ ## lvl "[" LOG_ICON_ ## lvl "] " fmt LOG_COLOR_RESET "\n", ##__VA_ARGS__);
#endif

#define LOG_FATAL(fmt, ...) LOG(FATAL, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) LOG(ERROR, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)  LOG(WARN, fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) LOG(DEBUG, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)  LOG(INFO,  fmt, ##__VA_ARGS__)

#endif // LOGGING_H
