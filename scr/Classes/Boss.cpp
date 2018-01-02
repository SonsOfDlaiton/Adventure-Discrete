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
    this->imunityTime=adc->getNumber("imunityTime",200);
    ADCode* adc_events=adc->getSubSection("events");
    pair<string,vector<string> > event;
    while(adc_events->next(event)){
        BossEvent be;
        be.event=event.first;
        be.params=ADCode::strToStrVector(event.second[0]);
        be.probability=Util::strToFloat(event.second[1]);
        be.minimumLife=Util::strToInt(event.second[2]);
        events.push_back(be);
    }
    this->pos=spawn;
    this->vSpeed=0;
    this->atacking=false;
    this->hSpeed=0;
    this->currentState=Entity::state_Idle;
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
    this->id=(unsigned int)Entity::bosses.size()-1;
    delete adc;
};

Boss::Boss(const Boss& orig) {
}

Boss::~Boss() {
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
                if(bo->hSpeed>0)
                    bo->orientation=Util::orientation_right;
                else
                    bo->orientation=Util::orientation_left;
                objCollision var=Mechanics::getCollision(nTRectangle::getCollision(bo->pos,bo->size),nTRectangle::getCollision(Player::getPlayerById(0)->pos,Player::getPlayerById(0)->size));
                if(var.firstObj==Mechanics::LEFT||var.firstObj==Mechanics::RIGHT||var.firstObj==Mechanics::BOTTOM||(!Player::getPlayerById(0)->atacking&&Player::getPlayerById(0)->atackDirection!=Util::direction_down&&var.firstObj==Mechanics::TOP))
                    Player::getPlayerById(0)->applyDamage(1);
                draw(bo);
            }
        }
    }
    FunctionAnalyser::endFunction("Boss::behave");
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
    GL::setFont("BITMAP_HELVETICA_10");
    GL::drawCentered_X_Text(nTPoint::get(bo->pos.x,bo->pos.y-bo->size.y/2.0,0.7),bo->nickname,nTColor::Black());
    Entity* ent=(Entity*)bo;
    ent->draw(nTColor::White());
}

void Boss::stateControl(){

}

/**
 *	Make the enemy immune to damage for a certain time
 *
**/
void Boss::makeInvencible(){
    damageState=true;
    timeToVunerability=GL::getGameMs()+imunityTime;
    if(life<=0&&life!=-666){
        life=-666;
        nTPoint ggPos=pos;
        ggPos.x+=orientation*Blocks::defaultBlockSize.x;
        Blocks* bl=new Blocks(Blocks::EndLevelBlock,ggPos,Blocks::defaultBlockSize);
        Map::dynamicBlocks.push_back(bl);
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
        tmp.push_back("");  tmp2.push_back(1);//5 -SpecialAtacking -none
    tmp.push_back("enem1Idle"); tmp2.push_back(5);//6 -Damage
    tmp.push_back("enem1Idle"); tmp2.push_back(5);//7 -Death
    tmp.push_back("enem1Idle"); tmp2.push_back(5);//8 -Spawn
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
    tmp.push_back("enem2Idle"); tmp2.push_back(5);//7 -Death
    tmp.push_back("enem2Idle"); tmp2.push_back(5);//8 -Spawn
    Boss::bossAnim.push_back(tmp);
    Boss::bossAnimSize.push_back(tmp2);
    tmp.clear();
    tmp2.clear();
}
