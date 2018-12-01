#ifndef SOCKET_H
#define SOCKET_H

#include <string>

/**
* @brief Module Socket
* @file Socket.h
*
* Cette classe permet de mettre en place un socket client ou serveur
*
* @version 1.0
* @date 2018/04/27
*/
class Socket{

private:
  int m_socket; /**< Donnée membre m_socket qui contient le file descriptor */
  unsigned int m_port; /**< Donnée membre m_port qui contient le port du socket */

public:
  /**
  * @brief Procédure pour initialiser le socket
  *
  * @return none
  */
  void s_init();

  /**
  * @brief Procédure pour lier un socket système sur un port de la machine
  *
  * @param [in] port numéro de port sur lequel on associe le socket système
  *
  * @return none
  */
  void s_bind(const unsigned int port);

  /**
  * @brief Procédure pour créer un socket client qui se connecte à un host
  *
  * @param [in] host numéro de port sur lequel on associe le socket système
  * @param [in] port numéro de port sur lequel on associe le socket système
  *
  * @return none
  */
  void s_connect(const std::string& host, const int port) const;
  
  /**
  * @brief Procédure pour mettre un socket en écoute sur un serveur
  *
  * @return none
  */
  void s_listen() const;

  /**
  * @brief Fonction bloquante qui attend un client sur un serveur et qui l'accepte (fonction blocante)
  *
  * @return int retourne un file descriptor avec le client pour pouvoir discuter
  */
  int s_accept() const;

  /**
  * @brief Procédure pour fermer le socket m_socket 
  *
  * @return none
  */
  void s_close();

  /**
  * @brief Accesseur pour récupérer le file descriptor de m_socket
  *
  * @return int file descriptor du socket m_socket
  */
  int get_sock() const;

};

#endif // SOCKET_H
