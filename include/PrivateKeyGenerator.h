#ifndef PRIVATE_KEY_GENERATOR_H
#define PRIVATE_KEY_GENERATOR_H

#include <gmpxx.h>
#include <vector>
#include <string>

#include "Socket.h"

/**
* @brief Classe pour la gestion du PKG
*
* Cette classe vous permet de gerer le PKG
*
* @version 1.0
* @date 2018/03/01
*/
class PrivateKeyGenerator{
private:
  std::vector<std::string> tab_cli; /**< Donnee membre: tableau de client */
  mpz_class n; /**< Donnee membre: master public key */
  mpz_class p; /**< Donnee membre: master secret key p */
  mpz_class q; /**< Donnee membre: master secret key q */

public:
  /**
  * @brief Constructeur par défaut
  *
  * @return none
  */
  PrivateKeyGenerator();

  /**
  * @brief Destructeur par défaut
  *
  * @return none
  */
  ~PrivateKeyGenerator();
  
  /**
  * @brief Procédure pour attendre des clients
  *
  * @return none
  */
  void attendre_client();

private:
  /**
  * @brief Procédure pour traite des clients
  *
  * @param [in] fd_client file descriptor du client
  *
  * @return none
  */
	void traiter_client(const int fd_client);

  /**
  * @brief Fonction pour calculer la secret key
  *
  * @param [in] ID adresse mail pour generer la secret key
  *
  * @return mpz_class
  */
  mpz_class calcul_sk(const std::string& ID) const;

  /**
  * @brief Fonction pour ajouter des clients dans le tableau de clients du serveur
  *
  * @param [in] mail adresse mail du nouveau client
  *
  * @return bool vrai si ajout, faut si déjà présent
  */
  bool ajouter_client(const std::string& mail);
  
  /**
  * @brief Procédure pour générer 2 nombres aléatoire premiers
  *
  * @param [in] rand mpz_t dans lequel va etre ajouté le nombre premier
  * @param [in] rand2 mpz_t dans lequel va etre ajouté le nombre premier
  *
  * @return none
  */
  void generer_nb_premier(mpz_t rand, mpz_t rand2) const;

  /**
  * @brief Procédure pour ecrire les clefs dans le fichier txt
  *
  * @return none
  */
  void ecrire_key() const;

  /**
  * @brief Fonction pour lire des clefs depuis un fichier txt
  *
  * @return std::vector<std::tring>
  */
  void lire_key();

  /**
  * @brief Procédure enregistrer les clients dans un fichier texte
  *
  * @param [in] _nom_fichier fichier texte cible
  *
  * @return none
  */
  void ecrire_clients(const std::string& nom_fichier) const;

  /**
  * @brief Procédure pour lire les clients d'un fichier texte
  *
  * @param [in] _nom_fichier fichier texte source
  *
  * @return none
  */
  void lire_clients(const std::string& nom_fichier);

  /**
  * @brief Fonction pour savoir si un client est dans la tableau de clients du serveur
  *
  * @param [in] mail adresse mail du client
  *
  * @return bool vrai si présent, faux sinon
  */
  bool client_est_present(const std::string& mail) const;

};

#endif // PRIVATE_KEY_GENERATOR_H
