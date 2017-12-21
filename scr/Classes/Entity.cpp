#include "Entity.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Blocks.hpp"

Entity::Entity() {
};

Entity::Entity(const Entity& orig) {
}

Entity::~Entity() {
}

const double Entity::SpritesPerSecond=4;
const double Entity::jumpSpeed=303;
const double Entity::horizontalRecoil=270;
const double Entity::verticalRecoil=300;
const double Entity::walkSpeed=150;
const double Entity::runAceleration=66;
const double Entity::maxRunSpeed=250;
const double Entity::imunityTime=200;
const int Entity::state_Idle=0;
const int Entity::state_Walking=1;
const int Entity::state_Running=2;
const int Entity::state_Jumping=3;
const int Entity::state_Atacking=4;
const int Entity::state_SpecialAtacking=5;
const int Entity::state_TakingDamage=6;
const int Entity::state_Dying=7;
const int Entity::state_Spawning=8;
const int Entity::state_Lowering=9;
const int Entity::state_MeleeAirAtackingDown=10;
const int Entity::state_MeleeAirAtackingUp=12;
const int Entity::state_MeleeAirAtackingHorizontal=11;
const int Entity::state_MeleeGroundAtackingUp=14;
const int Entity::state_MeleeGroundAtackingHorizontal=13;
const int Entity::state_RangedAirAtackingDown=15;
const int Entity::state_RangedAirAtackingUp=17;
const int Entity::state_RangedAirAtackingHorizontal=16;
const int Entity::state_RangedGroundAtackingUp=18;
const int Entity::state_RangedGroundAtackingHorizontal=18;
const int Entity::state_Holding=-1;
vector<void*> Entity::players;
vector<void*> Entity::enemys;

/**
 *	Change the entity state machine based on enviroments and conditions of the entity
**/
void Entity::stateControl(){
    FunctionAnalyser::startFunction("Entity::stateControl");
    if(currentState==state_Spawning){
        FunctionAnalyser::endFunction("Entity::stateControl");
        return;
    }
    if(pos.x<0||pos.y<0||pos.x>=Map::size.x+Blocks::defaultBlockSize.x*1.5||pos.y>=Map::size.y+Blocks::defaultBlockSize.y*1.5)
        if(!Player::getPlayerById(0)->god||pos.x!=Player::getPlayerById(0)->pos.x||pos.y!=Player::getPlayerById(0)->pos.y) life=0;
    if(life<=0){
        if(currentState!=state_Dying) currentIndex=0;
        currentState=state_Dying;
        nextState=state_Holding;
    }else{
        nextState=state_Idle;
        if(hSpeed!=0&&!reducing)
            nextState=state_Walking;
        if(ABS(hSpeed)>walkSpeed&&!reducing)
            nextState=state_Running;
        if(vSpeed!=0)
            nextState=state_Jumping;
        if(damageState)
            nextState=state_TakingDamage;
    }
    if(reducing||atacking){
        if(hSpeed>0)
            reduceSpeed(Util::direction_right);
        else if(hSpeed<0)
            reduceSpeed(Util::direction_left);
        if(hSpeed==0)
            reducing=false;
    }
    FunctionAnalyser::endFunction("Entity::stateControl");
}

/**
 *	Gets the default sprite animate interval
 *
 *	@return ammount of sprites per second
**/
double Entity::getSpriteMs(){
    return SpritesPerSecond;
}


