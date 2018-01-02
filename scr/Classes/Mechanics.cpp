#include "Mechanics.hpp"
#include "Player.hpp"
#include "Boss.hpp"
#include "PowerUp.hpp"
#include "Enemy.hpp"
#include "Blocks.hpp"
#include "Camera.hpp"

Mechanics::Mechanics(){
};

Mechanics::Mechanics(const Mechanics& orig) {
}

Mechanics::~Mechanics() {
}

const double Mechanics::gravity=500.0;
const int Mechanics::TOP=1;
const int Mechanics::BOTTOM=2;
const int Mechanics::RIGHT=3;
const int Mechanics::LEFT=4;
const int Mechanics::NOCOLLISION=0;
bool Mechanics::drawCollisionRec=false;

/**
 *	Apply gravity to the instantiated entitities on the world
**/
void Mechanics::applyGravity(){
    FunctionAnalyser::startFunction("Mechanics::applyGravity");
    if(GL::isPaused||Tutorials::isPaused){//||!Scenes::camera.isInTheXScreen(nTRectangle::getCollision(this->pos,this->size)))
        FunctionAnalyser::endFunction("Mechanics::applyGravity");
        return;
    }
    Player *pl;
    for(int i=0;i<Entity::players.size();i++){
        pl=(Player*)Entity::players[i];
        pl->lastMapCollCalc=false;
        if(!pl->itsInTheWater){
            if(!pl->checkNormalForce(pl->getGroundPos())){
                pl->lastMapCollCalc=false;
                pl->vSpeed+=gravity/GL::getFPS();
                if(!pl->god)pl->canJump=false;
            }else{
                if(pl->vSpeed>0)
                    pl->vSpeed=0;
                if(!pl->spacePressed)
                    pl->canJump=true;
            }
        }else{
            if(!pl->checkNormalForce())
                pl->vSpeed+=gravity/GL::getFPS()/3;
            else if(pl->vSpeed>0)
                pl->vSpeed=0;
            pl->canJump=true;
        }
    }
    Enemy *en;
    for(int i=0;i<Entity::enemys.size();i++){
        en=(Enemy*)Entity::enemys[i];
        en->lastMapCollCalc=false;
        if(!en->checkNormalForce()){
            en->vSpeed+=gravity/GL::getFPS();
            en->canJump=false;
        }else{
            if(en->vSpeed>0)
                en->vSpeed=0;
            en->canJump=true;
        }
    }
    Boss *bo;
    for(int i=0;i<Entity::bosses.size();i++){
        bo=(Boss*)Entity::bosses[i];
        bo->lastMapCollCalc=false;
        if(!bo->checkNormalForce()){
            bo->vSpeed+=gravity/GL::getFPS();
            bo->canJump=false;
        }else{
            if(bo->vSpeed>0)
                bo->vSpeed=0;
            bo->canJump=true;
        }
    }

    PowerUp *pu;
    for(int i=0;i<PowerUp::self.size();i++){
        pu=(PowerUp*)PowerUp::self[i];
        pu->lastMapCollCalc=false;
        if(!pu->checkNormalForce()){
            pu->vSpeed+=gravity/2/GL::getFPS();
        }else if(pu->vSpeed>0){
            pu->vSpeed=0;
        }
    }


    FunctionAnalyser::endFunction("Mechanics::applyGravity");
}

