#include "Player.hpp"

Player::Player(double life,nTPoint spawn,nTPoint size) {
    spawn.z=0.9;
    this->pos=spawn;
    this->size=size;
    this->vSpeed=0;
    this->hSpeed=0;
    this->currentState=Entity::state_Idle;
    this->currentIndex=0;
    this->nextState=Entity::state_Holding;
    if(life>defaultLife)
        life=defaultLife;
    if(life<1)
        life=1;
    this->life=life;
    this->defaultOrientation=Util::orientation_right;
    this->isHuman=true;
    this->damageState=false;
    this->sword=(int)life-1;
    this->atackDirection=Util::direction_right;
    this->orientation=Util::direction_right;
    this->canJump=true;
    this->isVisible=true;
    this->swordPos=spawn;
    this->atacking=false;
    this->haveBulletSword=0;
    this->haveBulletSpec=0;
    this->canWalk=1;
    this->swordSize=nTPoint::get(30,12,0.9);
    this->canTp=false;
    this->imuneToDamage=false;
    this->god=false;
    this->lowered=false;

    Entity::players.push_back(this);
    this->id=-((unsigned int)Entity::players.size());
};

Player::Player(const Player& orig) {
}

Player::~Player() {
    Player* pl;
    int idx=-id-1;
    for(int i=idx+1;i<Entity::players.size();i++){
        pl=(Player*)Entity::players[i];
        pl->id++;
    }
    Entity::players.erase(Entity::players.begin()+idx);
}

int Player::lives=3;
bool Player::beatGame=false;
const int Player::ranged=64651;
const int Player::meleeProjectile=16165;
const int Player::melee=165165;
const double Player::imunityTime=900;
const double Player::swordBaseDamage=2;
int Player::checkpoint=0;
int Player::stage=0;
const int Player::defaultLife=3;
int Player::loadedLife=defaultLife;
double Player::coeficiente=0;
double Player::globalCoeficiente=0;
int Player::enemysKilled=0;
int Player::PowerUpsActiveted=0;
const nTPoint Player::defaultPSize=nTPoint::get(28,60);
vector<vector<string> >Player::playerAnim;
vector<vector<int> >Player::playerAnimSize;

/**
 *	Modify the operator << to print this type of objects
 *	The parameters are passed automatically
 *
 *	@param strm current string stream
 *	@param player object address
 *	@return the old stream plus the object toString
**/
ostream& operator<<(ostream &strm, const Player &player){
    Player *pl=(Player*)&player;
    if(Util::DEBUG)
        return strm <<"Player:["<<"Pos("<<"x:"<<pl->pos.x<<" y:"<<pl->pos.y<<"),"<<"Speed("<<"hSpeed:"<<pl->hSpeed<<" vSpeed:"<<pl->vSpeed<<"),"<<"]\n";
    return strm;
}

/**
 *	Change the damageState of a player
 *
 *	@param id the id of the player
**/
void playerChangeDamageState(int id){
    Player *pl;
    int idx;
    if(id<0)
        idx=-id-1;
    else
        idx=id;
    try{
        pl=(Player*)Entity::players[idx];
        pl->damageState=false;
    }catch(const exception& e){
        cout<<"pCDS-catch error: "<<e.what()<<endl;
    }
}

/**
 *	Gets the position of the player fixed to detect bottom collsions
 *
 *	@return the ground position of the player
**/
nTPoint Player::getGroundPos(){
    return nTPoint::get(pos.x, pos.y+1, pos.z);
}

