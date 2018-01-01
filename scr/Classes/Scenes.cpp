#include "Scenes.hpp"
#include "Bullet.hpp"
#include "PowerUp.hpp"
#include "AL.hpp"
#include "Camera.hpp"

Scenes::Scenes() {
}

Scenes::Scenes(const Scenes& orig) {
}

Scenes::~Scenes() {
}

int Scenes::current=300;
const int Scenes::MapEdit=98746;
const int Scenes::menu=-565;
const int Scenes::game=6516;
const int Scenes::credits=12;
const int Scenes::splash=300;
const int Scenes::options=91;
const int Scenes::preGame=-144;
const int Scenes::preCampaign=-9;
const int Scenes::preFreeMode=-551;
const int Scenes::posGame=78;
const int Scenes::posGameEnd=69;
const int Scenes::posYouWin=7454;
bool Scenes::menuCalled=false;
bool Scenes::gameCalled=false;
bool Scenes::MapEditCalled=false;
bool Scenes::splashCalled=false;
bool Scenes::optionsCalled=false;
bool Scenes::creditsCalled=false;
bool Scenes::preGameCalled=false;
bool Scenes::preCampaignCalled=false;
bool Scenes::preFreeModeCalled=false;
bool Scenes::freeGameMode=false;
bool Scenes::testGameMode=false;
bool Scenes::skipScene=false;
bool Scenes::posGameCalled=false;
bool Scenes::posGameEndCalled=false;
bool Scenes::posYouWinCalled=false;

Camera Scenes::camera;

/**
 *	Gets a vector of int with a value
 *
 *	@param value the value to be inserted on the vector
 *	@return the vector of int with the value
**/
vector<int> Scenes::getUnityVector(int value){
    vector<int> out;
    out.push_back(value);
    return out;
}

/**
 *	Reset the first frame call for every scene except for thoose at a vector
 *
 *	@param except the vector containing the scenes to be ignored by this function
**/
void Scenes::setAllCalledFalseExcept(vector<int> except){
    gameCalled=false;
    MapEditCalled=false;
    menuCalled=false;
    splashCalled=false;
    optionsCalled=false;
    creditsCalled=false;
    preGameCalled=false;
    preCampaignCalled=false;
    preFreeModeCalled=false;
    posGameCalled=false;
    posGameEndCalled=false;
    posYouWinCalled=false;

    GL::clearUI();
    GL::framesInGame=0;
    camera.putCameraOnOrigin();
    for(int i=0;i<except.size();i++)
        switch(except[i]){
            case game: gameCalled=true; break;
            case Scenes::MapEdit: MapEditCalled=true; break;
            case menu: menuCalled=true; break;
            case splash:  splashCalled=true;break;
            case options: optionsCalled=true;break;
            case credits: creditsCalled=true;break;
            case preGame: preGameCalled=true;break;
            case preCampaign: preCampaignCalled=true;break;
            case preFreeMode: preFreeModeCalled=true;break;
            case posGame: posGameCalled=true;break;
            case posGameEnd: posGameEndCalled=true;break;
            case posYouWin: posYouWinCalled=true;break;
        }
}

/**
 *	Draw, behave and handle events of Game scene on the screen
**/
void Scenes::drawGame(){
    if(!gameCalled){//acontece na primeira vez
        if(freeGameMode||testGameMode){
            freeGameMode=true;
            if(testGameMode)
              Map::changeCurrentMap(Map::editingMap);
            else
              Map::changeCurrentMap(Map::usrMap);
        }else{
            if(Player::lives<0){ Map::GG(false); return;}
            if(Player::stage>=0&&Player::stage<Map::maps.size())
                Map::changeCurrentMap(Map::maps[Player::stage]);
            else
                Map::changeCurrentMap(Map::maps[0]);
        }
        setAllCalledFalseExcept(getUnityVector(game));//fala que ja chamou essa cena
        GL::isPaused=false;
        vector<int> tmp;
        tmp.push_back(AL::getSoundByName("mouse"));
        AL::singleton->stopAllSoundsExcept(tmp);
        camera.lookAt(Player::getPlayerById(0)->pos);
    }

    camera.behave(Player::getPlayerById(0));
    Mechanics::applyForce();
    Map::refresh();
    Entity::behave();
    PowerUp::behave();
    Bullet::behave();
    Tutorials::behave();
    Map::transparencyLayerDraw();
    playStageSong();
    if(GL::isPaused)
        GL::drawPause();
    else
        GL::drawHUD();
}