/**
 *  Transforms speed into moviment for the instantiated entitities on the world
**/
void Mechanics::applyForce(){
    FunctionAnalyser::startFunction("Mechanics::applyForce");
    applyGravity();
    if(GL::isPaused||Tutorials::isPaused){//||!Scenes::camera.isInTheScreen(nTRectangle::getCollision(this->pos,this->size)))
        FunctionAnalyser::endFunction("Mechanics::applyForce");
        return;
    }
    Player *pl;
    for(int i=0;i<Entity::players.size();i++){
        pl=(Player*)Entity::players[i];
        if(pl->hSpeed!=0){
            pl->move(Util::direction_left,pl->hSpeed/GL::getFPS());
        }
        if(pl->vSpeed>0){
            pl->move(Util::direction_down,pl->vSpeed/GL::getFPS());
        }else if(pl->vSpeed<0){
            pl->move(Util::direction_up,pl->vSpeed/GL::getFPS());
        }
    }

    Enemy *en;
    for(int i=0;i<Entity::enemys.size();i++){
        en=(Enemy*)Entity::enemys[i];
        if(en->hSpeed!=0){
            double tmp=en->hSpeed;
            en->move(Util::direction_left,en->hSpeed/GL::getFPS());
            if(en->hSpeed==0)
                en->hSpeed=-tmp;
        }
        if(en->vSpeed>0){
            en->move(Util::direction_down,en->vSpeed/GL::getFPS());
        }else if(en->vSpeed<0){
            en->move(Util::direction_up,en->vSpeed/GL::getFPS());
        }
    }

    Boss *bo;
    for(int i=0;i<Entity::bosses.size();i++){
        bo=(Boss*)Entity::bosses[i];
        if(bo->hSpeed!=0){
            double tmp=bo->hSpeed;
            bo->move(Util::direction_left,bo->hSpeed/GL::getFPS());
            if(bo->hSpeed==0)
                bo->hSpeed=-tmp;
        }
        if(bo->vSpeed>0){
            bo->move(Util::direction_down,bo->vSpeed/GL::getFPS());
        }else if(bo->vSpeed<0){
            bo->move(Util::direction_up,bo->vSpeed/GL::getFPS());
        }
    }

    PowerUp *pu;
    for(int i=0;i<PowerUp::self.size();i++){
        pu=(PowerUp*)PowerUp::self[i];
        if(pu->hSpeed!=0){
            double tmp=pu->hSpeed;
            pu->move(Util::direction_left,pu->hSpeed/GL::getFPS());
            if(pu->hSpeed==0)
                pu->hSpeed=-tmp;
        }
        if(pu->vSpeed>0){
            pu->move(Util::direction_down,pu->vSpeed/GL::getFPS());
        }else if(pu->vSpeed<0){
            pu->move(Util::direction_up,pu->vSpeed/GL::getFPS());
        }
    }

    Bullet *bu;
    for(int i=0;i<Bullet::self.size();i++){
        bu=(Bullet*)Bullet::self[i];
        if(bu->hSpeed!=0){
            bu->move(Util::direction_left,bu->hSpeed/GL::getFPS());
        }
    }

    FunctionAnalyser::endFunction("Mechanics::applyForce");
}

/**
 *	Move entitities arround the world
 *
 *	@param dir direction to where the bullet will move, could be Util::direction_left, Util::direction_right, Util::direction_up, Util::direction_down
 *	@param steeps amount of steps to move the block
**/
void Mechanics::move(int dir,double steeps){
    FunctionAnalyser::startFunction("Mechanics::move");
    if(!Scenes::camera.isInTheXScreen(nTRectangle::getCollision(this->pos,this->size))){
        FunctionAnalyser::endFunction("Mechanics::move");
        return;
    }
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
    Blocks *bl;
    int type;
    bool collision=false;
    if(dir==Util::direction_left||dir==Util::direction_right){
      nTPoint tmp;
      tmp.set(pos.x+steeps,pos.y,pos.z);
      collideWithMap(tmp);
      for(int i=0; i<lastMapColl.size(); i++){
        bl=Map::getBlockById(lastMapColl[i].blockId);
        type=bl->type;
        if((lastMapColl[i].collision.firstObj==RIGHT||lastMapColl[i].collision.firstObj==LEFT)&&Blocks::checkIfBlocksIsFilled(type)&&!ignoreCollisionWithPlayer(pos,type)){
          collision=true;
          break;
        }
      }
      if(!collision)
        pos.x+=steeps;
      else
        hSpeed=0;
    }else if(dir==Util::direction_up||dir==Util::direction_down){
      nTPoint tmp;
      tmp.set(pos.x,pos.y+steeps,pos.z);
      collideWithMap(tmp);
      for(int i=0; i<lastMapColl.size(); i++){
        bl=Map::getBlockById(lastMapColl[i].blockId);
        type=bl->type;
        bool fixPlayerInVerticalPlatform=!(Blocks::checkIfBlocksIsHalfBlockV(type)&&dir==Util::direction_down);
        if((lastMapColl[i].collision.firstObj==BOTTOM||lastMapColl[i].collision.firstObj==TOP)&&fixPlayerInVerticalPlatform&&Blocks::checkIfBlocksIsFilled(type)&&!ignoreCollisionWithPlayer(pos,type)){
            collision=true;
            break;
          }
      }
      if(!collision)
        pos.y+=steeps;
      else
        vSpeed=0;
    }
    FunctionAnalyser::endFunction("Mechanics::move");
    if(needToMoveAgain)
        move(dir,steepsAgain);
}


bool Mechanics::checkNormalForce(){
    return checkNormalForce(pos);
}

