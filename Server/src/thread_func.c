#include "../inc/server.h"

void* FuncListen(void* arg) {
  thread_data *th_data = (thread_data*)arg;
  int opt = 1;
  int listener;
  unsigned int client_len;
  struct sockaddr_in serv_addr;
  struct sockaddr_in client_address;
  char buf[BUF_SIZE] = {0};
  int bytes_read, bytes_send;
  struct epoll_event server_event;
  struct epoll_event events[MAX_EVENTS];
  int epfd, nfds;
  int i;

  if (pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL) != 0) {
    perror("Ошибка настройки отмены для потока");
    exit(EXIT_FAILURE);
  }
  /*Создание UDP сокета*/
  listener = socket(AF_INET, SOCK_DGRAM, 0);
  if (listener < 0) {
    perror("Ошибка создания сокета для сервера");
    exit(EXIT_FAILURE);
  }
  /*Создать дескриптор для вызовов epoll*/
  epfd = epoll_create(MAX_EVENTS);
  if (epfd == -1) {
    perror("Ошибка создания epoll");
    exit(EXIT_FAILURE);
  }
  server_event.data.fd = listener;
  server_event.events = EPOLLIN;
  /*Зарегистрировать событие для слушающего сокета*/
  if (epoll_ctl(epfd, EPOLL_CTL_ADD, listener, &server_event) == -1) {
    perror("Ошибка создания события epoll для сервера");
    exit(EXIT_FAILURE);
  }
  if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof (opt)) == -1) {
    perror("Ошибка настройки сокета");
    exit(EXIT_FAILURE);
  }
  /*Присвоить имя сокету*/
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(atoi(th_data->pPort));
  memset(serv_addr.sin_zero, '\0', sizeof serv_addr.sin_zero);
  if (bind(listener, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) != 0) {
    close(listener);
    perror("Ошибка присвоения имени сокету");
    printf("Примечание: запускать сервер необходимо с правами администратора\n");
    exit(EXIT_FAILURE);
  }
  while(1) {
    nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
    if (nfds == -1) {
      perror("Ошибка ожидания на epoll дескрипторе");
      exit(EXIT_FAILURE);
    }
    /*Анализ произошедших событий*/
    for (i = 0; i < nfds; i++) {
      if ((events[i].data.fd == listener) && (events[i].events & EPOLLIN)) {
        while(1) {
          client_len = sizeof(client_address);
          memset(buf, 0, sizeof(buf));
          bytes_read = recvfrom(listener, buf, sizeof(buf), 0,
                                (struct sockaddr *)&client_address,
                                &client_len);
          if (bytes_read == -1) {
            perror("Ошибка получения запроса от клиента");
            exit(EXIT_FAILURE);
          } else if (bytes_read == 0) {
            break;
          } else {
            printf("UDP запрос от IP %s: %s\n",
                   inet_ntoa(client_address.sin_addr), buf);
            /*Какой-то ответ клиенту*/
            strcpy(buf, "Answer to client");
            bytes_send = sendto(listener, buf, strlen(buf), 0,
                                (struct sockaddr *)&client_address,
                                sizeof(client_address));
            if (bytes_send == -1) {
              perror("Ошибка отправления ответа клиенту\n");
              exit(EXIT_FAILURE);
            }
          }
        }
      }
    }
  }
  pthread_exit(NULL);
}
