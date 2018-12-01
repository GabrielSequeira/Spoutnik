#include "../include/Mail.h"

#include "../include/Outils.h"
#include "../include/Socket.h"
#include "../include/rw/BufferedReaderWriter.h"
#include "../include/Chiffrement.h"
#include "../include/Dechiffrement.h"
#include "../include/Utilisateur.h"

#include <ctime>
#include <chrono>
#include <iostream>
#include <regex>
#include <stdio.h>
#include <fstream>
#include <array>

Mail::Mail() {
  
}

Mail::Mail(const int _id, const std::string& _expe, const std::string& _desti, const std::string& _date, const std::string& _obj, const std::vector<std::string>& _cle_aes_chiffre, const std::string& _idec, const std::string& _contenu_chiffre) {
  id = _id;
  expediteur = _expe;
  destinataire = _desti;
  date = _date;
  objet = _obj;
  cle_aes_chiffre = _cle_aes_chiffre;
  idec = _idec;
  contenu_chiffre = _contenu_chiffre;
}

Mail::Mail(const int _id, const std::string& _expe, const std::string& _desti, const std::string& _obj, const std::string& _contenu, const mpz_class& n) {
  id = _id;
  expediteur = _expe;
  destinataire = _desti;
  date = outils::smtp_get_current_date();
  objet = _obj;

  Chiffrement chiff;
  std::string ivec;
  contenu_chiffre = chiff.chiffrer_message(n, _desti, _contenu, ivec, cle_aes_chiffre);
  idec = outils::base64_encode((const unsigned char *)ivec.c_str(), ivec.length());
}

void Mail::envoyer_mail() const{
  std::string domaine_desti;
  std::string domaine_smtp;
  domaine_desti = Mail::get_domaine_email(destinataire);
  domaine_smtp = Mail::get_adresse_serveur_SMTP(domaine_desti);
  std::cout << "domaine desti: " << domaine_desti << std::endl;
  std::cout << "domaine smtp: " << domaine_desti << std::endl;

  Socket sock;
  sock.s_init();
  sock.s_connect(domaine_smtp, 25);
  
  BufferedReaderWriter buff(sock.get_sock());

  std::cout << "> " << "HELO spoutnik.fr \r\n";
  buff.write("HELO spoutnik.fr \r\n");
  std::cout << "< " << buff.read_line();

  std::cout << "> " << "MAIL FROM:<" + expediteur + ">\r\n";
  buff.write("MAIL FROM:<" + expediteur + ">\r\n");
  std::cout << "< " << buff.read_line();

  std::cout << "> " << "RCPT TO:<" + destinataire + ">\r\n";
  buff.write("RCPT TO:<" + destinataire + ">\r\n");
  std::cout << "< " << buff.read_line();

  std::cout << "> " << "DATA\r\n";
  buff.write("DATA\r\n");
  std::cout << "< " << buff.read_line();

  //std::cout << "" << smtp_get_contenu();
  buff.write(smtp_get_contenu());
  std::cout << "< " << buff.read_line();

  std::cout << "> " << "QUIT\r\n";
  buff.write("QUIT\r\n");
  std::cout << "< " << buff.read_line();
  std::cout << "< " << buff.read_line();
}

void Mail::test_regression() {
    std::vector<Utilisateur> tab_u;
    tab_u = Utilisateur::lire_utilisateurs("./data/utilisateurs.txt");
    Utilisateur expediteur = tab_u[1];
    Utilisateur destinataire = tab_u[2];

    mpz_class n(expediteur.get_mpk());
    mpz_class skid(destinataire.get_skid());
    
    Mail m(10, "test@test.fr", "mourad@spoutnik.fr", "obj", "test", n);
    
    std::cout << "\ncontenu chiffré: " << m.get_contenu_chiffre() << std::endl;
    std::cout << "contenu dechiffré: " << m.get_contenu_dechiffre(n, skid) << std::endl;
    std::cout << "--------------------------------------------" << std::endl;
    
    std::vector<Mail> tab_m;
    tab_m.push_back(m);
    Mail::enregistrer_mails(tab_m, "./data/clients/test@test.fr/mails.txt");
    tab_m = Mail::lire_mails("./data/clients/test@test.fr/mails.txt");

    Mail m1 = tab_m[0];
    std::cout << "\ncontenu chiffre: " << m1.get_contenu_chiffre() << std::endl;
    std::cout << "contenu dechiffre: " << m1.get_contenu_dechiffre(n, skid) << std::endl;
}

int Mail::get_id() const{
  return id;
}

std::string Mail::get_expediteur() const{
	return expediteur;
}

std::string Mail::get_destinataire() const{
	return destinataire;
}

