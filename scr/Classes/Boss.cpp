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
    if(sprite_str!="")
        this->animations=Entity::getAnimationVector(Boss::bossAnim[getSpritesId(sprite_str)],Boss::bossAnimSize[getSpritesId(sprite_str)]);

    this->life=adc->getInt("life",1);
    this->imunityTime=adc->getInt("imunityTime",200);
    ADCode* adc_events=adc->getSubSection("events");
    while(adc_events->hasNext()){
        BossEvent be;
        if( adc_events->next(be.event)&&
            adc_events->next(be.params)&&
            adc_events->next(be.probability)&&
            adc_events->next(be.minimumLife))
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

void Boss::draw(){

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