/**
 *	Draw, behave and handle events of MapEdit scene on the screen
**/
void Scenes::drawMapEdit(){
    if(!MapEditCalled){//acontece na primeira vez
        setAllCalledFalseExcept(getUnityVector(Scenes::MapEdit));//fala que ja chamou essa cena
        if(!Scenes::testGameMode){
          MapEdit::reset();
        }else{
          testGameMode=false;
        }
        Scenes::camera.moveSpeed=10;
        vector<int> tmp;
        tmp.push_back(AL::getSoundByName("mouse"));
        tmp.push_back(AL::getSoundByName("menuSong"));
        AL::singleton->stopAllSoundsExcept(tmp);
    }
    AL::singleton->playSoundByName("menuSong");
    if(MapEdit::size.x==0){
        GL::drawTexture(nTRectangle::get(0,GL::defaultSize.y,GL::defaultSize.x,0,-0.9),nTColor::get(0.4,0.4,0.4),GL::getTextureByName("background0"));
         if(MapEdit::isCreating==0){
            GL::setFont("BITMAP_TIMES_ROMAN_24");
            GL::drawText(nTPoint::get(100,75,1),"Selecione o modo de edi��o:",GL::getColorByName("violet"));
            GL::drawText(nTPoint::get(75,400,1),"Editar um mapa existente.",GL::getColorByName("violet"));
            GL::drawText(nTPoint::get(450,400,1),"Criar um novo mapa.",GL::getColorByName("violet"));
            if(GL::buttonBehave(nTRectangle::get(100,350,300,150),GL::getColorByName("mouseSelected"),GL::getTextureByName("editMapIcon")))
                MapEdit::isCreating=-1;
            if(GL::buttonBehave(nTRectangle::get(450,350,650,150),GL::getColorByName("mouseSelected"),GL::getTextureByName("newMapIcon")))
                MapEdit::isCreating=1;
            if(GL::buttonBehave(nTRectangle::get(400,550,540,500),GL::getColorByName("mouseSelected"),GL::getTextureByName("backIcon")))
              current=menu;
        }else if(MapEdit::isCreating==1){
            MapEdit::askForSize();
            if(GL::buttonBehave(nTRectangle::get(400,550,540,500),GL::getColorByName("mouseSelected"),GL::getTextureByName("backIcon"))){
                MapEdit::isCreating=0;
                GL::clearUI();
            }
        }else if(MapEdit::isCreating==-1){
            MapEdit::askForLoad();
            if(GL::buttonBehave(nTRectangle::get(400,550,540,500),GL::getColorByName("mouseSelected"),GL::getTextureByName("backIcon"))){
                MapEdit::isCreating=0;
                GL::clearUI();
            }
        }
    }else{
        gluLookAt(camera.moveSpeed*camera.x.movingCam,camera.moveSpeed*camera.y.movingCam,0,camera.moveSpeed*camera.x.movingCam,camera.moveSpeed*camera.y.movingCam,-1,0,1,0);
        if(camera.x.movingCam)
            camera.x.movedCam+=camera.moveSpeed*camera.x.movingCam;
        if(camera.y.movingCam)
            camera.y.movedCam+=camera.moveSpeed*camera.y.movingCam;
        MapEdit::draw();
    }
}

