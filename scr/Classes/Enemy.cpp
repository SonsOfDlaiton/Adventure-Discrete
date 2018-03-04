#include "Enemy.hpp"
#include "Boss.hpp"

Enemy::Enemy(int enemyType,double life,nTPoint spawn,nTPoint size,string spr_name) {
    this->pos=spawn;
    this->size=size;
    int spr_id=Enemy::getSpritesId(spr_name);
    if(spr_name=="%random%"||spr_id<0){
        spr_id=randomEnemy();
    }
    this->animations=Entity::getAnimationVector(Enemy::enemyAnim[spr_id],Enemy::enemyAnimSize[spr_id]);;
    this->vSpeed=0;
    this->atacking=false;
    this->hSpeed=Entity::walkSpeed;
    this->changeState(Entity::state_Spawning);
    this->nextState=Entity::state_Holding;
    this->life=life;
    this->defaultOrientation=Util::orientation_right;
    this->isHuman=false;
    this->type=enemyType;
    this->canJump=true;
    this->reducing=false;
    this->lowered=false;
    this->isVisible=true;
    this->damageState=false;
    this->damageVunerability=false;
    this->itsInTheWater=false;
    this->imuneToDamage=false;
    this->drawLetter=true;
    if(Scenes::freeGameMode)
        this->nickname="Provas";
    else if(Player::stage>=0&&Player::stage<nicks.size())
        this->nickname=nicks[Player::stage][rand()%nicks[Player::stage].size()];
    else
        this->nickname="NULL";
    Entity::enemys.push_back(this);
    this->id=(unsigned int)Entity::enemys.size()-1;
};

Enemy::Enemy(const Enemy& orig) {
}

Enemy::~Enemy() {
    Enemy* en;
    for(int i=0;i<Boss::enSummoned.size();i++){
        if(Boss::enSummoned[i]==this){
            Boss::enSummoned.erase(Boss::enSummoned.begin()+i);
            break;
        }
    }
    for(int i=id+1;i<Entity::enemys.size();i++){
        en=(Enemy*)Entity::enemys[i];
        en->id--;
    }
    Entity::enemys.erase(Entity::enemys.begin()+this->id);
}

const nTPoint Enemy::defaultSize=nTPoint::get(Constants::ENEMY_SizeX,Constants::ENEMY_SizeY);
const int Enemy::defaultLife=Constants::ENEMY_BaseLife;
vector<vector<string> >Enemy::nicks;
vector<vector<string> >Enemy::enemyAnim;
vector<vector<int> >Enemy::enemyAnimSize;
vector<string> Enemy::enemyName;
vector<vector<int> > Enemy::enemyRandom;

/**
 *  Override Entity::stateControl to add the function of check collions with lava blocks
**/
void Enemy::stateControl(){
    FunctionAnalyser::startFunction("Enemy::stateControl");
    if(!Scenes::camera.isInTheXScreen(nTRectangle::getCollision(this->pos,this->size))){
        FunctionAnalyser::endFunction("Enemy::stateControl");
        return;
    }
    Blocks *bl;
    collideWithMap();
    for(int i=0; i<lastMapColl.size(); i++){
        if(lastMapColl[i].blockId>=Map::staticBlocks.size()&&lastMapColl[i].blockId>0){
            bl=(Blocks*) Map::dynamicBlocks[lastMapColl[i].blockId-Map::staticBlocks.size()];
            if(lastMapColl[i].collision.firstObj!=Mechanics::NOCOLLISION){
                bool isLava=bl->type==Blocks::AnimatedLava1||bl->type==Blocks::AnimatedLava2||bl->type==Blocks::StaticLava;
                if(isLava)
                    life=0;
            }
        }
    }
    Entity::stateControl();
    FunctionAnalyser::endFunction("Enemy::stateControl");
}