/**
 *	Override Entity::stateControl to add the states of player attacks, detect liquid collisionions and teleport blocks collision
**/
void Player::stateControl(){
    FunctionAnalyser::startFunction("Player::stateControl");
    if(GL::getGameMs()>=timeToVunerability){
        if((imuneToDamage&&!damageState)&&!god)
            AL::singleton->stopSound(AL::getSoundByName("cafeSong"));
        damageState=false;
        imuneToDamage=false;
    }
    Entity::stateControl();
    if(nextState!=Entity::state_TakingDamage&&nextState!=Entity::state_Dying&&currentState!=Entity::state_TakingDamage&&currentState!=Entity::state_Dying&&atacking&&!lowered){
        if(atacking==Player::melee){
            if(vSpeed!=0){//air
                if(atackDirection==Util::direction_down)
                    currentState=Entity::state_MeleeAirAtackingDown;
                else if(atackDirection==Util::direction_up)
                    currentState=Entity::state_MeleeAirAtackingUp;
                else
                    currentState=state_MeleeAirAtackingHorizontal;
            }else{//ground
                if(atackDirection==Util::direction_up)
                    currentState=Entity::state_MeleeGroundAtackingUp;
                else
                    currentState=Entity::state_MeleeGroundAtackingHorizontal;
            }
            nextState=Entity::state_Holding;
            currentIndex=0;
        }else{
            if(vSpeed!=0){//air
                if(atackDirection==Util::direction_down)
                    currentState=Entity::state_RangedAirAtackingDown;
                else if(atackDirection==Util::direction_up)
                    currentState=Entity::state_RangedAirAtackingUp;
                else
                    currentState=Entity::state_RangedAirAtackingHorizontal;
            }else{//ground
                if(atackDirection==Util::direction_up)
                    currentState=Entity::state_RangedGroundAtackingUp;
                else
                    currentState=Entity::state_RangedGroundAtackingHorizontal;
            }
            nextState=Entity::state_Holding;
            currentIndex=0;
        }
    }
    if(atacking&&!lowered) atack(atacking);
    execAnimation();
    FunctionAnalyser::endFunction("Player::stateControl");
}

/**
 *	Make the player immune to damage for a certain time and update the current sword
 *
 *	@param time time who the player will be immune
**/
void Player::makeInvencible(double time){
    sword=life-1;
    if(sword>2)
        sword=2;
    else if(sword<0)
        sword=0;
    imuneToDamage=true;
    timeToVunerability=GL::getGameMs()+time;
}

/**
 *	Reset the variables to properly spawn the player
 *
 *	@param spawn coordinates to spawn
 *	@param life the spawn start life
**/
void Player::spawn(nTPoint spawn,double life){
    if(life>defaultLife)
        life=defaultLife;
    if(life<1)
        life=1;
    spawn.z=0.9;
    this->pos=spawn;
    this->vSpeed=0;
    this->hSpeed=0;
    this->currentState=Entity::state_Spawning;
    this->nextState=Entity::state_Idle;
    this->life=life;
    this->sword=(int)life-1;
    this->defaultOrientation=Util::orientation_right;
    this->damageState=false;
    this->imuneToDamage=false;
    this->atackDirection=Util::direction_right;
    this->orientation=Util::orientation_right;
    this->isVisible=true;
    this->reducing=false;
    this->swordPos=nTPoint::get(-100,-100);
    this->atacking=false;
    this->haveBulletSword=0;
    this->haveBulletSpec=0;
    this->canWalk=1;
    this->god=false;
    this->canTp=false;
    this->canJump=false;
    this->lowered=false;
    alReadyAtacked=false;
    enemysKilled=0;
    PowerUpsActiveted=0;
    int anim=0;
    if(Scenes::freeGameMode)
        anim=rand()%playerAnim.size();
    else
        anim=Player::stage;
    animations=Entity::getAnimationVector(Player::playerAnim[anim],Player::playerAnimSize[anim]);
    Scenes::camera.lookAt(this->pos);
}