/**
 *	Draw, behave and handle events of Menu scene on the screen
**/
void Scenes::drawMenu(){
    if(!menuCalled){//acontece na primeira vez
        setAllCalledFalseExcept(getUnityVector(menu));//fala que ja chamou essa cena
        vector<int> tmp;
        tmp.push_back(AL::getSoundByName("menuSong"));
        tmp.push_back(AL::getSoundByName("mouse"));
        AL::singleton->stopAllSoundsExcept(tmp);
    }
    AL::singleton->playSoundByName("menuSong");
    GL::drawTexture(nTRectangle::get(0,GL::defaultSize.y,GL::defaultSize.x,0,-0.9),nTColor::get(0.4,0.4,0.4),GL::getTextureByName("background0"));
    if(AL::getSoundState()){
    	GL::buttonBehave(nTRectangle::get(700,50,740,10,0.6),nTColor::Black(),GL::getTextureByName("soundOn"),false,NULL,*modifySound,NULL,NULL);
    }else{
    	GL::buttonBehave(nTRectangle::get(700,50,740,10,0.6),nTColor::Black(),GL::getTextureByName("soundOff"),false,NULL,*modifySound,NULL,NULL);
    }
    if(AL::getMusicState()){
    	GL::buttonBehave(nTRectangle::get(640,50,680,10,0.6),nTColor::Black(),GL::getTextureByName("musicOn"),false,NULL,*modifyMusic,NULL,NULL);
    }else{
    	GL::buttonBehave(nTRectangle::get(640,50,680,10,0.6),nTColor::Black(),GL::getTextureByName("musicOff"),false,NULL,*modifyMusic,NULL,NULL);
    }
    if(GL::buttonBehave(nTRectangle::get(350,362+10,450,330,0.4),nTColor::get(0.4,0.4,0.4),GL::getTextureByName("playIcon")))
     	Scenes::current=preGame;
    if(GL::buttonBehave(nTRectangle::get(250,404+10,550,372,0.4),nTColor::get(0.4,0.4,0.4),GL::getTextureByName("MapEditIcon")))
    	Scenes::current=MapEdit;
//    if(GL::buttonBehave(nTRectangle::get(340,446+10,460,414,0.4),nTColor::get(0.4,0.4,0.4),GL::getTextureByName("optionsIcon")))
//      Scenes::current=menu;
    if(GL::buttonBehave(nTRectangle::get(360,446+10,440,414,0.4),nTColor::get(0.4,0.4,0.4),GL::getTextureByName("quitIcon")))
      	exit(0);
    if(GL::buttonBehave(nTRectangle::get(57,557+23,2*57+23,500,0.4),nTColor::get(0.4,0.4,0.4),GL::getTextureByName("cefetLogo"))||
      GL::buttonBehave(nTRectangle::get(3*57+190,557+23,4*57+23+190,500,0.4),nTColor::get(0.4,0.4,0.4),GL::getTextureByName("decomLogo"))||
      GL::buttonBehave(nTRectangle::get(5*57+400,557+23,6*57+23+400,500,0.4),nTColor::get(0.4,0.4,0.4),GL::getTextureByName("theCOM")))
        Scenes::current=credits;
    GL::drawTexture(nTRectangle::get(100,300,700,0,0.4),GL::getTextureByName("legendsLogo"));
}

/**
 *	Draw, behave and handle events of Options scene on the screen
**/
void Scenes::drawOptions(){

}

/**
 *	Draw, behave and handle events of Credits scene on the screen
**/
void Scenes::drawCredits(){
  	if(!creditsCalled){
      	setAllCalledFalseExcept(getUnityVector(credits));
      	vector<int> tmp;
      	tmp.push_back(AL::singleton->getSoundByName("mouse"));
      	AL::singleton->stopAllSoundsExcept(tmp);
  	}
  	GL::drawTexture((nTRectangle::get(0,600,800,0)),GL::getTextureByName("credits"));
  	if(GL::buttonBehave(nTRectangle::get(57,500+50,187,450+50,0.4),nTColor::get(0.4,0.4,0.4),GL::getTextureByName("backIcon")))
    	Scenes::current=menu;
}