/**
 *  Run logic events of the Enemy on the map like move, change directions and textures, spawn bullets, apply damage and draw nicknames
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
            if(Scenes::camera.isInTheXScreen(nTRectangle::getCollision(en->pos,en->size))){
                if(en->hSpeed>0)
                    en->orientation=Util::orientation_right;
                else
                    en->orientation=Util::orientation_left;
                objCollision var=Mechanics::getCollision(nTRectangle::getCollision(en->pos,en->size),nTRectangle::getCollision(Player::getPlayerById(0)->pos,Player::getPlayerById(0)->size));
                if(var.firstObj==Mechanics::LEFT||var.firstObj==Mechanics::RIGHT||var.firstObj==Mechanics::BOTTOM||(!Player::getPlayerById(0)->atacking&&Player::getPlayerById(0)->atackDirection!=Util::direction_down&&var.firstObj==Mechanics::TOP))
                    Player::getPlayerById(0)->applyDamage(Constants::ENEMY_BaseDamage);
                draw(en);
            }
        }
    }
    FunctionAnalyser::endFunction("Enemy::behave");
}


void Enemy::draw(Enemy* en){
    GL::setFont("BITMAP_HELVETICA_10");
    GL::drawCentered_X_Text(nTPoint::get(en->pos.x,en->pos.y-en->size.y/2.0,0.7),en->nickname,nTColor::Black());
    en->Entity::draw(nTColor::White());
    if(en->drawLetter)
        GL::drawCenteredTexture(nTPoint::get(en->pos.x,en->pos.y,0.8999),nTPoint::get(en->size.x/1.5,-en->size.y/2.5,0),nTColor::get(1,1,1,0.75),en->lifeLetter());
}

/**
 *  Push on nicks vector a vector of nick for each enemy, each vector represents a level
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
 *  Gets the texture id of enemys life based on his current life
 *
 *  @return the texture id
**/
GLuint Enemy::lifeLetter(){
  double Life=this->life/Enemy::defaultLife;
  if(Player::stage==Map::lvlBadTeacher)
    Life=1-Life;
  if(Life<=0.2)
    return GL::getTextureByName("A");
  if(Life>=0.2&&Life<0.4)
    return GL::getTextureByName("B");
  if(Life>=0.4&&Life<0.6)
    return GL::getTextureByName("C");
  if(Life>=0.6&&Life<0.8)
    return GL::getTextureByName("D");
  if(Life>=0.8&&Life<1)
    return GL::getTextureByName("E");
  if(Life>=1)
    return GL::getTextureByName("F");
  return GL::getTextureByName("F");
}

int Enemy::getSpritesId(string name){
    int i=0;
    for(string n:enemyName)
        if(n==name)
            return i;
        else
            i++;
    return -1;
}


int Enemy::randomEnemy(){
    if(Scenes::freeGameMode)
        return enemyRandom[Map::nOfMaps][rand()%enemyRandom[Map::nOfMaps].size()];
    return enemyRandom[Player::stage][rand()%enemyRandom[Player::stage].size()];
}

