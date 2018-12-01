#include "../include/Outils.h"
#include "../include/Socket.h"

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netdb.h>
#include <errno.h>
#include <sys/types.h>

void Socket::s_init() {
  m_socket = socket(PF_INET, SOCK_STREAM, 0);
  if (m_socket == -1) {
    outils::exit_error("Probléme sur socket(...)", outils::is_debug, errno);
  }
  printf("Socket OK !\n");
}

void Socket::s_bind(const unsigned int port) {
  m_port = port;
  int res;

  // Informations sur le socket voulu
  struct sockaddr_in info_socket;
  bzero(&info_socket, sizeof(info_socket));
  info_socket.sin_family = AF_INET;
  info_socket.sin_port = htons(m_port);
  info_socket.sin_addr.s_addr = htonl(INADDR_ANY);
  
  // Association de la socket avec une adresse et un port de la machine
  res = bind(m_socket, (struct sockaddr *)&info_socket, sizeof(info_socket));
  if (res == -1) {
    outils::exit_error("Probléme sur bind(...)", outils::is_debug, errno);
  }
  printf("Bind OK !\n");
}

void Socket::s_connect(const std::string& host, const int port) const{
  struct addrinfo info_socket;
  memset(&info_socket, 0, sizeof(struct addrinfo));
  info_socket.ai_family = AF_UNSPEC;
  info_socket.ai_socktype = 0;

  struct addrinfo *res_dns;
  getaddrinfo(host.c_str(), NULL, &info_socket, &res_dns);
  //while (res_dns != nullptr) {
    printf("--------------------\n");
    
    char str[res_dns->ai_addrlen];
    inet_ntop(AF_INET, &((sockaddr_in *)res_dns->ai_addr)->sin_addr, str, res_dns->ai_addrlen);
    printf("ai_addr=%s\n", str);
    printf("--------------------\n");

    res_dns = res_dns->ai_next;
  //}
  freeaddrinfo(res_dns);
  
  int res;
  struct sockaddr_in sock_v4;
  sock_v4.sin_family = AF_INET;
  sock_v4.sin_port = htons(port);
	inet_pton(AF_INET, str, &sock_v4.sin_addr);

  res = connect(m_socket, (sockaddr *)&sock_v4, sizeof(sock_v4));
  if (res == -1) {
    outils::exit_error("Probléme sur connect("+ std::string(str) + "," + std::to_string(port) + ")", outils::is_debug, errno);
  }
  printf("Connect OK !\n");
}

void Socket::s_listen() const{
  int res;
  res = listen(m_socket, 0);
  if (res == -1) {
    outils::exit_error("Probléme sur listen(...)", outils::is_debug, errno);
  }
  printf("Listen OK !\n");
}

int Socket::s_accept() const{
  struct sockaddr_in c;
  socklen_t tadr_len = sizeof(struct sockaddr_in);
  int fd_client;
  fd_client = accept(m_socket, (struct sockaddr *)&c, &tadr_len);
  if (fd_client == -1) {
    outils::exit_error("Probléme sur accept(...)", outils::is_debug, errno);
  }
  printf("[%s]>", inet_ntoa(c.sin_addr));

  return fd_client;
}

void Socket::s_close() {
  int res;
  printf("Déconnexion !\n");
  res = close(m_socket);
  if(res == -1) {
    outils::exit_error("Probléme sur close(...)", outils::is_debug, errno);
  }
}

int Socket::get_sock() const {
  return m_socket;
}