/**
 *	Draw, behave and handle events of Splash scene on the screen
**/
void Scenes::drawSplash(){
    if(!splashCalled){
        setAllCalledFalseExcept(getUnityVector(splash));
        vector<int> tmp;
        tmp.push_back(AL::getSoundByName("mouse"));
        AL::singleton->stopAllSoundsExcept(tmp);
    }





    //TODO: DESCOMENTAR








  // double x=(GL::framesInGame/GL::getFPS())*300;
  // if(skipScene&&x>1000){
  //   skipScene=false;
  //   current=menu;}
  // if(x<500)
  //   GL::drawTexture((Util::nTRectangleSet(0,600,800,0,0,0)),GL::getTextureByName("legalNote"));
  // if(x>=500&&x<1000){
  //   x-=500;
  //   GL::drawTexture((Util::nTRectangleSet(0,600,800,0,0,0)),Util::nTColorSet(1,1,1,1-x/500),GL::getTextureByName("legalNote"));

  // }
  // if(x>=1000&&x<1500){
  //   x-=1000;
  //   GL::drawTexture((Util::nTRectangleSet(50,450,750,150,0,0)),GL::getTextureByName("decomCelebrations"));
  // }
  // if(x>=1500&&x<2000){
  //   x-=1500;
  //   GL::drawTexture((Util::nTRectangleSet(50,450,750,150,0,0)),Util::nTColorSet(1,1,1,1-x/500),GL::getTextureByName("decomCelebrations"));
  // }
  // if(x>=2000&&x<2500){
  //   x-=2000;
  //   GL::drawTexture((Util::nTRectangleSet(57,580,637-x,x,0.1,0.1)),GL::getTextureByName("cefetLogo"));
  // }
  // if(x>=2500&&x<3000){
  //   x-=2500;
  //   GL::drawTexture((Util::nTRectangleSet(57,580,137,500,0.1,0.1)),GL::getTextureByName("cefetLogo"));
  //   GL::drawTexture((Util::nTRectangleSet(111+x/2,580,691-x/2,x,0.2,0.2)),GL::getTextureByName("decomLogo"));
  // }
  // if(x>=3000&&x<3500){
  //   x-=3000;
  //   GL::drawTexture((Util::nTRectangleSet(57,580,137,500,0.1,0.1)),GL::getTextureByName("cefetLogo"));
  //   GL::drawTexture((Util::nTRectangleSet(361,580,441,500,0.2,0.2)),GL::getTextureByName("decomLogo"));
  //   GL::drawTexture((Util::nTRectangleSet(185+x,580,756,x,0.3,0.3)),GL::getTextureByName("theCOM"));
  // }
  // if(x>=3500&&x<4000){
  //   x-=3500;
  //   GL::drawTexture((Util::nTRectangleSet(57,580,137,500,0.1,0.1)),GL::getTextureByName("cefetLogo"));
  //   GL::drawTexture((Util::nTRectangleSet(361,580,441,500,0.2,0.2)),GL::getTextureByName("decomLogo"));
  //   GL::drawTexture((Util::nTRectangleSet(685,580,756,500,0.3,0.3)),GL::getTextureByName("theCOM"));
  //   GL::drawTexture((Util::nTRectangleSet(100,300,700,0,0.4,0.4)),Util::nTColorSet(1,1,1,x/500),GL::getTextureByName("legendsLogo"));

  // }
  // if(x>4000)
    Scenes::current=menu;
}

