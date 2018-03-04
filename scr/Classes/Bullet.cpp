#include "Bullet.hpp"
#include "Boss.hpp"

Bullet::Bullet(int type,double moveSpeed,nTPoint pos,nTPoint size) {
    this->size=size;
    this->pos=pos;
    this->type=type;
    this->self.push_back(this);
    this->isVisible=true;
    this->tex=new Texture();
    if(type==errorBlockBullet){
        tex->setTextures(GL::getTextureByName("WinBullet"));
        AL::singleton->playSoundByName("blockShoot");
    }
    if(type==coinBullet){
        tex->setTextures(GL::getTexturesByName("Coin",7));
        tex->setTimeToNext(70);
    }
    if(type==strongSwordBullet)
        tex->setTextures(GL::getTexturesByName("SwordBullet",2));
    if(type==strongXAtackBullet)
        tex->setTextures(GL::getTexturesByName("StrongAtk",4));
    if(type==weakXAtackBullet)
        tex->setTextures(GL::getTexturesByName("WeakAtk",3));
    if(type==busBullet)
        tex->setTextures(GL::getTextureByName("Intercampi"));
    if(type==hyperbolicParaboloidBullet)
        tex->setTextures(GL::getTextureByName("paraboloide hiperbolico<3"));
    if(type==rightAlternativeBullet||type==wrongAlternativeBullet||type==gateBullet){
        this->hSpeed=0;
        this->vSpeed=moveSpeed;
    }else
        this->hSpeed=moveSpeed;
};

Bullet::Bullet(const Bullet& orig) {
}

Bullet::~Bullet() {
    delete tex;
    for(int i=0;i<self.size();i++){
        if(self[i]==this){
            self.erase(self.begin()+i);
            break;
        }
    }
}

vector<void*> Bullet::self;
const double Bullet::baseDamage=Constants::BULLET_BaseDamage;
const double Bullet::baseSpeed=Constants::BULLET_BaseSpeed;
const double Bullet::timeToShoot=Constants::BULLET_BaseTimeToShoot;

const int Bullet::errorBlockBullet=0;
const int Bullet::strongSwordBullet=1;
const int Bullet::strongXAtackBullet=2;
const int Bullet::weakXAtackBullet=3;
const int Bullet::busBullet=4;
const int Bullet::hyperbolicParaboloidBullet=5;
const int Bullet::rightAlternativeBullet=6;
const int Bullet::wrongAlternativeBullet=7;
const int Bullet::gateBullet=8;
const int Bullet::coinBullet=9;

