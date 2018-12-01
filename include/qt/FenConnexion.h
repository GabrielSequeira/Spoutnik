#ifndef FEN_CONNEXION
#define FEN_CONNEXION

#include <QWidget>

#include "./FenClient.h"

/**
* @brief Classe pour la gestion de la fenetre Qt de connexion
*
* Cette classe vous permet de gerer des fenetres Qt pour les clients
*
* @version 1.0
* @date 2018/03/01
*/
class FenConnexion : public QWidget{
  Q_OBJECT

  private:
    std::vector<Utilisateur> tab_utilisateurs;

    QVBoxLayout *layoutPrincipal;
      QHBoxLayout *Hlayout;
        QFormLayout *formLayout;
          QLineEdit *id;
          QLineEdit *mdp;
          QCheckBox *choix_imap;
          QCheckBox *choix_envoi;
          QPushButton *btnConnexion;
          QPushButton *btnQuitter;

  public:
    /**
    * @brief Constructeur avec paramètres
    *
    * Constructeur: défini une nouvelle FenConnexion
    *
    * @return none
    */
    FenConnexion();

    ~FenConnexion();

  public slots:
    /**
    * @brief Procédure pour se connecter
    *
    * Procédure: permet de gerer la connexion
    *
    * @return none
    */
    void seConnecter();

    /**
    * @brief Procédure pour quitter
    *
    * Procédure: permet de gerer la déconnexion
    *
    * @return none
    */
    void quitter() const;
};

#endif // FEN_CONNEXION