void Player::behave(){
    FunctionAnalyser::startFunction("Player::behave");
    Player::refreshCoeficiente();
    Player* pl;
    Blocks *bl;
    for(int i=0;i<players.size();i++){
        pl=(Player*)players[i];
        if(pl->god){
            AL::singleton->playSoundByName("cafeSong");
            pl->life=5;
            pl->sword=2;
            if(lives<3)
                lives=3;
            pl->imuneToDamage=true;
            pl->canJump=true;
        }
        bool collisionWater=false;
        bool collisionIce=false;
        pl->collideWithMap(pl->getGroundPos());
        for(int i=0; i<pl->lastMapColl.size(); i++){
            bl=Map::getBlockById(pl->lastMapColl[i].blockId);
            if(pl->lastMapColl[i].collision.firstObj!=Mechanics::NOCOLLISION){
                if(bl->type==Blocks::AnimatedLava1||bl->type==Blocks::AnimatedLava2||bl->type==Blocks::StaticLava){
                    if(!pl->god) pl->life=0;
                    collisionWater=false;
                }else if(Blocks::checkIfBlocksIsLiquid(bl->type)){
                    collisionWater=true;
                }else if(bl->type==Blocks::IceHHalfBlock||bl->type==Blocks::IceVHalfBlock||bl->type==Blocks::IceBlock){
                    if(pl->lastMapColl[i].collision.firstObj==Mechanics::BOTTOM)
                        collisionIce=true;
                }
                if((Blocks::checkIfBlocksIsTeleportPipe(bl->type)||Blocks::checkIfBlocksIsTeleportDoor(bl->type))&&pl->canTp){
                    Blocks* tp;
                    for(int j=0;j<Map::dynamicBlocks.size();j++){
                       tp=(Blocks*)Map::dynamicBlocks[j];
                       if(tp->type==bl->type&&bl!=tp){
                         pl->canTp=false;
                           pl->pos=tp->pos;
                           AL::singleton->playSoundByName("TP");
                           if(Blocks::checkIfBlocksIsTeleportPipe(tp->type))
                                pl->pos.y-=Blocks::defaultBlockSize.y*1.5;
                           else
                               pl->pos.y-=Blocks::defaultBlockSize.y/2;
                           pl->pos.z=0.9;
                           Scenes::camera.lookAt(pl->pos);
                         }
                    }
                }
            }
        }
        pl->itsInTheWater=collisionWater;
        if(pl->itsInTheWater) AL::singleton->playSoundByName("agua");
        if(IOHandler::SpecReleaseKey!=0){
            if(collisionIce){
                AL::singleton->playSoundByName("ice");
                pl->reducing=true;
            }else{
                pl->hSpeed=0;
            }
            IOHandler::SpecReleaseKey=0;
        }else
        if(pl->hSpeed==0||!collisionIce)AL::singleton->stopSound(AL::singleton->getSoundByName("ice"));
        if(IOHandler::ReleaseZOffSet){
            if(!GL::isPaused){
                pl->atacking=Player::meleeProjectile;
            }
            IOHandler::ReleaseZOffSet=false;
        }
        if(pl->lowered&&pl->size.y==defaultPSize.y){
            pl->size.y=defaultPSize.y/2;
            pl->pos.y+=defaultPSize.y/4;
        }else if(!pl->lowered&&pl->size.y!=defaultPSize.y){
            pl->size.y=defaultPSize.y;
            pl->pos.y-=defaultPSize.y/4;
        }

        if(pl->god) pl->draw(nTColor::get(0.41,0.41,1,0.85));
        else pl->draw();
        pl->drawSword();
    }
    FunctionAnalyser::endFunction("Player::behave");
}


/**
 *	Draw the melee sword
**/
void Player::drawSword(){
    if(!isVisible)
        return;
    if(atacking&&!lowered){
        swordCollision.p0.z=1;
        if(atackDirection==Util::direction_up||atackDirection==Util::direction_down)
            GL::drawTexture(swordCollision,GL::getTextureByName("Sword"+Util::intToStr(sword)),Util::orientation_rotated);
        else{
            if(atackDirection==Util::direction_right)
                GL::drawTexture(swordCollision,GL::getTextureByName("Sword"+Util::intToStr(sword)));
            else
                GL::drawTexture(swordCollision,GL::getTextureByName("Sword"+Util::intToStr(sword)),Util::orientation_left);
        }
    }
}

