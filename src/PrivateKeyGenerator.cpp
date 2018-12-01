#include "../include/PrivateKeyGenerator.h"
#include "../include/rw/BufferedReaderWriter.h"
#include "../include/Outils.h"
#include "../include/Socket.h"

#include <sstream>
#include <openssl/sha.h>
#include <fstream>
#include <thread>
#include <unistd.h>
#include <iostream>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <signal.h>
#include <sys/select.h>

bool doit_sarreter = 0;

void signal_handler(int s) {
  std::cout << s;
  doit_sarreter = 1;
}

PrivateKeyGenerator::PrivateKeyGenerator() {
  lire_key();
  lire_clients("./data/pkg/clients.txt");
}

PrivateKeyGenerator::~PrivateKeyGenerator(){
  ecrire_clients("./data/pkg/clients.txt");
}

void PrivateKeyGenerator::attendre_client() {
  Socket sock;
  sock.s_init();
	sock.s_bind(6666);
  sock.s_listen();

  struct sigaction a;
  a.sa_handler = signal_handler;
  a.sa_flags = 0;
  sigemptyset( &a.sa_mask );
  sigaction(SIGINT, &a, NULL);

  int fd_max = 0; 
  fd_set initial_set; 
  FD_CLR(0, &initial_set);
  int s = sock.get_sock();
  FD_SET(s, &initial_set);
  fd_max = s;

  int res;
  int fd_client;
  while(!doit_sarreter) {
    fd_set temp_set; 
    FD_CLR(0, &temp_set);
    FD_SET(s, &temp_set);
    res = select(fd_max+1, &temp_set, NULL, NULL, NULL); 
    for (; res > 0;) { 
      if (FD_ISSET(s, &temp_set)) { 
        fd_client = sock.s_accept();
        traiter_client(fd_client);
        res--;
      }
    } 
  }
  close(fd_client);
  ecrire_clients("./data/pkg/clients.txt");
  sock.s_close();
}

void PrivateKeyGenerator::traiter_client(int fd_client) {
  BufferedReaderWriter buff(fd_client);
  std::string demande_cli = buff.read_line();
  demande_cli = demande_cli.substr(0, demande_cli.length()-1);
  
  if(demande_cli == "SKID") {
    std::string ID;
    ID = buff.read_line();
    ID = ID.substr(0, ID.length()-1);
    std::cout << " ID: " << ID;

    if(ajouter_client(ID)){
      mpz_class skid = calcul_sk(ID);
      std::string sskid = skid.get_str();
      sskid = sskid + "\n";
      buff.write(sskid);
      std::cout << " DONE\n";
    } else {
      buff.write("ALREADY_MAKE\n");
      std::cout << " ALREADY_MAKE\n";
    }
  } else if(demande_cli == "MPK") {
    std::string mpk = n.get_str() + "\n";
    buff.write(mpk);
  } else {
    buff.write("NO\n");
  }
  
  buff.close();
	close(fd_client);
}

mpz_class PrivateKeyGenerator::calcul_sk(const std::string& ID) const{
	mpz_class a;
	a = outils::calcul_a(n, ID);

  mpz_class skID;
  mpz_class puissance;
  puissance = (n + 5 - p - q) / 8;
  mpz_powm(skID.get_mpz_t(), a.get_mpz_t(), puissance.get_mpz_t(), n.get_mpz_t());

  return skID;
}

bool PrivateKeyGenerator::ajouter_client(const std::string& mail) {
  if (! client_est_present(mail)) {
    tab_cli.push_back(mail);
    return true;
  } 
  return false;
}

void PrivateKeyGenerator::generer_nb_premier(mpz_t rand, mpz_t rand2) const{
  // ############# Initialisation random #############
	gmp_randstate_t state;
  mpz_init (rand);
  gmp_randinit_default (state);
  gmp_randseed_ui (state, time(0));

  // ############# Initialisation pour la vérification n congru 3 mod 4 #############
  mpz_class res_mod4;
  mpz_class quatre = 4;
  mpz_class trois = 3;

  // ############# Génération nombre premier de taille 2^2048  #############
  do {
  	mpz_urandomb(rand, state, 2048);
    mpz_mod(res_mod4.get_mpz_t(), rand, quatre.get_mpz_t());
  } while(mpz_probab_prime_p(rand, 25) == 0
          || mpz_cmp(res_mod4.get_mpz_t(), trois.get_mpz_t()) != 0);

  do {
  	mpz_urandomb(rand2, state, 2048);
    mpz_mod(res_mod4.get_mpz_t(), rand2, quatre.get_mpz_t());
  } while(mpz_probab_prime_p(rand2, 25) == 0
          || mpz_cmp(res_mod4.get_mpz_t(), trois.get_mpz_t()) != 0);

  gmp_randclear(state);
}

