#include "Boss.hpp"
#include "Blocks.hpp"
#include "ADCode.hpp"


Boss::Boss(string data,nTPoint spawn) {
    ADCode* adc=new ADCode(data,"Boss");
    Tutorials::add(floor(spawn.x/Blocks::defaultBlockSize.x)-floor(GL::defaultSize.x/Blocks::defaultBlockSize.x/2)+ceil((float)Player::defaultPSize.x/(float)Blocks::defaultBlockSize.x)*3,adc->getSection("Tutorials"),true);
    this->nickname=adc->getString("name","");
    vector<int> size_vec=adc->getIntVector("size");
    if(size_vec.size()>=2){
        this->size.x=size_vec[0];
        this->size.y=size_vec[1];
        this->size.z=0.5;
    }else{
        this->size.x=64;
        this->size.y=96;
        this->size.z=0.5;
    }
    string sprite_str=adc->getString("sprite");
    int spr_id=getSpritesId(sprite_str);
    if(spr_id<0){
        GL::popupBoxBehave("Invalid sprite name("+sprite_str+")","BITMAP_HELVETICA_12",3000);
        spr_id=0;
    }
    this->animations=Entity::getAnimationVector(Boss::bossAnim[spr_id],Boss::bossAnimSize[spr_id]);
    this->life=adc->getNumber("life",1);
    this->startLife=this->life;
    this->imunityTime=adc->getNumber("imunityTime",200);
    ADCode* adc_events=adc->getSection("events");
    pair<string,vector<string> > event;
    while(adc_events->next(event)){
        BossEvent be;
        be.event=event.first;
        if(event.second.size()==4){
            be.params=ADCode::strToStrVector(event.second[0]);
            be.probability=Util::strToFloat(event.second[1]);
            be.minimumLife=Util::strToInt(event.second[2]);
            be.maximumLife=Util::strToInt(event.second[3]);
            events.push_back(be);
        }else{
            GL::popupBoxBehave("Events must have 4 arguments, {{args},probability,minLife,maxLife}","BITMAP_HELVETICA_12",5000);
        }
    }
    this->pos=spawn;
    this->vSpeed=0;
    this->atacking=false;
    this->hSpeed=0;
    this->changeState(Entity::state_Spawning);
    this->nextState=Entity::state_Holding;
    this->defaultOrientation=Util::orientation_right;
    this->isHuman=false;
    this->canJump=true;
    this->reducing=false;
    this->lowered=false;
    this->isVisible=true;
    this->damageState=false;
    this->damageVunerability=false;
    this->itsInTheWater=false;
    this->damageGate=0;
    this->imuneToDamage=false;
    this->alreadyHaveQuestions=false;
    Entity::bosses.push_back(this);
    this->id=(unsigned int)Entity::bosses.size()+60000;
    this->pos.y-=this->size.y/2;
    this->shieldMs=0;
    this->castMs=0;
    this->shieldBlock=0;
    this->canCast=true;
    this->shieldTex=new Texture();
    shieldTex->setTimeToNext(300);
    delete adc;
};

Boss::Boss(const Boss& orig) {
}

Boss::~Boss() {
    delete shieldTex;
    Boss* bo;
    for(int i=id+1;i<Entity::bosses.size();i++){
        bo=(Boss*)Entity::bosses[i];
        bo->id--;
    }
    Entity::bosses.erase(Entity::bosses.begin()+this->id-60001);
}

vector<void*> Boss::enSummoned;
vector<void*> Boss::buSummoned;
vector<string> Boss::bossName;
vector<vector<string> > Boss::bossAnim;
vector<vector<int> > Boss::bossAnimSize;


