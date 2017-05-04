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

float Entity::SpritesPerSecond=4;
const float Entity::finalJumpSpeed=303;
float Entity::jumpSpeed=Entity::finalJumpSpeed;
float Entity::walkSpeed=130;
float Entity::runAceleration=66;
float Entity::maxRunSpeed=250;
float Entity::imunityTime=200;
vector<void*>Entity::self;
vector<bool>Entity::selfIsPlayer;
vector<void*> Entity::players;
vector<void*> Entity::enemys;

void Entity::stateControl(){
    if(currentState==8) return;
    if(pos.x<0||pos.y<0||pos.x>Map::size.x||pos.y>Map::size.y+Blocks::defaultBlockSize.y*1.5)
        if(!player->god) life=0;
    if(life<=0){
        if(currentState!=7) currentIndex=0;
        currentState=7;
        nextState=-1;
    }else{
        nextState=0;
        if(hSpeed!=0&&!reducing)
            nextState=1;
        if(ABS(hSpeed)>walkSpeed&&!reducing)
            nextState=2;
        if(vSpeed!=0)
            nextState=3;
        if(damageState)
            nextState=6;
    }
    if(reducing||atacking){
        if(hSpeed>0)
            reduceSpeed(Util::right);
        else if(hSpeed<0)
            reduceSpeed(Util::left);
        if(hSpeed==0)
            reducing=false;
    }
}

float Entity::getSpriteMs(){
    return SpritesPerSecond;
}

void Entity::execAnimation(){//base nao chama essa funcao
    if(GL::isPaused)
        return;
    if(round(fmodl(GL::getGameMs(),(int)Entity::getSpriteMs()))==0){
        Player* pl;
        Enemy* en;
        if(currentIndex>=animations[currentState].size()){
            currentIndex=0;
            switch(currentState){
                case 6: damageState=false; break;
                case 7:
                    if(selfIsPlayer[id]){
                        al->playSoundByName("diePlayer");
                        pl=(Player*) self[id];
                        pl->currentState=7;
                        pl->currentIndex=0;
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
                    }else{
                      Player::enemysKilled++;
                        en=(Enemy*)self[id];
                        if(en->type>100&&en->isVisible){
                            Blocks* bl;
                            bool haveGG=false;
                            for(int i=0;i<Map::dynamicBlocks.size();i++){
                                bl=(Blocks*)Map::dynamicBlocks[i];
                                if(bl->type==666)
                                    haveGG=true;
                            }
                            if(!haveGG){
                                nTPoint ggPos=en->pos;
                                ggPos.x+=en->orientation*Blocks::defaultBlockSize.x;
                                bl=new Blocks(666,ggPos,Blocks::defaultBlockSize);
                                Map::dynamicBlocks.push_back(bl);
                            }
                        }
                        en->isVisible=false;
                        en->pos.setPoint(-10,-10,-1);//matar boss

                        //delete en;
                    }
                break;
                case 8: nextState=3;break;
            }
            if(nextState>=0){
                currentState=nextState;
                nextState=-1;
            }
        }
        if(currentState>=animations.size()||currentState<0)
            currentState=0;
        if(currentIndex>=animations[currentState].size()||currentIndex<0)
            currentIndex=0;
        currentTex=animations[currentState][currentIndex];
        currentIndex++;
    }
}

vector<vector<GLuint> >  Entity::getAnimationVector(vector<string> animations, vector<int>size){
    vector<vector<GLuint> > anim;
    for(int i=0;i<animations.size();i++){
        anim.push_back(GL::getTexturesByName(animations[i],size[i]));
    }
    return anim;
}
void Entity::applyDamage(float damage){
    if(damageState||superMan)
        return;
    life-=damage;
    damageState=true;
    makeInvencible(imunityTime);
    if(selfIsPlayer[id]){
        al->playSoundByName("damagePlayer");
    }else{
        if(rand()%23==0)
            al->playSoundByName("enemyDamageSpec");
        else
            al->playSoundByName("enemyDamage");
    }
}

void Entity::draw(nTColor color){
    if(GL::isPaused||!this->isVisible)
        return;
    if(Tutorials::isPaused&&this->isHuman==false)
        return;
    if((damageState||superMan)&&round(fmodl(GL::getGameMs(),200)==0))
        return;
    stateControl();
    glNewList(GLlist,GL_COMPILE_AND_EXECUTE);
    GL::drawTexture(Util::getCollisionRectangle(pos,size),color,currentTex,orientation);
    especificDraw();
    glEndList();
}

void Entity::especificDraw(){

}

void Entity::makeInvencible(float time){
    superMan=true;
    timeToVunerability=GL::getGameMs()+time;
}

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
void Entity::walk(int dir){
    if(life<=0)
        return;
    if(atacking){
        reducing=true;
        return;
    }
    reducing=false;
    if(dir==Util::left){
        hSpeed=-walkSpeed;
        orientation=-1;
    }else if(dir==Util::right){
        hSpeed=walkSpeed;
        orientation=1;
    }
}
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
        if(dir==Util::left){
            hSpeed-=runAceleration/GL::getFPS();
        }else if(dir==Util::right){
            hSpeed+=runAceleration/GL::getFPS();
        }
    }
}

void Entity::reduceSpeed(int dir){
    if(dir==Util::left){
        hSpeed+=runAceleration*1.3/GL::getFPS();
    }else if(dir==Util::right){
        hSpeed-=runAceleration*1.3/GL::getFPS();
    }
}

void Entity::setAnimations(vector<vector<GLuint> > animations){
    this->animations=animations;
}