/**
 *	Check if a body is touching the ground
 *
 *	@param pos_ position of the body
 *	@param size_ size of the body
 *	@return true if is touching the ground, otherwise false
**/
bool Mechanics::checkNormalForce(nTPoint pos_){
    FunctionAnalyser::startFunction("Mechanics::checkNormalForce");
    Blocks *bl;
    bool collision=false;
    collideWithMap(pos_);
    for(int i=0;i<lastMapColl.size();i++){
        bl=Map::getBlockById(lastMapColl[i].blockId);
        bool jesus=(Player::getPlayerById(0)->god&&Blocks::checkIfBlocksIsLiquid(bl->type));
        if(lastMapColl[i].collision.firstObj==BOTTOM&&(Blocks::checkIfBlocksIsFilled(bl->type)||jesus)&&!ignoreCollisionWithPlayer(pos_,bl->type)){
          collision=true;
          break;
        }
      }
    FunctionAnalyser::endFunction("Mechanics::checkNormalForce");
    return collision;
}

/**
 *	Check a block collision with the player must be ignored
 *
 *	@param pos position of the body
 *	@param blockType the type of the block
 *	@return true if is must be ignored, otherwise false
**/
bool Mechanics::ignoreCollisionWithPlayer(nTPoint pos,int blockType){
    if(!Blocks::checkIfBlocksIsEnemyCollider(blockType))
        return false;
    else{
        if(pos.x==Player::getPlayerById(0)->pos.x&&(pos.y==Player::getPlayerById(0)->pos.y||pos.y==Player::getPlayerById(0)->getGroundPos().y))
            return true;
    }
    return false;
}

/**
 *	Gets gravity
 *
 *	@return the value of gravity
**/
double Mechanics::getGravity(){
    return gravity;
}

/**
 *	Verify collision between two bodies
 *
 *	@param r1 coordinates of the collision rectangle of the first body
  *	@param r2 coordinates of the collision rectangle of the second body
 *	@return the collsion result, with informations about where the bodies touch each other
**/
objCollision Mechanics::getCollision(nTRectangle r1, nTRectangle r2){
    FunctionAnalyser::startFunction("Mechanics::getCollision");
    objCollision adc;
    nTPoint r1Center=nTPoint::get((r1.p1.x+r1.p0.x)/2, (r1.p1.y+r1.p0.y)/2, r1.p0.z);
    nTPoint r2Center=nTPoint::get((r2.p1.x+r2.p0.x)/2, (r2.p1.y+r2.p0.y)/2, r2.p0.z);
    nTPoint r1Size=nTPoint::get(r1.p1.x-r1.p0.x,r1.p0.y-r1.p1.y, r1.p0.z);
    nTPoint r2Size=nTPoint::get(r2.p1.x-r2.p0.x,r2.p0.y-r2.p1.y, r2.p0.z);
    double w=(r1Size.x+r2Size.x)/2;
    double h=(r1Size.y+r2Size.y)/2;
    double dx=r1Center.x-r2Center.x;
    double dy=r2Center.y-r1Center.y;
    if(ABS(dx)<=w&&ABS(dy)<=h){//colisao
        double wy=w*dy;
        double hx=h*dx;
        if(wy>hx){
            if(wy>-hx){
                adc.firstObj=BOTTOM;
                adc.secondObj=TOP;
                if(Mechanics::drawCollisionRec)
                    GL::drawCollisionLine(nTPoint::get(r1.p0.x, r1.p0.y),nTPoint::get(r1.p1.x, r1.p0.y));
            }else{
                adc.firstObj=RIGHT;
                adc.secondObj=LEFT;
                if(Mechanics::drawCollisionRec)
                    GL::drawCollisionLine(nTPoint::get(r1.p1.x, r1.p0.y),nTPoint::get(r1.p1.x, r1.p1.y));
            }
        }else{
            if(wy>-hx){
                adc.firstObj=LEFT;
                adc.secondObj=RIGHT;
                if(Mechanics::drawCollisionRec)
                    GL::drawCollisionLine(nTPoint::get(r1.p0.x, r1.p0.y),nTPoint::get(r1.p0.x, r1.p1.y));
            }else{
                adc.firstObj=TOP;
                adc.secondObj=BOTTOM;
                if(Mechanics::drawCollisionRec)
                    GL::drawCollisionLine(nTPoint::get(r1.p0.x, r1.p1.y),nTPoint::get(r1.p1.x, r1.p1.y));
            }
        }
    }else{
      adc.firstObj=adc.secondObj=NOCOLLISION;
    }
    FunctionAnalyser::endFunction("Mechanics::getCollision");
    return adc;
}

void Mechanics::collideWithMap(nTPoint pos_){
    if(lastMapCollCalc==false||floor(pos_.x)!=floor(lastMapCollPos.x)||ceil(pos_.y)!=ceil(lastMapCollPos.y)){
        lastMapCollCalc=true;
        lastMapCollPos=pos_;
        lastMapColl=Map::checkCollision(pos_,size);
    }
}


void Mechanics::collideWithMap(){
    collideWithMap(pos);
}