void Boss::behave(){
    FunctionAnalyser::startFunction("Boss::behave");
    Boss *bo;
    if(Tutorials::isPaused){
        for(int i=0;i<Entity::bosses.size();i++){
            bo=(Boss*)Entity::bosses[i];
            draw(bo);
        }
        FunctionAnalyser::endFunction("Boss::behave");
        return;
    }
    for(int i=0;i<Entity::bosses.size();i++){
        bo=(Boss*)Entity::bosses[i];
        if(bo->isVisible){
            if(Scenes::camera.isInTheXScreen(nTRectangle::getCollision(bo->pos,bo->size))){
                if(bo->hSpeed>0||Player::getPlayerById(0)->pos.x>=bo->pos.x)
                    bo->orientation=Util::orientation_right;
                else
                    bo->orientation=Util::orientation_left;
                bo->eventHandler();
                objCollision var=Mechanics::getCollision(nTRectangle::getCollision(bo->pos,bo->size),nTRectangle::getCollision(Player::getPlayerById(0)->pos,Player::getPlayerById(0)->size));
                if(var.firstObj==Mechanics::LEFT||var.firstObj==Mechanics::RIGHT||var.firstObj==Mechanics::BOTTOM||(!Player::getPlayerById(0)->atacking&&Player::getPlayerById(0)->atackDirection!=Util::direction_down&&var.firstObj==Mechanics::TOP))
                    Player::getPlayerById(0)->applyDamage(Constants::BOSS_BaseDamage);
                draw(bo);
            }
        }
    }
    FunctionAnalyser::endFunction("Boss::behave");
}

void Boss::eventHandler(){
    FunctionAnalyser::startFunction("Boss::eventHandler");
    if(currentState!=Entity::state_Spawning){
        for(int e=0;e<events.size();e++){
            BossEvent be=events[e];
            if((life>=be.minimumLife||be.minimumLife==0)&&(life<=be.maximumLife)&&(rand()%(be.probability*(int)GL::getFPS()/10)==0||be.probability==1)){
                if(be.event=="reincarnation")
                    reincarnation(be.params,e);
                else if(be.event=="summon")
                    summon(be.params,e);
                else if(be.event=="shield")
                    shield(be.params,e);
                else if(be.event=="questions")
                    questions(be.params,e);
                else if(be.event=="gate")
                    gate(be.params,e);
            }
        }
        if(!canCast){
            if(GL::getGameMs()-castMs>=questionsHex)
                canCast=true;
        }
    }
    FunctionAnalyser::endFunction("Boss::eventHandler");
}

int Boss::getSpritesId(string name){
    int i=0;
    for(string n:bossName)
        if(n==name)
            return i;
        else
            i++;
    return -1;
}

void Boss::draw(Boss* bo){
    GL::setFont("BITMAP_TIMES_ROMAN_24");
    nTRectangle lifebar=nTRectangle::getCollision(nTPoint::get(bo->pos.x,bo->pos.y-bo->size.y+30,0.68),nTPoint::get(bo->size.x*2.1,30));
    GL::drawRectangle(lifebar,nTColor::get(1,0,0));
    lifebar.p0.z=0.69;
    lifebar.p0.y-=1;
    lifebar.p1.y+=1;
    lifebar.p0.x+=1;
    lifebar.p1.x=lifebar.p0.x-2+bo->size.x*2.1*bo->life/bo->startLife;
    GL::drawRectangle(lifebar,nTColor::get(0,1,0));
    GL::drawCentered_X_Text(nTPoint::get(bo->pos.x,bo->pos.y-bo->size.y/2.0,0.7),bo->nickname,nTColor::Black());
    if(GL::isPaused||!bo->isVisible)
        return;
    if(Tutorials::isPaused){
        GL::drawTexture(nTRectangle::getCollision(bo->pos,bo->size),nTColor::White(),bo->currentTex->get(),bo->orientation);
        if(bo->shieldBlock>0)
            GL::drawTexture(nTRectangle::getCollision(bo->pos,nTPoint::get(bo->size.x*1.7,bo->size.y*1.7,bo->size.z+0.02)),nTColor::White(),bo->shieldTex->get(),bo->orientation);
        return;
    }
    bo->stateControl();
    if(bo->shieldBlock>0){
        GL::drawTexture(nTRectangle::getCollision(bo->pos,bo->size),nTColor::White(),bo->currentTex->get(),bo->orientation);
        if((bo->damageState||bo->imuneToDamage)&&Util::timerWithInterval(200))
            return;
        GL::drawTexture(nTRectangle::getCollision(bo->pos,nTPoint::get(bo->size.x*1.7,bo->size.y*1.7,bo->size.z+0.02)),nTColor::White(),bo->shieldTex->get(),bo->orientation);
    }else{
        if((bo->damageState||bo->imuneToDamage)&&Util::timerWithInterval(200))
            return;
        GL::drawTexture(nTRectangle::getCollision(bo->pos,bo->size),nTColor::White(),bo->currentTex->get(),bo->orientation);
    }
    if(Mechanics::drawCollisionRec)GL::drawCollision(nTRectangle::getCollision(bo->pos, bo->size));
}

