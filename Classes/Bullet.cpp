#include "Bullet.hpp"
#include "AL.hpp"
#include "Scenes.hpp"

Bullet::Bullet(int type,float moveSpeed,nTPoint pos,nTPoint size) {
    this->size=size;
    this->pos=pos;
    this->hSpeed=moveSpeed;
    this->type=type;
    this->self.push_back(this);
    this->spriteIndex=0;
    this->GLlist=glGenLists(1);
    this->isVisible=true;
    if(type==0){
        tex=GL::getTextureByName("WinBullet");al->playSoundByName("blockShoot");
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
float Bullet::baseDamage=1;
float Bullet::baseSpeed=280;
float Bullet::timeToShoot=5000;

void Bullet::behave(){
    if(GL::isPaused)
        return;
    Bullet *bu; 
    for(int i=0;i<self.size();i++){
        bu=(Bullet*)self[i];
        if(!Scenes::isInTheScreen(Util::getCollisionRectangle(bu->pos,bu->size))){
            if(bu->type==1)
                player->haveBulletSword=false;
            if(bu->type==2||bu->type==3)
                player->haveBulletSpec=false;
            if(bu->type==4)
                al->stopSound(AL::getSoundByName("intercampi"));
            bu->isVisible=false;
            delete bu;
            return;
        }
        if(bu->hSpeed!=0){
            bu->move(Util::left,bu->hSpeed/GL::getFPS());
        }  
        if(!bu->isVisible)
            return;
        if(bu->type==0||bu->type==5){//tiro de bloco
            bu->checkCollisionWithEntity(player->pos,player->size,true);
        }else if(bu->type==1){//espada
            if(round(fmodl(GL::getGameMs(),(int)Entity::getSpriteMs()*3))==0){
                if(bu->tex==GL::getTextureByName("SwordBullet0"))
                    bu->tex=GL::getTextureByName("SwordBullet1");
                else
                    bu->tex=GL::getTextureByName("SwordBullet0");
            }
            bu->checkCollisionWithEntity(Util::nTPointSet(0,0,0),Util::nTPointSet(0,0,0),false);
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
            bu->checkCollisionWithEntity(Util::nTPointSet(0,0,0),Util::nTPointSet(0,0,0),false);     
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
            bu->checkCollisionWithEntity(Util::nTPointSet(0,0,0),Util::nTPointSet(0,0,0),false);
        }else if(bu->type==4){//intercamp
            bu->checkCollisionWithEntity(player->pos,player->size,true);
            al->playSoundByName("intercampi");
        }
        if(!bu->isVisible)
            return;
       bu->draw();
    }
}

void Bullet::move(int dir,float steeps){
    bool needToMoveAgain=false;
    float steepsAgain;  
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
        bool condition=true;
        if(dir==Util::left||dir==Util::right){
          nTPoint tmp;
          tmp.setPoint(pos.x+steeps,pos.y,pos.z);
          var=Map::checkCollision(tmp,size);
          for(int i=0; i<var.size(); i++){
            if(var[i].collision.firstObj>=3)
              condition=false;
          }
          if(condition)
            pos.x+=steeps;
          else{
            if(type==1)
                player->haveBulletSword=false;
            if(type==2||type==3)
                player->haveBulletSpec=false;
            if(type>=0){
                isVisible=false;
                delete this;
            }
            if(!isVisible)
                return;
            hSpeed*=-1;
          }

        }
    }else{
        pos.x+=steeps;
    }
    if(needToMoveAgain)
        move(dir,steepsAgain);
}

void Bullet::checkCollisionWithEntity(nTPoint pos,nTPoint size, bool withPlayer){
    objCollision var;
    if(withPlayer){
        var=Mechanics::getCollision(Util::getCollisionRectangle(this->pos,this->size),Util::getCollisionRectangle(pos,size));
        if(var.firstObj){
            if(type==4){
                player->applyDamage(2);
                al->stopSound(AL::getSoundByName("intercampi"));
            }else
                player->applyDamage(1);
            isVisible=false;
            delete this;
        }else
        if(type!=4){
            var=Mechanics::getCollision(Util::getCollisionRectangle(this->pos,this->size),player->swordCollision);
            if(var.firstObj){
//                player->atacking=false;
                isVisible=false;
                delete this;
            }
        }
    }else{
        Enemy *en;
        for(int i=0;i<Entity::enemys.size();i++){
            en=(Enemy*)Entity::enemys[i];
            var=Mechanics::getCollision(Util::getCollisionRectangle(this->pos,this->size),Util::getCollisionRectangle(en->pos,en->size));
            if(var.firstObj){       
                if(type==1)
                    player->haveBulletSword=false;
                if(type==2||type==3)
                    player->haveBulletSpec=false;
                en->applyDamage(player->sword);
                if(en->life<=0)
                        Player::enemysKilled++;
                isVisible=false;
                pos.x=-100;
                //delete this;
            }
            if(type==1){
                Bullet* bu;
                for(int i=0;i<Bullet::self.size();i++){
                    bu=(Bullet*)Bullet::self[i];
                    if(bu->type!=1&&bu->type!=3&&bu->type!=2){
                        if(Mechanics::getCollision(Util::getCollisionRectangle(this->pos,this->size),Util::getCollisionRectangle(bu->pos,bu->size)).firstObj){
                            isVisible=false;
                            pos.x=-100;
                            player->haveBulletSword=false;
                        }                                
                    }
                }
            }
        }
    }
}

void Bullet::draw(){
    if(GL::isPaused)
        return;
    glNewList(GLlist,GL_COMPILE_AND_EXECUTE);
    pos.z=1;
    if (type==1)
        pos.z=0.9;
    if(isVisible)
        GL::drawTexture(Util::getCollisionRectangle(pos,size),GL::getColorByName("white"),tex,1);
    glEndList();
}