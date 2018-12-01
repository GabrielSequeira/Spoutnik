#include "../../include/rw/ReaderWriter.h"
#include <sstream>
#include <iostream>
#include <cerrno>
#include <fcntl.h>

#include "../../include/Outils.h"


namespace codeC {
  // je suis obligé de metre un namespace pour eviter la confusion entre
  // la méthode close() et la fonction close la libc
  #include <unistd.h>
}

ReaderWriter::ReaderWriter(int fd) : fd(fd) {
}

void
ReaderWriter::close() {
  int r = codeC::close(this->fd);
  outils::exit_error("Erreur à la fermeture du file descriptor", r==-1, errno);
}


int ReaderWriter::read(std::vector<char> &buf, bool wait_all) {
  int taille = buf.size();
  if (taille == 0) {
    std::ostringstream c;
    c << "ATTENTION fichier " << __FILE__ << " ligne " <<  __LINE__ << " : vous essayez de lire des données dans un buffer de taille 0" << std::endl;
    std::cerr << c.str();
  }

  // on utilise la fonction read de base sur le tableau de donnée du vector
  int lu = this->read(buf.data(), 0, buf.size(), wait_all);

  return lu;
}

int ReaderWriter::read(char *buf, int offset, int len, bool wait_all) {
  int deja = 0;

  while (deja < len) {
    // on fait une lecture
    // mais si la demande de lecture est trop importante, on lit par morceau
    unsigned long alire = len-deja;
    if (alire > SIZE_MAX) {
      alire = SIZE_MAX;
    }
    int nblus = codeC::read(this->fd, buf+offset+deja, alire);
    outils::exit_error("erreur lors de la lecture", nblus==-1, errno);
    if (nblus == 0) {
      // la socket vient d'être fermée
      // si on a déjà lu les données cela ne change rien,
      // si il en manque cela risque de poser des problèmes alors
      // je met des 0 et je leve une exception
      if (deja < len) {
	std::ostringstream c;
	c << "Le flux de lecture est fermé après " << deja
			<< " octets alors que vous en attendez " << len;
	//	std::cerr << "Attention fichier " << __FILE__ << " ligne " << __LINE__
	//		  << " " << c.str() << std::endl;
	if (wait_all) {
	  outils::exit_error(c.str().c_str(), true, 0);
	  //	  throw std::runtime_error(c.str());
	} else {
	  outils::warning_error(c.str().c_str(), true, 0);
	}
      }
      break;
    }
    deja += nblus;

    //    std::cerr << "lu : " << nblus << "caractères " << 

    if (!wait_all) {
      // si on ne demande pas d'attendre, on sort qu'en même
      break;
    }
  }

  // on renvoie le nombre d'octets lus
  return deja;
}

int
ReaderWriter::test_and_read(char *buff, int offset, int len) {
  int r;
  // on récupère les anciennes options
  int oldattr = fcntl(fd, F_GETFL);
  outils::exit_error("fcntl impossible", oldattr == -1, errno);
  // on ajoute l'option non blocante
  r = fcntl(fd, F_SETFL, oldattr | O_NONBLOCK);
  outils::exit_error("fcntl impossible", r == -1, errno);
  int nblus = codeC::read(fd, buff+offset, len);
  if ((nblus == -1)
      &&((errno==EAGAIN)||(errno==EWOULDBLOCK))
      )
    {
    nblus = 0;
  }
  outils::exit_error("read impossible", nblus == -1, errno);
  r = fcntl(fd, F_SETFL, oldattr);
  outils::exit_error("fcntl impossible", r == -1, errno);

  return nblus;
}

int ReaderWriter::write(const std::vector<char> &buf) {
  int taille = buf.size();
  if (taille == 0) {
    std::ostringstream c;
    c << "ATTENTION fichier " << __FILE__ << " ligne " <<  __LINE__ << " : vous essayez d'écrire des données depuis un buffer de taille 0" << std::endl;
    std::cerr << c.str();
  }

  // on utilise la fonction read de base sur le tableau de donnée du vector
  int ecrit = this->write(buf.data(), 0, buf.size());

  return ecrit;  
}

int ReaderWriter::write(const char* buf, int offset, int len) {
  int deja = 0;

  while (deja < len) {
    // on fait une ecriture
    int nbecr = codeC::write(this->fd, buf+offset, len-deja);
    outils::exit_error("erreur lors de la lecture", nbecr==-1, errno);
    deja += nbecr;
  }

  // on renvoie le nombre d'octets lus
  return deja;
}
