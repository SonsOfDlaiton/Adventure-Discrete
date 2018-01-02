#include "Boss.hpp"
#include "Blocks.hpp"


Boss::Boss(string data,nTPoint spawn) {
    size_t found=data.find("tutorial");
	if(found!=string::npos){
		found=data.find("[",found);
		string tutorial=data.substr(found+1);
		tutorial=tutorial.substr(0,tutorial.find("]"));
		Tutorials::add(floor(spawn.x/Blocks::defaultBlockSize.x)-floor(GL::defaultSize.x/Blocks::defaultBlockSize.x/2)+ceil((float)Player::defaultPSize.x/(float)Blocks::defaultBlockSize.x)*3,tutorial,true);
	}else if(Util::DEBUG) cout<<"ERRORRR - missing \"tutorial\" on boss\n";

    this->nickname="";
	found=data.find("name");
	if(found!=string::npos){
		found=data.find("\"",found);
		this->nickname=data.substr(found+1);
		this->nickname=this->nickname.substr(0,this->nickname.find("\""));
	}if(this->nickname=="" && Util::DEBUG)
		cout<<"ERRORRR - missing \"name\" on boss\n";

    found=data.find("size");
	if(found!=string::npos){
		found=data.find("{",found);
		string str_size=data.substr(found+1);
		str_size=str_size.substr(0,str_size.find("}"));
		bool sx=true;
		int last=0;
		str_size+=",";
		for(int i=0;i<str_size.size();i++){
			if(str_size[i]==','){
				string str=str_size.substr(last,i-last);
                if(sx)
                    this->size.x=Util::strToInt(str);
                else
                    this->size.y=Util::strToInt(str);
                sx=!sx;
				last=i;
			}
		}
	}else if(Util::DEBUG) cout<<"ERRORRR - missing \"size\" on boss\n";

    string sprite_str="";
    found=data.find("sprite");
	if(found!=string::npos){
		found=data.find("\"",found);
		sprite_str=data.substr(found+1);
		sprite_str=sprite_str.substr(0,sprite_str.find("\""));
	} if(sprite_str==""&&Util::DEBUG)
		cout<<"ERRORRR - missing \"sprite\" on boss\n";
    this->animations=Entity::getAnimationVector(Boss::bossAnim[getSpritesId(sprite_str)],Boss::bossAnimSize[getSpritesId(sprite_str)]);

    this->life=1;
    found=data.find("life");
	if(found!=string::npos){
        string life_str="";
		bool first=false;
		for(int i=found+1;i<data.size();i++)
			if(isdigit(data[i])){
				first=true;
				life_str+=data[i];
			}else if(first)
				break;
        this->life=Util::strToInt(life_str);
	}else if(Util::DEBUG) cout<<"ERRORRR - missing \"life\" on boss\n";

    this->imunityTime=200;
    found=data.find("imunityTime");
	if(found!=string::npos){
        string imu_str="";
		bool first=false;
		for(int i=found+1;i<data.size();i++)
			if(isdigit(data[i])){
				first=true;
				imu_str+=data[i];
			}else if(first)
				break;
        this->imunityTime=Util::strToInt(imu_str);
	}else if(Util::DEBUG) cout<<"ERRORRR - missing \"imunityTime\" on boss\n";

	found=data.find("events");
    if(found!=string::npos){

    }else if(Util::DEBUG) cout<<"ERRORRR - missing \"events\" on boss\n";

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

