#ifndef MAIL_H
#define MAIL_H

#include <string>
#include <vector>
#include <gmpxx.h>

/**
* @brief Module Mail
* @file Mail.h
*
* Cette classe permet de gérer des objets de type mail
*
* @version 1.0
* @date 2018/04/10
*/
class Mail {
private:
  int id; /** <Donnée membre: id associé au mail */
  std::string expediteur; /** <Donnée membre: adresse de l'expediteur */
  std::string destinataire; /** <Donnée membre: adresse du destinataire */
  std::string date; /** <Donnée membre: date d'envoi */
  std::string objet; /** <Donnée membre: objet associé */
  std::vector<std::string> cle_aes_chiffre; /** <Donnée membre: clés AES chiffrée */
  std::string idec; /** <Donnée membre: idec necessaire au dechiffrement */
  std::string contenu_chiffre; /** <Donnée membre: contenu chiffré du mail */

public:
  /**
  * @brief Constructeur pour créer un mail
  *
  * @return none
  */
  Mail();

  /**
  * @brief Constructeur pour créer un mail
  * 
  * @param [in] _id identifiant du Mail
  * @param [in] _expe expediteur du Mail
  * @param [in] _desti destinataire du Mail
  * @param [in] _date date d'envoi du Mail
  * @param [in] _obj objet de Mail
  * @param [in] _cle_aes_chiffre tableau de la cle AES chiffrée avec IBE de Cocks
  * @param [in] _idec idec associé au chiffrement
  * @param [in] _contenu_chiffre contenu chiffré du Mail
  *
  * @return none
  */
  Mail(const int _id, const std::string& _expe, const std::string& _desti, const std::string& _date, const std::string& _obj, const std::vector<std::string>& _cle_aes_chiffre, const std::string& _idec, const std::string& _contenu_chiffre);

  /**
  * @brief Constructeur pour créer un mail
  * 
  * @param [in] _id identifiant du Mail
  * @param [in] _expe expediteur du Mail
  * @param [in] _desti destinataire du Mail
  * @param [in] _obj objet de Mail
  * @param [in] _contenu contenu chiffré du Mail
  * @param [in] _n clé publique du destinataire
  *
  * @return none
  */
  Mail(const int _id, const std::string& _expe, const std::string& _desti, const std::string& _obj, const std::string& _contenu, const mpz_class& n);

  /**
  * @brief Procédure pour envoyer le Mail
  *
  * @return none
  */
  void envoyer_mail() const;

  /**
  * @brief Test de regression du module Mail
  * 
  * @return none
  */
  static void test_regression();

  /**
  * @brief Accesseur pour récupérer l'id
  *
  * @return int id du Mail
  */
  int get_id() const;

  /**
  * @brief Accesseur pour récupérer l'expediteur
  *
  * @return string adresse de l'expediteur
  */
	std::string get_expediteur() const;

  /**
  * @brief Accesseur pour récupérer le destinataire
  *
  * @return string adresse du destinataire
  */
	std::string get_destinataire() const;

  /**
  * @brief Accesseur pour récupérer la date
  *
  * @return string date SMTP
  */
	std::string get_date() const;

  /**
  * @brief Accesseur pour récupérer l'objet
  *
  * @return string objet
  */
	std::string get_objet() const;

  /**
  * @brief Accesseur pour récupérer la clé AES chiffrée
  *
  * @return vector<std::string> tableau de la clé AES
  */
  std::vector<std::string> get_cle_aes_chiffre() const;

  /**
  * @brief Accesseur pour récupérer l'idec
  *
  * @return string idec du chiffrement
  */
  std::string get_idec() const;

  /**
  * @brief Accesseur pour récupérer le contenu chiffré
  *
  * @return string objet
  */
	std::string get_contenu_chiffre() const;

  /**
  * @brief Accesseur pour récupérer le contenu dechiffré
  * 
  * @param [in] n Master Public Key
  * @param [in] skid clé privé secrète
  *
  * @return string contenu déchiffré
  */
  std::string get_contenu_dechiffre(const mpz_class& n, const mpz_class& skid) const;

  /**
  * @brief Procédure pour enregistrer un tableau de Mail dans un fichier texte
  * 
  * @param [in] _tab_mails tableau de Mail à sauvegarder
  * @param [in] _nom_fichier fichier de destination
  *
  * @return none
  */
  static void enregistrer_mails(const std::vector<Mail>& _tab_mails, const std::string& _nom_fichier);
  
  /**
  * @brief Fontion pour lire des Mail depuis un fichier
  * 
  * @param [in] _nom_fichier fichier source de mails
  * @return vector<Mail> tableau des Mail lu
  */
  static std::vector<Mail> lire_mails(const std::string& _nom_fichier);

  /**
  * @brief Fontion pour lire un Mail depuis un fichier
  * 
  * @param [in] _nom_fichier fichier source du mail
  * @return Mail Mail du fichier
  */
  static Mail lire_mail_imap(const std::string& _nom_fichier);

  /**
  * @brief Fontion pour recuperer un mail selon son id
  * 
  * @param [in] tab_mails fichier source de mails
  * @param [in] id identifiant du mail cherché
  * @return Mail* pointeur sur le mail en question
  */
  static Mail* get_mail_by_id(std::vector<Mail>& tab_mails, const int id);

private:
  /**
  * @brief Fontion pour recuperer le domaine d'une adresse mail (aprés le @)
  * 
  * @param [in] _adresse adresse mail à traiter 
  * @return string domaine de l'adresse mail
  */
  static std::string get_domaine_email(const std::string& _adresse);

  /**
  * @brief Fontion pour recuperer le serveur SMTP d'un domaine
  * 
  * @param [in] _domaine domaine de l'adresse mail
  * @return string serveur SMTP du domaine
  */
  static std::string get_adresse_serveur_SMTP(const std::string& _domaine);
  
  /**
  * @brief Accesseur pour récupérer le contenu pour SMTP
  *
  * @return string contenu
  */
  std::string smtp_get_contenu() const;

};

#endif // MAIL_H
