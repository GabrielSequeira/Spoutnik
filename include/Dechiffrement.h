#ifndef DECHIFFREMENT_H
#define DECHIFFREMENT_H

#include <gmpxx.h>
#include <vector>

/**
* @brief Module Dechiffrement
* @file Dechiffrement.h
*
* Cette classe permet de déchiffrer un message et une clé AES
*
* @version 1.0
* @date 2018/04/10
*/
class Dechiffrement {
private:
  mpz_class c1; /** <Donnée membre: c1 la première valeur de la clé codée*/
  mpz_class c2; /** <Donnée membre: c2 la deuxième valeur de la clé codée*/
  mpz_class a; /** <Donnée membre: a étant l'itéré du hachage.*/
  mpz_class alpha; /** <Donnée membre: alpha étant la valeur choisie de c1 ou c2 par bit*/
  mpz_class sk; /** <Donnée membre: sk la valeur de la clé secrète*/

  std::vector<int> tab_binaire; /** <Donnée membre: tab_binaire contenant l'ensemble des valeurs binaires de la clé*/
  std::vector<int> tab_ascii; /** <Donnée membre: tab_ascii contenant l'ensemble des valeurs ascii de la clé*/

public:
  /**
  * @brief Constructeur par défaut
  *
  * @return none
  */
  Dechiffrement();

  /**
  * @brief Constructeur par défaut
  * 
  * @param [in] skid clé privée d'un utilisateur
  *
  * @return none
  */
  Dechiffrement(const mpz_class& skid);

  /**
  * @brief Fonction permettant de déchiffrer le message, le contenu, en utilisant la clé AES
  * 
  * @param [in] n master public key
  * @param [in] ID string adresse mail du destinataire
  * @param [in] cle_aes_chiffre clé AES chiffrée
  * @param [in] message_chiffre message dechiffré
  * @param [in] idec_64 chaine necessaire au dechiffrement AES
  *
  * @return string chaine dechiffrée
  */
  std::string dechiffrer_message(const mpz_class& n, const std::string& ID, const std::vector<std::string>& cle_aes_chiffre, const std::string& message_chiffre, const std::string& idec_64);

private:
  /**
  * @brief Fonction permettant de déchiffrer la clé AES
  *
  * @param [in] n la clé MPK
  * @param [in] cle_aes_chiffre le tableau contenant c1 et c2 à chaque case
  * @param [in] ID string contenant l'adresse mail de la personne lisant les mails
  *
  * @return string
  */
  std::string dechiffrer_cle_aes(const mpz_class& n, const std::vector<std::string>& cle_aes_chiffre, const std::string& ID);

  /**
  * @brief Procédure permettant de calculer alpha
  *
  * @param [in] n la MPK utilisée tout au long
  *
  * @return none
  */
  void calcul_alpha(const mpz_class& n);

   /**
  * @brief Procédure permettant de calculer m, la valeur d'un bit d'un caractère
  *
  * @param [in] n la MPK utilisée tout au long
  *
  * @return none
  */
  void calcul_m(const mpz_class& n);

  /**
  * @brief Procédure permettant de modifier la valeur de c1 qui sera calculé plus tard
  *
  * @param [in] nc1 le nouveau c1 inséré dans le tableau
  *
  * @return none
  */
  void set_c1(const std::string& nc1);
  
  /**
  * @brief Procédure permettant de modifier la valeur de c2 qui sera calculé plus tard
  *
  * @param [in] nc2 le nouveau c2 inséré dans le tableau
  *
  * @return none
  */
  void set_c2(const std::string& nc2);
};

#endif // DECHIFFREMENT_H
