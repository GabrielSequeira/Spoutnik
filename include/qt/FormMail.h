#ifndef FORM_MAIL_H
#define FORM_MAIL_H

#include <QtWidgets>
#include "../Mail.h"

/**
* @brief Classe pour la gestion de formulaire de mail
*
* @version 1.0
* @date 2018/03/01
*/
class FormMail : public QWidget {
  Q_OBJECT

public:
  /**
  * @brief Constructeur avec un mail en paramètre
  *
  * @param [in] mail mail a utiliser pour remplir le formulaire
  *
  * @return none
  */
  FormMail(const std::string& titre, const std::string& expediteur = "");

  std::string get_destinataire() const;
  std::string get_objet() const;
  std::string get_corps() const;

  QGroupBox* get_group_box();

  void set_expediteur(const std::string& expediteur);
  void set_destinataire(const std::string& destinataire);
  void set_objet(const std::string& objet);
  void set_corps(const std::string& corps);

public slots:
  /**
  *  @brief Procédure vider
  *
  * Procédure: vide les champs après l'envoi d'un mail
  *
  * @return none
  */
  void vider_champs();

private:
  QGroupBox *group_box;
    QFormLayout *formulaire;
      QLabel *expediteur;
        QLineEdit *champs_expediteur;

      QLabel *destinataire;
        QLineEdit *champs_destinataire;

      QLabel *objet;
        QLineEdit *champs_objet;

      QLabel *corps;
        QTextEdit *champs_corps;
};

#endif // FORM_MAIL_H