/**
 *	Draw, behave and handle events of PreCampaing scene on the screen
**/
void Scenes::drawPreCampaign(){
	if(!preCampaignCalled){//acontece na primeira vez
		setAllCalledFalseExcept(getUnityVector(preCampaign));//fala que ja chamou essa cena
		freeGameMode=false;
		testGameMode=false;
		vector<int> tmp;
		tmp.push_back(AL::getSoundByName("mouse"));
		AL::singleton->stopAllSoundsExcept(tmp);
	}
	if(Player::beatGame){
			if(GL::buttonBehave(nTRectangle::get(37.5,250,262.5,100),GL::getColorByName("mouseSelected"),GL::getTextureByName("miniLvl0"))){
			Player::stage=0;
			Player::checkpoint=0;
			Player::getPlayerById(0)->life=Player::defaultLife;
      current=game;
		}
		if(GL::buttonBehave(nTRectangle::get(287.5,250,512.5,100),GL::getColorByName("mouseSelected"),GL::getTextureByName("miniLvl1"))){
			Player::stage=1;
			Player::checkpoint=0;
			Player::getPlayerById(0)->life=Player::defaultLife;
      current=game;
		}
		if(GL::buttonBehave(nTRectangle::get(537.5,250,762.5,100),GL::getColorByName("mouseSelected"),GL::getTextureByName("miniLvl2"))){
			Player::stage=2;
			Player::checkpoint=0;
			Player::getPlayerById(0)->life=Player::defaultLife;
      current=game;
		}
		if(GL::buttonBehave(nTRectangle::get(37.5,450,262.5,300),GL::getColorByName("mouseSelected"),GL::getTextureByName("miniLvl3"))){
			Player::stage=3;
			Player::checkpoint=0;
			Player::getPlayerById(0)->life=Player::defaultLife;
      current=game;
		}
		if(GL::buttonBehave(nTRectangle::get(287.5,450,512.5,300),GL::getColorByName("mouseSelected"),GL::getTextureByName("miniLvl4"))){
			Player::stage=4;
			Player::checkpoint=0;
			Player::getPlayerById(0)->life=Player::defaultLife;
      current=game;
		}
		if(GL::buttonBehave(nTRectangle::get(537.5,450,762.5,300),GL::getColorByName("mouseSelected"),GL::getTextureByName("miniLvl5"))){
			Player::stage=5;
			Player::checkpoint=0;
			Player::getPlayerById(0)->life=Player::defaultLife;
      current=game;
		}
		if(GL::buttonBehave(nTRectangle::get(340,550,480,500),GL::getColorByName("mouseSelected"),GL::getTextureByName("backIcon")))
			current=menu;
  	}else{
    //TODO: DESCOMENTAR
    //nao descomentar v
  // if(GL::buttonBehave(Util::nTRectangleSet(340,550,480,500,0,0),GL::getColorByName("mouseSelected"),GL::getTextureByName("backIcon")))
    // current=preGame;
  //nao descomentar ^
  // double x=(GL::framesInGame/GL::getFPS())*300;
  // GL::setFontByIndex(0);
  // if(x<500)
  //   GL::drawText(Util::nTPointSet(250,200,1),"Realizando prova do CEFET",GL::getColorByName("violet"));
  // if(x>=500&&x<1000){
  //   x-=500;
  //   GL::drawText(Util::nTPointSet(250,200,1),"Realizando prova do CEFET",GL::getColorByName("violet"));
  //   GL::drawRectangle(Util::nTRectangleSet(240,320,280,280,0,0),Util::nTColorSet(1,1,1,x/500));
  // }
  // if(x>=1000&&x<1500){
  //   x-=1000;
  //   GL::drawText(Util::nTPointSet(250,200,1),"Realizando prova do CEFET",GL::getColorByName("violet"));
  //   GL::drawRectangle(Util::nTRectangleSet(240,320,280,280,0,0),Util::nTColorSet(1,1,1,1));
  //   GL::drawRectangle(Util::nTRectangleSet(360,320,400,280,0,0),Util::nTColorSet(1,1,1,x/500));
  // }
  // if(x>=1500&&x<2000){
  //   x-=1500;
  //   GL::drawText(Util::nTPointSet(250,200,1),"Realizando prova do CEFET",GL::getColorByName("violet"));
  //   GL::drawRectangle(Util::nTRectangleSet(240,320,280,280,0,0),Util::nTColorSet(1,1,1,1));
  //   GL::drawRectangle(Util::nTRectangleSet(360,320,400,280,0,0),Util::nTColorSet(1,1,1,1));
  //   GL::drawRectangle(Util::nTRectangleSet(480,320,520,280,0,0),Util::nTColorSet(1,1,1,x/500));
  // }
  // if(x>=2000&&x<2500){
  //   x-=2000;
  //   GL::drawRectangle(Util::nTRectangleSet(360,200,400,350,0,0),Util::nTColorSet(1,1,1,1));
  //   GL::drawRectangle(Util::nTRectangleSet(360,390,400,430,0,0),Util::nTColorSet(1,1,1,1));
  // }
  // if(x>=2500&&x<3000){
  //   x-=2500;
  //   GL::drawTexture(Util::nTRectangleSet(0,GL::defaultSize.y,GL::defaultSize.x,0,-0.9,-0.9),Util::nTColorSet(1,1,1,x/500),GL::getTextureByName("aprovado"));
  //   GL::drawRectangle(Util::nTRectangleSet(360,200,400,350,0,0),Util::nTColorSet(1,1,1,1-x/500));
  //   GL::drawRectangle(Util::nTRectangleSet(360,390,400,430,0,0),Util::nTColorSet(1,1,1,1-x/500));
  // }
  // if(x>=3000&&x<3500){
  //   x-=3000;
  //   GL::drawTexture(Util::nTRectangleSet(0,GL::defaultSize.y,GL::defaultSize.x,0,-0.9,-0.9),GL::getTextureByName("aprovado"));
  // }
  // if(x>=3500&&x<4000){
  //   x-=3500;
  //   GL::drawTexture(Util::nTRectangleSet(0,GL::defaultSize.y,GL::defaultSize.x,0,-0.9,-0.9),Util::nTColorSet(1,1,1,1-x/500),GL::getTextureByName("aprovado"));
  // }
  // if(x>=4500||Player::stage!=0||Player::checkpoint!=0)
		current=game;
  // else
  //     al->playSoundByName("makeTest");
  	}
}