std::string Mail::get_date() const{
	return date;
}

std::string Mail::get_objet() const{
	return objet;
}

std::vector<std::string> Mail::get_cle_aes_chiffre() const{
  return cle_aes_chiffre;
}

std::string Mail::get_idec() const{
  return idec;
}

std::string Mail::get_contenu_chiffre() const{
	return contenu_chiffre;
}

std::string Mail::get_contenu_dechiffre(const mpz_class& n, const mpz_class& skid) const {
  Dechiffrement d(skid);

  return d.dechiffrer_message(n, destinataire, cle_aes_chiffre, contenu_chiffre, idec);
}

void Mail::enregistrer_mails(const std::vector<Mail>& _tab_mails, const std::string& _nom_fichier) {
  std::ofstream ecriture(_nom_fichier);

  if(ecriture.is_open()){
    ecriture << _tab_mails.size() << std::endl;
    for(unsigned int i = 0; i < _tab_mails.size(); i++) {
      ecriture << _tab_mails[i].get_id() << std::endl;
      ecriture << _tab_mails[i].get_expediteur() << std::endl;
      ecriture << _tab_mails[i].get_destinataire() << std::endl;
      ecriture << _tab_mails[i].get_date() << std::endl;
      ecriture << _tab_mails[i].get_objet() << std::endl;
      ecriture << _tab_mails[i].get_cle_aes_chiffre().size() << std::endl;
      for(unsigned int j = 0; j < _tab_mails[i].get_cle_aes_chiffre().size(); j++)
        ecriture << _tab_mails[i].get_cle_aes_chiffre()[j] << std::endl;
        
      ecriture << _tab_mails[i].get_idec() << std::endl;

      ecriture << "--Corps--" << std::endl;
      ecriture << _tab_mails[i].get_contenu_chiffre() << std::endl;
      ecriture  << "--Corps--" << std::endl;
    }
    ecriture.close();
  } else {
    std::cout << "Impossible d'écrire les données !" << std::endl;
  }
}

Mail* Mail::get_mail_by_id(std::vector<Mail>& tab_mails, const int id) {
  for(unsigned int i = 0; i < tab_mails.size(); i++)
    if(tab_mails[i].get_id() == id) {
      return &tab_mails[i];
		}
  return nullptr;
}

std::vector<Mail> Mail::lire_mails(const std::string& _nom_fichier){
  std::ifstream lecture (_nom_fichier);
  std::regex reg_corps("--Corps--");

  std::string taille;
  std::string taille_tab_cle_aes_chiffre;
  if(lecture.is_open()){
    getline(lecture, taille);
    unsigned int taille_int = std::stoi(taille.c_str());
    std::vector<Mail> tab_mail;
    for(unsigned int i = 0; i < taille_int; i++){
      std::string id, expediteur, destinataire, date, objet, cle_aes_chiffre, idec, corps, delemiteur, ligne;
        getline(lecture, id);
        getline(lecture, expediteur);
        getline(lecture, destinataire);
        getline(lecture, date);
        getline(lecture, objet);
        getline(lecture, taille_tab_cle_aes_chiffre);
        unsigned int taille_tab_cle_aes_chiffre_int = std::stoi(taille_tab_cle_aes_chiffre.c_str());
        std::vector<std::string> cle_aes_chiffre_lue;
        for(unsigned int j = 0; j < taille_tab_cle_aes_chiffre_int; j++) {
          getline(lecture, cle_aes_chiffre);
          cle_aes_chiffre_lue.push_back(cle_aes_chiffre);
        }
        getline(lecture, idec);
        getline(lecture, delemiteur);
        getline(lecture, ligne);
        while(!std::regex_match(ligne, reg_corps)){
          corps += ligne;
          getline(lecture, ligne);
        }
        int id_int = stoi(id);
        corps = corps.substr(0, corps.length() - 1);
        
        Mail m(id_int, expediteur, destinataire, date, objet, cle_aes_chiffre_lue, idec,  corps);
        tab_mail.push_back(m);
    }
    lecture.close();
    return tab_mail;
  } else {
    std::cout << "Impossible de lire les données (mails) !" << std::endl;
    exit(1);
  }
}

std::string Mail::get_domaine_email(const std::string& _adresse) {
  std::string res = "";
  int i = 0;
  while(_adresse[i] != '@'){
    i++;
  }
  for(unsigned j = i + 1; j < _adresse.length(); j++){
    res += _adresse[j];
  }
  return res;
}

