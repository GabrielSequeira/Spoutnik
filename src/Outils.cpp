#include "../include/Outils.h"

#include <openssl/sha.h>
#include <sstream>
#include <iostream>
#include <math.h>
#include <fstream>
#include <stdlib.h>

std::vector<int> outils::chaine_to_ascii(const std::string& message) {
  std::vector<int> tab_ascii;
  for (unsigned int i = 0; i < message.size(); ++i)
    tab_ascii.push_back((int)message[i]);

  return tab_ascii;
}

std::vector<int> outils::ascii_to_binaire(const std::vector<int>& tab_ascii) {
  std::vector<int> tab_binaire;
  for (unsigned int j = 0; j < tab_ascii.size(); ++j) {
    int valeur = tab_ascii[j];
    int val_binaire;
    for (unsigned int i = 0; i < 8; ++i) {
      if (pow(2,7-i) <= valeur) {
        val_binaire = 1;
        valeur -= pow(2,7-i);
      }
      else {
        val_binaire = -1;
      }
      tab_binaire.push_back(val_binaire);
    }
  }
  
  return tab_binaire;
}

std::string outils::ascii_to_chaine(const std::vector<int>& tab_ascii) {
  std::string message;
  for (unsigned int i = 0; i < tab_ascii.size(); ++i)
    message += (char)tab_ascii[i];
  return message;
}

std::vector<int> outils::binaire_to_ascii(const std::vector<int>& tab_binaire) {
  std::vector<int> tab_ascii;
  int val = 0;
  for (unsigned int i = 0; i < tab_binaire.size(); i += 8) {
    for (unsigned int j = i; j < i+8 ; ++j) {
      if(tab_binaire[j] == 1) {
        val += pow(2, 7-(j%8));
      }
    }
    tab_ascii.push_back(val);
    val = 0;
  }
  return tab_ascii;
}

void outils::affiche_ascii(const std::vector<int>& tab_ascii) {
  std::cout << "ASCII: [" ;
  for (unsigned int i = 0; i < tab_ascii.size(); ++i)
    std::cout << tab_ascii[i] << ", ";
  std::cout << "\b\b]\n\n";
}

void outils::affiche_binaire(const std::vector<int> tab_binaire) {
  std::cout << "Binaire: [" ;
  for (unsigned int i = 0; i < tab_binaire.size(); ++i)
    std::cout << tab_binaire[i] << ", ";
  std::cout << "\b\b]\n\n";
}

 mpz_class outils::calcul_a(const mpz_class& n, const std::string& ID) {
   mpz_class a;
   // ############# Initialisation #############
  unsigned char tab_hash_base_64[SHA256_DIGEST_LENGTH]; //Contient les résultats du hachage
  SHA256_CTX ctx;

  // ############# Hachages successifs #############
  std::string to_hash = ID;
  do {
    // on prépare le contexte pour le hash
    SHA256_Init(&ctx);
    SHA256_Update(&ctx, to_hash.c_str(), to_hash.length());
    SHA256_Final(tab_hash_base_64, &ctx);

    // on transforme le base 64 du hash en base 32
    char tab_hash_base_32[SHA256_DIGEST_LENGTH*2+1];
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
      sprintf(&tab_hash_base_32[i*2], "%02x", tab_hash_base_64[i]);

    // on met dans une mpz_class le hash et base 32
    mpz_class hh;
    mpz_set_str(hh.get_mpz_t(), tab_hash_base_32, 16);

    // on teste la condition de bouclage
    if(mpz_jacobi(hh.get_mpz_t(), n.get_mpz_t()) == 1) {
      a = hh;
      break;
    }
    to_hash = tab_hash_base_32;
  } while(1);
  return a;
}

std::string outils::generer_cle_aleatoire_AES() {
  int valeur;
  std::string res = "";
  for (unsigned int i=0; i < 16; ++i) {
    valeur = rand()%128+1;
    res += (char)valeur;
  }

  return res;
}

