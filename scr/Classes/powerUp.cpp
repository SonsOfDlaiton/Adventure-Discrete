#include "powerUp.hpp"
#include "Scenes.hpp"
#include "Camera.hpp"

powerUp::powerUp(int type,nTPoint pos,nTPoint size) {
    this->size=size;
    this->pos=pos;
    this->vSpeed=0;
    this->hSpeed=moveSpeed;
    this->type=type;
    this->isVisible=true;
    this->self.push_back(this);
    this->id=(unsigned int)self.size()-1;
    if(type==swordUpgrade){
        if(Player::getPlayerById(0)->sword==0)
            tex=GL::getTextureByName("cogumelo0");
        if(Player::getPlayerById(0)->sword==1)
            tex=GL::getTextureByName("cogumelo1");
        if(Player::getPlayerById(0)->sword==2){
            tex=GL::getTextureByName("gcc");
            this->type=1;
        }
    }else if(type==extraLife){
        tex=GL::getTextureByName("gcc");
    }else if(type==imortality){
        tex=GL::getTextureByName("cafe");
    }else if(type==minusLife){
        tex=GL::getTextureByName("win95");
    }
};

powerUp::powerUp(const powerUp& orig) {
}

powerUp::~powerUp() {
    powerUp* pu;
    for(int i=id+1;i<self.size();i++){
        pu=(powerUp*)self[i];
        pu->id--;
    }
    self.erase(self.begin()+this->id);
}

vector<void*> powerUp::self;
const double powerUp::moveSpeed=50;

const int powerUp::swordUpgrade=0;
const int powerUp::extraLife=1;
const int powerUp::imortality=2;
const int powerUp::minusLife=3;

/**
 *	Run logic events of the powerups on the map like move, change textures, check if is in the screen, check collisions
**/
void powerUp::behave(){
    FunctionAnalyser::startFunction("powerUp::behave");
    if(GL::isPaused){
        FunctionAnalyser::endFunction("powerUp::behave");
        return;
    }
    powerUp *pu;
    for(int i=0;i<self.size();i++){
        pu=(powerUp*)self[i];
        if(!pu->isVisible||!Scenes::camera.isInTheScreen(nTRectangle::getCollision(pu->pos,pu->size))){
            delete pu;
        }else{
            pu->checkCollisionWithPlayer(Player::getPlayerById(0)->pos,Player::getPlayerById(0)->size);
            pu->draw();
        }
    }
    FunctionAnalyser::endFunction("powerUp::behave");
}


/**
 *	Check collisions with the player and make actions case it so
 *
 *	@param pos position of the player
 *	@param size size of the player
**/
void powerUp::checkCollisionWithPlayer(nTPoint pos,nTPoint size){
    if(!isVisible) return;
    FunctionAnalyser::startFunction("powerUp::checkCollWPlayer");
    objCollision var;
    var=Mechanics::getCollision(nTRectangle::getCollision(this->pos,this->size),nTRectangle::getCollision(pos,size));
    if(var.firstObj!=Mechanics::NOCOLLISION){
        AL::singleton->playSoundByName("powerUp");
        if(type==swordUpgrade){
            Player::getPlayerById(0)->life++;
            Player::getPlayerById(0)->sword++;
            if(Player::getPlayerById(0)->sword>2||Player::getPlayerById(0)->life>Player::defaultLife){
                Player::getPlayerById(0)->sword=Player::defaultLife-1;
                Player::getPlayerById(0)->life=Player::defaultLife;
            }
        }else if(type==extraLife){
            if(!Scenes::freeGameMode)Player::getPlayerById(0)->lives++;
        }else if(type==imortality){
            Player::getPlayerById(0)->makeInvencible(7000);
            AL::singleton->playSoundByName("cafeSong");
        }else if(type==minusLife){
            Player::getPlayerById(0)->sword=0;
            Player::getPlayerById(0)->life=1;
        }
        isVisible=false;
    }
    FunctionAnalyser::endFunction("powerUp::checkCollWPlayer");
}

/**
 *	Draw this powerUp on the screen
**/
void powerUp::draw(){
    if(GL::isPaused)
        return;
    pos.z=1;
    GL::drawTexture(nTRectangle::getCollision(pos,size),tex);
    if(Mechanics::drawCollisionRec)GL::drawCollision(nTRectangle::getCollision(pos, size));
}
