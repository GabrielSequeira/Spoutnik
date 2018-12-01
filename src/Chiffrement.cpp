#include "../include/Chiffrement.h"
#include "../include/Outils.h"

#include <iostream>
#include <math.h>
#include <string>
#include <cassert>
#include <openssl/sha.h>
#include <openssl/aes.h>
#include <cstdlib>
#include <fstream>

Chiffrement::Chiffrement() {
  t1 = 0;
  t2 = 0;
  a = 0;
  c1 = 0;
  c2 = 0;

  initialiser_temps();
}

Chiffrement::~Chiffrement() {
  gmp_randclear(etat); // on libert le temps
}

std::string Chiffrement::chiffrer_message(const mpz_class& n, const std::string& ID, const std::string& message, std::string& ivec, std::vector<std::string>& cle_aes_chiffre) {
  std::string cle = outils::generer_cle_aleatoire_AES();
  ivec = outils::generer_cle_aleatoire_AES();
  std::string i_vec = ivec;
  unsigned long out_size = ((message.size()/AES_BLOCK_SIZE)+1)*AES_BLOCK_SIZE;
  unsigned char * message_chiffre = new unsigned char[1024];
  AES_KEY cleAES;
  AES_set_encrypt_key((unsigned char *)cle.c_str(), 128, &cleAES);
  AES_cbc_encrypt((unsigned char *)message.c_str(),message_chiffre, out_size ,&cleAES, (unsigned char *)i_vec.c_str() ,AES_ENCRYPT);
  std::string res = (char *)message_chiffre;
  std::string cleez = cle;
  
  delete[] message_chiffre;

  cle_aes_chiffre = chiffrer_cle_aes(n, ID, cle);

  std::string cle_base_64 = outils::base64_encode((unsigned char *)res.c_str(), res.size());
  return cle_base_64;
}

std::vector<std::string> Chiffrement::chiffrer_cle_aes(const mpz_class& n, const std::string& ID, const std::string& cle) {
  std::vector<std::string> cle_aes_chiffre;
  std::string tmp = "";
  
  a = outils::calcul_a(n, ID);

  tab_chaine_ascii = outils::chaine_to_ascii(cle);
  tab_chaine_binaire = outils::ascii_to_binaire(tab_chaine_ascii);

  for (unsigned int i = 0; i < tab_chaine_binaire.size(); ++i) {
    calcul_jacobi(n, i);
    std::string c1_tmp;
    std::string c2_tmp;
    c1_tmp = calcul_c1(n);
    c2_tmp = calcul_c2(n);

    cle_aes_chiffre.push_back("(" + c1_tmp + "," + c2_tmp + ")");
  }
  return cle_aes_chiffre;
}

void Chiffrement::calcul_jacobi(const mpz_class& n, const int indice_bit) {
  int m = tab_chaine_binaire[indice_bit];
  mpz_class r1;
  mpz_class r2;

  do {
    mpz_urandomm(r1.get_mpz_t(), etat,n.get_mpz_t());
    if (m == mpz_jacobi(r1.get_mpz_t(), n.get_mpz_t())) {
      t1 = r1;
      break;
    }
  } while (1);

  do {
    mpz_urandomm(r2.get_mpz_t(),etat,n.get_mpz_t());
    if (r1 != r2 && m == mpz_jacobi(r2.get_mpz_t(), n.get_mpz_t())) {
      t2 = r2;
      break;
    }
  } while (1);
}

std::string Chiffrement::calcul_c1(const mpz_class& n) {
  mpz_class t1_invert;
  mpz_invert(t1_invert.get_mpz_t(), t1.get_mpz_t(), n.get_mpz_t());

  c1 = t1 + a*t1_invert;
  mpz_mod(c1.get_mpz_t(), c1.get_mpz_t(), n.get_mpz_t());

  char * temp = mpz_get_str(nullptr, 10, c1.get_mpz_t());
  std::string str = std::string(temp);
  return outils::base64_encode((unsigned char*)str.c_str(), str.length());
}

std::string Chiffrement::calcul_c2(const mpz_class& n) {
  mpz_class t2_invert;
  mpz_invert(t2_invert.get_mpz_t(), t2.get_mpz_t(), n.get_mpz_t());

  c2 = t2 - a*t2_invert;
  mpz_mod(c2.get_mpz_t(), c2.get_mpz_t(), n.get_mpz_t());

  char * temp = mpz_get_str(nullptr,10, c2.get_mpz_t());
  std::string str = std::string(temp);
  return outils::base64_encode((const unsigned char*)str.c_str(), str.length());
}

void Chiffrement::initialiser_temps() {
  gmp_randinit_default(etat); // initialisation du random
  gmp_randseed_ui(etat, time(nullptr)); // remise à zéro à chaque lancement du programme
  srand(time(nullptr));
}