/**
 *	Exec animations changing the textures based on the current state and next state. Also spawn end level blocks and behave death of the player
**/
void Entity::execAnimation(){
    FunctionAnalyser::startFunction("Entity::execAnimation");
    if(GL::isPaused){
        FunctionAnalyser::endFunction("Entity::execAnimation");
        return;
    }
    if(round(fmodl(GL::getGameMs(),(int)Entity::getSpriteMs()))==0){
        Player* pl;
        Enemy* en;
        if(currentIndex>=animations[currentState].size()){
            currentIndex=0;
            switch(currentState){
                case state_TakingDamage: damageState=false; break;
                case state_Dying:
                    if(id<0){
                        AL::singleton->playSoundByName("diePlayer");
                        pl=(Player*) Entity::players[-id-1];
                        pl->currentState=state_Dying;
                        pl->currentIndex=state_Idle;
                        if(!pl->god){
                            if(!Scenes::freeGameMode){
                                pl->isVisible=false;
                                Player::lives--;
                                if(Player::lives)
                                    Map::next();
                                else
                                    Map::GG(false);
                            }else{
                                pl->isVisible=false;
                                Map::next();
                            }
                        }
                    }else{
                      Player::enemysKilled++;
                        en=(Enemy*)Entity::enemys[id];
                        if(Enemy::checkIfEnemyIsBoss(en->type)&&en->isVisible){
                            Blocks* bl;
                            bool haveGG=false;
                            for(int i=0;i<Map::dynamicBlocks.size();i++){
                                bl=(Blocks*)Map::dynamicBlocks[i];
                                if(Blocks::checkIfBlocksIsEndLevel(bl->type))
                                    haveGG=true;
                            }
                            if(!haveGG){
                                nTPoint ggPos=en->pos;
                                ggPos.x+=en->orientation*Blocks::defaultBlockSize.x;
                                bl=new Blocks(Blocks::EndLevelBlock,ggPos,Blocks::defaultBlockSize);
                                Map::dynamicBlocks.push_back(bl);
                            }
                        }
                        en->isVisible=false;
                        delete en;
                        FunctionAnalyser::endFunction("Entity::execAnimation");
                        return;
                    }
                break;
                case state_Spawning: nextState=state_Jumping;break;
            }
            if(nextState>=state_Idle){
                currentState=nextState;
                nextState=state_Holding;
            }
        }
        if(currentState>=animations.size()||currentState==state_Holding)
            currentState=state_Idle;
        if(currentIndex>=animations[currentState].size()||currentIndex<0)
            currentIndex=0;
        currentTex=animations[currentState][currentIndex];
        currentIndex++;
    }
    FunctionAnalyser::endFunction("Entity::execAnimation");
}

/**
 *	Get an vetor of vectors containing the texture id of each animation passed as parameter
 *
 *	@param animations vector contaning the animation base name
 *	@param size vector contaning the size of each animation
 *	@return the vector of texture ids for each animation
**/
vector<vector<GLuint> >  Entity::getAnimationVector(vector<string> animations, vector<int>size){
    vector<vector<GLuint> > anim;
    for(int i=0;i<animations.size();i++){
        anim.push_back(GL::getTexturesByName(animations[i],size[i]));
    }
    return anim;
}

/**
 *	Apply damage to entities
 *
 *	@param damage quantity of damage to be applied
**/
void Entity::applyDamage(double damage){
    if(damageState||imuneToDamage)
        return;
    life-=damage;
    damageState=true;
    makeInvencible(imunityTime);
    if(id<0){
        AL::singleton->playSoundByName("damagePlayer");
    }else{
        if(rand()%23==0)
            AL::singleton->playSoundByName("enemyDamageSpec");
        else
            AL::singleton->playSoundByName("enemyDamage");
    }
}

/**
 *	Draw this entity on the screen
 *
 *	@param color base color for drawing, the default is white(1,1,1,1)
**/
void Entity::draw(nTColor color){
    if(GL::isPaused||!this->isVisible)
        return;
    if(Tutorials::isPaused&&this->isHuman==false)
        return;
    if((damageState||imuneToDamage)&&round(fmodl(GL::getGameMs(),200)==0)>0)
        return;
    stateControl();
    if(!isVisible) return;
    GL::drawTexture(nTRectangle::getCollision(pos,size),color,currentTex,orientation);
    if(Mechanics::drawCollisionRec)GL::drawCollision(nTRectangle::getCollision(pos, size));
    especificDraw();
}

/**
 *	Run logic events of the Enemy on the map like move, change directions and textures, spawn bullets, apply damage and draw nicknames and draw players
**/
void Entity::behave(){
    FunctionAnalyser::startFunction("Entity::behave");
    Enemy::behave();
    Player* pl;
    for(int i=0;i<players.size();i++){
        pl=(Player*)players[i];
        if(pl->god) pl->draw(nTColor::get(0.41,0.41,1,0.85));
        else pl->draw();
    }
    FunctionAnalyser::endFunction("Entity::behave");
}

/**
 *	Draw this entity on the screen
**/
void Entity::draw(){
    draw(nTColor::White());
}

/**
 *	Does nothing, this function must be override in a child
**/
void Entity::especificDraw(){

}

/**
 *	Make the entity immune to damage for a certain time
 *
 *	@param time time who the enity will be immune
**/
void Entity::makeInvencible(double time){
    imuneToDamage=true;
    timeToVunerability=GL::getGameMs()+time;
}

