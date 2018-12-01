#ifndef OUTILS_H
#define OUTILS_H

#include <vector>
#include <gmpxx.h>

/**
* @brief Module Outils
* @file Outils.h
*
* Ce module permet de stocker toutes les fonctions génériques de l'application
* qui n'ont pas forcément de liens entre elles
*
* @version 1.0
* @date 2018/04/27
*/
namespace outils {
	const bool is_debug = true; /**< Variable pour activer les traces lors du debug */
	
	/**
  * @brief Fonction pour transformer une string en tableau de code ASCII
  *
  * @param [in] message chaine de caractéres à manipuler
  *
  * @return std::vector<int> tableau d'entiers du codage ASCII
  */
	std::vector<int> chaine_to_ascii(const std::string& message);

	/**
  * @brief Fonction pour transformer un tableau ASCII en tableau au codage binaire
  *
  * @param [in] tab_ascii tableau ascii à manipuler
  *
  * @return std::vector<int> tableau d'entiers du codage binaire
  */
	std::vector<int> ascii_to_binaire(const std::vector<int>& tab_ascii);

	/**
  * @brief Fonction pour transformer un tableau binaire en tableau au codage ASCII
  *
  * @param [in] tab_binaire tableau binaire à manipuler
  *
  * @return std::vector<int> tableau d'entiers du codage ASCII
  */
	std::vector<int> binaire_to_ascii(const std::vector<int>& tab_binaire);

	/**
  * @brief Fonction pour transformer un tableau ASCII en une chaine de caractères
  *
  * @param [in] tab_ascii tableau ASCII à manipuler
  *
  * @return std::string chaine de caractères
  */
	std::string ascii_to_chaine(const std::vector<int>& tab_ascii);

	/**
  * @brief Procédure pour afficher un tableau ASCII
  *
  * @param [in] tab_ascii tableau ASCII à manipuler
  *
  * @return none
  */
	void affiche_ascii(const std::vector<int>& tab_ascii);

	/**
  * @brief Procédure pour afficher un tableau binaire
  *
  * @param [in] tab_binaire tableau binaire à manipuler
  *
  * @return none
  */
	void affiche_binaire(const std::vector<int> tab_binaire);

	/**
  * @brief Fonction pour calculer la valeur a
  *
  * @param [in] n correspond à la clé publique maitre (MPK)
	* @param [in] ID id utilisé pour le calcul de a
  *
  * @return mpz_class valeur de a
  */
	mpz_class calcul_a(const mpz_class& n, const std::string& ID);

	/**
  * @brief Fonction pour genérer une clé aleatoire de 16 caractéres pour AES
  *
  * @return std::string clé aléatoire de 16 caractéres
  */
	std::string generer_cle_aleatoire_AES();

	/**
  * @brief Fonction générique de la gestion d'erreur
  *   
  * @param [in] file le fichier où a lieu l'erreur
  * @param [in] ligne la ligne du fichier où a lieu l'erreur
  * @param [in] msg le message à afficher en cas d'erreur
  * @param [in] cond la condition (l'erreure est déclanchée si elle est vraie)
  * @param [in] errnum code d'erreur système
  * @param [in] exception si vrai envoie une exception en cas d'erreur, sinon, juste un warning
  * 
  * @return none
  */
  void error_(const char *file, const int ligne, const std::string &msg, bool cond, int errnum, bool exception);

	#define exit_error(msg, cond, errnum) error_(__FILE__, __LINE__, msg, cond, errnum, true)
	#define warning_error(msg, cond, errnum) error_(__FILE__, __LINE__, msg, cond, errnum, false)

  /**
  * @brief Fonction pour encoder une chaine en base64
  *
  * @param [in] bytes_to_encode pointeur sur la chaine de caractères
	* @param [in] len taille de la chaine à encoder
  *
  * @return std::string chaine en base64
  */
	std::string base64_encode(const unsigned char *bytes_to_encode, unsigned int len);
	
  /**
  * @brief Fonction pour decoder une chaine en base64
  *
  * @param [in] chaine chaine à décoder
  *
  * @return std::string chaine decodée
  */
  std::string base64_decode(const std::string& chaine);

  /**
  * @brief Fonction de hashage en SHA256
  *
  * @param [in] to_hash chaine de caractères à hasher
  *
  * @return std::string chaine hashé
  */
	std::string hasher(const std::string& to_hash);


  /**
  * @brief Fonction pour savoir si un fichier existe
  *
  * @param [in] _nom_fichier chemin vers le fichier
  *
  * @return bool
  */
	bool fichier_existe(const std::string& _nom_fichier);

  /**
  * @brief Procédure pour créer une arborescence de dossiers
  *
  * @param [in] chemin chemin à créer
  *
  * @return none
  */
	void creer_dossiers(const std::string& chemin);

  /**
  * @brief Fonction qui retourne la date et l'heure courante
  *
  * @return std::string
  */
  std::string smtp_get_current_date();

  /**
  * @brief Procédure pour créer une arborescence de dossiers
  *
  * @param [in] chemin chemin à créer
  *
  * @return none
  */
  int nombre_aleatoire(int min, int max);

  /**
   * @brief Procedure pour compiler src/curl et créer le fichier mail_recu.txt
   * 
   * @param mail adresse mail du compte
   * @param mdp mot de passe pour se connecter
   */
  void recuperer_dernier_mail(std::string mail, std::string mdp);

}

#endif // OUTILS_H