#include "Enemy.hpp"
#include "Bullet.hpp"
#include "Scenes.hpp"
#include "../Libs/Globals.h"

Enemy::Enemy(int enemyType,float life,nTPoint spawn,nTPoint size,vector<vector<GLuint> > animations,bool isHuman) {
    this->life=0;
    this->pos=spawn;
    this->size=size;
    this->animations=animations;
    this->vSpeed=0;
    this->atacking=false;
    if(enemyType<100)
        this->hSpeed=Entity::walkSpeed;
    else
        this->hSpeed=0;
    this->currentState=0;
    this->currentIndex=0;
    this->nextState=-1;
    this->life=life;
    this->defaultOrientation=1;
    this->isHuman=isHuman;
    this->type=enemyType;
    this->GLlist=glGenLists(1);
    this->canJump=true;
    this->reducing=false;
    this->isVisible=true;
    this->damageState=false;
    this->superMan=false;
    if(enemyType<100){
      if(Scenes::freeGameMode)
        this->nickname="      Provas";
      else if(Player::stage>=0&&Player::stage<nicks.size())
          this->nickname=nicks[Player::stage][rand()%nicks[Player::stage].size()];
      else
          this->nickname="NULL";
    }else{
      if(Scenes::freeGameMode)
        this->nickname="       BOSS";
      else if(Player::stage==0)
        this->nickname="       ENEM";
      else if(Player::stage==1)
        this->nickname="       TCC";
      else if(Player::stage==2)
        this->nickname="  MONOGRAFIA";
      else
        this->nickname="NULL";
    }
    Mechanics::enemys.push_back(this);
    Entity::enemys.push_back(this);
    self.push_back(this);
    selfIsPlayer.push_back(false);
    this->id=self.size()-1;
};

Enemy::Enemy(const Enemy& orig) {
}

Enemy::~Enemy() {
    Player* pl;
    Enemy* en;
    for(int i=id+1;i<self.size();i++){
        if(i<selfIsPlayer.size())
        if(selfIsPlayer[i]){
            pl=(Player*)self[i];
            pl->id--;
        }else{
            en=(Enemy*)self[i];
            en->id--;
        }
    }
    self.erase(self.begin()+this->id);
    selfIsPlayer.erase(selfIsPlayer.begin()+this->id);

    for(int i=0;i<Entity::enemys.size();i++){
        if(Entity::enemys[i]==this){
            Entity::enemys.erase(Entity::enemys.begin()+i);
            break;
        }
    }
    for(int i=0;i<Mechanics::enemys.size();i++){
        if(Mechanics::enemys[i]==this){
            Mechanics::enemys.erase(Mechanics::enemys.begin()+i);
            break;
        }
    }
}

float Enemy::imunityTime=200;
nTPoint Enemy::defaultSize=Util::nTPointSet(40,48,0);
nTPoint Enemy::bossSize=Util::nTPointSet(48,100,0);
int Enemy::bossLife=33;
int Enemy::defaultLife=3;
vector<vector<string> >Enemy::nicks;

void Enemy::stateControl(){
    Entity::stateControl();
    Blocks *bl;
    vector <mapCollision> var;
    bool condition=false;
    nTPoint point;
    point=pos;
    point.y=pos.y+2;
      var=Map::checkCollision(point,size);
      for(int i=0; i<var.size(); i++){
        if(var[i].blockRef>=Map::staticBlocks.size()&&var[i].blockRef>0){
            bl=(Blocks*) Map::dynamicBlocks[var[i].blockRef-Map::staticBlocks.size()];
            if(var[i].collision.firstObj){
                if(bl->type==377||bl->type==379){
                    life=0;
                }
            }
        }
      }
    execAnimation();
    if(GL::framesInGame>=timeToVunerability)
        damageState=false;
}

void Enemy::behave(){
    if(GL::isPaused||Tutorials::isPaused)
        return;
    Enemy *en;
    for(int i=0;i<Entity::enemys.size();i++){
        en=(Enemy*)Entity::enemys[i];
        if(en->type<100){
            if(en->hSpeed>0)
                en->orientation=1;
            else
                en->orientation=-1;
        }else{
            if(player->pos.x>=en->pos.x)
                en->orientation=1;
            else
                en->orientation=-1;
            if(round(fmodl(GL::getGameMs(),1200)==0)&&!en->damageState){
                Bullet *bu=new Bullet(5,Bullet::baseSpeed*en->orientation/1.5,Util::nTPointSet(en->pos.x,player->pos.y+4+((rand()%300)/10-17),1),Util::nTPointSet(23,16,1));
            }
        }
        objCollision var=Mechanics::getCollision(Util::getCollisionRectangle(en->pos,en->size),Util::getCollisionRectangle(player->pos,player->size));
        if(var.firstObj)
            player->applyDamage(1);
        if(en->isVisible){
          int maxChars=15;
          for(int i=0;i<en->nickname.size();i+=maxChars){
            GL::drawText(Util::nTPointSet(en->pos.x-en->size.x*1.01,
                                          en->pos.y-en->size.y/2.0-(en->nickname.size()-i),
                                          0.7)
                                          ,(char*)(en->nickname.substr(i,maxChars)).c_str(),
                                          GL::getColorByName("black"));

          }
            GL::setFontByIndex(4);
            en->draw(GL::getColorByName("white"));
            GL::drawTexture(Util::nTRectangleSet(en->pos.x-en->size.x/3.5,en->pos.y+en->size.y/4.0,en->pos.x+en->size.x/3,en->pos.y-en->size.y/4.0,0.899,0.899),Util::nTColorSet(1,1,1,0.75),en->lifeLetter(),1);
        }
    }
}

