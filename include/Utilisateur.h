#ifndef UTILISATEUR_H
#define UTILISATEUR_H

#include <string>
#include "Mail.h"

/**
* @brief Module Utilisateur
* @file Mail.h
*
* Cette classe permet de gérer des utilisateurs de notre application
*
* @version 1.0
* @date 2018/04/27
*/
class Utilisateur {
private:
  std::string email; /**< Donnée membre: adresse mail de l'utilisateur */
  std::string mdp; /**< Donnée membre: mot de passe */
  std::string nom; /**< Donnée membre: le nom */
  std::string prenom; /**< Donnée membre: le prenom */

  std::string skid; /**< Donnée membre: clé privée */
  std::string mpk; /**< Donnée membre: Master Public Key */

  std::vector<Mail> tab_mails; /**< Donnée membre: mails envoyés */

public:
  /**
  * @brief Constructeur pour créer un utilisateur
  * 
  * @param [in] _adresse_mail adresse mail
  * @param [in] mdp mot de passe associé
  * @param [in] _nom le nom
  * @param [in] _prenom le prenom
  *
  * @return none
  */
  Utilisateur(const std::string& _email, const std::string& _mdp, const std::string& _nom, const std::string& _prenom);

  /**
  * @brief Procédure pour ajouter un mail au tableau de mail de l'utilisateur
  * 
  * @param [in] _mail mail à ajouter
  * 
  * @return none
  */
  void ajouter_mail(const Mail& mail);

  /**
  * @brief Procédure pour enregistrer le tableau de mails de l'utilisateur
  * 
  * @return none
  */
  void enregistrer_tab_mails() const;

  /**
  * @brief Fontion pour lire des utilisateurs depuis un fichier
  * 
  * @param [in] _nom_fichier fichier source d'utilisateurs
  *
  * @return std::vector<Utilisateur> tableau des utilisateurs lu
  */
  static std::vector<Utilisateur> lire_utilisateurs(const std::string& nom_fichier);

  /**
  * @brief Fonction que retourne un utilisateur identifié par son mail
  * 
  * @param [in] tab_utilisateurs tableau d'utilisateurs
  * @param [in] mail mail associé à la recherche
  *
  * @return Utilisateur* utilisateur associé au mail
  */
  static Utilisateur* get_utilisateur_by_email(std::vector<Utilisateur>* tab_utilisateurs, const std::string& email);

  /**
  * @brief Accesseur pour récupérer l'adresse mail
  *
  * @return std::string adresse mail
  */
  std::string get_email() const;

  /**
  * @brief Accesseur pour récupérer le hash du mot de passe
  *
  * @return std::string hash du mot de passe
  */
  std::string get_mdp() const;

  /**
  * @brief Accesseur pour récupérer le nom
  *
  * @return std::string le nom
  */
  std::string get_nom() const;

  /**
  * @brief Accesseur pour récupérer le prénom
  *
  * @return std::string le prénom
  */
  std::string get_prenom() const;

  /**
  * @brief Accesseur pour récupérer la clé privée (skID)
  *
  * @return std::string skID
  */
  std::string get_skid() const;

  /**
  * @brief Accesseur pour récupérer la Master Pulic Key
  *
  * @return std::string Master Public Key
  */
  std::string get_mpk() const;

  /**
  * @brief Accesseur pour récupérer le tableau de mails
  *
  * @return std::vector<Mail> tableau de mails
  */
  std::vector<Mail> get_tab_mails() const;

  /**
  * @brief Procédure pour créer des utilisateurs de bases dans un fichier texte
  * 
  * @param [in] _chemin_fichier fichier cible
  * 
  * @return none
  */
  static void creer_utilisateurs(const std::string& chemin_fichier);

private:
  /**
  * @brief Fontion pour recuperer la clé privée (skID) depuis un serveur ou en local
  * 
  * @return none
  */
  void recuperer_skid();

  /**
  * @brief Fontion pour recuperer la Master Public Key MPK depuis un serveur ou en local
  * 
  * @return none
  */
  void recuperer_mpk();

  /**
  * @brief Procédure pour enregistrer un tableau d'utilisateurs dans un fichier texte
  * 
  * @param [in] _tab_utilisateurs tableau d'utilisateurs à sauvegarder
  * @param [in] _nom_fichier fichier de destination
  *
  * @return none
  */
  static void enregistrer_utilisateurs(const std::string& nom_fichier, const std::vector<Utilisateur>& tab_utilisateurs);

  

  /**
  * @brief Fontion pour lire les mails d'un utilisateur depuis un fichier
  *
  * @return std::vector<Mail> tableau des Mail lu
  */
  std::vector<Mail> lire_mails();

};

#endif // UTILISATEUR_H
