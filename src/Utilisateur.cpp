#include "../include/Utilisateur.h"

#include <fstream>
#include <iostream>

#include "../include/Socket.h"
#include "../include/rw/BufferedReaderWriter.h"
#include "../include/Outils.h"

Utilisateur::Utilisateur(const std::string& _email, const std::string& _mdp, const std::string& _nom, const std::string& _prenom) {
  email = _email;
  mdp = _mdp;
  nom = _nom;
  prenom = _prenom;
  tab_mails = lire_mails();
  recuperer_skid();
  recuperer_mpk();
}

void Utilisateur::ajouter_mail(const Mail &_mail) {
  tab_mails.push_back(_mail);
}

void Utilisateur::enregistrer_tab_mails() const{
  Mail::enregistrer_mails(tab_mails, "./data/clients/" + email + "/mails.txt");
}

std::vector<Utilisateur> Utilisateur::lire_utilisateurs(const std::string& nom_fichier) {
  std::vector<Utilisateur> tab_utilisateurs;

  std::string taille;
  if(outils::fichier_existe(nom_fichier)) {
    std::ifstream lecture (nom_fichier);
    if(lecture.is_open()) {
      getline(lecture, taille);
      unsigned int taille_int = std::stoi(taille.c_str());
      for(unsigned int i = 0; i < taille_int; i++){
        std::string adresse_mail, mdp, nom, prenom;
        getline(lecture, adresse_mail);
        getline(lecture, mdp);
        getline(lecture, nom);
        getline(lecture, prenom);
        
        Utilisateur usr(adresse_mail, mdp, nom, prenom);
        tab_utilisateurs.push_back(usr);
      }
      lecture.close();
    } else {
      std::cout << "Impossible de lire les données (utilisateurs) !" << std::endl;
      exit(1);
    }
  } else {
    outils::creer_dossiers("./data/");
    Utilisateur i("inconnu@inconnu.fr", outils::hasher("inconnu"), "inconnu", "inconnu");
    tab_utilisateurs.push_back(i);
    Utilisateur::enregistrer_utilisateurs(nom_fichier, tab_utilisateurs);
  }
  return tab_utilisateurs;
}

Utilisateur* Utilisateur::get_utilisateur_by_email(std::vector<Utilisateur> *tab_utilisateurs, const std::string& email) {
  for(unsigned int i = 0; i < tab_utilisateurs->size(); i++){
    if(tab_utilisateurs->at(i).get_email() == email)
      return &tab_utilisateurs->at(i);
  }
  return &tab_utilisateurs->at(0);
}

std::string Utilisateur::get_email() const{
  return email;
}

std::string Utilisateur::get_mdp() const{
  return mdp;
}

std::string Utilisateur::get_nom() const{
  return nom;
}

std::string Utilisateur::get_prenom() const{
  return prenom;
}

std::string Utilisateur::get_skid() const{
  return skid;
}

std::string Utilisateur::get_mpk() const{
  return mpk;
}

std::vector<Mail> Utilisateur::get_tab_mails() const{
  return tab_mails;
}

void Utilisateur::recuperer_skid() {
  std::string nom_fichier_skid = "./data/clients/" + email + "/skid.txt";
  
  if(outils::fichier_existe(nom_fichier_skid)) {
    std::ifstream lecture (nom_fichier_skid);

    if(lecture.is_open()){
      getline(lecture, skid);
      lecture.close();
    } else {
      std::cout << "Impossible de lire skid !" << std::endl;
      exit(1);
    }
  } else {
    Socket sock;
    sock.s_init();
    sock.s_connect("127.0.0.1", 6666);
    BufferedReaderWriter buff(sock.get_sock());
    
    buff.write("SKID\n");
    buff.write(email + "\n");
    skid = buff.read_line();

    buff.close();

    outils::creer_dossiers("./data/clients/" + email);

    std::ofstream nv_fichier_skid;
    nv_fichier_skid.open ("./data/clients/" + email + "/skid.txt");
    nv_fichier_skid << skid;
    nv_fichier_skid.close();
  }
}

void Utilisateur::recuperer_mpk() {
  std::string nom_fichier_mpk = "./data/clients/" + email + "/mpk.txt";
  
  if(outils::fichier_existe(nom_fichier_mpk)) {
    std::ifstream lecture (nom_fichier_mpk);

    if(lecture.is_open()){
      getline(lecture, mpk);
      lecture.close();
    } else {
      std::cout << "Impossible de lire mpk !" << std::endl;
      exit(1);
    }
  } else {
    Socket sock;
    sock.s_init();

    sock.s_connect("127.0.0.1", 6666);
    BufferedReaderWriter buff(sock.get_sock());

    buff.write("MPK\n");
    mpk = buff.read_line();

    buff.close();

    outils::creer_dossiers("./data/clients/" + email);

    std::ofstream nv_fichier_mpk;
    nv_fichier_mpk.open ("./data/clients/" + email + "/mpk.txt");
    nv_fichier_mpk << mpk;
    nv_fichier_mpk.close();
  }
}

void Utilisateur::enregistrer_utilisateurs(const std::string& nom_fichier, const std::vector<Utilisateur>& tab_utilisateurs) {
  std::ofstream ecriture(nom_fichier);

  unsigned int taille = tab_utilisateurs.size();
  if(ecriture.is_open()){
    ecriture << taille << std::endl;
    for(auto usr: tab_utilisateurs) {
      ecriture << usr.get_email() << std::endl;
      ecriture << usr.get_mdp() << std::endl;
      ecriture << usr.get_nom() << std::endl;
      ecriture << usr.get_prenom() << std::endl;
    }
    ecriture.close();
  }else{
    std::cout << "Impossible d'écrire les données !" << std::endl;
  }
}

std::vector<Mail> Utilisateur::lire_mails() {
  std::string nom_fichier = "./data/clients/" + email + "/mails.txt";
  if(!outils::fichier_existe(nom_fichier)) {
    outils::creer_dossiers("./data/clients/" + email);
    std::ofstream nv_fichier_mails;
    nv_fichier_mails.open (nom_fichier);
    nv_fichier_mails << "0";
    nv_fichier_mails.close();
  }
  return Mail::lire_mails(nom_fichier);
}

void Utilisateur::creer_utilisateurs(const std::string& _chemin_fichier) {
  Utilisateur i("inconnu@inconnu.fr", outils::hasher("inconnu"), "inconnu", "inconnu");
  Utilisateur t("test@test.fr", outils::hasher("mdp"), "test", "test");
  Utilisateur m("mourad.allam07@gmail.com", outils::hasher("mourad"), "Allam", "Mourad");
  Utilisateur s("spoutnik2018@sfr.fr", outils::hasher("spoutnik"), "Spoutnik", "Jean");

  std::vector<Utilisateur> tab_u;
  tab_u.push_back(i);
  tab_u.push_back(t);
  tab_u.push_back(m);
  tab_u.push_back(s);

  Utilisateur::enregistrer_utilisateurs(_chemin_fichier, tab_u);
}