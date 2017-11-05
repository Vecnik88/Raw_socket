#include "../inc/server.h"

int main(int argc, char *argv[]) {
  char command[50];
  pthread_t UDP_thread;
  thread_data th_data_udp;

  th_data_udp.pPort = "7777";
  th_data_udp.pProtocol = "UDP";
  if (pthread_create(&UDP_thread, NULL, FuncListen, (void*)&th_data_udp) != 0) {
    perror("Не удалось создать поток UDP-сервера");
    exit(EXIT_FAILURE);
  }
  printf("UDP-cервер запущен. Ожидание запросов от клиентов...\n");
  /*Команды управления сервером*/
  while(1) {
    scanf("%s", command);
    if (strcmp(command, "выход") == 0) {
      break;
    } else if (strcmp(command, "exit") == 0) {
      break;
    }
  }
  if (pthread_cancel(UDP_thread) != 0) {
    perror("Не удалось завершить поток UDP-сервера");
  }
  exit(EXIT_SUCCESS);
}
