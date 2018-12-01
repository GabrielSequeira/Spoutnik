#ifndef CHIFFREMENT_H
#define CHIFFREMENT_H

#include <gmpxx.h>
#include <vector>

/**
* @brief Module Chiffrement
* @file Chiffrement.h
*
* Cette classe permet de chiffrer un message et une clé AES
*
* @version 1.0
* @date 2018/04/10
*/
class Chiffrement {
private:
  mpz_class t1; /** <Donnée membre: t1 un nombre généré aléatoirement */
  mpz_class t2; /** <Donnée membre: t2 un nombre généré aléatoirement */
  mpz_class a; /** <Donnée membre: a étant l'itéré du hachage */
  mpz_class c1; /** <Donnée membre: c1 étant la première valeur du bit envoyé */
  mpz_class c2; /** <Donnée membre: c2 étant la deuxième valeur du bit envoyé */

  std::vector<int> tab_chaine_ascii; /**<Donnée membre: tab_chaine_ascii contenant l'ensemble des valeurs ascii de la clé */
  std::vector<int> tab_chaine_binaire; /**<Donnée membre: tab_chaine_binaire contenant l'ensemble des valeurs binaires de la clé */

  gmp_randstate_t etat; /**<Donnée membre: état pour générer un aléatoire correct à chaque nouvel appel */

public:
  /**
  * @brief Constructeur par défaut
  *
  * @return none
  */
  Chiffrement();
  
  /**
  * @brief Destructeur par défaut
  *
  * @return none
  */
  ~Chiffrement();

  /**
  * @brief Fonction permettant de chiffrer un message à partir d'AES
  *
  * @param [in] n master public key
  * @param [in] ID adresse de destinataire
  * @param [in] message un string correspondant au message que l'on va chiffrer
  * @param [out] ivec string générée aléatoirement necessaire à AES
  * @param [out] cle_aes_chiffre clé AES chiffrée
  *
  * @return string message chiffré
  */
  std::string chiffrer_message(const mpz_class& n, const std::string& ID, const std::string& message, std::string& ivec, std::vector<std::string>& cle_aes_chiffre);

private:
  /**
  * @brief Procédure pour chiffrer une clé AES à partir d'un identifiant, la MPK, et la clé
  *
  * @param [in] n grand nombre aléatoire correspondant à la MPK
  * @param [in] ID correspond à l'adresse mail à laquelle on envoie le mail
  * @param [in] cle correspond à la clé AES en claire qui sera chiffrée
  *
  * @return none
  */
  std::vector<std::string> chiffrer_cle_aes(const mpz_class& n, const std::string& ID, const std::string& cle);

  /**
  * @brief Procédure permettant de calculer le symbole de jacobi en tirant deux t (t1,t2) aléatoire pour chaque bit de la clé
  *
  * @param [in] n la MPK utilisée tout au long
  * @param [in] indice_bit pour dire quel bit on regarde à partir du tableau binaire
  *
  * @return none
  */
  void calcul_jacobi(const mpz_class& n, const int indice_bit);
  
  /**
  * @brief Procédure permettant de calculer c1, la première valeur envoyée dans c (c=(c1,c2))
  *
  * @param [in] n la MPK utilisée tout au long
  *
  * @return string valeur de c1
  */
  std::string calcul_c1(const mpz_class& n);
  
  /**
  * @brief Procédure permettant de calculer c2, la deuxième valeur envoyée dans c (c=(c1,c2))
  *
  * @param [in] n la MPK utilisée tout au long
  *
  * @return string valeur de c2
  */
  std::string calcul_c2(const mpz_class& n);

  /**
  * @brief Procédure permettant d'initialiser le temps
  *
  * @return none
  */
  void initialiser_temps();
};

#endif // CHIFFREMENT_H