/**
 *  Run logic events of the Bullets on the map like move, change textures, check if is in the screen, check collisions
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
        if(!bu->isVisible){
            FunctionAnalyser::endFunction("Bullet::behave");
            return;
        }
        if(!Scenes::camera.isInTheScreen(nTRectangle::getCollision(bu->pos,bu->size))&&bu->type!=coinBullet){
            if(bu->type==strongSwordBullet)
                Player::getPlayerById(0)->haveBulletSword=false;
            if(bu->type==strongXAtackBullet||bu->type==weakXAtackBullet)
                Player::getPlayerById(0)->haveBulletSpec=false;
            if(bu->type==busBullet)
                AL::singleton->stopSound(AL::getSoundByName("intercampi"));
            bu->isVisible=false;
            delete bu;
            FunctionAnalyser::endFunction("Bullet::behave");
            return;
        }
        if(bu->type==errorBlockBullet||bu->type==hyperbolicParaboloidBullet){//tiro de bloco
            bu->checkCollisionWithEntity(true);
        }else if(bu->type==rightAlternativeBullet||bu->type==wrongAlternativeBullet){//questions
            bu->checkCollisionWithEntity(true);
        }else if(bu->type==strongSwordBullet){//espada
            bu->checkCollisionWithEntity(false);
        }else if(bu->type==strongXAtackBullet){//atk forte
            bu->checkCollisionWithEntity(false);
        }else if(bu->type==weakXAtackBullet){//atk fraco
            bu->checkCollisionWithEntity(false);
        }else if(bu->type==coinBullet){//moeda
            bu->checkCollisionWithEntity(true);
        }else if(bu->type==busBullet){//intercamp
            bu->checkCollisionWithEntity(true);
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
 *  Override Mechanics::move to move bullets across the map
 *
 *  @param dir direction to where the bullet will move, could be Util::direction_left, Util::direction_right, Util::direction_up, Util::direction_down
 *  @param steeps amount of steps to move the bullet
**/
void Bullet::move(int dir,double steeps){
    FunctionAnalyser::startFunction("Bullet::move");
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
    if(type==busBullet){
        pos.x+=steeps;
    }else if(type==rightAlternativeBullet||type==wrongAlternativeBullet||type==gateBullet){
        pos.y+=steeps;
        if(type==gateBullet){
            if(pos.y>=Player::getPlayerById(0)->pos.y+Player::getPlayerById(0)->size.y/2){
                isVisible=false;
                Boss::fallGate(pos.x);
                delete this;
                return;
            }
        }else if(pos.y>=GL::defaultSize.y){
            isVisible=false;
            Boss::wrongAnswer();
            return;
        }
    }else{
        bool collision=false;
        if(dir==Util::direction_left||dir==Util::direction_right){
            nTPoint tmp;
            tmp.set(pos.x+steeps,pos.y,pos.z);
            collideWithMap(tmp);
            for(int i=0; i<lastMapColl.size(); i++){
                if(lastMapColl[i].collision.firstObj==Mechanics::LEFT||lastMapColl[i].collision.firstObj==Mechanics::RIGHT){
                    collision=true;
                    if(lastMapColl[i].blockId>=Map::staticBlocks.size()){
                        Blocks* bl=(Blocks*) Map::dynamicBlocks[lastMapColl[i].blockId-Map::staticBlocks.size()];
                        if(Blocks::checkIfBlocksIsEnemyCollider(bl->type))
                            collision=false;
                        else
                            break;
                    }
                }
            }
            if(!collision)
                pos.x+=steeps;
            else{
                if(type==strongSwordBullet)
                    Player::getPlayerById(0)->haveBulletSword=false;
                if(type==strongXAtackBullet||type==weakXAtackBullet)
                    Player::getPlayerById(0)->haveBulletSpec=false;
                isVisible=false;
                delete this;
                if(!isVisible){
                    FunctionAnalyser::endFunction("Bullet::move");
                    return;
                }
                hSpeed*=-1;
            }
        }
    }
    FunctionAnalyser::endFunction("Bullet::move");
    if(needToMoveAgain)
        move(dir,steepsAgain);
}

