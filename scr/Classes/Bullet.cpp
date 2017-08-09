#include "Bullet.hpp"

Bullet::Bullet(int type,float moveSpeed,nTPoint pos,nTPoint size) {
    this->size=size;
    this->pos=pos;
    this->hSpeed=moveSpeed;
    this->type=type;
    this->self.push_back(this);
    this->spriteIndex=0;
    this->isVisible=true;
    if(type==0){
        tex=GL::getTextureByName("WinBullet");
        AL::singleton->playSoundByName("blockShoot");
    }
    if(type==1)
        tex=GL::getTextureByName("SwordBullet0");
    if(type==2)
        tex=GL::getTextureByName("StrongAtk0");
    if(type==3)
        tex=GL::getTextureByName("WeakAtk0");
    if(type==4)
        tex=GL::getTextureByName("Intercampi");
     if(type==5)
        tex=GL::getTextureByName("paraboloide hiperbolico<3");
};

Bullet::Bullet(const Bullet& orig) {
}

Bullet::~Bullet() {
    for(int i=0;i<self.size();i++){
        if(self[i]==this){
            self.erase(self.begin()+i);
            break;
        }
    }
}

vector<void*> Bullet::self;
const float Bullet::baseDamage=1;
const float Bullet::baseSpeed=280;
const float Bullet::timeToShoot=5000;

/**
 *	Run logic events of the Bullets on the map like move, change textures, check if is in the screen, check collisions
**/
void Bullet::behave(){
    FunctionAnalyser::startFunction("Bullet::behave");
    if(GL::isPaused){
        FunctionAnalyser::endFunction("Bullet::behave");
        return;
    }
    Bullet *bu;
    for(int i=0;i<self.size();i++){
        bu=(Bullet*)self[i];
        if(!Scenes::camera.isInTheScreen(nTRectangle::getCollision(bu->pos,bu->size))){
            if(bu->type==1)
                Player::getPlayerById(0)->haveBulletSword=false;
            if(bu->type==2||bu->type==3)
                Player::getPlayerById(0)->haveBulletSpec=false;
            if(bu->type==4)
                AL::singleton->stopSound(AL::getSoundByName("intercampi"));
            bu->isVisible=false;
            delete bu;
            FunctionAnalyser::endFunction("Bullet::behave");
            return;
        }
        if(bu->hSpeed!=0){
            bu->move(Util::direction_left,bu->hSpeed/GL::getFPS());
        }
        if(!bu->isVisible){
            FunctionAnalyser::endFunction("Bullet::behave");
            return;
        }
        if(bu->type==0||bu->type==5){//tiro de bloco
            bu->checkCollisionWithEntity(Player::getPlayerById(0)->pos,Player::getPlayerById(0)->size,true);
        }else if(bu->type==1){//espada
            if(round(fmodl(GL::getGameMs(),(int)Entity::getSpriteMs()*3))==0){
                if(bu->tex==GL::getTextureByName("SwordBullet0"))
                    bu->tex=GL::getTextureByName("SwordBullet1");
                else
                    bu->tex=GL::getTextureByName("SwordBullet0");
            }
            bu->checkCollisionWithEntity(nTPoint::Origin(),nTPoint::Origin(),false);
        }else if(bu->type==2){//atk forte
            if(round(fmodl(GL::getGameMs(),(int)Entity::getSpriteMs()))==0){
                bu->spriteIndex++;
                if(bu->spriteIndex==4)
                    bu->spriteIndex=0;
                char buffer[5];
                snprintf(buffer,5,"%d",bu->spriteIndex);
                string str(buffer);
                bu->tex=GL::getTextureByName("StrongAtk"+str);
            }
            bu->checkCollisionWithEntity(nTPoint::Origin(),nTPoint::Origin(),false);
        }else if(bu->type==3){//atk fraco
            if(round(fmodl(GL::getGameMs(),(int)Entity::getSpriteMs()))==0){
                bu->spriteIndex++;
                if(bu->spriteIndex==3)
                    bu->spriteIndex=0;
                char buffer[5];
                snprintf(buffer,5,"%d",bu->spriteIndex);
                string str(buffer);
                bu->tex=GL::getTextureByName("WeakAtk"+str);
            }
            bu->checkCollisionWithEntity(nTPoint::Origin(),nTPoint::Origin(),false);
        }else if(bu->type==4){//intercamp
            bu->checkCollisionWithEntity(Player::getPlayerById(0)->pos,Player::getPlayerById(0)->size,true);
            AL::singleton->playSoundByName("intercampi");
        }
        if(!bu->isVisible){
            FunctionAnalyser::endFunction("Bullet::behave");
            return;
        }
       bu->draw();
    }
    FunctionAnalyser::endFunction("Bullet::behave");
}

