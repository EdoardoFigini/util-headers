#ifndef LOGGING_H
#define LOGGING_H

#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#define SEP "\\"
#else
#define SEP "/"
#endif

#define __FILENAME__ strrchr(SEP __FILE__, SEP[0]) + 1

#define LVL_FATAL 4
#define LVL_ERROR 3
#define LVL_WARN  2
#define LVL_DEBUG 1
#define LVL_INFO  0

#define ICON_FATAL "-"
#define ICON_ERROR "-"
#define ICON_WARN  "!"
#define ICON_DEBUG "*"
#define ICON_INFO  "i"

#define COLOR_RESET "\033[0m"
#define COLOR_FATAL "\033[41m"
#define COLOR_ERROR "\033[31m"
#define COLOR_WARN  "\033[33m"
#define COLOR_DEBUG "\033[37m"
#define COLOR_INFO  "\033[36m"

#ifdef LOG_FILE_LOC
#define LOG(lvl, fmt, ...) fprintf(LVL_ ## lvl < LVL_DEBUG ? stdout : stderr, COLOR_ ## lvl "[" ICON_ ## lvl "] " fmt "\n    at %s:%d" COLOR_RESET "\n", ##__VA_ARGS__, __FILENAME__, __LINE__);
#else
#define LOG(lvl, fmt, ...) fprintf(LVL_ ## lvl < LVL_DEBUG ? stdout : stderr, COLOR_ ## lvl "[" ICON_ ## lvl "] " fmt COLOR_RESET "\n", ##__VA_ARGS__);
#endif

#define LOG_FATAL(fmt, ...) LOG(FATAL, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) LOG(ERROR, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)  LOG(WARN, fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) LOG(DEBUG, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)  LOG(INFO,  fmt, ##__VA_ARGS__)

#endif // LOGGING_H