/**
 *  Check collisions with entities and make actions case it so
 *
 *  @param pos position of the entity
 *  @param size size of the entity
 *  @param withPlayer must be true case the entity in question is the player
**/
void Bullet::checkCollisionWithEntity(bool withPlayer){
    FunctionAnalyser::startFunction("Bullet::ckCollWithEntity");
    objCollision var;
    if(withPlayer){
        var=Mechanics::getCollision(nTRectangle::getCollision(this->pos,this->size),nTRectangle::getCollision(Player::getPlayerById(0)->pos,Player::getPlayerById(0)->size));
        if(var.firstObj!=Mechanics::NOCOLLISION){
            if(type==coinBullet){
                AL::singleton->playSoundByName("Coin");
                Player::coinsPicked++;
            }else if(type==busBullet){
                Player::getPlayerById(0)->applyDamage(Constants::BULLET_BaseBusDamage);
                AL::singleton->stopSound(AL::getSoundByName("intercampi"));
            }else if(type!=rightAlternativeBullet)
                Player::getPlayerById(0)->applyDamage(baseDamage);
            if(type==rightAlternativeBullet||type==wrongAlternativeBullet){
                isVisible=false;
                cout<<"tocou player\n";
                Boss::wrongAnswer();
                FunctionAnalyser::endFunction("Bullet::ckCollWithEntity");
                return;
            }else{
                isVisible=false;
                delete this;
                FunctionAnalyser::endFunction("Bullet::ckCollWithEntity");
                return;
            }
        }else if(type!=busBullet){
            var=Mechanics::getCollision(nTRectangle::getCollision(this->pos,this->size),Player::getPlayerById(0)->swordCollision);
            if(var.firstObj!=Mechanics::NOCOLLISION){
                if(type==rightAlternativeBullet){
                    isVisible=false;
                    cout<<"espada no certo\n";
                    Boss::rightAnswer();
                    FunctionAnalyser::endFunction("Bullet::ckCollWithEntity");
                    return;
                }else if(type==wrongAlternativeBullet){
                    isVisible=false;
                     cout<<"espada no errado\n";
                    Boss::wrongAnswer();
                    FunctionAnalyser::endFunction("Bullet::ckCollWithEntity");
                    return;
                }else{
                    isVisible=false;
                    delete this;
                    FunctionAnalyser::endFunction("Bullet::ckCollWithEntity");
                    return;
                }
            }
        }
    }else{
        Entity *en;
        vector<void*> nonPlayerUnities=Entity::enemys;
        nonPlayerUnities.insert(nonPlayerUnities.end(),Entity::bosses.begin(),Entity::bosses.end());
        for(int i=0;i<nonPlayerUnities.size();i++){
            en=(Entity*)nonPlayerUnities[i];
            var=Mechanics::getCollision(nTRectangle::getCollision(this->pos,this->size),nTRectangle::getCollision(en->pos,en->size));
            if(var.firstObj!=Mechanics::NOCOLLISION){
                if(type==strongSwordBullet)
                    Player::getPlayerById(0)->haveBulletSword=false;
                if(type==strongXAtackBullet||type==weakXAtackBullet)
                    Player::getPlayerById(0)->haveBulletSpec=false;
                en->applyDamage(Player::getPlayerById(0)->sword);
                if(en->life<=0)
                        Player::enemysKilled++;
                isVisible=false;
                delete this;
                i--;
                FunctionAnalyser::endFunction("Bullet::ckCollWithEntity");
                return;
            }
            if(type==strongSwordBullet){
                Bullet* bu;
                for(int i=0;i<Bullet::self.size();i++){
                    bu=(Bullet*)Bullet::self[i];
                    if(bu->type!=strongSwordBullet&&bu->type!=weakXAtackBullet&&bu->type!=strongXAtackBullet){
                        if(Mechanics::getCollision(nTRectangle::getCollision(this->pos,this->size),nTRectangle::getCollision(bu->pos,bu->size)).firstObj!=Mechanics::NOCOLLISION){
                            isVisible=false;
                            Player::getPlayerById(0)->haveBulletSword=false;
                            bu->isVisible=false;
                            delete bu;
                            FunctionAnalyser::endFunction("Bullet::ckCollWithEntity");
                            return;
                        }
                    }
                }
            }
        }
    }
    FunctionAnalyser::endFunction("Bullet::ckCollWithEntity");
}

/**
 *  Draw this bullet on the screen
**/
void Bullet::draw(){
    if(GL::isPaused||!Scenes::camera.isInTheScreen(nTRectangle::getCollision(pos,size)))
        return;
    pos.z=1;
    if (type==strongSwordBullet)
        pos.z=0.9;
    if(isVisible){
        if(type==rightAlternativeBullet)
            GL::drawTexture(nTRectangle::getCollision(pos,size),nTColor::get(0,1,0),tex->get());
        else if(type==wrongAlternativeBullet)
            GL::drawTexture(nTRectangle::getCollision(pos,size),nTColor::get(1,0,0),tex->get());
        else
            GL::drawTexture(nTRectangle::getCollision(pos,size),tex->get());
        if(Mechanics::drawCollisionRec)GL::drawCollision(nTRectangle::getCollision(pos, size));
    }
}