/**
 *	Override Mechanics::move to move bullets across the map
 *
 *	@param dir direction to where the bullet will move, could be Util::direction_left, Util::direction_right, Util::direction_up, Util::direction_down
 *	@param steeps amount of steps to move the bullet
**/
void Bullet::move(int dir,float steeps){
    FunctionAnalyser::startFunction("Bullet::move");
    bool needToMoveAgain=false;
    float steepsAgain=0;
    if(ABS(steeps)>Entity::walkSpeed/GL::getFPS()){
        int signal=1;
        needToMoveAgain=true;
        if(steeps<0)
            signal=-1;
        steepsAgain=signal*(ABS(steeps)-Entity::walkSpeed/GL::getFPS());
        steeps=signal*Entity::walkSpeed/GL::getFPS();
    }
    if(type!=4){
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
          else{
            if(type==1)
                Player::getPlayerById(0)->haveBulletSword=false;
            if(type==2||type==3)
                Player::getPlayerById(0)->haveBulletSpec=false;
            if(type>=0){
                isVisible=false;
                delete this;
            }
            if(!isVisible){
                FunctionAnalyser::endFunction("Bullet::move");
                return;
            }
            hSpeed*=-1;
          }

        }
    }else{
        pos.x+=steeps;
    }
    FunctionAnalyser::endFunction("Bullet::move");
    if(needToMoveAgain)
        move(dir,steepsAgain);
}

/**
 *	Check collisions with entities and make actions case it so
 *
 *	@param pos position of the entity
 *	@param size size of the entity
 *	@param withPlayer must be true case the entity in question is the player
**/
void Bullet::checkCollisionWithEntity(nTPoint pos,nTPoint size, bool withPlayer){
    FunctionAnalyser::startFunction("Bullet::ckCollWithEntity");
    objCollision var;
    if(withPlayer){
        var=Mechanics::getCollision(nTRectangle::getCollision(this->pos,this->size),nTRectangle::getCollision(pos,size));
        if(var.firstObj!=Mechanics::NOCOLLISION){
            if(type==4){
                Player::getPlayerById(0)->applyDamage(2);
                AL::singleton->stopSound(AL::getSoundByName("intercampi"));
            }else
                Player::getPlayerById(0)->applyDamage(1);
            isVisible=false;
            delete this;
        }else if(type!=4){
            var=Mechanics::getCollision(nTRectangle::getCollision(this->pos,this->size),Player::getPlayerById(0)->swordCollision);
            if(var.firstObj!=Mechanics::NOCOLLISION){
//              Player::getPlayerById(0)->atacking=false;
                isVisible=false;
                delete this;
            }
        }
    }else{
        Enemy *en;
        for(int i=0;i<Entity::enemys.size();i++){
            en=(Enemy*)Entity::enemys[i];
            var=Mechanics::getCollision(nTRectangle::getCollision(this->pos,this->size),nTRectangle::getCollision(en->pos,en->size));
            if(var.firstObj!=Mechanics::NOCOLLISION){
                if(type==1)
                    Player::getPlayerById(0)->haveBulletSword=false;
                if(type==2||type==3)
                    Player::getPlayerById(0)->haveBulletSpec=false;
                en->applyDamage(Player::getPlayerById(0)->sword);
                if(en->life<=0)
                        Player::enemysKilled++;
                isVisible=false;
                pos.x=-100;
                delete this;
                i--;
                FunctionAnalyser::endFunction("Bullet::ckCollWithEntity");
                return;
            }
            if(type==1){
                Bullet* bu;
                for(int i=0;i<Bullet::self.size();i++){
                    bu=(Bullet*)Bullet::self[i];
                    if(bu->type!=1&&bu->type!=2&&bu->type!=3){
                        if(Mechanics::getCollision(nTRectangle::getCollision(this->pos,this->size),nTRectangle::getCollision(bu->pos,bu->size)).firstObj!=Mechanics::NOCOLLISION){
                            isVisible=false;
                            pos.x=-100;
                            Player::getPlayerById(0)->haveBulletSword=false;
                            bu->isVisible=false;
                            delete bu;
                        }
                    }
                }
            }
        }
    }
    FunctionAnalyser::endFunction("Bullet::ckCollWithEntity");
}

/**
 *	Draw this bullet on the screen
**/
void Bullet::draw(){
    if(GL::isPaused||!Scenes::camera.isInTheScreen(nTRectangle::getCollision(pos,size)))
        return;
    pos.z=1;
    if (type==1)
        pos.z=0.9;
    if(isVisible){
        GL::drawTexture(nTRectangle::getCollision(pos,size),tex);
        if(Mechanics::drawCollisionRec)GL::drawCollision(nTRectangle::getCollision(pos, size));
    }
}
