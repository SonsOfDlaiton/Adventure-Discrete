#include "Tutorials.hpp"
#include "Scenes.hpp"
#include "ADCode.hpp"

Tutorials::Tutorials(){
};

Tutorials::Tutorials(const Tutorials& orig) {
}

Tutorials::~Tutorials() {
}


bool Tutorials::showTutorials=true;
bool Tutorials::isPaused=false;
vector<int> Tutorials::checkpoints;
vector<int> Tutorials::tutorials;
vector<string> Tutorials::texts;
vector<vector<char> > Tutorials::keys;
vector<int> Tutorials::time;
vector<bool> Tutorials::pause;
vector<bool> Tutorials::called;
double Tutorials::startTutMs=0;
int Tutorials::activeTut=-1;
vector<string> Tutorials::sinucaTuts;
int Tutorials::sinucaCurrent=-1;
double Tutorials::sinucaMs;
int Tutorials::lastFriendTex=-1;
Texture* Tutorials::friendTex=new Texture();

/**
 *	Draw the tutorial message text on the screen
 *
 *	@param text the string to be drawed
**/
void Tutorials::drawWhispText(string text){
    GL::setFont("BITMAP_HELVETICA_18");
    float lineWidth=GL::calcTextBoundaries("_").x;
    nTPoint boundaries=GL::calcTextBoundaries(text);
    float maxSize=420;
    if(boundaries.x>maxSize){
        int maxChars=floor((float)maxSize/(float)lineWidth);
        for(int i=maxChars-1;i<text.size();i+=maxChars){
            text.insert(i,"\n");
        }
    }
    GL::drawText(nTPoint::get(320+Scenes::camera.x.movedCam,Scenes::camera.y.movedCam+90,1),text,GL::getColorByName("yellow"));
}

/**
 *	Draw the tutorial on the screen
**/
void Tutorials::draw(string text){
  if(showTutorials){
    int currentFriendTex=0;
    if(!Scenes::freeGameMode){
 		if(Player::stage==Map::lvlTechnical)
        	 currentFriendTex=1;
      	else if(Player::stage==Map::lvlGraduation)
        	 currentFriendTex=2;
		else if(Player::stage==Map::lvlMasters)
			currentFriendTex=3;
		else if(Player::stage==Map::lvlWork)
			currentFriendTex=4;
		else if(Player::stage==Map::lvlGoodTeacher)
			currentFriendTex=5;
		else if(Player::stage==Map::lvlBadTeacher)
			currentFriendTex=6;
    }

	if(text.find("Douglas")!=string::npos)
		currentFriendTex=7;
	if(GL::isPaused)
		currentFriendTex=8;
    if(lastFriendTex<0&&lastFriendTex!=currentFriendTex){
        switch(currentFriendTex){
            default: friendTex->setTextures(GL::getTextureByName("Whisp")); break;
            case 1: friendTex->setTextures(GL::getTextureByName("technical")); break;
            case 2: friendTex->setTextures(GL::getTextureByName("graduation")); break;
            case 3: friendTex->setTextures(GL::getTextureByName("masters")); break;
            case 4: friendTex->setTextures(GL::getTextureByName("work")); break;
            case 5: friendTex->setTextures(GL::getTextureByName("teacher")); break;
            case 6: friendTex->setTextures(GL::getTextureByName("teacher")); break;
            case 7: friendTex->setTextures(GL::getTextureByName("Douglinhas")); break;
            case 8: friendTex->setTextures(GL::getTextureByName("Cica")); break;
        }
    }
    GL::drawTexture((nTRectangle::get(Scenes::camera.x.movedCam,
        300+Scenes::camera.y.movedCam,
        800+Scenes::camera.x.movedCam,
        Scenes::camera.y.movedCam,0.999)),
        GL::getTextureByName("TutTextHolder"));
    GL::drawTexture((nTRectangle::get(Scenes::camera.x.movedCam,
        300+Scenes::camera.y.movedCam,
        300+Scenes::camera.x.movedCam,
        Scenes::camera.y.movedCam,0.99998)),
        friendTex->get());
    drawWhispText(text);
  }
}