/**
 *	Make the entity jump
**/
void Entity::jump(){
    if(life<=0)
        return;
    if(canJump&&!atacking){
        vSpeed-=jumpSpeed;
        canJump=false;
        if(vSpeed<jumpSpeed*1.3)
            vSpeed=-jumpSpeed;
    }
}

/**
 *	Make the entity walk
 *
 *	@param dir walk direction, can be Util::direction_left or Util::direction_right
**/
void Entity::walk(int dir){
    if(life<=0)
        return;
    if(atacking){
        reducing=true;
        return;
    }
    reducing=false;
    if(dir==Util::direction_left){
        hSpeed=-walkSpeed;
        orientation=Util::orientation_left;
    }else if(dir==Util::direction_right){
        hSpeed=walkSpeed;
        orientation=Util::orientation_right;
    }
}

/**
 *	Make the entity run
 *
 *	@param dir run direction, can be Util::direction_left or Util::direction_right
**/
void Entity::run(int dir){
    if(life<=0)
        return;
    if(atacking){
        reducing=true;
        return;
    }
    reducing=false;
    if(ABS(hSpeed)<walkSpeed)
        walk(dir);
    if(ABS(hSpeed)<maxRunSpeed){
        if(dir==Util::direction_left){
            hSpeed-=runAceleration/GL::getFPS();
        }else if(dir==Util::direction_right){
            hSpeed+=runAceleration/GL::getFPS();
        }
    }
}

/**
 *	Reduce speed slowly causing a low friction effect
 *
 *	@param dir reduce direction, can be Util::direction_left or Util::direction_right
**/
void Entity::reduceSpeed(int dir){
    if(dir==Util::direction_left){
        hSpeed+=runAceleration*1.3/GL::getFPS();
    }else if(dir==Util::direction_right){
        hSpeed-=runAceleration*1.3/GL::getFPS();
    }
}

/**
 *	Sets the vector of texture ids for each animation
 *
 *	@param animations vector of texture ids for each animation
**/
void Entity::setAnimations(vector<vector<GLuint> > animations){
    this->animations=animations;
}

