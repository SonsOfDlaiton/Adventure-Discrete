#include "Tutorials.hpp"
#include "Scenes.hpp"

Tutorials::Tutorials(string text,int key){
  clear();
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

/**
 *	Draw the tutorial message text on the screen
 *
 *	@param text the string to be drawed
**/
void Tutorials::drawWhispText(string text){
    GL::setFont("BITMAP_HELVETICA_18");
    int maxChars=50;
    for(int i=0;i<text.size();i+=maxChars){
        GL::drawText(nTPoint::get(320+Scenes::camera.x.movedCam,Scenes::camera.y.movedCam+90+(i/maxChars)*25,1),text.substr(i,maxChars),GL::getColorByName("yellow"));
    }
}

/**
 *	Draw the tutorial on the screen
**/
void Tutorials::draw(){
  if(GL::isPaused)
    return;
  if(showTutorials){
    GLuint friendTex=GL::getTextureByName("Whisp"); //default for unkown levels

    //TODO mudar textura dependendo da fase

    GL::drawTexture((nTRectangle::get(Scenes::camera.x.movedCam,
        300+Scenes::camera.y.movedCam,
        800+Scenes::camera.x.movedCam,
        Scenes::camera.y.movedCam,0.9999)),
        friendTex);
    drawWhispText(texts[activeTut]);
  }
}

void Tutorials::behave(){
  if(activeTut<0){
    double playerPos=Player::getPlayerById(0)->pos.x/Blocks::defaultBlockSize.x;
    for(int i=0;i<tutorials.size();i++){
      if(!called[i]&&tutorials[i]<=playerPos){
        called[i]=true;
        AL::playSoundByName("heyListen");
        activeTut=i;
        startTutMs=-GL::getGameMs();
        if(pause[i])
          isPaused=true;
        break;
      }
    }
  }
  if(activeTut>=0){
    draw();
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
      if(tutorials[j]<=checkpoints[i]){
        called[j]=true;
      }
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
  if(text=="")
    if(Util::DEBUG) cout<<"ERRORRR - missing \"text\" on tutorial block\n";

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