void PrivateKeyGenerator::ecrire_key() const{
  std::string mpk_file = "./data/pkg/mpk.txt";
  std::string msk_file = "./data/pkg/msk.txt";

  if(!outils::fichier_existe(mpk_file))
    outils::creer_dossiers("./data/pkg/");

  std::ofstream ecriture_mpk(mpk_file.c_str());
  if(ecriture_mpk.is_open()) {
    ecriture_mpk << outils::base64_encode((unsigned char*)n.get_str().c_str(), n.get_str().length()) << std::endl;
    ecriture_mpk.close();
  }

  std::ofstream ecriture_msk(msk_file.c_str());
  if(ecriture_msk.is_open()) {
    ecriture_msk << outils::base64_encode((unsigned char*)p.get_str().c_str(), p.get_str().length()) << std::endl;
    ecriture_msk << outils::base64_encode((unsigned char*)q.get_str().c_str(), q.get_str().length()) << std::endl;
    ecriture_msk.close();
  }
}

void PrivateKeyGenerator::lire_key() {
  std::string mpk_file = "./data/pkg/mpk.txt";
  std::string msk_file = "./data/pkg/msk.txt";
  std::vector<std::string> vect;

  if(outils::fichier_existe(mpk_file) && outils::fichier_existe(msk_file)) {
    std::ifstream lecture_mpk(mpk_file.c_str());
    std::string n, q, p;
    if(lecture_mpk.is_open()){
      std::getline(lecture_mpk, n);
      vect.push_back(outils::base64_decode(n));
      lecture_mpk.close();
    }

    std::ifstream lecture_msk(msk_file.c_str());
    if(lecture_msk.is_open()){
      std::getline(lecture_msk, q);
      std::getline(lecture_msk, p);

      vect.push_back(outils::base64_decode(q));
      vect.push_back(outils::base64_decode(p));
      lecture_msk.close();
    }
  }

  if(vect.empty() || vect[0] == ""){
    generer_nb_premier(p.get_mpz_t(), q.get_mpz_t());
    n = p * q;
    ecrire_key();
  } else if(vect.size() == 3) {
    mpz_set_str(n.get_mpz_t(), vect[0].c_str(), 10);
    mpz_set_str(p.get_mpz_t(), vect[1].c_str(), 10);
    mpz_set_str(q.get_mpz_t(), vect[2].c_str(), 10);
  } else {
    outils::exit_error("Probléme lors de la lecture de p, q et n", outils::is_debug, 1);
  }
}

void PrivateKeyGenerator::ecrire_clients(const std::string& _nom_fichier) const{
  std::ofstream ecriture (_nom_fichier);
  unsigned int taille = tab_cli.size();
  if(ecriture.is_open()) {
    ecriture << taille << std::endl;
    for(unsigned int i = 0; i < taille; i++) {
      ecriture << "--------------------\n" 
               << tab_cli[i] << "\n";
    }
    ecriture.close();
  }
}

void PrivateKeyGenerator::lire_clients(const std::string& _nom_fichier){
  std::ifstream lecture (_nom_fichier);

  std::string taille;
  std::string separateur;
  if(lecture.is_open()) {
    getline(lecture, taille);
    unsigned int taille_int = atoi(taille.c_str());
    for(unsigned int i = 0; i < taille_int; i++) {
      std::string cli_mail, cli_p, cli_q, cli_n;

      getline(lecture, separateur);
      getline(lecture, cli_mail);

      tab_cli.push_back(cli_mail);
    }
    lecture.close();
  }
}

bool PrivateKeyGenerator::client_est_present(const std::string& mail) const{
  for (unsigned int i = 0; i < tab_cli.size(); i++) {
    if (tab_cli[i] == mail)
      return true;
  }
  return false;
}