void outils::error_(const char *file, const int ligne, const std::string &msg, bool cond, int errnum, bool exception) {
  if (cond) {
    std::ostringstream c;
    if (exception) {
      c <<  "\033[31;11mERREUR\033[00m";
    } else {
      c <<  "\033[34;11mWARNING\033[00m";
    }
    c << " fichier " << file << " ligne " <<  ligne << " : ";
    if (errnum != 0) {
      std::system_error err = std::system_error(errno, std::system_category(), msg);
      c << err.what() << std::endl;
      std::cerr << c.str();
      if (exception) {
        throw err;
      }
    } else {
      std::runtime_error err = std::runtime_error(msg);
      c << err.what() << std::endl;
      std::cerr << c.str();
      if (exception) {
        throw err;
      }
    }
  }
}

static const std::string base64_chars = 
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

static inline bool is_base64(unsigned char c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string outils::base64_encode(const unsigned char * bytes_to_encode, unsigned int in_len) {
  std::string ret;
  int i = 0;
  int j = 0;
  unsigned char char_array_3[3];
  unsigned char char_array_4[4];

  while (in_len--) {
    char_array_3[i++] = *(bytes_to_encode++);
    if (i == 3) {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for(i = 0; (i <4) ; i++)
        ret += base64_chars[char_array_4[i]];
      i = 0;
    }
  }

  if (i)
  {
    for(j = i; j < 3; j++)
      char_array_3[j] = '\0';

    char_array_4[0] = ( char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);

    for (j = 0; (j < i + 1); j++)
      ret += base64_chars[char_array_4[j]];

    while((i++ < 3))
      ret += '=';

  }

  return ret;

}

std::string outils::base64_decode(const std::string& chaine) {
  int in_len = chaine.size();
  int i = 0;
  int j = 0;
  int in_ = 0;
  unsigned char char_array_4[4], char_array_3[3];
  std::string ret;

  while (in_len-- && ( chaine[in_] != '=') && is_base64(chaine[in_])) {
    char_array_4[i++] = chaine[in_]; in_++;
    if (i ==4) {
      for (i = 0; i <4; i++)
        char_array_4[i] = base64_chars.find(char_array_4[i]);

      char_array_3[0] = ( char_array_4[0] << 2       ) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) +   char_array_4[3];

      for (i = 0; (i < 3); i++)
        ret += char_array_3[i];
      i = 0;
    }
  }

  if (i) {
    for (j = 0; j < i; j++)
      char_array_4[j] = base64_chars.find(char_array_4[j]);

    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);

    for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
  }

  return ret;
}

std::string outils::hasher(const std::string& to_hash) {
  unsigned char tab_hash_base_64[SHA256_DIGEST_LENGTH]; //Contient les résultats du hachage
  SHA256_CTX ctx;

    // on prépare le contexte pour le hash
    SHA256_Init(&ctx);
    SHA256_Update(&ctx, to_hash.c_str(), to_hash.length());
    SHA256_Final(tab_hash_base_64, &ctx);

    // on transforme le base 64 du hash en base 32
    char tab_hash_base_32[SHA256_DIGEST_LENGTH*2+1];
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
      sprintf(&tab_hash_base_32[i*2], "%02x", tab_hash_base_64[i]);
    
    std::string res(tab_hash_base_32);
    return res;
}

bool outils::fichier_existe(const std::string& _nom_fichier) {
    std::ifstream f(_nom_fichier.c_str());
    return f.good();
}

void outils::creer_dossiers(const std::string& chemin) {
  std::string commande = "mkdir -p " + chemin;
  int dir_err = system(commande.c_str());
  if (-1 == dir_err) {
    printf("Error creating directory!n");
    exit(1);
  }
}

std::string outils::smtp_get_current_date() {
  time_t rawtime;
  struct tm * timeinfo;
  char buffer[80];

  time (&rawtime);
  timeinfo = localtime(&rawtime);

  strftime(buffer,sizeof(buffer),"%a, %d %b %Y %T %z",timeinfo);
  std::string res(buffer);
  
  return res;
}

int outils::nombre_aleatoire(int min, int max){
    return rand() % (max - min) + min;
}

void outils::recuperer_dernier_mail(std::string mail, std::string mdp) {
  system("g++ -lcurl src/curl.cpp -o bin/curl.app");
  std::string cmd = "./bin/curl.app " + mail + " " + mdp + " > data/clients/" + mail + "/mail_recu.txt";
  system(cmd.c_str());

}
