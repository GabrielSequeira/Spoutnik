#ifndef FEN_CLIENT
#define FEN_CLIENT

#include <QtWidgets>
#include "../../include/Mail.h"
#include "../../include/Utilisateur.h"
#include "../../include/qt/FormMail.h"

/**
* @brief Classe pour la gestion de la fenetre Qt de connexion
*
* Cette classe vous permet de gerer des fenetres Qt pour les clients
*
* @version 1.0
* @date 2018/03/01
*/
class FenClient : public QWidget {
Q_OBJECT

public:
  /**
  * @brief Constructeur avec l'utilisateur qui se connecte
  *
  * @return none
  */
  FenClient(Utilisateur *utilisateur, bool bool_imap, bool bool_envoi);
		
public slots:
  /**
  *  @brief Procédure envoyerMail
  *
  * Procédure: envoie un mail
  *
  * @return none
  */
  void envoyer_mail();

  /**
  *  @brief Procédure remplir
  *
  * Procédure: remplit les champs expéditeur, destinataires, objet, contenu
  *
  * @param [in] item qui contient les données du mail
  *
  * @return none
  */
  void remplir(QListWidgetItem* item) const;

  /**
  *  @brief Procédure remplir
  *
  * Procédure: remplit les champs expéditeur, destinataires, objet, contenu
  *
  * @param [in] item qui contient les données du mail
  *
  * @return none
  */
  void remplir3(Mail m) const;

  /**
  *  @brief Procédure dechiffrer
  *
  * Procédure: dechiffre le contenu du mail
  *
  * @return none
  */
  void dechiffrer() const;

  /**
  *  @brief Procédure dechiffrer
  *
  * Procédure: dechiffre le contenu du dernier mail reçu
  *
  * @return none
  */
  void dechiffrer3() const;

  /**
  * @brief Procédure pour quitter
  *
  * Procédure: permet de gerer la déconnexion
  *
  * @return none
  */
  void quitter() const;

private:
  Utilisateur *utilisateur;
  bool envoi_mail;
  
  /* -------- Élements fenêtre Qt -------- */
  QVBoxLayout *layout_principal;

    /* -------- Conteneur d'onglets -------- */
    QTabWidget *onglets;
    QWidget *page1 = new QWidget;
    QWidget *page2 = new QWidget;
    QWidget *page3 = new QWidget;

      /* -------- Onglet 1 -------- */
      QVBoxLayout *layout_onglet_1;
        FormMail *form_mail_1;

        QHBoxLayout *foot_layout;
          QPushButton *btn_envoyer;
          QPushButton *btn_quitter;

      /* -------- Onglet 2 -------- */
      QHBoxLayout *layout_onglet_2;
        QVBoxLayout *layout_liste_mail;
          QListWidget *liste;

        QVBoxLayout *layout_contenu;
          FormMail *form_mail_2;
        
        QHBoxLayout *foot_layout2;
          QPushButton *btn_dechiffrer;

      /* -------- Onglet 3 -------- */
      QHBoxLayout *layout_onglet_3;

        QVBoxLayout *layout_contenu3;
          FormMail *form_mail_3;
        
        QHBoxLayout *foot_layout3;
          QPushButton *btn_dechiffrer3;
};

#endif // FEN_CLIENT