void Boss::stateControl(){
    FunctionAnalyser::startFunction("Boss::stateControl");
    if(!Scenes::camera.isInTheXScreen(nTRectangle::getCollision(this->pos,this->size))){
        FunctionAnalyser::endFunction("Boss::stateControl");
        return;
    }
    Entity::stateControl();
    if(!canCast&&shieldBlock==0){
        damageVunerability=true;
        changeState(state_TakingDamage);
    }
    FunctionAnalyser::endFunction("Boss::stateControl");
}

/**
 *  Make the enemy immune to damage for a certain time
 *
**/
void Boss::makeInvencible(){
    Entity::makeInvencible();
    if(life<=0&&life!=-666){
        eventHandler();
        if(life<=0){
            life=-666;
            nTPoint ggPos=pos;
            ggPos.x+=orientation*Blocks::defaultBlockSize.x;
            Blocks* bl=new Blocks(Blocks::EndLevelBlock,ggPos,Blocks::defaultBlockSize);
            Map::dynamicBlocks.push_back(bl);
            this->isVisible=false;
            delete this;
        }
    }
}

void Boss::disableCasts(){
    shieldBlock=0;
    shieldMs=GL::getGameMs();
    canCast=false;
    castMs=GL::getGameMs();
}

void Boss::rightAnswer(){
    Bullet *bu;
    for(int i=0;i<buSummoned.size();i++){
        bu=(Bullet*)buSummoned[i];
        if(bu!=nullptr){
            delete bu;
            bu=nullptr;
        }
        buSummoned.erase(buSummoned.begin()+(i--));
    }
    Boss* bo;
    for(int i=0;i<Entity::bosses.size();i++){
        bo=(Boss*)Entity::bosses[i];
        if(bo->alreadyHaveQuestions){
            bo->alreadyHaveQuestions=false;
            if(bo->questionsHex>0){
                bo->disableCasts();
            }
            if(bo->questionsDmg>0)
                bo->applyDamage(bo->questionsDmg);
        }
    }
}

void Boss::wrongAnswer(){
    Bullet *bu;
    for(int i=0;i<buSummoned.size();i++){
        bu=(Bullet*)buSummoned[i];
        if(bu!=nullptr){
            delete bu;
            bu=nullptr;
        }
        buSummoned.erase(buSummoned.begin()+(i--));
    }
    Boss* bo;
    for(int i=0;i<Entity::bosses.size();i++){
        bo=(Boss*)Entity::bosses[i];
        if(bo->alreadyHaveQuestions)
            bo->alreadyHaveQuestions=false;
    }
}

void Boss::applyDamage(double damage){
    if(damageState||imuneToDamage)
        return;
    life-=damage*(1-shieldBlock/100);
    if(shieldBlock==0)
        damageVunerability=true;
    damageState=true;
    makeInvencible();
    if(rand()%23==0)
        AL::singleton->playSoundByName("enemyDamageSpec");
    else
        AL::singleton->playSoundByName("enemyDamage");
}

void Boss::reincarnation(vector<string> params, int& eid){
    if(params.size()==4){
        if(life<=0){
            nickname=params[0];
            animations=Entity::getAnimationVector(Boss::bossAnim[getSpritesId(params[1])],Boss::bossAnimSize[getSpritesId(params[1])]);
            life=Util::strToFloat(params[2]);
            startLife=life;
            changeState(Entity::state_Dying);
            int livesRemaning=Util::strToInt(params[3])-1;
            if(livesRemaning<=0)
                events.erase(events.begin()+(eid--));
            else
                params[3]=Util::intToStr(livesRemaning);
        }
    }else{
        GL::popupBoxBehave("\"reincarnation\" event must have 4 arguments("+Util::intToStr(params.size())+"), {name,sprite,life,lives}","BITMAP_HELVETICA_12",5000);
        events.erase(events.begin()+(eid--));
    }
}

