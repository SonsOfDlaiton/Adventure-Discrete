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
const double Entity::jumpSpeed=312;
const double Entity::horizontalRecoil=280;
const double Entity::verticalRecoil=330;
const double Entity::walkSpeed=166;
const double Entity::runAceleration=70;
const double Entity::maxRunSpeed=280;
const double Entity::imunityTime=300;
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
        if(!Player::getPlayerById(0)->god) life=0;
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
        if(lowered)
            nextState=state_Lowering;
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
    if(Util::timerWithInterval(Entity::getSpriteMs())){
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
    if(Tutorials::isPaused){
        GL::drawTexture(nTRectangle::getCollision(pos,size),color,currentTex,orientation);
        return;
    }
    if((damageState||imuneToDamage)&&Util::timerWithInterval(200))
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
    if(life<=0||lowered)
        return;
    if(canJump&&!atacking){
        float js=jumpSpeed;
        if(itsInTheWater)
            js/=2.6;
        vSpeed-=js;
        canJump=false;
        if(vSpeed<js*1.3)
            vSpeed=-js;
    }
}

/**
 *	Make the entity walk
 *
 *	@param dir walk direction, can be Util::direction_left or Util::direction_right
**/
void Entity::walk(int dir){
    if(life<=0||lowered)
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
    if(life<=0||lowered)
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
    //technical
    tmp.push_back("technicalIdle");                 tmp2.push_back(5);//0
    tmp.push_back("technicalMoveWalk");             tmp2.push_back(4); //1
    tmp.push_back("technicalMoveRun");              tmp2.push_back(4);//2
    tmp.push_back("technicalJump");                 tmp2.push_back(1);//3
        tmp.push_back("");                  tmp2.push_back(1);//4 -none
        tmp.push_back("");                  tmp2.push_back(1);//5 -ne
    tmp.push_back("technicalDmgGround");            tmp2.push_back(1);//6
    tmp.push_back("technicalDie");                  tmp2.push_back(4);//7
    tmp.push_back("technicalSpawn");                tmp2.push_back(7);//8
    tmp.push_back("technicalLowered");              tmp2.push_back(3);//9
    tmp.push_back("technicalAtkMeleeAirDown");      tmp2.push_back(1);//10
    tmp.push_back("technicalAtkMeleeAirFront");     tmp2.push_back(1);//11
    tmp.push_back("technicalAtkMeleeAirUp");        tmp2.push_back(1);//12
    tmp.push_back("technicalAtkMeleeGroundFront");  tmp2.push_back(1);//13
    tmp.push_back("technicalAtkMeleeGroundUp");     tmp2.push_back(1);//14
    tmp.push_back("technicalAtkRangedAirDown");     tmp2.push_back(1);//15
    tmp.push_back("technicalAtkRangedAirFront");    tmp2.push_back(1);//16
    tmp.push_back("technicalAtkRangedAirUp");       tmp2.push_back(1);//17
    tmp.push_back("technicalAtkRangedGroundFront"); tmp2.push_back(1);//18
    tmp.push_back("technicalAtkRangedGroundUp");    tmp2.push_back(1);//19
    Player::playerAnim.push_back(tmp);
    Player::playerAnimSize.push_back(tmp2);
    tmp.clear();
    //graduation
    tmp.push_back("graduationIdle");                 tmp2.push_back(5);//0
    tmp.push_back("graduationMoveWalk");             tmp2.push_back(4); //1
    tmp.push_back("graduationMoveRun");              tmp2.push_back(4);//2
    tmp.push_back("graduationJump");                 tmp2.push_back(1);//3
        tmp.push_back("");                  tmp2.push_back(1);//4 -none
        tmp.push_back("");                  tmp2.push_back(1);//5 -ne
    tmp.push_back("graduationDmgGround");            tmp2.push_back(1);//6
    tmp.push_back("graduationDie");                  tmp2.push_back(4);//7
    tmp.push_back("graduationSpawn");                tmp2.push_back(7);//8
    tmp.push_back("graduationLowered");              tmp2.push_back(3);//9
    tmp.push_back("graduationAtkMeleeAirDown");      tmp2.push_back(1);//10
    tmp.push_back("graduationAtkMeleeAirFront");     tmp2.push_back(1);//11
    tmp.push_back("graduationAtkMeleeAirUp");        tmp2.push_back(1);//12
    tmp.push_back("graduationAtkMeleeGroundFront");  tmp2.push_back(1);//13
    tmp.push_back("graduationAtkMeleeGroundUp");     tmp2.push_back(1);//14
    tmp.push_back("graduationAtkRangedAirDown");     tmp2.push_back(1);//15
    tmp.push_back("graduationAtkRangedAirFront");    tmp2.push_back(1);//16
    tmp.push_back("graduationAtkRangedAirUp");       tmp2.push_back(1);//17
    tmp.push_back("graduationAtkRangedGroundFront"); tmp2.push_back(1);//18
    tmp.push_back("graduationAtkRangedGroundUp");    tmp2.push_back(1);//19
    Player::playerAnim.push_back(tmp);
    Player::playerAnimSize.push_back(tmp2);
    tmp.clear();
    //masters
    tmp.push_back("mastersIdle");                 tmp2.push_back(5);//0
    tmp.push_back("mastersMoveWalk");             tmp2.push_back(4); //1
    tmp.push_back("mastersMoveRun");              tmp2.push_back(4);//2
    tmp.push_back("mastersJump");                 tmp2.push_back(1);//3
        tmp.push_back("");                  tmp2.push_back(1);//4 -none
        tmp.push_back("");                  tmp2.push_back(1);//5 -ne
    tmp.push_back("mastersDmgGround");            tmp2.push_back(1);//6
    tmp.push_back("mastersDie");                  tmp2.push_back(4);//7
    tmp.push_back("mastersSpawn");                tmp2.push_back(7);//8
    tmp.push_back("mastersLowered");              tmp2.push_back(3);//9
    tmp.push_back("mastersAtkMeleeAirDown");      tmp2.push_back(1);//10
    tmp.push_back("mastersAtkMeleeAirFront");     tmp2.push_back(1);//11
    tmp.push_back("mastersAtkMeleeAirUp");        tmp2.push_back(1);//12
    tmp.push_back("mastersAtkMeleeGroundFront");  tmp2.push_back(1);//13
    tmp.push_back("mastersAtkMeleeGroundUp");     tmp2.push_back(1);//14
    tmp.push_back("mastersAtkRangedAirDown");     tmp2.push_back(1);//15
    tmp.push_back("mastersAtkRangedAirFront");    tmp2.push_back(1);//16
    tmp.push_back("mastersAtkRangedAirUp");       tmp2.push_back(1);//17
    tmp.push_back("mastersAtkRangedGroundFront"); tmp2.push_back(1);//18
    tmp.push_back("mastersAtkRangedGroundUp");    tmp2.push_back(1);//19
    Player::playerAnim.push_back(tmp);
    Player::playerAnimSize.push_back(tmp2);
    tmp.clear();
    //work
    tmp.push_back("workIdle");                 tmp2.push_back(5);//0
    tmp.push_back("workMoveWalk");             tmp2.push_back(4); //1
    tmp.push_back("workMoveRun");              tmp2.push_back(4);//2
    tmp.push_back("workJump");                 tmp2.push_back(1);//3
        tmp.push_back("");                  tmp2.push_back(1);//4 -none
        tmp.push_back("");                  tmp2.push_back(1);//5 -ne
    tmp.push_back("workDmgGround");            tmp2.push_back(1);//6
    tmp.push_back("workDie");                  tmp2.push_back(4);//7
    tmp.push_back("workSpawn");                tmp2.push_back(7);//8
    tmp.push_back("workLowered");              tmp2.push_back(3);//9
    tmp.push_back("workAtkMeleeAirDown");      tmp2.push_back(1);//10
    tmp.push_back("workAtkMeleeAirFront");     tmp2.push_back(1);//11
    tmp.push_back("workAtkMeleeAirUp");        tmp2.push_back(1);//12
    tmp.push_back("workAtkMeleeGroundFront");  tmp2.push_back(1);//13
    tmp.push_back("workAtkMeleeGroundUp");     tmp2.push_back(1);//14
    tmp.push_back("workAtkRangedAirDown");     tmp2.push_back(1);//15
    tmp.push_back("workAtkRangedAirFront");    tmp2.push_back(1);//16
    tmp.push_back("workAtkRangedAirUp");       tmp2.push_back(1);//17
    tmp.push_back("workAtkRangedGroundFront"); tmp2.push_back(1);//18
    tmp.push_back("workAtkRangedGroundUp");    tmp2.push_back(1);//19
    Player::playerAnim.push_back(tmp);
    Player::playerAnimSize.push_back(tmp2);
    tmp.clear();
    //gteacher
    tmp.push_back("gteacherIdle");                 tmp2.push_back(5);//0
    tmp.push_back("gteacherMoveWalk");             tmp2.push_back(4); //1
    tmp.push_back("gteacherMoveRun");              tmp2.push_back(4);//2
    tmp.push_back("gteacherJump");                 tmp2.push_back(1);//3
        tmp.push_back("");                  tmp2.push_back(1);//4 -none
        tmp.push_back("");                  tmp2.push_back(1);//5 -ne
    tmp.push_back("gteacherDmgGround");            tmp2.push_back(1);//6
    tmp.push_back("gteacherDie");                  tmp2.push_back(4);//7
    tmp.push_back("gteacherSpawn");                tmp2.push_back(7);//8
    tmp.push_back("gteacherLowered");              tmp2.push_back(3);//9
    tmp.push_back("gteacherAtkMeleeAirDown");      tmp2.push_back(1);//10
    tmp.push_back("gteacherAtkMeleeAirFront");     tmp2.push_back(1);//11
    tmp.push_back("gteacherAtkMeleeAirUp");        tmp2.push_back(1);//12
    tmp.push_back("gteacherAtkMeleeGroundFront");  tmp2.push_back(1);//13
    tmp.push_back("gteacherAtkMeleeGroundUp");     tmp2.push_back(1);//14
    tmp.push_back("gteacherAtkRangedAirDown");     tmp2.push_back(1);//15
    tmp.push_back("gteacherAtkRangedAirFront");    tmp2.push_back(1);//16
    tmp.push_back("gteacherAtkRangedAirUp");       tmp2.push_back(1);//17
    tmp.push_back("gteacherAtkRangedGroundFront"); tmp2.push_back(1);//18
    tmp.push_back("gteacherAtkRangedGroundUp");    tmp2.push_back(1);//19
    Player::playerAnim.push_back(tmp);
    Player::playerAnimSize.push_back(tmp2);
    tmp.clear();
    //bteacher
    tmp.push_back("bteacherIdle");                 tmp2.push_back(5);//0
    tmp.push_back("bteacherMoveWalk");             tmp2.push_back(4); //1
    tmp.push_back("bteacherMoveRun");              tmp2.push_back(4);//2
    tmp.push_back("bteacherJump");                 tmp2.push_back(1);//3
        tmp.push_back("");                  tmp2.push_back(1);//4 -none
        tmp.push_back("");                  tmp2.push_back(1);//5 -ne
    tmp.push_back("bteacherDmgGround");            tmp2.push_back(1);//6
    tmp.push_back("bteacherDie");                  tmp2.push_back(4);//7
    tmp.push_back("bteacherSpawn");                tmp2.push_back(7);//8
    tmp.push_back("bteacherLowered");              tmp2.push_back(3);//9
    tmp.push_back("bteacherAtkMeleeAirDown");      tmp2.push_back(1);//10
    tmp.push_back("bteacherAtkMeleeAirFront");     tmp2.push_back(1);//11
    tmp.push_back("bteacherAtkMeleeAirUp");        tmp2.push_back(1);//12
    tmp.push_back("bteacherAtkMeleeGroundFront");  tmp2.push_back(1);//13
    tmp.push_back("bteacherAtkMeleeGroundUp");     tmp2.push_back(1);//14
    tmp.push_back("bteacherAtkRangedAirDown");     tmp2.push_back(1);//15
    tmp.push_back("bteacherAtkRangedAirFront");    tmp2.push_back(1);//16
    tmp.push_back("bteacherAtkRangedAirUp");       tmp2.push_back(1);//17
    tmp.push_back("bteacherAtkRangedGroundFront"); tmp2.push_back(1);//18
    tmp.push_back("bteacherAtkRangedGroundUp");    tmp2.push_back(1);//19
    Player::playerAnim.push_back(tmp);
    Player::playerAnimSize.push_back(tmp2);
    tmp.clear();

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
