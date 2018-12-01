#include <cstdio>
#include <string.h>
#include <iostream>
#include "../include/PrivateKeyGenerator.h"
#include "../include/Utilisateur.h"
#include "../include/Chiffrement.h"
#include "../include/Dechiffrement.h"
#include "../include/Outils.h"
#include "../include/qt/FenConnexion.h"
#include "../include/Mail.h"

int main(int argc, char *argv[]) {
  
  if(argc < 2) {
    std::printf("Merci de fournir un paramétre !!");
    
    return EXIT_FAILURE;
  } else if(strcmp(argv[1], "client") == 0 ) {
    QApplication app(argc, argv);

    FenConnexion fenetreCo;
    fenetreCo.show();

    return app.exec();
  } else if(strcmp(argv[1], "serveur") == 0) {
    PrivateKeyGenerator	s;
    s.attendre_client();

    return EXIT_SUCCESS;
  }else if(strcmp(argv[1], "m") == 0) {

    std::printf("----------------------------------------------------------");  
    //Utilisateur::creer_utilisateurs("./data/utilisateurs.txt");
  
    //Mail::test_regression();

    return EXIT_SUCCESS;
  }else if(strcmp(argv[1], "test") == 0) {
    Mail m = Mail::lire_mail_imap("exemple_mail_imap.txt");

    std::string email = "spoutnik2018@sfr.fr";

    mpz_class n("105747958038018799737463469384215293534297042036873087487093634437970489191021022853615710162897894079582249523018916368207344427599962787630345283859929102419067516423623532650176525408368404148847331950587011304282744795219118191208893482371559834799562880258734349213483350209185802555307291380278669514275190998349056786359538309763083699000053213804376648479009157287163494857932458607403710799807174566180674505757163897212224246938143998239838309174612913395862097273639804270016600422903604782144640607736548853600214435245821398802658344061583621743775100580712932450828143517148584313408210680215345115756827624512405482532837664625022098478368638611764214464921882895312087877468413846226664746111862100486075537029623210076605146834827750980023964100614432032678348573659740548920657090168076660531136199720514045602053254466613828567221639211627733355465451140825389732045099181365276611389312916612402270773882919735516540340959767357005128549880209607725538583444844183428313062546111280175999082606899240251847623128317167231048310332210994461423407596064582354425255511270841900751085587565362058144220104525369419495621209370059167613995489435058700190163966338391250725941864593070931387853548405424292990167251089");

    mpz_class skid("18046046718635916646297142942906917914631789911450404247283412590218908924547286374278870517963355528860750538719854920870860099145481347997347118050185773630459111106433936750328771506780121928985481170263433657425956899045616547508245210726189492434941050906443498524397365089140727107575788253148026038558040736568545445866426532882928254539077919291655649182387311427502509192149849233125434619637831166652896751389978459042460092365616050426477970909626460138166834721528512402008184705603138836378375224393028368262684693695724208314870741167746547226576840963525617772980847640305049754558375575833117412633754307118024040822911274592454244247232039012421503869573047661660737879848461391560725575728104483457753431864932088868838487795906847325692074330561284464437161601473832781651635405030376045569446446351029817291059031501867013132396585116322364171989717407893156203244058348257570257252548699784352211075505580384834354530011207637309558954144015441684223255031671827814154184314231026919762444693816990589074526142412030961937732469490862838425412755993323479210931636355594078898703001720531807078229897239368309506091706686302614818006973984198647797767058203953398784401510541027908696389823091454501177220724787");

    std::cout << "\n----------VIA CURL----------\n";
    std::cout << "expediteur: " << m.get_expediteur() << std::endl;
    std::cout << "destinataire: " << m.get_destinataire() << std::endl;
    std::cout << "objet: " << m.get_objet() << std::endl;
    std::cout << "size aes: " << m.get_cle_aes_chiffre().size() << std::endl;
    std::cout << "idec: " << m.get_idec() << std::endl;
    std::cout << "    size -> " << m.get_idec().size() << std::endl;
    std::cout << "corps_chiffre: " << m.get_contenu_chiffre() << std::endl;
    std::cout << "    size -> " << m.get_contenu_chiffre().size() << std::endl;
    std::cout << "corps_dechiffre: " << m.get_contenu_dechiffre(n, skid) << std::endl;

  }
  return EXIT_SUCCESS;
}
