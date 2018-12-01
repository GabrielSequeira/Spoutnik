#include "../include/Dechiffrement.h"
#include "../include/Outils.h"

#include <math.h>
#include <openssl/sha.h>
#include <openssl/aes.h>
#include <iostream>

Dechiffrement::Dechiffrement() {
  c1 = 0;
  c2 = 0;
  sk = 0;
  a = 0;
  alpha = 0;
}

Dechiffrement::Dechiffrement(const mpz_class& skid) {
  c1 = 0;
  c2 = 0;
  sk = skid;
  a = 0;
  alpha = 0;
}

std::string Dechiffrement::dechiffrer_message(const mpz_class& n, const std::string& ID, const std::vector<std::string>& cle_aes_chiffre, const std::string& message_chiffre, const std::string& idec_64) {
  std::string cle_aes = dechiffrer_cle_aes(n, cle_aes_chiffre, ID);
  
  std::string tmp = outils::base64_decode(message_chiffre);
  std::string idec = outils::base64_decode(idec_64);
  
  unsigned char * message = new unsigned char[1024];
  AES_KEY cleAES;
  AES_set_decrypt_key((unsigned char *)cle_aes.c_str(), 128, &cleAES);
  AES_cbc_encrypt((unsigned char *)tmp.c_str(), message, tmp.size(), &cleAES, (unsigned char *)idec.c_str(), AES_DECRYPT);
  std::string str = (char *)message;
  delete[] message;
  return str;
}

std::string Dechiffrement::dechiffrer_cle_aes(const mpz_class& n, const std::vector<std::string>& cle_aes_chiffre, const std::string& ID) {
  std::string mes_dec = "";
  a = outils::calcul_a(n, ID);
  for (unsigned int k = 0; k < cle_aes_chiffre.size(); k++) {
    set_c1(cle_aes_chiffre[k]);
    set_c2(cle_aes_chiffre[k]);

    calcul_alpha(n);
    calcul_m(n);
  }

  tab_ascii = outils::binaire_to_ascii(tab_binaire);
  mes_dec = outils::ascii_to_chaine(tab_ascii);

  return mes_dec;
}

void Dechiffrement::calcul_alpha(const mpz_class& n) {
  mpz_class tmp;
  mpz_pow_ui(tmp.get_mpz_t(), sk.get_mpz_t(), 2);
  mpz_mod(tmp.get_mpz_t(), tmp.get_mpz_t(), n.get_mpz_t());
  mpz_mod(a.get_mpz_t(), a.get_mpz_t(), n.get_mpz_t());
  if(tmp == a) {
    alpha = c1 + 2*sk;
  }
  else {
    alpha = c2 + 2*sk;
  }
}

void Dechiffrement::calcul_m(const mpz_class& n) {
  int bit;
  bit = mpz_jacobi(alpha.get_mpz_t(), n.get_mpz_t());
  tab_binaire.push_back(bit);
}

void Dechiffrement::set_c1(const std::string& nc1) {
  std::string tmp_base64 = std::string(nc1.cbegin() + 1, find(nc1.cbegin(), nc1.cend(), ','));
  std::string tmp_base10 = outils::base64_decode(tmp_base64);

  mpz_set_str(c1.get_mpz_t(), tmp_base10.c_str(), 10);
}

void Dechiffrement::set_c2(const std::string& nc2) {
  std::string tmp_base64 = std::string(find(nc2.cbegin(), nc2.cend(), ',') + 1, find(nc2.cbegin(), nc2.cend(), ')'));
  std::string tmp_base10 = outils::base64_decode(tmp_base64);
  
  mpz_set_str(c2.get_mpz_t(), tmp_base10.c_str(), 10);
}
