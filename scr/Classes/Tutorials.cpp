#include "Tutorials.hpp"
#include "Scenes.hpp"

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
    GLuint friendTex=GL::getTextureByName("Whisp"); //default for unkown levels
    if(!Scenes::freeGameMode){
 		if(Player::stage==Map::lvlTechnical)
        	friendTex=GL::getTextureByName("technical");
      	else if(Player::stage==Map::lvlGraduation)
        	friendTex=GL::getTextureByName("graduation");
		else if(Player::stage==Map::lvlMasters)
			friendTex=GL::getTextureByName("masters");
		else if(Player::stage==Map::lvlWork)
			friendTex=GL::getTextureByName("work");
		else if(Player::stage==Map::lvlGoodTeacher)
			friendTex=GL::getTextureByName("teacher");
		else if(Player::stage==Map::lvlBadTeacher)
			friendTex=GL::getTextureByName("teacher");
    }
	if(text.find("Douglas")!=string::npos)
		friendTex=GL::getTextureByName("Douglinhas");
	if(GL::isPaused)
		friendTex=GL::getTextureByName("Cica");

    GL::drawTexture((nTRectangle::get(Scenes::camera.x.movedCam,
        300+Scenes::camera.y.movedCam,
        800+Scenes::camera.x.movedCam,
        Scenes::camera.y.movedCam,0.999)),
        GL::getTextureByName("TutTextHolder"));
    GL::drawTexture((nTRectangle::get(Scenes::camera.x.movedCam,
        300+Scenes::camera.y.movedCam,
        300+Scenes::camera.x.movedCam,
        Scenes::camera.y.movedCam,0.99998)),
        friendTex);
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
	string text="";
	string keys="";
	string duration="";
	vector<char> keys_vec;
	int duration_int=0;
	size_t found=data.find("text");
	if(found!=string::npos){
		found=data.find("\"",found);
		text=data.substr(found+1);
		text=text.substr(0,text.find("\""));
	}
	if(text=="" && Util::DEBUG)
		cout<<"ERRORRR - missing \"text\" on tutorial block\n";

	found=data.find("keys");
	if(found!=string::npos){
		found=data.find("{",found);
		keys=data.substr(found+1);
		keys=keys.substr(0,keys.find("}"));
		int last=0;
		keys+=",";
		for(int i=0;i<keys.size();i++){
			if(keys[i]==','){
				string str=keys.substr(last,i-last);
				last=i;
				if(str.size()==1){
					keys_vec.push_back(str[0]);
				}else{
					if(str.find("up")!=string::npos){
						keys_vec.push_back(1);
					}else if(str.find("down")!=string::npos){
						keys_vec.push_back(2);
					}else if(str.find("left")!=string::npos){
						keys_vec.push_back(3);
					}else if(str.find("right")!=string::npos){
						keys_vec.push_back(4);
					}
				}
			}
		}
	}else if(Util::DEBUG) cout<<"ERRORRR - missing \"keys\" on tutorial block\n";


	found=data.find("duration");
	if(found!=string::npos){
		bool first=false;
		for(int i=found+1;i<data.size();i++)
			if(isdigit(data[i])){
				first=true;
				duration+=data[i];
			}else if(first)
				break;
		istringstream (duration)>>duration_int;
	}else if(Util::DEBUG) cout<<"ERRORRR - missing \"duration\" on tutorial block\n";
	if(text!=""&&(keys_vec.size()>0||duration_int>0))
		add(pos,text,keys_vec,duration_int,pause_);
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