void Boss::summon(vector<string> params, int& eid){
    if(canCast){
        if(params.size()==7){
            int amount=(rand()%(Util::strToInt(params[2])-Util::strToInt(params[1])))+Util::strToInt(params[1]);
            vector<int> sz=ADCode::strToIntVector(params[6]);
            nTPoint pl_size=Player::getPlayerById(0)->size;
            pl_size.x*=2;
            nTRectangle pl_coll=nTRectangle::getCollision(Player::getPlayerById(0)->pos,pl_size);
            nTPoint en_size=nTPoint::get(sz[0],sz[1]);
            amount-=enSummoned.size();
            if(amount>0)
                changeState(state_SpecialAtacking);
            for(int i=0;i<amount;i++){
                nTPoint en_pos=nTPoint::get(pos.x+(size.x/2+en_size.x+en_size.x*i*1.5)*orientation,pos.y+en_size.y/4,0.89);
                if(Mechanics::getCollision(nTRectangle::getCollision(en_pos,en_size),pl_coll).firstObj==Mechanics::NOCOLLISION){
                    Enemy* en=new Enemy(666,Util::strToFloat(params[3]),en_pos,en_size,params[4]);
                    en->nickname=params[0];
                    en->drawLetter=Util::strToBool(params[5]);
                    en->hSpeed*=orientation;
                    Map::nOfEnemys++;
                    enSummoned.push_back(en);
                }
            }
        }else{
            GL::popupBoxBehave("\"summon\" event must have 7 arguments("+Util::intToStr(params.size())+"), {name,min,max,life,sprite,drawLetter,{size}}","BITMAP_HELVETICA_12",5000);
            events.erase(events.begin()+(eid--));
        }
    }
}

void Boss::shield(vector<string> params, int& eid){
    if(canCast){
        if(params.size()==3){
            if(shieldBlock!=Util::strToFloat(params[0])){
                if(GL::getGameMs()-shieldMs>=Util::strToInt(params[2])||shieldMs==0){
                    changeState(state_SpecialAtacking);
                    shieldBlock=Util::strToFloat(params[0]);
                    shieldTex->setTextures(GL::getTexturesByName("enemShieldSummon",4));
                    shieldMs=GL::getGameMs();
                }
            }else{
                if(Util::strToInt(params[1])!=0&&GL::getGameMs()-shieldMs>=Util::strToInt(params[1])){
                    shieldMs=GL::getGameMs();
                    shieldBlock=0;
                }
                if(shieldTex->finishedAnimation())
                    shieldTex->setTextures(GL::getTextureByName("enemShield"));
            }
        }else{
            GL::popupBoxBehave("\"shield\" event must have 3 arguments("+Util::intToStr(params.size())+"), {damage block,duration,time_to_cast}","BITMAP_HELVETICA_12",5000);
            events.erase(events.begin()+(eid--));
        }
    }
}

void Boss::questions(vector<string> params, int& eid){
    if(canCast){
        if(params.size()==5){
            if(alreadyHaveQuestions==false){
                alreadyHaveQuestions=true;
                changeState(state_SpecialAtacking);
                questionsDmg=Util::strToFloat(params[3]);
                questionsHex=Util::strToFloat(params[4]);
                float indidualSize=0;
                int amount=Util::strToInt(params[0]);
                int rights=Util::strToInt(params[1]);
                if(orientation==Util::orientation_left)
                    indidualSize=(pos.x-Scenes::camera.x.movedCam)/(amount*1.5);
                else
                    indidualSize=(GL::defaultSize.x-(pos.x-Scenes::camera.x.movedCam))/(amount*1.5);
                vector<bool> awnsers;
                for(int i=0;i<amount;i++)
                    awnsers.push_back(i<rights);
                random_shuffle(awnsers.begin(),awnsers.end());
                Bullet *bu;
                for(int i=0;i<amount;i++){
                    int type;
                    if(awnsers[i])
                        type=Bullet::rightAlternativeBullet;
                    else
                        type=Bullet::wrongAlternativeBullet;
                    nTPoint bu_pos=nTPoint::get(pos.x+(size.x/2+indidualSize+indidualSize*(amount-1-i)*1.3)*orientation,Scenes::camera.y.movedCam,1);
                    bu=new Bullet(type,80,bu_pos,nTPoint::get(indidualSize/1.2f,indidualSize/1.2f,1));
                    bu->tex->setTextures(GL::getTextureByName(params[2]+(char)(65+i)));
                    buSummoned.push_back(bu);
                }
            }
        }else{
            GL::popupBoxBehave("\"questions\" event must have 5 arguments("+Util::intToStr(params.size())+"), {amount,amount_right,sprite,damage_to_enemy,time_without_cast}","BITMAP_HELVETICA_12",5000);
            events.erase(events.begin()+(eid--));
        }
    }
}