/**
 *	Handle player attack logic and update sword collision rectangle
 *
 *	@param type the type of attack witch can be Player::ranged, Player::meleeProjectile or Player::melee
**/
void Player::atack(int type){
    FunctionAnalyser::startFunction("Player::atack");
    if(type==Player::ranged){
        canJump=true;
            atacking=false;
        double cof;
        if(Player::globalCoeficiente==0)
            cof=Player::coeficiente;
        else
            cof=Player::globalCoeficiente;
        nTPoint tmp=swordSize;
        tmp.x*=orientation;
        if(!haveBulletSpec&&(cof>=75||Scenes::freeGameMode)){
            AL::singleton->playSoundByName("SpecialAtk");
            if(cof>=85||Scenes::freeGameMode){
                new Bullet(Bullet::strongXAtackBullet,orientation*Bullet::baseSpeed,pos,tmp);
            }else{
                new Bullet(Bullet::weakXAtackBullet,orientation*Bullet::baseSpeed,pos,tmp);
            }
            haveBulletSpec=true;
        }
    }else if(type==Player::meleeProjectile){
        if(sword>1&&!haveBulletSword){
            nTPoint tmp=swordSize;
            tmp.x*=orientation;
            new Bullet(Bullet::strongSwordBullet,orientation*Bullet::baseSpeed,pos,tmp);
            haveBulletSword=true;
        }
        canWalk=true;
        atacking=false;
        alReadyAtacked=false;
        swordCollision=nTRectangle::getCollision(nTPoint::Origin(),swordSize);
    }else if(type==Player::melee){//
            int angle=0;
            if(orientation>=Util::orientation_right)
                swordPos.set(pos.x+20,pos.y,1);
            else
                swordPos.set(pos.x+(swordSize.x-10),pos.y,1);
            if(atackDirection==Util::direction_right){
                angle=0;
            }else if(atackDirection==Util::direction_up){
                angle=3;
                swordPos.set(pos.x,pos.y,1);
            }else if(atackDirection==Util::direction_left){
                angle=2;
            }else if(atackDirection==Util::direction_down){
                angle=1;
                swordPos.set(pos.x,pos.y,1);
            }
            canJump=false;
            canWalk=false;
            swordCollision=nTRectangle::getCollision(swordPos,swordSize);
            swordCollision.p0=GL::rotatePoint(swordCollision.p0,pos,90*angle);
            swordCollision.p1=GL::rotatePoint(swordCollision.p1,pos,90*angle);
            swordCollision.p0.z=1;
            swordCollision.p1.z=1;
            if(atackDirection==Util::direction_up){
                swordCollision.p0.y-=swordSize.y*3;
                swordCollision.p1.y-=swordSize.y*3;
            }else if(atackDirection==Util::direction_down){
                swordCollision.p0.y+=swordSize.y*3;
                swordCollision.p1.y+=swordSize.y*3;
            }else if(atackDirection==Util::direction_left){
                nTPoint tmp=swordCollision.p0;
                swordCollision.p0=swordCollision.p1;
                swordCollision.p1=tmp;
            }
            if(Mechanics::drawCollisionRec)GL::drawCollision(swordCollision);
            if(!alReadyAtacked){
                int SS=rand()%6;
                AL::singleton->playSoundByName("sword"+Util::intToStr(SS));
                alReadyAtacked=true;
            }

            Entity *en;
            vector<void*> nonPlayerUnities=Entity::enemys;
            nonPlayerUnities.insert(nonPlayerUnities.end(),Entity::bosses.begin(),Entity::bosses.end());
            objCollision col;
            for(int i=0;i<nonPlayerUnities.size();i++){
                en=(Entity*)nonPlayerUnities[i];
                col=Mechanics::getCollision(swordCollision,nTRectangle::getCollision(en->pos,en->size));
                if(col.firstObj!=Mechanics::NOCOLLISION){
                    en->applyDamage(swordBaseDamage*(sword+1));
                    if(atackDirection==Util::direction_down){
                        move(Util::direction_up,-(Entity::verticalRecoil+vSpeed)/GL::getFPS());
                        vSpeed=-Entity::verticalRecoil;
                    }else if(atackDirection==Util::direction_right)
                        move(Util::direction_left,-Entity::horizontalRecoil/GL::getFPS());
                    else if(atackDirection==Util::direction_left)
                        move(Util::direction_left,Entity::horizontalRecoil/GL::getFPS());

                }
            }
    }
    FunctionAnalyser::endFunction("Player::atack");
}
/**
 *	Calculates and update the local coeficiente
**/
void Player::refreshCoeficiente(){
    coeficiente=0;
    if(Scenes::freeGameMode) return;
    if(Map::nOfEnemys)
        coeficiente+=100*Player::enemysKilled/Map::nOfEnemys         *3;
    if(Map::totalPowerUps)
        coeficiente+=100*Player::PowerUpsActiveted/Map::totalPowerUps*2;
    if(GL::getGameMs()){
        double temp=100000*Map::expetedTime/GL::getGameMs()            *1;
        if(temp>100)
          temp=100;
        coeficiente+=temp;
      }
    coeficiente/=6;

}