/**
 *	Draw, behave and handle events of PreFreeMode scene on the screen
**/
void Scenes::drawPreFreeMode(){
	if(!preFreeModeCalled){//acontece na primeira vez
		setAllCalledFalseExcept(getUnityVector(preFreeMode));//fala que ja chamou essa cena
		freeGameMode=true;
		testGameMode=false;
		vector<int> tmp;
		tmp.push_back(AL::getSoundByName("menuSong"));
		tmp.push_back(AL::getSoundByName("mouse"));
		AL::singleton->stopAllSoundsExcept(tmp);
	}
//if(GL::buttonBehave(nTRectangle::get(350,362+10,450,330,0.4),Util::nTColorSet(0.4,0.4,0.4,1),GL::getTextureByName("playIcon")))
  	Scenes::current=game;
  	if(GL::buttonBehave(nTRectangle::get(340,550,480,500),GL::getColorByName("mouseSelected"),GL::getTextureByName("backIcon")))
    	current=preGame;
  //TODO implementar modo de escolher mapa
}

/**
 *	Draw, behave and handle events of PreGame scene on the screen
**/
void Scenes::drawPreGame(){
	if(!preGameCalled){//acontece na primeira vez
		setAllCalledFalseExcept(getUnityVector(preGame));//fala que ja chamou essa cena
		vector<int> tmp;
		tmp.push_back(AL::getSoundByName("menuSong"));
		tmp.push_back(AL::getSoundByName("mouse"));
		AL::singleton->stopAllSoundsExcept(tmp);
	}
  	AL::singleton->playSoundByName("menuSong");
	GL::setFont("BITMAP_TIMES_ROMAN_24");
	GL::drawTexture(nTRectangle::get(0,GL::defaultSize.y,GL::defaultSize.x,0,-0.9),nTColor::get(0.2,0.2,0.2),GL::getTextureByName("background0"));
	GL::drawText(nTPoint::get(250,75,1),"Selecione o modo de Jogo:",GL::getColorByName("violet"));
	GL::drawText(nTPoint::get(110,350,1),"Modo Campanha.",GL::getColorByName("violet"));
	GL::drawText(nTPoint::get(500,350,1),"Modo livre.",GL::getColorByName("violet"));
	GL::setFont("BITMAP_HELVETICA_18");
	GL::drawText(nTPoint::get(95,325,1),"Coeficiente Global: "+Util::floatToStr(Player::globalCoeficiente),GL::getColorByName("blue"));
	if(GL::buttonBehave(nTRectangle::get(200,550,340,500),GL::getColorByName("mouseSelected"),GL::getTextureByName("backIcon")))
		current=menu;
	if(GL::buttonBehave(nTRectangle::get(50,300,350,100),GL::getColorByName("mouseSelected"),GL::getTextureByName("campaignIcon")))
		current=preCampaign;
	if(GL::buttonBehave(nTRectangle::get(400,300,700,100),GL::getColorByName("mouseSelected"),GL::getTextureByName("freeModeIcon")))
		current=preFreeMode;
}

