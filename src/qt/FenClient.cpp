#include <QtWidgets>
#include <unistd.h>
#include <sys/wait.h>

#include <iostream>
#include "../../include/Mail.h"
#include "../../include/Chiffrement.h"
#include "../../include/Dechiffrement.h"
#include "../../include/Outils.h"
#include "../../include/PrivateKeyGenerator.h"
#include "../../include/qt/FenClient.h"

FenClient::FenClient(Utilisateur *_utilisateur, bool bool_imap, bool bool_envoi){
	utilisateur = _utilisateur;
	envoi_mail = bool_envoi;
  setMinimumSize(800, 600);
	std::string titre = "Spoutnik Mail | " + _utilisateur->get_prenom() + " " + _utilisateur->get_nom();
  setWindowTitle(QString::fromStdString(titre));
	
  layout_principal = new QVBoxLayout();
    onglets = new QTabWidget();

			page1 = new QWidget;
				/* -------- Onglet 1 -------- */
		    layout_onglet_1 = new QVBoxLayout;
		      form_mail_1 = new FormMail("Envoyer mail", utilisateur->get_email());

		    foot_layout = new QHBoxLayout();
		      btn_envoyer = new QPushButton("Envoyer");
					btn_quitter = new QPushButton("Quitter");

		    foot_layout->addWidget(btn_envoyer);
				foot_layout->addWidget(btn_quitter);

		    layout_onglet_1->addWidget(form_mail_1->get_group_box());
		    layout_onglet_1->addLayout(foot_layout);
			page1->setLayout(layout_onglet_1);

			page2 = new QWidget;
				/* -------- Onglet 2 -------- */  
		    layout_onglet_2 = new QHBoxLayout;
					layout_liste_mail = new QVBoxLayout;
		        liste = new QListWidget;
						
						for(const Mail& m: utilisateur->get_tab_mails()){
							QListWidgetItem *item = new QListWidgetItem();
							item->setData(1, QString::fromStdString(m.get_expediteur()));
							item->setData(2, QString::fromStdString(m.get_destinataire()));
							item->setData(3, QString::fromStdString(m.get_objet()));
							item->setData(4, QString::fromStdString(m.get_contenu_chiffre()));
							item->setData(5, QString::number(m.get_id()));
							liste->addItem(item);
            }

					layout_liste_mail->addWidget(liste);

					layout_contenu = new QVBoxLayout;
						form_mail_2 = new FormMail("Messages reçus", utilisateur->get_email());
						
						foot_layout2 = new QHBoxLayout();
				    btn_dechiffrer = new QPushButton("Dechiffrer");

		      foot_layout2->addWidget(btn_dechiffrer);
					layout_contenu->addWidget(form_mail_2->get_group_box());
					layout_contenu->addLayout(foot_layout2);
				layout_onglet_2->addLayout(layout_liste_mail);
				layout_onglet_2->addLayout(layout_contenu);
			page2->setLayout(layout_onglet_2);

			if(bool_imap){
			page3 = new QWidget;
				/* -------- Onglet 3 -------- */
		    layout_onglet_3 = new QHBoxLayout;
					layout_contenu3 = new QVBoxLayout;
						form_mail_3 = new FormMail("Messages reçus");

						Mail mail_recu = Mail::lire_mail_imap("./data/clients/" + utilisateur->get_email() +"/mail_recu.txt");
						remplir3(mail_recu);
						
						foot_layout3 = new QHBoxLayout();
				    btn_dechiffrer3 = new QPushButton("Dechiffrer");

		      foot_layout3->addWidget(btn_dechiffrer3);
					layout_contenu3->addWidget(form_mail_3->get_group_box());
					layout_contenu3->addLayout(foot_layout3);
				layout_onglet_3->addLayout(layout_contenu3);
			page3->setLayout(layout_onglet_3);
			}
		onglets->addTab(page1, "Envoyer mail");
		onglets->addTab(page2, "Consulter mails envoyés");
		if(bool_imap){
		onglets->addTab(page3, "Consulter dernier mail reçu");
		}
	connect(btn_envoyer, SIGNAL(clicked()), this, SLOT(envoyer_mail()));
	connect(btn_quitter, SIGNAL(clicked()), this, SLOT(quitter()));
	connect(btn_dechiffrer, SIGNAL(clicked()), this, SLOT(dechiffrer()));
	if(bool_imap){
		connect(btn_dechiffrer3, SIGNAL(clicked()), this, SLOT(dechiffrer3()));
	}
	
	connect(liste, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(remplir(QListWidgetItem*)));
	layout_principal->addWidget(onglets);
	setLayout(layout_principal);
}

void FenClient::envoyer_mail(){
  if(form_mail_1->get_destinataire() != "") {
    std::string expediteur = utilisateur->get_email();
    std::string destinataire = form_mail_1->get_destinataire();
    std::string objet = form_mail_1->get_objet();
    std::string contenu = form_mail_1->get_corps();

		mpz_class n;
		mpz_set_str(n.get_mpz_t(), utilisateur->get_mpk().c_str(), 10);
		int id = outils::nombre_aleatoire(0, 10000);
		Mail m(id, utilisateur->get_email(), destinataire, objet, contenu, n);
		utilisateur->ajouter_mail(m);
		if(envoi_mail) {
			m.envoyer_mail();
		}

		QMessageBox::information(this, "Information", "Mail envoyé avec succès !");

		utilisateur->enregistrer_tab_mails();
		form_mail_1->vider_champs();
  } else {
    QMessageBox::information(this, "Problème", "Merci de remplir tous les champs");
  }
}

void FenClient::remplir(QListWidgetItem* item) const{
	form_mail_2->set_destinataire(item->data(2).toString().toStdString());
	form_mail_2->set_objet(item->data(3).toString().toStdString());
	form_mail_2->set_corps(item->data(4).toString().toStdString());
}

void FenClient::remplir3(Mail m) const{
	form_mail_3->set_expediteur(m.get_expediteur());
	form_mail_3->set_destinataire(m.get_destinataire());
	form_mail_3->set_objet(m.get_objet());
	form_mail_3->set_corps(m.get_contenu_chiffre());
}

void FenClient::dechiffrer() const {
	int id_mail = liste->currentItem()->data(5).toInt();
	
	std::vector<Mail> tab_m = utilisateur->get_tab_mails();
	Mail* mail = Mail::get_mail_by_id(tab_m, id_mail);
	
	if(utilisateur->get_email() == mail->get_destinataire()) {
		mpz_class n(utilisateur->get_mpk());
		mpz_class skid(utilisateur->get_skid());

		std::string contenu_dechiffre = mail->get_contenu_dechiffre(n, skid);
		form_mail_2->set_corps(contenu_dechiffre);
	} else {
		QMessageBox::information(form_mail_2, "oulala", "Vous ne pouvez pas déchiffrer un mail qui ne vous est pas destiné");
	}
}

void FenClient::dechiffrer3() const {
	
	Mail mail_recu = Mail::lire_mail_imap("./data/clients/" + utilisateur->get_email() +"/mail_recu.txt");
	mpz_class n(utilisateur->get_mpk());
	mpz_class skid(utilisateur->get_skid());

	std::string contenu_dechiffre = mail_recu.get_contenu_dechiffre(n, skid);
	form_mail_3->set_corps(contenu_dechiffre);
}

void FenClient::quitter() const {
	QApplication::quit();
}