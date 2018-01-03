#include "Boss.hpp"
#include "Blocks.hpp"
#include "ADCode.hpp"


Boss::Boss(string data,nTPoint spawn) {
    ADCode* adc=new ADCode(data,"Boss");
    Tutorials::add(floor(spawn.x/Blocks::defaultBlockSize.x)-floor(GL::defaultSize.x/Blocks::defaultBlockSize.x/2)+ceil((float)Player::defaultPSize.x/(float)Blocks::defaultBlockSize.x)*3,adc->getSubSection("Tutorials"),true);
    this->nickname=adc->getString("name","");
    vector<int> size_vec=adc->getIntVector("size");
    if(size_vec.size()>=2){
        this->size.x=size_vec[0];
        this->size.y=size_vec[1];
    }else{
        this->size.x=64;
        this->size.y=96;
    }
    string sprite_str=adc->getString("sprite");
    this->animations=Entity::getAnimationVector(Boss::bossAnim[getSpritesId(sprite_str)],Boss::bossAnimSize[getSpritesId(sprite_str)]);

    this->life=adc->getNumber("life",1);
    this->startLife=this->life;
    this->imunityTime=adc->getNumber("imunityTime",200);
    ADCode* adc_events=adc->getSubSection("events");
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
            GL::popupBoxBehave("Events must have 4 arguments, {{args},probability,minLife,maxLife}","BITMAP_HELVETICA_12",2000);
        }
    }
    this->pos=spawn;
    this->vSpeed=0;
    this->atacking=false;
    this->hSpeed=0;
    this->currentState=Entity::state_Spawning;
    this->currentIndex=0;
    this->nextState=Entity::state_Holding;
    this->defaultOrientation=Util::orientation_right;
    this->isHuman=false;
    this->canJump=true;
    this->reducing=false;
    this->lowered=false;
    this->isVisible=true;
    this->damageState=false;
    this->itsInTheWater=false;
    this->imuneToDamage=false;
    Entity::bosses.push_back(this);
    this->id=(unsigned int)Entity::bosses.size()+60000;
    this->pos.y-=this->size.y/2;
    delete adc;
};

Boss::Boss(const Boss& orig) {
}

Boss::~Boss() {
    Boss* bo;
    for(int i=id+1;i<Entity::bosses.size();i++){
        bo=(Boss*)Entity::bosses[i];
        bo->id--;
    }
    Entity::bosses.erase(Entity::bosses.begin()+this->id);
}

vector<string> Boss::bossName;
vector<vector<string> > Boss::bossAnim;
vector<vector<int> > Boss::bossAnimSize;


void Boss::behave(){
    FunctionAnalyser::startFunction("Boss::behave");
    Boss *bo;
    if(GL::isPaused||Tutorials::isPaused){
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
                    Player::getPlayerById(0)->applyDamage(1);
                draw(bo);
            }
        }
    }
    FunctionAnalyser::endFunction("Boss::behave");
}

void Boss::eventHandler(){
    FunctionAnalyser::startFunction("Boss::eventHandler");
    for(int e=0;e<events.size();e++){
        BossEvent be=events[e];
        if((life>=be.minimumLife||be.minimumLife==0)&&(life<=be.maximumLife)&&rand()%be.probability==0){
            if(be.event=="reincarnation")
                reincarnation(be.params,e);
            else if(be.event=="summon")
                summon(be.params,e);
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
    return 0;
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
    Entity* ent=(Entity*)bo;
    ent->draw(nTColor::White());
}

void Boss::stateControl(){
    FunctionAnalyser::startFunction("Boss::stateControl");
    if(!Scenes::camera.isInTheXScreen(nTRectangle::getCollision(this->pos,this->size))){
        FunctionAnalyser::endFunction("Boss::stateControl");
        return;
    }
    Entity::stateControl();
    execAnimation();
    if(GL::framesInGame>=timeToVunerability)
        damageState=false;
    FunctionAnalyser::endFunction("Boss::stateControl");
}

/**
 *	Make the enemy immune to damage for a certain time
 *
**/
void Boss::makeInvencible(){
    damageState=true;
    timeToVunerability=GL::getGameMs()+imunityTime;
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

void Boss::reincarnation(vector<string> params, int& eid){
    if(params.size()==4){
        if(life<=0){
            nickname=params[0];
            animations=Entity::getAnimationVector(Boss::bossAnim[getSpritesId(params[1])],Boss::bossAnimSize[getSpritesId(params[1])]);
            life=Util::strToFloat(params[2]);
            startLife=life;
            currentState=Entity::state_Dying;
            currentIndex=0;
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
    if(params.size()==7){
        int amount=(rand()%(Util::strToInt(params[2])-Util::strToInt(params[1])))+Util::strToInt(params[1]);
        int spr_id=Enemy::getSpritesId(params[4]);
        vector<vector<GLuint> > anim_vec=Entity::getAnimationVector(Enemy::enemyAnim[spr_id],Enemy::enemyAnimSize[spr_id]);
        vector<int> sz=ADCode::strToIntVector(params[6]);
        nTPoint en_size=nTPoint::get(sz[0],sz[1]);
        for(int i=0;i<amount;i++){
            Enemy* en=new Enemy(666,Util::strToFloat(params[3]),nTPoint::get(pos.x+(size.x/2+en_size.x+en_size.x/2*i)*orientation,pos.y,0.89),en_size,anim_vec);
            en->nickname=params[0];
            en->drawLetter=Util::strToBool(params[5]);
        }
    }else{
        GL::popupBoxBehave("\"summon\" event must have 7 arguments("+Util::intToStr(params.size())+"), {name,min,max,life,sprite,drawLetter,{size}}","BITMAP_HELVETICA_12",5000);
        events.erase(events.begin()+(eid--));
    }
}

void Boss::shield(vector<string> params, int& eid){

}

void Boss::questions(vector<string> params, int& eid){

}

void Boss::gate(vector<string> params, int& eid){

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
        tmp.push_back("");  tmp2.push_back(1);//5 -SpecialAtacking -none
    tmp.push_back("enem1Idle"); tmp2.push_back(5);//6 -Damage
    tmp.push_back("calcIdle"); tmp2.push_back(1);//7 -Death
    tmp.push_back("bookIdle"); tmp2.push_back(1);//8 -Spawn
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
        tmp.push_back("");  tmp2.push_back(1);//5 -SpecialAtacking -none
    tmp.push_back("enem2Idle"); tmp2.push_back(5);//6 -Damage
    tmp.push_back("calcIdle"); tmp2.push_back(1);//7 -Death
    tmp.push_back("bookIdle"); tmp2.push_back(1);//8 -Spawn
    Boss::bossAnim.push_back(tmp);
    Boss::bossAnimSize.push_back(tmp2);
    tmp.clear();
    tmp2.clear();
}