std::string Mail::get_adresse_serveur_SMTP(const std::string& _domaine) {
  std::string chaine = "dig +short MX " + _domaine;
  const char * cmd = chaine.c_str();
  std::array<char, 128> buffer;
  std::string resultat;
  std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
  if (!pipe) throw std::runtime_error("popen() raté!");
  while (!feof(pipe.get())) {
    if (fgets(buffer.data(), 128, pipe.get()) != NULL)
      resultat += buffer.data();
  }

  std::string serveur_final = "";
  for(unsigned int j = 4; j < resultat.length(); j++){
    if(resultat[j] == '\r' || resultat[j] == '\n'){
      break;
    }else{
      serveur_final += "a";
      serveur_final[j-4] = resultat[j-1];
    }
  }
  return serveur_final;
}

std::string Mail::smtp_get_contenu() const{
  std::string separateur = "----09D7C080709C63631";
  std::string res = "";
  res += "From: " + expediteur + "\r\n";
  res += "To: " + destinataire + "\r\n";
  res += "Date: " + date + "\r\n";
  res += "Subject: " + objet + "\r\n";
  res += std::to_string(get_id()) + "\r\n";
  res += std::to_string(get_cle_aes_chiffre().size()) + "\r\n";
  for(unsigned int i = 0; i < get_cle_aes_chiffre().size(); i++)
    res += get_cle_aes_chiffre()[i] + "\r\n";
  res += get_idec() + "\r\n";
  res += "Content-Type: multipart/mixed; boundary=\"" + separateur + "\"\r\n";
  res += "MIME-Version: 1.0\r\n";
  res += "\r\n";
  res += separateur + "\r\n";
  res += "Content-Type: text/plain; charset=\"utf-8\"\r\nContent-Transfer-Encoding: 8bit\r\n";
  res += "\r\n" + contenu_chiffre + "\r\n";
  res += "\r\n";
  res += "\r\n";
  res += separateur + "--\r\n";
  res += ".\r\n";

  return res;
}


Mail Mail::lire_mail_imap(const std::string& _nom_fichier){
  std::ifstream lecture (_nom_fichier);

  if(lecture.is_open()){
    std::string expediteur, _expediteur, destinataire, date, objet, delimiteur, ligne, corps, id, taille_tab_cle_aes_chiffre, cle_aes_chiffre, idec, p;
    std::vector<std::string> aes;
    do{
      getline(lecture, expediteur);
      _expediteur=expediteur.substr(0, 4);
    }while(_expediteur != "From");
    expediteur = expediteur.substr(6,expediteur.length()-1);
    
    getline(lecture, destinataire);
    destinataire = destinataire.substr(4,destinataire.length()-1);

    getline(lecture, date);
    date = date.substr(6,date.length()-1);

    getline(lecture, objet);
    objet = objet.substr(9,objet.length()-1);

    do{
      getline(lecture, delimiteur);
    }while(delimiteur != "\r");

    getline(lecture, id);
    int id_int = stoi(id);

    getline(lecture, taille_tab_cle_aes_chiffre);
    unsigned int taille_tab_cle_aes_chiffre_int = std::stoi(taille_tab_cle_aes_chiffre);
    std::vector<std::string> cle_aes_chiffre_lue;

    for(unsigned int j = 0; j < taille_tab_cle_aes_chiffre_int; j++) {
      cle_aes_chiffre="";
      getline(lecture, p);
      cle_aes_chiffre += p.substr(0,p.length()-1);
      std::regex e ("[)].*", std::regex_constants::basic);
      while(std::regex_match (p,e) != true){
        getline(lecture, p);
        cle_aes_chiffre += p.substr(1,p.length()-2);
        p=p.substr(p.length()-2, p.length()-2);
      }
      cle_aes_chiffre_lue.push_back(cle_aes_chiffre);
    }

    getline(lecture, idec);
    idec = idec.substr(0,idec.length()-1);
    do{
      getline(lecture, delimiteur);
      delimiteur=delimiteur.substr(0, 16);
    }while(delimiteur != "Content-Transfer");

    getline(lecture, delimiteur);
    getline(lecture, ligne);
    corps = ligne.substr(0,ligne.length()-1);
    std::regex corpss ("(----09D7C080709C63631--)(.*)", std::regex_constants::basic);
      while(std::regex_match (ligne,corpss) != true){
        getline(lecture, ligne);
        if(ligne=="")break;
        if(ligne =="----09D7C080709C63631--\r")break;
        corps += ligne.substr(1,ligne.length()-1);
      }
      corps = corps.substr(0,corps.length()-1);

    Mail m(id_int, expediteur, destinataire, date, objet, cle_aes_chiffre_lue, idec,  corps);
    lecture.close();
    return m;
  }
  else {
    std::cout << "Impossible de lire le mail !" << std::endl;
    exit(1);
  }
}