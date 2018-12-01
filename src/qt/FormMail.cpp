#include <QtWidgets>

#include "../../include/qt/FormMail.h"

FormMail::FormMail(const std::string& titre, const std::string& expediteur_mail) {

  QRegExp regex_mail("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");

  group_box = new QGroupBox(QString::fromStdString(titre));
    formulaire = new QFormLayout;
      expediteur = new QLabel("Expediteur");
        champs_expediteur = new QLineEdit;
        if(expediteur_mail != "")
          champs_expediteur->setText(QString::fromStdString(expediteur_mail));
        else
          champs_expediteur->setPlaceholderText("jean.david@exemple.fr");
        champs_expediteur->setReadOnly(true);
        champs_expediteur->setValidator(new QRegExpValidator(regex_mail, this));

      destinataire = new QLabel("Destinataire");
        champs_destinataire = new QLineEdit;
        champs_destinataire->setPlaceholderText("jean.dupont@orange.fr");
        champs_destinataire->setValidator(new QRegExpValidator(regex_mail, this));

      objet = new QLabel("Objet");
        champs_objet = new QLineEdit;
        champs_objet->setPlaceholderText("Important");

      corps = new QLabel("Corps");
        champs_corps = new QTextEdit;
        champs_corps->setPlaceholderText("Exemple de contenu");

    formulaire->addRow(expediteur);
    formulaire->addRow(champs_expediteur);
    formulaire->addRow(destinataire);
    formulaire->addRow(champs_destinataire);
    formulaire->addRow(objet);
    formulaire->addRow(champs_objet);
    formulaire->addRow(corps);
    formulaire->addRow(champs_corps);
  group_box->setLayout(formulaire);
}

std::string FormMail::get_destinataire() const {
  if(champs_destinataire->hasAcceptableInput())
    return champs_destinataire->text().toStdString();
  return "";
}

std::string FormMail::get_objet() const {
  return champs_objet->text().toStdString();
}

std::string FormMail::get_corps() const {
  return champs_corps->toPlainText().toStdString();
}

QGroupBox* FormMail::get_group_box() {
  return group_box;
}

void FormMail::vider_champs() {
  champs_destinataire->clear();
  champs_objet->clear();
  champs_corps->clear();
}

void FormMail::set_expediteur(const std::string& expediteur) {
  champs_expediteur->setText(QString::fromStdString(expediteur));
}

void FormMail::set_destinataire(const std::string& destinataire) {
  champs_destinataire->setText(QString::fromStdString(destinataire));
}

void FormMail::set_objet(const std::string& objet) {
  champs_objet->setText(QString::fromStdString(objet));
}

void FormMail::set_corps(const std::string& corps) {
  champs_corps->setPlainText(QString::fromStdString(corps));
}