void Enemy::setSprites(){
    vector<string> tmp;
    vector<int> tmp2;
    Enemy::enemyAnim.clear();
    Enemy::enemyAnimSize.clear();
    Enemy::enemyName.clear();

    //Prova
    Enemy::enemyName.push_back("paper");//sprite name
    tmp.push_back("paperWalk"); tmp2.push_back(1);//0 -Idle
    tmp.push_back("paperWalk"); tmp2.push_back(2);//1 -Walking
    tmp.push_back("paperWalk"); tmp2.push_back(2);//2 -Running
    tmp.push_back("paperWalk"); tmp2.push_back(1);//3 -Jumping
        tmp.push_back("");  tmp2.push_back(1);//4 -Atacking -none
        tmp.push_back("");  tmp2.push_back(1);//5 -SpecialAtacking -none
    tmp.push_back("paperWalk"); tmp2.push_back(1);//6 -Damage
    tmp.push_back("paperWalk"); tmp2.push_back(1);//7 -Death
    tmp.push_back("paperWalk"); tmp2.push_back(1);//8 -Spawn
    Enemy::enemyAnim.push_back(tmp);
    Enemy::enemyAnimSize.push_back(tmp2);
    tmp.clear();
    tmp2.clear();

    //Livro
    Enemy::enemyName.push_back("book");//sprite name
    tmp.push_back("bookIdle"); tmp2.push_back(1);//0 -Idle
    tmp.push_back("bookWalk"); tmp2.push_back(5);//1 -Walking
    tmp.push_back("bookWalk"); tmp2.push_back(5);//2 -Running
    tmp.push_back("bookIdle"); tmp2.push_back(1);//3 -Jumping
        tmp.push_back(""); tmp2.push_back(1);//4 -Atacking -none
        tmp.push_back(""); tmp2.push_back(1);//5 -SpecialAtacking -nome
    tmp.push_back("bookIdle"); tmp2.push_back(1);//6 -Damage
    tmp.push_back("bookIdle"); tmp2.push_back(1);//7 -Death
    tmp.push_back("bookIdle"); tmp2.push_back(1);//8 -Spawn
    Enemy::enemyAnim.push_back(tmp);
    Enemy::enemyAnimSize.push_back(tmp2);
    tmp.clear();
    tmp2.clear();

    //Calculadora
    Enemy::enemyName.push_back("calc");//sprite name
    tmp.push_back("calcIdle"); tmp2.push_back(1);//0 -Idle
    tmp.push_back("calcWalk"); tmp2.push_back(4);//1 -Walking
    tmp.push_back("calcWalk"); tmp2.push_back(4);//2 -Running
    tmp.push_back("calcIdle"); tmp2.push_back(1);//3 -Jumping
        tmp.push_back(""); tmp2.push_back(1);//4 -Atacking -none
        tmp.push_back(""); tmp2.push_back(1);//5 -SpecialAtacking -none
    tmp.push_back("calcIdle"); tmp2.push_back(1);//6 -Damage
    tmp.push_back("calcIdle"); tmp2.push_back(1);//7 -Death
    tmp.push_back("calcIdle"); tmp2.push_back(1);//8 -Spawn
    Enemy::enemyAnim.push_back(tmp);
    Enemy::enemyAnimSize.push_back(tmp2);
    tmp.clear();
    tmp2.clear();

    //Nanokid
    Enemy::enemyName.push_back("enem_nano_kid");//sprite name
    tmp.push_back("nanokidIdle"); tmp2.push_back(4);//0 -Idle
    tmp.push_back("nanokidIdle"); tmp2.push_back(4);//1 -Walking
    tmp.push_back("nanokidIdle"); tmp2.push_back(4);//2 -Running
    tmp.push_back("nanokidIdle"); tmp2.push_back(4);//3 -Jumping
        tmp.push_back(""); tmp2.push_back(1);//4 -Atacking -none
        tmp.push_back(""); tmp2.push_back(1);//5 -SpecialAtacking -none
    tmp.push_back("nanokidIdle"); tmp2.push_back(4);//6 -Damage
    tmp.push_back("nanokidIdle"); tmp2.push_back(1);//7 -Death
    tmp.push_back("nanokidIdle"); tmp2.push_back(4);//8 -Spawn
    Enemy::enemyAnim.push_back(tmp);
    Enemy::enemyAnimSize.push_back(tmp2);
    tmp.clear();
    tmp2.clear();

    //Random List
    tmp2.push_back(0);
    for(int i=0;i<=Map::nOfMaps;i++)
        enemyRandom.push_back(tmp2);
    tmp2.clear();
    tmp2.push_back(0);//PAPER
    tmp2.push_back(1);//BOOK
    tmp2.push_back(2);//CALC
    enemyRandom[Map::lvlTechnical]=tmp2;
    enemyRandom[Map::lvlGraduation]=tmp2;
    enemyRandom[Map::lvlMasters]=tmp2;
    enemyRandom[Map::lvlWork]=tmp2;
    enemyRandom[Map::lvlGoodTeacher]=tmp2;
    enemyRandom[Map::lvlBadTeacher]=tmp2;
    enemyRandom[Map::nOfMaps]=tmp2;

}