/**
 *	Set sprites and animations of entities using vectors to store texture names and size of animations
**/
void Entity::setSprites(){
    vector<string> tmp;
    vector<int> tmp2;
    //cefetinho
    tmp.push_back("cefetinhoIdle");
    tmp.push_back("cefetinhoMoveWalk");
    tmp.push_back("cefetinhoMoveRun");
    tmp.push_back("cefetinhoJump");
        tmp.push_back("");
        tmp.push_back("");
    tmp.push_back("cefetinhoDmgGround");
    tmp.push_back("cefetinhoDie");
    tmp.push_back("cefetinhoSpawn");
    tmp.push_back("cefetinhoDown");
    tmp.push_back("cefetinhoAtkMeleeAirDown");
    tmp.push_back("cefetinhoAtkMeleeAirFront");
    tmp.push_back("cefetinhoAtkMeleeAirUp");
    tmp.push_back("cefetinhoAtkMeleeGroundFront");
    tmp.push_back("cefetinhoAtkMeleeGroundUp");
    tmp.push_back("cefetinhoAtkRangedAirDown");
    tmp.push_back("cefetinhoAtkRangedAirFront");
    tmp.push_back("cefetinhoAtkRangedAirUp");
    tmp.push_back("cefetinhoAtkRangedGroundFront");
    tmp.push_back("cefetinhoAtkRangedGroundUp");

    tmp2.push_back(5);
    tmp2.push_back(4);
    tmp2.push_back(4);
    tmp2.push_back(1);
        tmp2.push_back(1);
        tmp2.push_back(1);
    tmp2.push_back(1);
    tmp2.push_back(4);
    tmp2.push_back(7);
    tmp2.push_back(1);
    tmp2.push_back(1);
    tmp2.push_back(1);
    tmp2.push_back(1);
    tmp2.push_back(1);
    tmp2.push_back(1);
    tmp2.push_back(1);
    tmp2.push_back(1);
    tmp2.push_back(1);
    tmp2.push_back(1);
    tmp2.push_back(1);

    Player::playerAnim.push_back(tmp);
    Player::playerAnimSize.push_back(tmp2);
    tmp.clear();

    //grad
    tmp.push_back("graduacaoIdle");
    tmp.push_back("graduacaoMoveWalk");
    tmp.push_back("graduacaoMoveRun");
    tmp.push_back("graduacaoJump");
        tmp.push_back("");
        tmp.push_back("");
    tmp.push_back("graduacaoDmgGround");
    tmp.push_back("graduacaoDie");
    tmp.push_back("graduacaoSpawn");
    tmp.push_back("graduacaoDown");
    tmp.push_back("graduacaoAtkMeleeAirDown");
    tmp.push_back("graduacaoAtkMeleeAirFront");
    tmp.push_back("graduacaoAtkMeleeAirUp");
    tmp.push_back("graduacaoAtkMeleeGroundFront");
    tmp.push_back("graduacaoAtkMeleeGroundUp");
    tmp.push_back("graduacaoAtkRangedAirDown");
    tmp.push_back("graduacaoAtkRangedAirFront");
    tmp.push_back("graduacaoAtkRangedAirUp");
    tmp.push_back("graduacaoAtkRangedGroundFront");
    tmp.push_back("graduacaoAtkRangedGroundUp");

    Player::playerAnim.push_back(tmp);
    Player::playerAnimSize.push_back(tmp2);
    tmp.clear();

    //posgrad
    tmp.push_back("posgradIdle");
    tmp.push_back("posgradMoveWalk");
    tmp.push_back("posgradMoveRun");
    tmp.push_back("posgradJump");
        tmp.push_back("");
        tmp.push_back("");
    tmp.push_back("posgradDmgGround");
    tmp.push_back("posgradDie");
    tmp.push_back("posgradSpawn");
    tmp.push_back("posgradDown");
    tmp.push_back("posgradAtkMeleeAirDown");
    tmp.push_back("posgradAtkMeleeAirFront");
    tmp.push_back("posgradAtkMeleeAirUp");
    tmp.push_back("posgradAtkMeleeGroundFront");
    tmp.push_back("posgradAtkMeleeGroundUp");
    tmp.push_back("posgradAtkRangedAirDown");
    tmp.push_back("posgradAtkRangedAirFront");
    tmp.push_back("posgradAtkRangedAirUp");
    tmp.push_back("posgradAtkRangedGroundFront");
    tmp.push_back("posgradAtkRangedGroundUp");

    Player::playerAnim.push_back(tmp);
    Player::playerAnimSize.push_back(tmp2);
    tmp.clear();
    tmp2.clear();

    //enemy
    tmp.push_back("paperWalk");
    tmp.push_back("paperWalk");
    tmp.push_back("paperWalk");
    tmp.push_back("paperWalk");
        tmp.push_back("");
        tmp.push_back("");
    tmp.push_back("paperWalk");
    tmp.push_back("paperWalk");

    tmp2.push_back(1);
    tmp2.push_back(2);
    tmp2.push_back(2);
    tmp2.push_back(1);
        tmp2.push_back(1);
        tmp2.push_back(1);
    tmp2.push_back(1);
    tmp2.push_back(1);

    Enemy::enemyAnim.push_back(tmp);
    Enemy::Enemy::enemyAnimSize.push_back(tmp2);
    tmp.clear();
    tmp2.clear();

    //enemy 2
    tmp.push_back("bookIdle");
    tmp.push_back("bookWalk");
    tmp.push_back("bookWalk");
    tmp.push_back("bookIdle");
        tmp.push_back("");
        tmp.push_back("");
    tmp.push_back("bookIdle");
    tmp.push_back("bookIdle");

    tmp2.push_back(1);
    tmp2.push_back(5);
    tmp2.push_back(5);
    tmp2.push_back(1);
        tmp2.push_back(1);
        tmp2.push_back(1);
    tmp2.push_back(1);
    tmp2.push_back(1);

    Enemy::enemyAnim.push_back(tmp);
    Enemy::Enemy::enemyAnimSize.push_back(tmp2);
    tmp.clear();
    tmp2.clear();

    //enemy 3
    tmp.push_back("calcIdle");
    tmp.push_back("calcWalk");
    tmp.push_back("calcWalk");
    tmp.push_back("calcIdle");
        tmp.push_back("");
        tmp.push_back("");
    tmp.push_back("calcIdle");
    tmp.push_back("calcIdle");

    tmp2.push_back(1);
    tmp2.push_back(4);
    tmp2.push_back(4);
    tmp2.push_back(1);
        tmp2.push_back(1);
        tmp2.push_back(1);
    tmp2.push_back(1);
    tmp2.push_back(1);

    Enemy::enemyAnim.push_back(tmp);
    Enemy::enemyAnimSize.push_back(tmp2);
    tmp.clear();
    tmp2.clear();
}