void Tutorials::behave(){
	if(GL::isPaused)
		return;
	if(activeTut<0){
		double playerPos=Player::getPlayerById(0)->pos.x/Blocks::defaultBlockSize.x;
		for(int i=0;i<tutorials.size();i++){
			if(!called[i]&&tutorials[i]<=playerPos){
				if(pause[i])
					isPaused=true;
				called[i]=true;
				AL::playSoundByName("heyListen");
				activeTut=i;
				startTutMs=-GL::getGameMs();
				break;
			}
		}
	}
	if(activeTut>=0){
		draw(texts[activeTut]);
		if(startTutMs+GL::getGameMs()>=time[activeTut]&&time[activeTut]){
			activeTut=-1;
			isPaused=false;
		}
	}
}


void Tutorials::clear() {
	activeTut=-1;
	isPaused=false;
	Tutorials::checkpoints.clear();
	Tutorials::tutorials.clear();
	Tutorials::texts.clear();
	Tutorials::keys.clear();
	Tutorials::time.clear();
	Tutorials::pause.clear();
	Tutorials::called.clear();
}

void Tutorials::processCheckpoints() {
	for(int i=0;i<Player::checkpoint;i++){
		for(int j=0;j<tutorials.size();j++){
			if(i<checkpoints.size())
				if(tutorials[j]<=checkpoints[i])
					called[j]=true;
		}
	}
}

void Tutorials::add(int pos,string text,vector<char> keys_, int duration, bool pause_){
	tutorials.push_back(pos);
	texts.push_back(text);
	keys.push_back(keys_);
	time.push_back(duration);
	pause.push_back(pause_);
	called.push_back(false);
}

void Tutorials::add(int pos,string data, bool pause_){
    add(pos,new ADCode(data,"Tutorials"),pause_);
}

void Tutorials::add(int pos,ADCode* adc, bool pause_){
    string text=adc->getString("text");
	vector<string> keys_str=adc->getStringVector("keys");
	int duration=adc->getInt("duration");
	vector<char> keys;
	for(int i=0;i<keys_str.size();i++)
        if(keys_str[i].size()==1){
            keys.push_back(keys_str[i][0]);
        }else{
            if(keys_str[i].find("up")!=string::npos){
                keys.push_back(1);
            }else if(keys_str[i].find("down")!=string::npos){
                keys.push_back(2);
            }else if(keys_str[i].find("left")!=string::npos){
                keys.push_back(3);
            }else if(keys_str[i].find("right")!=string::npos){
                keys.push_back(4);
            }
        }
	if(text!=""&&(keys.size()>0||duration>0))
		add(pos,text,keys,duration,pause_);
    delete adc;
}

void Tutorials::pressKey(char key){
	if(activeTut>=0){
		for(int i=0;i<keys[activeTut].size();i++){
			if(keys[activeTut][i]>0&&keys[activeTut][i]==key){
				activeTut=-1;
				isPaused=false;
				break;
			}
		}
	}
}

string Tutorials::getSinucaTut(){
	if(sinucaTuts.size()==0)
		return "Que displicencia!";
	if(sinucaMs+GL::getGameMs()>=5000){
		int newSinuca=-1;
		do{
			newSinuca=rand()%sinucaTuts.size();
		}while(newSinuca==sinucaCurrent);
		sinucaCurrent=newSinuca;
		sinucaMs=-GL::getGameMs();
	}
	return sinucaTuts[sinucaCurrent];
}

void Tutorials::registerSinucaTuts(){
	sinucaMs=-GL::getGameMs();
	sinucaCurrent=0;
	sinucaTuts.clear();
	sinucaTuts.push_back("Voce pode tentar a jogar do parafuso pra acertar uma bola dificil!");
	sinucaTuts.push_back("So bate na bola e vai");
}

void Tutorials::catchPause(){
	sinucaMs=-GL::getGameMs();
}