void Boss::gate(vector<string> params, int& eid){
    if(canCast){
        if(params.size()==5){
            if(damageGate==0){
                damageGate=Util::strToFloat(params[3]);
                float position=0;
                if(orientation==Util::orientation_left)
                    position=pos.x-Scenes::camera.x.movedCam;
                else
                    position=GL::defaultSize.x-(pos.x-Scenes::camera.x.movedCam);
                position*=(rand()%ABS(Util::strToInt(params[1])-Util::strToInt(params[0])))+Util::strToInt(params[0]);
                position/=100;
                position=pos.x+(size.x/2+position)*orientation;
                vector<int> sz=ADCode::strToIntVector(params[4]);
                Bullet* bu=new Bullet(Bullet::gateBullet,150,nTPoint::get(position,Scenes::camera.y.movedCam,1),nTPoint::get(sz[0],sz[1],1));
                bu->tex->setTextures(GL::getTextureByName(params[2]));
            }
        }else{
            GL::popupBoxBehave("\"gate\" event must have 5 arguments("+Util::intToStr(params.size())+"), {min_pos,max_pos,sprite,damage_to_player,{size}}","BITMAP_HELVETICA_12",5000);
            events.erase(events.begin()+(eid--));
        }
    }
}


void Boss::fallGate(float xpos){
    Boss* bo;
    for(int i=0;i<Entity::bosses.size();i++){
        bo=(Boss*)Entity::bosses[i];
        if(Player::getPlayerById(0)->pos.x<xpos){
            Player::getPlayerById(0)->applyDamage(bo->damageGate);
        }
        bo->damageGate=0;
    }
}

void Boss::setSprites(){
    vector<string> tmp;
    vector<int> tmp2;
    Boss::bossAnim.clear();
    Boss::bossAnimSize.clear();
    Boss::bossName.clear();

    //Enem_1
    Boss::bossName.push_back("enem_1");//sprite name
    tmp.push_back("enem1Idle"); tmp2.push_back(5);//0 -Idle
    tmp.push_back("enem1Idle"); tmp2.push_back(5);//1 -Walking
    tmp.push_back("enem1Idle"); tmp2.push_back(5);//2 -Running
    tmp.push_back("enem1Idle"); tmp2.push_back(5);//3 -Jumping
        tmp.push_back("");  tmp2.push_back(1);//4 -Atacking -none
    tmp.push_back("enem1Action");  tmp2.push_back(9);//5 -SpecialAtacking
    tmp.push_back("enem1Damage"); tmp2.push_back(1);//6 -Damage
    tmp.push_back("enemDeath"); tmp2.push_back(7);//7 -Death
    tmp.push_back("enemSpawn"); tmp2.push_back(7);//8 -Spawn
    Boss::bossAnim.push_back(tmp);
    Boss::bossAnimSize.push_back(tmp2);
    tmp.clear();
    tmp2.clear();

    //Enem_2
    Boss::bossName.push_back("enem_2");//sprite name
    tmp.push_back("enem2Idle"); tmp2.push_back(5);//0 -Idle
    tmp.push_back("enem2Idle"); tmp2.push_back(5);//1 -Walking
    tmp.push_back("enem2Idle"); tmp2.push_back(5);//2 -Running
    tmp.push_back("enem2Idle"); tmp2.push_back(5);//3 -Jumping
        tmp.push_back("");  tmp2.push_back(1);//4 -Atacking -none
    tmp.push_back("enem2Action");  tmp2.push_back(9);//5 -SpecialAtacking
    tmp.push_back("enem2Damage"); tmp2.push_back(1);//6 -Damage
    tmp.push_back("enemDeath"); tmp2.push_back(7);//7 -Death
    tmp.push_back("enemSpawn"); tmp2.push_back(7);//8 -Spawn
    Boss::bossAnim.push_back(tmp);
    Boss::bossAnimSize.push_back(tmp2);
    tmp.clear();
    tmp2.clear();
}
