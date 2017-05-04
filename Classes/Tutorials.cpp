#include "Tutorials.hpp"
#include "Scenes.hpp"


Tutorials::Tutorials(string text,int key){
  this->text=text;
  this->key=key;
  this->isCalled=true;
  this->isPaused=false;
  // this->draw();
  pressedKey=0;
  //GLlist=glGenLists(1);
};

Tutorials::Tutorials(const Tutorials& orig) {
}

Tutorials::~Tutorials() {
}
//string declarations:
  //ao iniciar o jogo(pela primeira vez?)
  string Tutorials::initHelp="Bem Vindo a sua vida academica!!!                 Eu represento o DECOM, vim para te ajudar em sua  jornada, para comecar, use as setas do teclado    para se orientar;";
  //ao aparecer(por completo?) o primeiro inimigo na tela   [zero=new Tutorials (Tutorials::atackHelp,'z');]
  string Tutorials::atackHelp="Em sua jornada voce precisara vencer diversas     materias e provas dificeis para prosseguir,       experimente segurar uma seta direcional e usar a  sua espada do poder para atacar nesta direcao,    utilizando a tecla [ Z ] ;";
  //ao aparecer(por completo?) o primeiro obstaculo no mapa [zero=new Tutorials (Tutorials::jumpHelp,' ');]
  string Tutorials::jumpHelp="Voce pode atacar em qualquer direcao enquanto     estiver no ar , para pular pressione [ ESPACO ] e para correr pressione [ SHIFT ] ;";
  //ao pegar o primeiro powerup                             [zero=new Tutorials (Tutorials::powerupHelp,' ');]
  string Tutorials::powerupHelp="Parabens! voce pegou o seu primeiro power-up,     power-ups sao ferramentas que auxiliam em seus    estudos, fortalecendo sua espada do poder,        facilitando bastante na hora de vencer as materias a frente, pressione [ ESPACO ] para continuar;";
  //ao atingir 75 de coeficiente                            [zero=new Tutorials (Tutorials::rangedAtkHelp,'x');]
  string Tutorials::rangedAtkHelp="Quando voce atingir 75 em seu coeficiente de rendimento, o minimo necessario para usar seu ataque especial,pressione [ X ] para lancar uma poderosa onda de  poder em qualquer direcao;";
  //ao surgir o primeiro boss                               [zero=new Tutorials (Tutorials::bossHelp,' ');]
  string Tutorials::bossHelp="Cuidado! voce chegou ao primeiro boss, para       concluir o estagio atual,voce precisa vence-lo,   utilize todo seu poder para passar por este       desafio, pressione [ ESPACO ] para continuar;";
  //ao entrar para graduacao                                [zero=new Tutorials (Tutorials::gradHelp,' ');]
  string Tutorials::breakHelp="Hey, um bloco fragil  'a sua frente,               tente quebra-lo com seu ataque                    [ z ] para continuar;";
  //ao entrar para a pos graduacao                          [zero=new Tutorials (Tutorials::posHelp,' ');]
  string Tutorials::moveHelp="Atencao! Que tal pegar um atalho?                  Se mova para andar na plataforma,                 pressione [ ESPACE ] para continuar;";
  //ao vencer o jogo                                        [zero=new Tutorials (Tutorials::winHelp,' ');]
  string Tutorials::tpHelp="Mais rapido que fibra optica! Precione para baixo     para teletransportar,   Pressione [ ESPACE ] ;";

  string Tutorials::intercampiHelp="Eles vem do C1! Não deixe o intercampi te         atropelar, use suas habilidades de movimentacao  para desviar! Pressione [ ESPACE ] ;";

  string Tutorials::jumpSpringHelp="Suba na mola para dar, grandes saltos de      conhecimento! Pressione [ ESPACE] ;";

  string Tutorials::shooterHelp="Segmentation Fault!                               Não deixe esse erro arruinar seu TP devie!!!         Pressione [ ESPACE ] ;";
  
  string Tutorials::checkHelp="Va coletando diplomas e certificados, assim        garantira que suas horas nao foram perdidas         e salvara o jogo. Pressione [ ESPACE] ;";

  string Tutorials::hudHelp="Os coracoes/espada indicam sua vida atual! As       vidas são a quantidade de chances que voce tem.     E o coeficiente e calculado de acordo com seu aproveitamento da fase [ ESPACE ] ;";

  bool Tutorials::initHelpC=false;
  bool Tutorials::atackHelpC=false;
  bool Tutorials::jumpHelpC=false;
  bool Tutorials::powerupHelpC=false;
  bool Tutorials::rangedAtkHelpC=false;
  bool Tutorials::bossHelpC=false;
  bool Tutorials::breakHelpC=false;
  bool Tutorials::moveHelpC=false;
  bool Tutorials::tpHelpC=false;
  bool Tutorials::intercampiHelpC=false;
  bool Tutorials::jumpSpringHelpC=false;
  bool Tutorials::shooterHelpC=false;
  bool Tutorials::checkHelpC=false;
  
  //===================
bool Tutorials::showTutorials=true;
int Tutorials::pressedKey;
bool Tutorials::isPaused=false;

void Tutorials::drawWhispText(string text){
  GL::setFontByIndex(6);
  int maxChars=50;
  for(int i=0;i<text.size();i+=maxChars){
      GL::drawText(Util::nTPointSet(320+Scenes::camera.x.movedCam,Scenes::camera.y.movedCam+90+(i/maxChars)*25,1),(char*)(text.substr(i,maxChars)).c_str(),GL::getColorByName("yellow"));
      }
}
void Tutorials::draw(){
    if(GL::isPaused)
        return;
  if(showTutorials){
    if(isCalled){
      if(pressedKey!=key){
        isPaused=true;
        glNewList(GLlist,GL_COMPILE_AND_EXECUTE);
        GL::drawTexture((Util::nTRectangleSet(Scenes::camera.x.movedCam,
        300+Scenes::camera.y.movedCam,
        800+Scenes::camera.x.movedCam,
        Scenes::camera.y.movedCam,0.9999,0.999)),
        Util::nTColorSet(1,1,1,1),
        GL::getTextureByName("Whisp"),1);
        drawWhispText(text);
        glEndList();
      }else{
        // cout<<"\n\npressed\n\n";
        isCalled=false;
        isPaused=false;
        pressedKey=0;
      }
    }
  }
}

void Tutorials::refresh(string text,int key){
  this->text=text;
  this->key=key;
  this->isCalled=true;
  // this->draw();
  AL::playSoundByName("heyListen");
  this->isPaused=true;
  pressedKey=0;
}
