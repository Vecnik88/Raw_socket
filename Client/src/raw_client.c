#include "../inc/raw_client.h"

int main(int argc, char *argv[]) {
  int sock;
	struct sockaddr_in serv_addr;
	char datagram[DATA_SIZE] = {0};
	char *data;
	struct iphdr *ip_h;
	struct udphdr *udp_h;
	char *server_ip = "127.0.0.1";
	char *server_port = "7777";
	char *client_ip = "127.0.0.1";
	char *client_port = "1111";
	int bytes_read, bytes_send;
	struct timeval tv;
	int pack_size = 0;
	int one;
	const int *val;

  sock = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
  if (sock == -1) {
    perror("Failed to create raw socket");
    exit(EXIT_FAILURE);
  }
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(atoi(server_port));
  serv_addr.sin_addr.s_addr = inet_addr(server_ip);
  /*таймаут на операции чтения и записи*/
  tv.tv_sec = 1;
  tv.tv_usec = 0;
  if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) == -1) {
    perror("Failed to setsockopt (SO_RCVTIMEO)");
    exit(EXIT_FAILURE);
  }
  if (setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv)) == -1) {
    perror("Failed to setsockopt (SO_SNDTIMEO)");
    exit(EXIT_FAILURE);
  }
  one = 1;
  val = &one;
  /*указать, что в пакете будет IP-заголовок*/
  if (setsockopt (sock, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) == -1) {
    perror("Failed to setsockopt (IP_HDRINCL)");
    exit(EXIT_FAILURE);
  }
#ifdef DEBUG
  while (1) {
#endif
    /*Подготовка пакета к отправке*/
    ip_h = (struct iphdr *)datagram;
    udp_h = (struct udphdr *)(datagram + sizeof(struct iphdr));
    data = datagram + sizeof(struct iphdr) + sizeof(struct udphdr);
    strcpy(data, "Data or request for server");
    if(strlen(data) > (sizeof(datagram) - sizeof(struct iphdr) -
        sizeof(struct udphdr))) {
      printf("Size limit data is exceeded\n");
      exit(EXIT_FAILURE);
    }
    pack_size = sizeof(struct iphdr) + sizeof(struct udphdr) + strlen(data);
    /*Заполнение UDP-заголовка*/
    udp_h->source = htons(atoi(client_port)); /*порт источника, т.е. "мы"*/
    udp_h->dest = htons(atoi(server_port));   /*порт назначения, т.е. сервера*/
    udp_h->len = htons(sizeof(struct udphdr) + strlen(data)); /*размер заголовка*/
    udp_h->check = 0;
    /*Заполнение IP-заголовка*/
    ip_h->ihl = 5;
    ip_h->version = 4;          /*версия протокола IP*/
    ip_h->tos = 0;              /*тип сервиса*/
    /*длина IP-пакета*/
    ip_h->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + strlen(data);
    ip_h->id = htonl(10150);    /*id этого пакета*/
    ip_h->frag_off = 0;         /*смещение фрагмента*/
    ip_h->ttl = 64;             /*время жизни пакета*/
    ip_h->protocol = IPPROTO_UDP; /*указать следующий за ним протокол*/
    ip_h->check = 0;
    ip_h->saddr = inet_addr(client_ip);  /*IP источника*/
    ip_h->daddr = inet_addr(server_ip);  /*IP назначения*/
    printf("Internet Protocol, Src: %s", client_ip);
    printf(", Dst: %s\n", server_ip);
    printf("\tType of Service: %d\n\tTotal Length: %d\n\tIdentification: %d\n\t"
        "Time to live: %d\n", ip_h->tos, ip_h->tot_len, ip_h->id, ip_h->ttl);
    printf("User Datagram Protocol, Src Port: %d", ntohs(udp_h->source));
    printf(" Dst Port: %d\n", ntohs(udp_h->dest));
    /*Пакет готов, отправляем*/
    bytes_send = sendto(sock, datagram, pack_size, 0, (struct sockaddr *)
                        &serv_addr, sizeof(serv_addr));
    if (bytes_send == -1) {
      perror("Error: sendto");
      exit(EXIT_FAILURE);
    }
    if (bytes_send > 0) {
      print_data_hex(datagram, bytes_send);
      memset(datagram, 0, sizeof(datagram));
      /*Пытаемся получить данные*/
      while(1) {
        bytes_read = recvfrom(sock, datagram, sizeof(datagram), 0, NULL, NULL);
        if (bytes_read == -1) {
          perror("Error: recvfrom");
          exit(EXIT_FAILURE);
        } else if (bytes_read == 0) {
          break;
        } else {
          /*Разбор полученных данных*/
          ip_h = (struct iphdr *)datagram;
          udp_h = (struct udphdr *)(datagram + sizeof(struct iphdr));
          data = datagram + sizeof(struct iphdr) + sizeof(struct udphdr);
          /*Пакет предназначен для нас? (Прим.: RAW слушает все пакеты)*/
          if((ip_h->daddr == inet_addr(client_ip)) &&
              (udp_h->dest == htons(atoi(client_port)))) {
            printf("Internet Protocol, Src: %s", client_ip);
            printf(", Dst: %s\n", server_ip);
            printf("\tType of Service: %d\n\tTotal Length: %d\n\tIdentification:"
                " %d\n\tTime to live: %d\n", ip_h->tos, ip_h->tot_len, ip_h->id,
                ip_h->ttl);
            printf("User Datagram Protocol, Src Port: %d", ntohs(udp_h->source));
            printf(" Dst Port: %d\n", ntohs(udp_h->dest));
            print_data_hex(datagram, bytes_read);
            break;
          }
        }
      }
    }
#ifdef DEBUG
    sleep(2);
  }
#endif
  close(sock);
  exit(EXIT_SUCCESS);
}
