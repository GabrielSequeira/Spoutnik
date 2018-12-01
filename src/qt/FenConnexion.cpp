#include <vector>
#include <iostream>

#include "./../../include/qt/FenConnexion.h"
#include "./../../include/Outils.h"
#include "./../../include/Utilisateur.h"

FenConnexion::FenConnexion() : QWidget(){

  setWindowTitle("Connexion | Spoutnik");
  id = new QLineEdit;
  mdp = new QLineEdit;
  choix_imap = new QCheckBox;
  choix_envoi = new QCheckBox;
    mdp->setEchoMode(QLineEdit::Password);

  btnConnexion = new QPushButton("Connexion");
  btnQuitter = new QPushButton("Quitter");

  formLayout = new QFormLayout;
    formLayout->addRow("Adresse e-mail: ", id);
    formLayout->addRow("Mot de passe: ", mdp);
    formLayout->addRow("Recuperer dernier email", choix_imap);
    formLayout->addRow("Envoi en ligne", choix_envoi);

  Hlayout = new QHBoxLayout;
    Hlayout->addWidget(btnQuitter);
    Hlayout->addWidget(btnConnexion);

  layoutPrincipal = new QVBoxLayout;
    layoutPrincipal->addLayout(formLayout);
    layoutPrincipal->addLayout(Hlayout);

  setLayout(layoutPrincipal);

  connect(btnConnexion, SIGNAL(clicked()), this, SLOT(seConnecter()));
  connect(id, SIGNAL(returnPressed()), mdp, SLOT(setFocus()));
  connect(mdp, SIGNAL(returnPressed()), this, SLOT(seConnecter()));
  connect(btnQuitter, SIGNAL(clicked()), this, SLOT(quitter()));

  std::string chemin =  QDir::currentPath().toStdString();
  std::cout << "Début chargement données !\n";

  /* ------------------------------------------------------- */
  // Utilisateur::creer_utilisateurs();
  tab_utilisateurs = Utilisateur::lire_utilisateurs(chemin + "/data/utilisateurs.txt");
  /* ------------------------------------------------------- */

  std::cout << "Fin chargement données !\n";

}

FenConnexion::~FenConnexion(){
  
}

void FenConnexion::seConnecter(){

  std::string mail_saisi = id->text().toStdString();
  std::string mdp_saisie = mdp->text().toStdString();

  Utilisateur* utilisateur = Utilisateur::get_utilisateur_by_email(&tab_utilisateurs, mail_saisi);
  QString message_utilisateur;

  // INCONNU
  if(utilisateur->get_email() == "inconnu@inconnu.fr"){
      message_utilisateur = "Désole, email inconnu au bataillon !";
      QMessageBox::information(this, "Titre", message_utilisateur);
  } else {
    // UTILISATEUR
    if(utilisateur->get_mdp() == outils::hasher(mdp_saisie)) {
      if(choix_imap->isChecked())
        outils::recuperer_dernier_mail(utilisateur->get_email(), mdp_saisie);
      FenClient *fenCli = new FenClient(utilisateur, choix_imap->isChecked(), choix_envoi->isChecked());
      fenCli->show();
      this->close();
    } else {
      message_utilisateur = "Mot de passe incorrecte " + QString::fromStdString(utilisateur->get_prenom()) + " !";
      QMessageBox::information(this, "Titre", message_utilisateur);
    }
  }
}

void FenConnexion::quitter() const{
  QApplication::quit();
}
