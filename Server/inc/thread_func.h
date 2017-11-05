#ifndef UDP_SERVER_THREAD_FUNC_H_
#define UDP_SERVER_THREAD_FUNC_H_

#include <pthread.h>

/*данные для передачи в поток*/
typedef struct {
  char *pPort;
  char *pProtocol;
} thread_data;

void* FuncListen(void* arg);

#endif