void Enemy::makeInvencible(float time){
    damageState=true;
    if(type>100)
        timeToVunerability=GL::getGameMs()+imunityTime*2;
    else
        timeToVunerability=GL::getGameMs()+imunityTime;
}

void Enemy::registerNicks(){
    vector<string> tmp;
    tmp.push_back("Algoritmo e    Logica de      Programacao");
    tmp.push_back("Linguagem de   Programacao");
    tmp.push_back("Sistemas       Digitais");
    tmp.push_back("Aplicacoes paraWeb");
    tmp.push_back("Projeto de     Sistemas");
    tmp.push_back("Sistemas       Operacionais");
    tmp.push_back("Banco de Dados");
    tmp.push_back("Redes de       Computadores");
    tmp.push_back("Tecnologia e   Comunicacao de Dados");
    tmp.push_back("Seguranca de   Redes");
    tmp.push_back("Sistemas       Operacionais deRedes");
    tmp.push_back("Projeto de     Redes");
    tmp.push_back("Infraestrutura Fisica de Redes");
    nicks.push_back(tmp);
    tmp.clear();
    tmp.push_back("Programacao de Computadores");
    tmp.push_back("Matematica     Discreta");
    tmp.push_back("Introducao a   Engenharia da  Computacao");
    tmp.push_back("Sistemas       Digitais");
    tmp.push_back("Computacao     Grafica");
    tmp.push_back("Arquitetura de Computadores");
    tmp.push_back("Algoritmos e   Estruturas de  Dados");
    tmp.push_back("Metodos        Numericos      Computacionais");
    tmp.push_back("Linguagens de  Programacao");
    tmp.push_back("Banco de Dados");
    tmp.push_back("Modelagem e    Desenvolvimentode Software");
    tmp.push_back("Linguagens     Formais e      Automatos");
    tmp.push_back("Principios de  Comunicacao de Dados");
    tmp.push_back("Sistemas       Operacionais");
    tmp.push_back("Engenharia de  Software");
    tmp.push_back("Redes de       Computadores");
    tmp.push_back("Controle de    Sistemas       Dinamicos");
    tmp.push_back("Controle       Digital de     Sistemas       Dinamicos");
    tmp.push_back("Compiladores");
    tmp.push_back("Inteligencia   Artificial");
    tmp.push_back("Inteligencia   Artificial");//twice
    tmp.push_back("Otimizacao");
    tmp.push_back("Sistemas       Distribuidos");
    tmp.push_back("Inteligencia   Computacional");
    tmp.push_back("Interacao HumanoComputador");
    tmp.push_back("Contexto Sociale Profissional da Eng de Comp");
    nicks.push_back(tmp);
    tmp.clear();
    tmp.push_back("Tecnologia da  Informacao");
    tmp.push_back("Cabeamento     Estruturado");
    tmp.push_back("Arquitetura");
    tmp.push_back("Projeto Fisico de Redes");
    tmp.push_back("Projeto Logico de Redes");
    tmp.push_back("Sistemas       Operacionais   Corporativos");
    tmp.push_back("Gerencia de    Redes          Corporativas");
    tmp.push_back("Seguranca em   Ambiente de TI");
    tmp.push_back("Sistemas       Inteligentes");
    tmp.push_back("Otimizacao de  Processos");
    tmp.push_back("Metodos        Matematicos    Aplicados");
    tmp.push_back("Programacao    paralela");
    tmp.push_back("Inteligencia   Artificial");
    tmp.push_back("Computacao     quantica");
    nicks.push_back(tmp);
    tmp.clear();
}

GLuint Enemy::lifeLetter(){
  float Life=this->life/Enemy::defaultLife;
  if(Life<=0.1)
    return GL::getTextureByName("A");
  if(Life<=0.3&&Life>0.1)
    return GL::getTextureByName("B");
  if(Life<=0.4&&Life>0.3)
    return GL::getTextureByName("C");
  if(Life<=0.6&&Life>0.4)
    return GL::getTextureByName("D");
  if(Life<=0.7&&Life>0.6)
    return GL::getTextureByName("E");
  if(Life>0.7)
    return GL::getTextureByName("F");
  return GL::getTextureByName("F");
}
