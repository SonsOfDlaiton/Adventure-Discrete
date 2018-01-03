#include "PowerUp.hpp"
#include "Scenes.hpp"
#include "Camera.hpp"

PowerUp::PowerUp(int type,nTPoint pos,nTPoint size) {
    this->size=size;
    this->pos=pos;
    this->vSpeed=0;
    this->hSpeed=moveSpeed;
    this->type=type;
    this->isVisible=true;
    this->self.push_back(this);
    this->id=(unsigned int)self.size()-1;
    this->tex=new Texture();
    if(type==swordUpgrade){
        if(Player::getPlayerById(0)->sword==0)
            tex->setTextures(GL::getTextureByName("cogumelo0"));
        if(Player::getPlayerById(0)->sword==1)
            tex->setTextures(GL::getTextureByName("cogumelo1"));
        if(Player::getPlayerById(0)->sword==2){
            tex->setTextures(GL::getTextureByName("gcc"));
            this->type=1;
        }
    }else if(type==extraLife){
        tex->setTextures(GL::getTextureByName("gcc"));
    }else if(type==imortality){
        tex->setTextures(GL::getTextureByName("cafe"));
    }else if(type==minusLife){
        tex->setTextures(GL::getTextureByName("win95"));
    }
};

PowerUp::PowerUp(const PowerUp& orig) {
}

PowerUp::~PowerUp() {
    delete tex;
    PowerUp* pu;
    for(int i=id+1;i<self.size();i++){
        pu=(PowerUp*)self[i];
        pu->id--;
    }
    self.erase(self.begin()+this->id);
}

vector<void*> PowerUp::self;
const double PowerUp::moveSpeed=50;

const int PowerUp::swordUpgrade=0;
const int PowerUp::extraLife=1;
const int PowerUp::imortality=2;
const int PowerUp::minusLife=3;

/**
 *	Run logic events of the PowerUps on the map like move, change textures, check if is in the screen, check collisions
**/
void PowerUp::behave(){
    FunctionAnalyser::startFunction("PowerUp::behave");
    if(GL::isPaused){
        FunctionAnalyser::endFunction("PowerUp::behave");
        return;
    }
    PowerUp *pu;
    for(int i=0;i<self.size();i++){
        pu=(PowerUp*)self[i];
        if(!pu->isVisible||!Scenes::camera.isInTheScreen(nTRectangle::getCollision(pu->pos,pu->size))){
            delete pu;
        }else{
            pu->checkCollisionWithPlayer(Player::getPlayerById(0)->pos,Player::getPlayerById(0)->size);
            pu->draw();
        }
    }
    FunctionAnalyser::endFunction("PowerUp::behave");
}


/**
 *	Check collisions with the player and make actions case it so
 *
 *	@param pos position of the player
 *	@param size size of the player
**/
void PowerUp::checkCollisionWithPlayer(nTPoint pos,nTPoint size){
    if(!isVisible) return;
    FunctionAnalyser::startFunction("PowerUp::checkCollWPlayer");
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
    FunctionAnalyser::endFunction("PowerUp::checkCollWPlayer");
}

/**
 *	Draw this PowerUp on the screen
**/
void PowerUp::draw(){
    if(GL::isPaused)
        return;
    pos.z=1;
    GL::drawTexture(nTRectangle::getCollision(pos,size),tex->get());
    if(Mechanics::drawCollisionRec)GL::drawCollision(nTRectangle::getCollision(pos, size));
}
