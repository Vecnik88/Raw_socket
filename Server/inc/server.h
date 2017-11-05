#ifndef UDP_SERVER_H_
#define UDP_SERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <ctype.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include "thread_func.h"

#define BUF_SIZE 1492
#define MAX_EVENTS 32

#endif
