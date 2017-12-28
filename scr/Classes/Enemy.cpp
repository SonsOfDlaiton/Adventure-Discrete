#include "Enemy.hpp"

Enemy::Enemy(int enemyType,double life,nTPoint spawn,nTPoint size,vector<vector<GLuint> > animations,bool isHuman) {
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
    this->currentState=Entity::state_Idle;
    this->currentIndex=0;
    this->nextState=Entity::state_Holding;
    this->life=life;
    this->defaultOrientation=Util::orientation_right;
    this->isHuman=isHuman;
    this->type=enemyType;
    this->canJump=true;
    this->reducing=false;
    this->isVisible=true;
    this->damageState=false;
    this->imuneToDamage=false;
    if(!checkIfEnemyIsBoss(type)){
      if(Scenes::freeGameMode)
        this->nickname="Provas";
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
    Entity::enemys.push_back(this);
    this->id=(unsigned int)Entity::enemys.size()-1;
};

Enemy::Enemy(const Enemy& orig) {
}

Enemy::~Enemy() {
    Enemy* en;
    for(int i=id+1;i<Entity::enemys.size();i++){
        en=(Enemy*)Entity::enemys[i];
        en->id--;
    }
    Entity::enemys.erase(Entity::enemys.begin()+this->id);
}

const double Enemy::imunityTime=200;
const nTPoint Enemy::defaultSize=nTPoint::get(40,48);
const nTPoint Enemy::bossSize=nTPoint::get(48,100);
const int Enemy::bossLife=33;
const int Enemy::defaultLife=3;
vector<vector<string> >Enemy::nicks;
vector<vector<string> >Enemy::enemyAnim;
vector<vector<int> >Enemy::enemyAnimSize;

/**
 *	Override Entity::stateControl to add the function of check collions with lava blocks
**/
void Enemy::stateControl(){
    FunctionAnalyser::startFunction("Enemy::stateControl");
    if(!Scenes::camera.isInTheXScreen(nTRectangle::getCollision(this->pos,this->size))){
        FunctionAnalyser::endFunction("Enemy::stateControl");
        return;
    }
    Entity::stateControl();
    Blocks *bl;
    vector <mapCollision> var;
    var=Map::checkCollision(pos,size);
    for(int i=0; i<var.size(); i++){
        if(var[i].blockId>=Map::staticBlocks.size()&&var[i].blockId>0){
            bl=(Blocks*) Map::dynamicBlocks[var[i].blockId-Map::staticBlocks.size()];
            if(var[i].collision.firstObj!=Mechanics::NOCOLLISION){
				bool isLava=bl->type==Blocks::AnimatedLava1||bl->type==Blocks::AnimatedLava2||bl->type==Blocks::StaticLava;
                if(isLava)
                    life=0;
            }
        }
    }
    execAnimation();
    if(GL::framesInGame>=timeToVunerability)
        damageState=false;
    FunctionAnalyser::endFunction("Enemy::stateControl");
}

/**
 *	Run logic events of the Enemy on the map like move, change directions and textures, spawn bullets, apply damage and draw nicknames
**/
void Enemy::behave(){
    FunctionAnalyser::startFunction("Enemy::behave");
    Enemy *en;
    if(GL::isPaused||Tutorials::isPaused){
        for(int i=0;i<Entity::enemys.size();i++){
            en=(Enemy*)Entity::enemys[i];
            draw(en);
        }
        FunctionAnalyser::endFunction("Enemy::behave");
        return;
    }
    for(int i=0;i<Entity::enemys.size();i++){
        en=(Enemy*)Entity::enemys[i];
        if(en->isVisible){
            if(!checkIfEnemyIsBoss(en->type)){
                if(en->hSpeed>0)
                    en->orientation=Util::orientation_right;
                else
                    en->orientation=Util::orientation_left;
            }else{
                if(Player::getPlayerById(0)->pos.x>=en->pos.x)
                    en->orientation=Util::orientation_right;
                else
                    en->orientation=Util::orientation_left;
                if(Util::timerWithInterval(1200)&&!en->damageState&&Scenes::camera.isInTheXScreen(nTRectangle::getCollision(en->pos,en->size))){
                    new Bullet(Bullet::hyperbolicParaboloidBullet,Bullet::baseSpeed*en->orientation/1.5,nTPoint::get(en->pos.x,Player::getPlayerById(0)->pos.y+4+((rand()%300)/10-17),1),nTPoint::get(23,16,1));
                }
            }
            if(Scenes::camera.isInTheXScreen(nTRectangle::getCollision(en->pos,en->size))){
                objCollision var=Mechanics::getCollision(nTRectangle::getCollision(en->pos,en->size),nTRectangle::getCollision(Player::getPlayerById(0)->pos,Player::getPlayerById(0)->size));
                if(var.firstObj==Mechanics::LEFT||var.firstObj==Mechanics::RIGHT||var.firstObj==Mechanics::BOTTOM||(!Player::getPlayerById(0)->atacking&&Player::getPlayerById(0)->atackDirection!=Util::direction_down&&var.firstObj==Mechanics::TOP))
                    Player::getPlayerById(0)->applyDamage(1);
                draw(en);
            }
        }
    }
    FunctionAnalyser::endFunction("Enemy::behave");
}


void Enemy::draw(Enemy* en){
    GL::setFont("BITMAP_HELVETICA_10");
    GL::drawCentered_X_Text(nTPoint::get(en->pos.x,en->pos.y-en->size.y/2.0,0.7),en->nickname,GL::getColorByName("black"));
    Entity* ent=(Entity*)en;
    ent->draw(GL::getColorByName("white"));
    GL::drawCenteredTexture(nTPoint::get(en->pos.x,en->pos.y,0.8999),nTPoint::get(en->size.x/1.5,-en->size.y/2.5,0),nTColor::get(1,1,1,0.75),en->lifeLetter());
}

/**
 *	Make the enemy immune to damage for a certain time
 *
 *	@param time time who the enemy will be immune
**/
void Enemy::makeInvencible(double time){
    damageState=true;
    if(checkIfEnemyIsBoss(type))
        timeToVunerability=GL::getGameMs()+imunityTime*2;
    else
        timeToVunerability=GL::getGameMs()+imunityTime;
}

/**
 *	Push on nicks vector a vector of nick for each enemy, each vector represents a level
**/
void Enemy::registerNicks(){
    vector<string> tmp;
    tmp.push_back("Algoritmo e Logica de Programacao");
    tmp.push_back("Linguagem de Programacao");
    tmp.push_back("Sistemas Digitais");
    tmp.push_back("Aplicacoes para Web");
    tmp.push_back("Projeto de Sistemas");
    tmp.push_back("Sistemas Operacionais");
    tmp.push_back("Banco de Dados");
    tmp.push_back("Redes de Computadores");
    tmp.push_back("Tecnologia e Comunicacao de Dados");
    tmp.push_back("Seguranca de Redes");
    tmp.push_back("Sistemas Operacionais deRedes");
    tmp.push_back("Projeto de Redes");
    tmp.push_back("Infraestrutura Fisica de Redes");
    nicks.push_back(tmp);
    tmp.clear();
    tmp.push_back("Programacao de Computadores");
    tmp.push_back("Matematica Discreta");
    tmp.push_back("Introducao a Engenharia da Computacao");
    tmp.push_back("Sistemas Digitais");
    tmp.push_back("Computacao Grafica");
    tmp.push_back("Arquitetura de Computadores");
    tmp.push_back("Algoritmos e Estruturas de  Dados");
    tmp.push_back("Metodos Numericos Computacionais");
    tmp.push_back("Linguagens de Programacao");
    tmp.push_back("Banco de Dados");
    tmp.push_back("Modelagem e Desenvolvimentode Software");
    tmp.push_back("Linguagens Formais e Automatos");
    tmp.push_back("Principios de Comunicacao de Dados");
    tmp.push_back("Sistemas Operacionais");
    tmp.push_back("Engenharia de Software");
    tmp.push_back("Redes de Computadores");
    tmp.push_back("Controle de Sistemas Dinamicos");
    tmp.push_back("Controle Digital de Sistemas Dinamicos");
    tmp.push_back("Compiladores");
    tmp.push_back("Inteligencia Artificial");
    tmp.push_back("Inteligencia Artificial");//twice
    tmp.push_back("Otimizacao");
    tmp.push_back("Sistemas Distribuidos");
    tmp.push_back("Inteligencia Computacional");
    tmp.push_back("Interacao HumanoComputador");
    tmp.push_back("Contexto Sociale Profissional da Eng de Comp");
    nicks.push_back(tmp);
    tmp.clear();
    tmp.push_back("Tecnologia da Informacao");
    tmp.push_back("Cabeamento Estruturado");
    tmp.push_back("Arquitetura");
    tmp.push_back("Projeto Fisico de Redes");
    tmp.push_back("Projeto Logico de Redes");
    tmp.push_back("Sistemas Operacionais Corporativos");
    tmp.push_back("Gerencia de Redes Corporativas");
    tmp.push_back("Seguranca em Ambiente de TI");
    tmp.push_back("Sistemas Inteligentes");
    tmp.push_back("Otimizacao de  Processos");
    tmp.push_back("Metodos Matematicos    Aplicados");
    tmp.push_back("Programacao paralela");
    tmp.push_back("Inteligencia Artificial");
    tmp.push_back("Computacao quantica");
    nicks.push_back(tmp);
    tmp.clear();
}


/**
 *	Gets the texture id of enemys life based on his current life
 *
 *	@return the texture id
**/
GLuint Enemy::lifeLetter(){
  double Life=this->life/Enemy::defaultLife;
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

bool Enemy::checkIfEnemyIsBoss(int type){
    return type>=1000;
}
