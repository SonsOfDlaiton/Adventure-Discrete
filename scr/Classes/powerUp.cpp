#include "powerUp.hpp"
#include "Scenes.hpp"
#include "Camera.hpp"

powerUp::powerUp(int type,nTPoint pos,nTPoint size,bool haveGravity) {
    this->size=size;
    this->pos=pos;
    this->haveGravity=haveGravity;
    this->vSpeed=0;
    this->hSpeed=moveSpeed;
    this->type=type;
    this->self.push_back(this);
    if(type==0){
        if(Player::getPlayerById(0)->sword==0)
            tex=GL::getTextureByName("cogumelo0");
        if(Player::getPlayerById(0)->sword==1)
            tex=GL::getTextureByName("cogumelo1");
        if(Player::getPlayerById(0)->sword==2){
            tex=GL::getTextureByName("gcc");
            this->type=1;
        }
    }else if(type==1){
        tex=GL::getTextureByName("gcc");
    }else if(type==2){
        tex=GL::getTextureByName("cafe");
    }else if(type==3){
        tex=GL::getTextureByName("win95");
    }
};

powerUp::powerUp(const powerUp& orig) {
}

powerUp::~powerUp() {
    for(int i=0;i<self.size();i++){
        if(self[i]==this){
            self.erase(self.begin()+i);
            break;
        }
    }
}

vector<void*> powerUp::self;
const double powerUp::moveSpeed=50;

/**
 *	Run logic events of the powerups on the map like move, change textures, check if is in the screen, check collisions
**/
void powerUp::behave(){
    FunctionAnalyser::startFunction("powerUp::behave");
    if(GL::isPaused){
        FunctionAnalyser::endFunction("powerUp::behave");
        return;
    }
    double Gravity=Mechanics::getGravity()/3;
    powerUp *pu;
    for(int i=0;i<self.size();i++){
        pu=(powerUp*)self[i];
        if(!Scenes::camera.isInTheScreen(nTRectangle::getCollision(pu->pos,pu->size))){
            delete pu;
            i--;
            break;
        }
        if(pu->haveGravity){
            if(!Mechanics::checkNormalForce(pu->pos,pu->size)){
                pu->vSpeed+=Gravity/GL::getFPS();
            }else if((pu->vSpeed<0&&Gravity<0)||(pu->vSpeed>0&&Gravity>0)){
                pu->vSpeed=0;
            }
        }
        if(pu->hSpeed!=0){
            pu->move(Util::direction_left,pu->hSpeed/GL::getFPS());
        }
        if(pu->vSpeed>0){
            pu->move(Util::direction_down,pu->vSpeed/GL::getFPS());
        }else if(pu->vSpeed<0){
            pu->move(Util::direction_up,pu->vSpeed/GL::getFPS());
        }
       pu->checkCollisionWithPlayer(Player::getPlayerById(0)->pos,Player::getPlayerById(0)->size);
       pu->draw();
    }
    FunctionAnalyser::endFunction("powerUp::behave");
}

/**
 *	Override Mechanics::move to move powerups across the map
 *
 *	@param dir direction to where the powerup will move, could be Util::direction_left, Util::direction_right, Util::direction_up, Util::direction_down
 *	@param steeps amount of steps to move the powerups
**/
void powerUp::move(int dir,double steeps){
    FunctionAnalyser::startFunction("powerUp::move");
    bool needToMoveAgain=false;
    double steepsAgain=0;
    if(ABS(steeps)>Entity::walkSpeed/GL::getFPS()){
        int signal=1;
        needToMoveAgain=true;
        if(steeps<0)
            signal=-1;
        steepsAgain=signal*(ABS(steeps)-Entity::walkSpeed/GL::getFPS());
        steeps=signal*Entity::walkSpeed/GL::getFPS();
    }
    vector <mapCollision> var;
    bool collision=false;
    if(dir==Util::direction_left||dir==Util::direction_right){
      nTPoint tmp;
      tmp.set(pos.x+steeps,pos.y,pos.z);
      var=Map::checkCollision(tmp,size);
      for(int i=0; i<var.size(); i++){
        if(var[i].collision.firstObj==Mechanics::LEFT||var[i].collision.firstObj==Mechanics::RIGHT){
          collision=true;
          break;
        }
      }
      if(!collision)
        pos.x+=steeps;
      else
        hSpeed=-moveSpeed;

    }else if(dir==Util::direction_down||dir==Util::direction_up){
      nTPoint tmp;
      tmp.set(pos.x,pos.y+steeps,pos.z);
      var=Map::checkCollision(tmp,size);
      for(int i=0; i<var.size(); i++){
        if(var[i].collision.firstObj==Mechanics::BOTTOM){
          collision=true;
          break;
        }
      }
      if(!collision){
        pos.y+=steeps;
      }
    }
    FunctionAnalyser::endFunction("powerUp::move");
    if(needToMoveAgain)
        move(dir,steepsAgain);
}

/**
 *	Check collisions with the player and make actions case it so
 *
 *	@param pos position of the player
 *	@param size size of the player
**/
void powerUp::checkCollisionWithPlayer(nTPoint pos,nTPoint size){
    FunctionAnalyser::startFunction("powerUp::checkCollWPlayer");
    objCollision var;
    var=Mechanics::getCollision(nTRectangle::getCollision(this->pos,this->size),nTRectangle::getCollision(pos,size));
    if(var.firstObj!=Mechanics::NOCOLLISION){
        AL::singleton->playSoundByName("powerUp");
        if(type==0){
            Player::getPlayerById(0)->life++;
            Player::getPlayerById(0)->sword++;
            if(Player::getPlayerById(0)->sword>2||Player::getPlayerById(0)->life>Player::defaultLife){
                Player::getPlayerById(0)->sword=Player::defaultLife-1;
                Player::getPlayerById(0)->life=Player::defaultLife;
            }
        }else if(type==1){
            if(!Scenes::freeGameMode)Player::getPlayerById(0)->lives++;
        }else if(type==2){
            Player::getPlayerById(0)->makeInvencible(7000);
            AL::singleton->playSoundByName("cafeSong");
        }else if(type==3){
            Player::getPlayerById(0)->sword=0;
            Player::getPlayerById(0)->life=1;
        }
        delete this;
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