/**
 *	Draw, behave and handle events of PosGame scene on the screen
**/
void Scenes::drawPosGame(){
	if(!posGameCalled){//acontece na primeira vez
		setAllCalledFalseExcept(getUnityVector(posGame));//fala que ja chamou essa cena
		vector<int> tmp;
		tmp.push_back(AL::getSoundByName("mouse"));
		AL::singleton->stopAllSoundsExcept(tmp);
		AL::singleton->playSoundByName("diePlayer");
		if(Scenes::testGameMode){
			current=MapEdit;
		}
	}
	GL::drawTexture(nTRectangle::get(camera.x.movedCam,GL::defaultSize.y+camera.y.movedCam,GL::defaultSize.x+camera.x.movedCam,camera.y.movedCam,-0.9),GL::getTextureByName("Cefet"));
	GL::drawTexture(nTRectangle::get(100+camera.x.movedCam,GL::defaultSize.y-200+camera.y.movedCam,GL::defaultSize.x-100+camera.x.movedCam,200+camera.x.movedCam,-0.8),nTColor::get(1,0,0),GL::getTextureByName("GameOver"));

	if(GL::buttonBehave(nTRectangle::get(150+camera.x.movedCam,550+camera.y.movedCam,290+camera.x.movedCam,500+camera.y.movedCam),GL::getColorByName("mouseSelected"),GL::getTextureByName("Menu1")))
		current=menu;
	if(GL::buttonBehave(nTRectangle::get(450+camera.x.movedCam,550+camera.y.movedCam,690+camera.x.movedCam,500+camera.y.movedCam),GL::getColorByName("mouseSelected"),GL::getTextureByName("Continuar1")))
		current=game;
}

/**
 *	Draw, behave and handle events of EndGame scene on the screen
**/
void Scenes::drawEndGame(){
	if(!posGameEndCalled){//acontece na primeira vez
		setAllCalledFalseExcept(getUnityVector(posGameEnd));//fala que ja chamou essa cena
		vector<int> tmp;
		tmp.push_back(AL::getSoundByName("mouse"));
		AL::singleton->stopAllSoundsExcept(tmp);
	}
	AL::singleton->playSoundByName("loseSong");
	GL::drawTexture(nTRectangle::get(0,GL::defaultSize.y,GL::defaultSize.x,0,-0.9),GL::getTextureByName("Cefet"));
	GL::drawTexture(nTRectangle::get(0,GL::defaultSize.y-100,GL::defaultSize.x,0,-0.8),nTColor::get(1,0,0),GL::getTextureByName("Jubilado"));

	if(GL::buttonBehave(nTRectangle::get(300,500,440,450),GL::getColorByName("mouseSelected"),GL::getTextureByName("Menu1")))
		current=menu;
}

/**
 *	Draw, behave and handle events of YouWin scene on the screen
**/
void Scenes::drawYouWin(){
	if(!posYouWinCalled){//acontece na primeira vez
		setAllCalledFalseExcept(getUnityVector(posYouWin));//fala que ja chamou essa cena
		vector<int> tmp;
		tmp.push_back(AL::getSoundByName("mouse"));
		AL::singleton->stopAllSoundsExcept(tmp);
	}
	AL::singleton->playSoundByName("GG");
	GL::drawTexture(nTRectangle::get(0,GL::defaultSize.y,GL::defaultSize.x,0,-0.9),GL::getTextureByName("Win"));
	GL::drawText(nTPoint::get(345,350,1),"Parabens!! Voce cumpriu sua vida",GL::getColorByName("violet"));
	GL::drawText(nTPoint::get(345,385,1),"academica no DECOM com sucesso.",GL::getColorByName("violet"));
	GL::drawText(nTPoint::get(345,420,1),"Coeficiente Global: "+Util::floatToStr(Player::globalCoeficiente),GL::getColorByName("green"));
	if(GL::buttonBehave(nTRectangle::get(460,500,600,425),GL::getColorByName("mouseSelected"),GL::getTextureByName("Menu1")))
		current=credits;
}

void Scenes::playStageSong(){
  if(freeGameMode){
      AL::singleton->playSoundByName("userSong");
  }else{
      if(Player::stage==0){
          AL::singleton->playSoundByName("stage0");
      }else if(Player::stage==1){
          AL::singleton->playSoundByName("stage1");
      }else if(Player::stage==2){
          AL::singleton->playSoundByName("stage2");
      }else if(Player::stage==3){
          AL::singleton->playSoundByName("stage2");
      }else if(Player::stage==4){
          AL::singleton->playSoundByName("stage2");
      }else if(Player::stage==5){
          AL::singleton->playSoundByName("stage2");
      }
  }
  if(!GL::isPaused)AL::singleton->stopSound(AL::getSoundByName("sinuca"));
}
