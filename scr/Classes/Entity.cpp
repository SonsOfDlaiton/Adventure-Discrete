#include "Entity.hpp"
#include "Player.hpp"
#include "Boss.hpp"
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
vector<void*> Entity::bosses;

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
                    }else if(id<60000){
                        Player::enemysKilled++;
                        en=(Enemy*)Entity::enemys[id];
                        en->isVisible=false;
                        delete en;
                        FunctionAnalyser::endFunction("Entity::execAnimation");
                        return;
                    }else{
                        currentState=state_Spawning;
                        currentIndex=0;
                    }
                break;
                case state_Spawning: nextState=state_Jumping;break;
            }
            if(nextState>=state_Idle){
                currentState=nextState;
                currentIndex=0;
                nextState=state_Holding;
            }
        }
        if(currentState>=animations.size()||currentState==state_Holding){
            currentState=state_Idle;
            currentIndex=0;
        }
        if(currentIndex>=animations[currentState].size()||currentIndex<0)
            currentIndex=0;
        if(animations.size()==0||animations[currentState].size()==0){
            if(Util::DEBUG){
                string info="Error ";
                if(this->id<0){
                    info="Error on player";
                }else if(id<60000){
                    Enemy *en=(Enemy*)Entity::enemys[id];
                    info="Error on enemy("+en->spritename+")";
                }else{
                    info="Error on boss";
                }
                cout<<info<<" animations vector null at state:"<<currentState<<" index:"<<currentIndex<<endl;
            }
            return;
        }
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
    makeInvencible();
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
}

/**
 *	Run logic events of the Enemy on the map like move, change directions and textures, spawn bullets, apply damage and draw nicknames and draw players
**/
void Entity::behave(){
    FunctionAnalyser::startFunction("Entity::behave");
    Enemy::behave();
    Boss::behave();
    Player::behave();
    FunctionAnalyser::endFunction("Entity::behave");
}

/**
 *	Draw this entity on the screen
**/
void Entity::draw(){
    draw(nTColor::White());
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
 *	Make the entity immune to damage for a certain time
 *
**/
void Entity::makeInvencible(){
    imuneToDamage=true;
    timeToVunerability=GL::getGameMs()+imunityTime;
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
    Player::setSprites();
    Enemy::setSprites();
    Boss::setSprites();
}