/**
 *	Calculates and update the global coeficiente
**/
void Player::refreshGlobalcoeficiente(){
    refreshCoeficiente();
    if(globalCoeficiente!=0){
        globalCoeficiente+=coeficiente;
        globalCoeficiente/=2;
    }else{
        globalCoeficiente=coeficiente;
    }
    coeficiente=0;
}

/**
 *	Gets the player object from its id
 *
 *	@param id the id of the player
 *  @return the address of the player object
**/
Player* Player::getPlayerById(int id){
    int idx;
    if(id<0)
        idx=-id-1;
    else
        idx=id;
    if(idx>=Entity::players.size())
        return nullptr;
    else
        return (Player*)Entity::players[idx];
}

void Player::setSprites(){
    vector<string> tmp;
    vector<int> tmp2;
    Player::playerAnim.clear();
    Player::playerAnimSize.clear();

    //technical
    tmp.push_back("technicalIdle");                 tmp2.push_back(5);//0 -Idle
    tmp.push_back("technicalMoveWalk");             tmp2.push_back(4); //1 -Walking
    tmp.push_back("technicalMoveRun");              tmp2.push_back(4);//2 -Running
    tmp.push_back("technicalJump");                 tmp2.push_back(1);//3 -Jumping
        tmp.push_back("");                  tmp2.push_back(1);//4 -Atacking -none
        tmp.push_back("");                  tmp2.push_back(1);//5 -SpecialAtacking -none
    tmp.push_back("technicalDmgGround");            tmp2.push_back(1);//6 -Damage
    tmp.push_back("technicalDie");                  tmp2.push_back(4);//7 -Death
    tmp.push_back("technicalSpawn");                tmp2.push_back(7);//8 -Spawn
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
    tmp2.clear();

    //graduation
    tmp.push_back("graduationIdle");                 tmp2.push_back(5);//0 -Idle
    tmp.push_back("graduationMoveWalk");             tmp2.push_back(4); //1 -Walking
    tmp.push_back("graduationMoveRun");              tmp2.push_back(4);//2 -Running
    tmp.push_back("graduationJump");                 tmp2.push_back(1);//3 -Jumping
        tmp.push_back("");                  tmp2.push_back(1);//4 -Atacking -none
        tmp.push_back("");                  tmp2.push_back(1);//5 -SpecialAtacking -none
    tmp.push_back("graduationDmgGround");            tmp2.push_back(1);//6 -Damage
    tmp.push_back("graduationDie");                  tmp2.push_back(4);//7 -Death
    tmp.push_back("graduationSpawn");                tmp2.push_back(7);//8 -Spawn
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
    tmp2.clear();

    //masters
    tmp.push_back("mastersIdle");                 tmp2.push_back(5);//0 -Idle
    tmp.push_back("mastersMoveWalk");             tmp2.push_back(4); //1 -Walking
    tmp.push_back("mastersMoveRun");              tmp2.push_back(4);//2 -Running
    tmp.push_back("mastersJump");                 tmp2.push_back(1);//3 -Jumping
        tmp.push_back("");                  tmp2.push_back(1);//4 -Atacking -none
        tmp.push_back("");                  tmp2.push_back(1);//5 -SpecialAtacking -none
    tmp.push_back("mastersDmgGround");            tmp2.push_back(1);//6 -Damage
    tmp.push_back("mastersDie");                  tmp2.push_back(4);//7 -Death
    tmp.push_back("mastersSpawn");                tmp2.push_back(7);//8 -Spawn
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
    tmp2.clear();

    //work
    tmp.push_back("workIdle");                 tmp2.push_back(5);//0 -Idle
    tmp.push_back("workMoveWalk");             tmp2.push_back(4); //1 -Walking
    tmp.push_back("workMoveRun");              tmp2.push_back(4);//2 -Running
    tmp.push_back("workJump");                 tmp2.push_back(1);//3 -Jumping
        tmp.push_back("");                  tmp2.push_back(1);//4 -Atacking -none
        tmp.push_back("");                  tmp2.push_back(1);//5 -SpecialAtacking -none
    tmp.push_back("workDmgGround");            tmp2.push_back(1);//6 -Damage
    tmp.push_back("workDie");                  tmp2.push_back(4);//7 -Death
    tmp.push_back("workSpawn");                tmp2.push_back(7);//8 -Spawn
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
    tmp2.clear();

    //gteacher
    tmp.push_back("gteacherIdle");                 tmp2.push_back(5);//0 -Idle
    tmp.push_back("gteacherMoveWalk");             tmp2.push_back(4); //1 -Walking
    tmp.push_back("gteacherMoveRun");              tmp2.push_back(4);//2 -Running
    tmp.push_back("gteacherJump");                 tmp2.push_back(1);//3 -Jumping
        tmp.push_back("");                  tmp2.push_back(1);//4 -Atacking -none
        tmp.push_back("");                  tmp2.push_back(1);//5 -SpecialAtacking -none
    tmp.push_back("gteacherDmgGround");            tmp2.push_back(1);//6 -Damage
    tmp.push_back("gteacherDie");                  tmp2.push_back(4);//7 -Death
    tmp.push_back("gteacherSpawn");                tmp2.push_back(7);//8 -Spawn
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
    tmp2.clear();

    //bteacher
    tmp.push_back("bteacherIdle");                 tmp2.push_back(5);//0 -Idle
    tmp.push_back("bteacherMoveWalk");             tmp2.push_back(4); //1 -Walking
    tmp.push_back("bteacherMoveRun");              tmp2.push_back(4);//2 -Running
    tmp.push_back("bteacherJump");                 tmp2.push_back(1);//3 -Jumping
        tmp.push_back("");                  tmp2.push_back(1);//4 -Atacking -none
        tmp.push_back("");                  tmp2.push_back(1);//5 -SpecialAtacking -none
    tmp.push_back("bteacherDmgGround");            tmp2.push_back(1);//6 -Damage
    tmp.push_back("bteacherDie");                  tmp2.push_back(4);//7 -Death
    tmp.push_back("bteacherSpawn");                tmp2.push_back(7);//8 -Spawn
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
    tmp2.clear();
}
