#include "Scenes.hpp"
#include "Bullet.hpp"
#include "powerUp.hpp"
#include "AL.hpp"
#include "Camera.hpp"

Scenes::Scenes() {
}

Scenes::Scenes(const Scenes& orig) {
}

Scenes::~Scenes() {
}

int Scenes::current=300;
const int Scenes::mapEdit=98746;
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
bool Scenes::mapEditCalled=false;
bool Scenes::splashCalled=false;
bool Scenes::optionsCalled=false;
bool Scenes::creditsCalled=false;
bool Scenes::preGameCalled=false;
bool Scenes::preCampaignCalled=false;
bool Scenes::preFreeModeCalled=false;
bool Scenes::freeGameMode=false;
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
    mapEditCalled=false;
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

    GL::framesInGame=0;
    camera.putCameraOnOrigin();
    for(int i=0;i<except.size();i++)
        switch(except[i]){
            case game: gameCalled=true; break;
            case Scenes::mapEdit: mapEditCalled=true; break;
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
        if(freeGameMode)
            Map::changeCurrentMap(Map::usrMap);
        else{
            if(Player::lives<0){ Map::GG(false); return;}
            if(Player::stage>=0&&Player::stage<Map::maps.size())
                Map::changeCurrentMap(Map::maps[Player::stage]);
            else
                Map::changeCurrentMap(Map::maps[0]);
            if(Tutorials::singleton==nullptr)
                new Tutorials("",0);
            if(Player::stage==0&&Player::checkpoint==0)
                Tutorials::singleton->refresh(Tutorials::hudHelp,(int)' ');
        }
        setAllCalledFalseExcept(getUnityVector(game));//fala que ja chamou essa cena
        GL::isPaused=false;
        vector<int> tmp;
        AL::singleton->stopAllSoundsExcept(tmp);
        camera.lookAt(Player::getPlayerById(0)->pos);
    }
    if(ABS(Player::getPlayerById(0)->hSpeed)>0)
        Scenes::camera.moveSpeed=ABS(Player::getPlayerById(0)->hSpeed/GL::getFPS());
//    else if(Player::getPlayerById(0)->OnMOveBLock)
//        Scenes::camera.moveSpeed=ABS(150/GL::getFPS());
    else
        Scenes::camera.moveSpeed=ABS(Entity::walkSpeed/GL::getFPS());

    if((Player::getPlayerById(0)->pos.x>GL::defaultSize.x/2*1.11+Scenes::camera.x.movedCam))
        Scenes::camera.x.movingCam=1;
    else if((Player::getPlayerById(0)->pos.x<GL::defaultSize.x/2*0.99+Scenes::camera.x.movedCam)&&(Scenes::camera.x.movedCam>1.11))
        Scenes::camera.x.movingCam=-1;
    else
        Scenes::camera.x.movingCam=0;

    if(Player::getPlayerById(0)->pos.y<Map::size.y*1.3){
         if((Player::getPlayerById(0)->pos.y>GL::defaultSize.y/1.5*1.11+Scenes::camera.y.movedCam))
            Scenes::camera.y.movingCam=1;
         else if((Player::getPlayerById(0)->pos.y<GL::defaultSize.y/2*0.49+Scenes::camera.y.movedCam)&&(Scenes::camera.y.movedCam>1.11))
            Scenes::camera.y.movingCam=-1;
        else
            Scenes::camera.y.movingCam=0;
    }else if(!Camera::freeCam)
            Scenes::camera.y.movingCam=0;


    gluLookAt(camera.moveSpeed*camera.x.movingCam,camera.moveSpeed*camera.y.movingCam,0,camera.moveSpeed*camera.x.movingCam,camera.moveSpeed*camera.y.movingCam,-1,0,1,0);
    if(camera.x.movingCam)
        camera.x.movedCam+=camera.moveSpeed*camera.x.movingCam;
    if(camera.y.movingCam)
        camera.y.movedCam+=camera.moveSpeed*camera.y.movingCam;
    Mechanics::singleton->applyForce();
    Map::refresh();
    Entity::behave();
    powerUp::behave();
    Bullet::behave();
    Map::transparencyLayerDraw();

    if(!Scenes::freeGameMode&&Player::stage==0){
        double jPos=Player::getPlayerById(0)->pos.x/Blocks::defaultBlockSize.x;
        if(Player::checkpoint==0){
            // if(!Tutorials::initHelpC&&jPos>2){
            //     Tutorials::singleton->refresh(Tutorials::initHelp,GLUT_KEY_UP);
            //     Tutorials::initHelpC=true;
            // }

            // if(!Tutorials::jumpHelpC&&jPos>5){
            //     Tutorials::singleton->refresh(Tutorials::jumpHelp,(int)' ');
            //     Tutorials::jumpHelpC=true;
            // }

            if(!Tutorials::atackHelpC&&jPos>10){
                Tutorials::singleton->refresh(Tutorials::atackHelp,(int)'z');
                Tutorials::atackHelpC=true;
            }

            // if(!Tutorials::rangedAtkHelpC&&jPos>12){
            //     Tutorials::singleton->refresh(Tutorials::rangedAtkHelp,(int)'x');
            //     Tutorials::rangedAtkHelpC=true;
            // }

            // if(!Tutorials::powerupHelpC&&jPos>15){
            //     Tutorials::singleton->refresh(Tutorials::powerupHelp,(int)' ');
            //     Player::getPlayerById(0)->canJump=false;
            //     Tutorials::powerupHelpC=true;
            // }

            // if(!Tutorials::breakHelpC&&jPos>20){
            //     Tutorials::singleton->refresh(Tutorials::breakHelp,(int)'z');
            //     Tutorials::breakHelpC=true;
            // }

            // if(!Tutorials::tpHelpC&&jPos>23){
            //     Tutorials::singleton->refresh(Tutorials::tpHelp,(int)' ');
            //     Player::getPlayerById(0)->canJump=false;
            //     Tutorials::tpHelpC=true;
            // }

            // if(!Tutorials::jumpSpringHelpC&&jPos>30){
            //     Tutorials::singleton->refresh(Tutorials::jumpSpringHelp,(int)' ');
            //     Player::getPlayerById(0)->canJump=false;
            //     Tutorials::jumpSpringHelpC=true;
            // }


            // if(!Tutorials::shooterHelpC&&jPos>36){
            //     Tutorials::singleton->refresh(Tutorials::shooterHelp,(int)' ');
            //     Player::getPlayerById(0)->canJump=false;
            //     Tutorials::shooterHelpC=true;
            // }

            // if(!Tutorials::moveHelpC&&jPos>40){
            //     Tutorials::singleton->refresh(Tutorials::moveHelp,(int)' ');
            //     Player::getPlayerById(0)->canJump=false;
            //     Tutorials::moveHelpC=true;
            // }

            // if(!Tutorials::intercampiHelpC&&jPos>46){
            //     Tutorials::singleton->refresh(Tutorials::intercampiHelp,(int)' ');
            //     Player::getPlayerById(0)->canJump=false;
            //     Tutorials::intercampiHelpC=true;
            // }

            // if(!Tutorials::checkHelpC&&jPos>=50){
            //     Tutorials::singleton->refresh(Tutorials::checkHelp,(int)' ');
            //     Player::getPlayerById(0)->canJump=false;
            //     Tutorials::checkHelpC=true;
            // }
        }
        // if(!Tutorials::bossHelpC&&jPos>190){
        //     Tutorials::singleton->refresh(Tutorials::bossHelp,(int)' ');
        //     Player::getPlayerById(0)->canJump=false;
        //     Tutorials::bossHelpC=true;
        // }
    }
    if(!freeGameMode)Tutorials::singleton->draw();
    if(GL::isPaused)
        GL::drawPause();
    else{
        GL::drawHUD();
        if(freeGameMode){
            AL::singleton->playSoundByName("userSong");
        }else{
            if(Player::stage==0){
                AL::singleton->playSoundByName("stage0");
            }else if(Player::stage==1){
                AL::singleton->playSoundByName("stage1");
            }else if(Player::stage==2){
                AL::singleton->playSoundByName("stage2");
            }
        }
        AL::singleton->stopSound(AL::getSoundByName("sinuca"));
    }
}

/**
 *	Draw, behave and handle events of MapEdit scene on the screen
**/
void Scenes::drawMapEdit(){
    if(!mapEditCalled){//acontece na primeira vez
        setAllCalledFalseExcept(getUnityVector(Scenes::mapEdit));//fala que ja chamou essa cena
        mapEdit::reset();
        Scenes::camera.moveSpeed=10;
        vector<int> tmp;
        tmp.push_back(AL::getSoundByName("menuSong"));
        AL::singleton->stopAllSoundsExcept(tmp);
    }
    AL::singleton->playSoundByName("menuSong");
    if(mapEdit::size.x==0){
        GL::drawTexture(nTRectangle::get(0,GL::defaultSize.y,GL::defaultSize.x,0,-0.9),nTColor::get(0.4,0.4,0.4),GL::getTextureByName("background0"));
        if(fmodl(GL::framesInGame,(int)GL::getFPS()/3)==0){
            if(mapEdit::blankField==""){
                mapEdit::blankField="_";
            }else{
                mapEdit::blankField="";
            }
        }
         if(mapEdit::isCreating==0){
            GL::setFont("BITMAP_TIMES_ROMAN_24");
            GL::drawText(nTPoint::get(100,75,1),"Selecione o modo de edi��o:",GL::getColorByName("violet"));
            GL::drawText(nTPoint::get(75,400,1),"Editar um mapa existente.",GL::getColorByName("violet"));
            GL::drawText(nTPoint::get(450,400,1),"Criar um novo mapa.",GL::getColorByName("violet"));
            if(GL::buttonBehave(nTRectangle::get(100,350,300,150),GL::getColorByName("mouseSelected"),GL::getTextureByName("editMapIcon"),NULL,NULL,NULL,NULL))
                mapEdit::isCreating=-1;
            if(GL::buttonBehave(nTRectangle::get(450,350,650,150),GL::getColorByName("mouseSelected"),GL::getTextureByName("newMapIcon"),NULL,NULL,NULL,NULL))
                mapEdit::isCreating=1;
            if(GL::buttonBehave(nTRectangle::get(200,550,340,500),GL::getColorByName("mouseSelected"),GL::getTextureByName("backIcon"),NULL,NULL,NULL,NULL))
            current=menu;
        }else if(mapEdit::isCreating==1){
            mapEdit::askForSize();
            if(GL::buttonBehave(nTRectangle::get(400,550,540,500),GL::getColorByName("mouseSelected"),GL::getTextureByName("backIcon"),NULL,NULL,NULL,NULL))
                mapEdit::isCreating=0;
        }else if(mapEdit::isCreating==-1){
            mapEdit::askForLoad();
            if(GL::buttonBehave(nTRectangle::get(400,550,540,500),GL::getColorByName("mouseSelected"),GL::getTextureByName("backIcon"),NULL,NULL,NULL,NULL))
                mapEdit::isCreating=0;
        }
    }else{
        gluLookAt(camera.moveSpeed*camera.x.movingCam,camera.moveSpeed*camera.y.movingCam,0,camera.moveSpeed*camera.x.movingCam,camera.moveSpeed*camera.y.movingCam,-1,0,1,0);
        if(camera.x.movingCam)
            camera.x.movedCam+=camera.moveSpeed*camera.x.movingCam;
        if(camera.y.movingCam)
            camera.y.movedCam+=camera.moveSpeed*camera.y.movingCam;
        mapEdit::draw();
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
        AL::singleton->stopAllSoundsExcept(tmp);
    }
    AL::singleton->playSoundByName("menuSong");
    GL::drawTexture(nTRectangle::get(0,GL::defaultSize.y,GL::defaultSize.x,0,-0.9),nTColor::get(0.4,0.4,0.4),GL::getTextureByName("background0"));
    if(AL::getSoundState()){
      GL::buttonBehave(nTRectangle::get(700,50,740,10,0.6),nTColor::Black(),GL::getTextureByName("soundOn"),NULL,*modifySound,NULL,NULL);
    }else{
      GL::buttonBehave(nTRectangle::get(700,50,740,10,0.6),nTColor::Black(),GL::getTextureByName("soundOff"),NULL,*modifySound,NULL,NULL);
    }
    if(AL::getMusicState()){
      GL::buttonBehave(nTRectangle::get(640,50,680,10,0.6),nTColor::Black(),GL::getTextureByName("musicOn"),NULL,*modifyMusic,NULL,NULL);
    }else{
      GL::buttonBehave(nTRectangle::get(640,50,680,10,0.6),nTColor::Black(),GL::getTextureByName("musicOff"),NULL,*modifyMusic,NULL,NULL);
    }
    if(GL::buttonBehave(nTRectangle::get(350,362+10,450,330,0.4),nTColor::get(0.4,0.4,0.4),GL::getTextureByName("playIcon"),NULL,NULL,NULL,NULL))
      Scenes::current=preGame;
    if(GL::buttonBehave(nTRectangle::get(250,404+10,550,372,0.4),nTColor::get(0.4,0.4,0.4),GL::getTextureByName("mapEditIcon"),NULL,NULL,NULL,NULL))
      Scenes::current=mapEdit;
//    if(GL::buttonBehave(nTRectangle::get(340,446+10,460,414,0.4),nTColor::get(0.4,0.4,0.4),GL::getTextureByName("optionsIcon"),NULL,NULL,NULL,NULL))
//      Scenes::current=menu;
    if(GL::buttonBehave(nTRectangle::get(360,446+10,440,414,0.4),nTColor::get(0.4,0.4,0.4),GL::getTextureByName("quitIcon"),NULL,NULL,NULL,NULL))
      exit(0);
        if(GL::buttonBehave(nTRectangle::get(57,557+23,2*57+23,500,0.4),nTColor::get(0.4,0.4,0.4),GL::getTextureByName("cefetLogo"),NULL,NULL,NULL,NULL))
        Scenes::current=credits;
        if(GL::buttonBehave(nTRectangle::get(3*57+190,557+23,4*57+23+190,500,0.4),nTColor::get(0.4,0.4,0.4),GL::getTextureByName("decomLogo"),NULL,NULL,NULL,NULL))
          Scenes::current=credits;
          if(GL::buttonBehave(nTRectangle::get(5*57+400,557+23,6*57+23+400,500,0.4),nTColor::get(0.4,0.4,0.4),GL::getTextureByName("theCOM"),NULL,NULL,NULL,NULL))
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
      AL::singleton->stopAllSoundsExcept(tmp);
  }
  GL::drawTexture((nTRectangle::get(0,600,800,0)),GL::getTextureByName("credits"));
  if(GL::buttonBehave(nTRectangle::get(57,500,187,450,0.4),nTColor::get(0.4,0.4,0.4),GL::getTextureByName("backIcon"),NULL,NULL,NULL,NULL))
    Scenes::current=menu;

}

/**
 *	Draw, behave and handle events of Splash scene on the screen
**/
void Scenes::drawSplash(){
    if(!splashCalled){
        setAllCalledFalseExcept(getUnityVector(splash));
        vector<int> tmp;
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
      vector<int> tmp;
        AL::singleton->stopAllSoundsExcept(tmp);
  }





    //TODO: DESCOMENTAR








  //nao descomentar v
  // if(GL::buttonBehave(Util::nTRectangleSet(340,550,480,500,0,0),GL::getColorByName("mouseSelected"),GL::getTextureByName("backIcon"),NULL,NULL,NULL,NULL))
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

/**
 *	Draw, behave and handle events of PreFreeMode scene on the screen
**/
void Scenes::drawPreFreeMode(){
  if(!preFreeModeCalled){//acontece na primeira vez
      setAllCalledFalseExcept(getUnityVector(preFreeMode));//fala que ja chamou essa cena
      freeGameMode=true;
      vector<int> tmp;
      tmp.push_back(AL::getSoundByName("menuSong"));
      AL::singleton->stopAllSoundsExcept(tmp);
  }

//if(GL::buttonBehave(nTRectangle::get(350,362+10,450,330,0.4),Util::nTColorSet(0.4,0.4,0.4,1),GL::getTextureByName("playIcon"),NULL,NULL,NULL,NULL))
  Scenes::current=game;
  if(GL::buttonBehave(nTRectangle::get(340,550,480,500),GL::getColorByName("mouseSelected"),GL::getTextureByName("backIcon"),NULL,NULL,NULL,NULL))
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
      AL::singleton->stopAllSoundsExcept(tmp);
  }
  AL::singleton->playSoundByName("menuSong");
  GL::setFont("BITMAP_TIMES_ROMAN_24");
  GL::drawTexture(nTRectangle::get(0,GL::defaultSize.y,GL::defaultSize.x,0,-0.9),nTColor::get(0.2,0.2,0.2),GL::getTextureByName("background0"));
  GL::drawText(nTPoint::get(250,75,1),"Selecione o modo de Jogo:",GL::getColorByName("violet"));
  GL::drawText(nTPoint::get(110,350,1),"Modo Campanha.",GL::getColorByName("violet"));
  GL::drawText(nTPoint::get(500,350,1),"Modo livre.",GL::getColorByName("violet"));
  GL::setFont("BITMAP_HELVETICA_18");
  char buffer[5];
  snprintf(buffer,5,"%.2f",Player::globalCoeficiente);
  string strC(buffer);
  GL::drawText(nTPoint::get(95,325,1),"Coeficiente Global: "+strC,GL::getColorByName("blue"));
  if(GL::buttonBehave(nTRectangle::get(200,550,340,500),GL::getColorByName("mouseSelected"),GL::getTextureByName("backIcon"),NULL,NULL,NULL,NULL))
    current=menu;
  if(GL::buttonBehave(nTRectangle::get(50,300,350,100),GL::getColorByName("mouseSelected"),GL::getTextureByName("campaignIcon"),NULL,NULL,NULL,NULL))
    current=preCampaign;
  if(GL::buttonBehave(nTRectangle::get(400,300,700,100),GL::getColorByName("mouseSelected"),GL::getTextureByName("freeModeIcon"),NULL,NULL,NULL,NULL))
    current=preFreeMode;
}

/**
 *	Draw, behave and handle events of PosGame scene on the screen
**/
void Scenes::drawPosGame(){
  if(!posGameCalled){//acontece na primeira vez
      setAllCalledFalseExcept(getUnityVector(posGame));//fala que ja chamou essa cena
      vector<int> tmp;
      AL::singleton->stopAllSoundsExcept(tmp);
      AL::singleton->playSoundByName("diePlayer");
  }
  GL::drawTexture(nTRectangle::get(camera.x.movedCam,GL::defaultSize.y+camera.y.movedCam,GL::defaultSize.x+camera.x.movedCam,camera.y.movedCam,-0.9),GL::getTextureByName("Cefet"));
  GL::drawTexture(nTRectangle::get(100+camera.x.movedCam,GL::defaultSize.y-200+camera.y.movedCam,GL::defaultSize.x-100+camera.x.movedCam,200+camera.x.movedCam,-0.8),nTColor::get(1,0,0),GL::getTextureByName("GameOver"));

  if(GL::buttonBehave(nTRectangle::get(150+camera.x.movedCam,550+camera.y.movedCam,290+camera.x.movedCam,500+camera.y.movedCam),GL::getColorByName("mouseSelected"),GL::getTextureByName("Menu1"),NULL,NULL,NULL,NULL))
    current=menu;
  if(GL::buttonBehave(nTRectangle::get(450+camera.x.movedCam,550+camera.y.movedCam,690+camera.x.movedCam,500+camera.y.movedCam),GL::getColorByName("mouseSelected"),GL::getTextureByName("Continuar1"),NULL,NULL,NULL,NULL))
    current=game;
}

/**
 *	Draw, behave and handle events of EndGame scene on the screen
**/
void Scenes::drawEndGame(){
  if(!posGameEndCalled){//acontece na primeira vez
      setAllCalledFalseExcept(getUnityVector(posGameEnd));//fala que ja chamou essa cena
      vector<int> tmp;
      AL::singleton->stopAllSoundsExcept(tmp);
  }
  AL::singleton->playSoundByName("loseSong");
  GL::drawTexture(nTRectangle::get(0,GL::defaultSize.y,GL::defaultSize.x,0,-0.9),GL::getTextureByName("Cefet"));
  GL::drawTexture(nTRectangle::get(0,GL::defaultSize.y-100,GL::defaultSize.x,0,-0.8),nTColor::get(1,0,0),GL::getTextureByName("Jubilado"));

  if(GL::buttonBehave(nTRectangle::get(300,500,440,450),GL::getColorByName("mouseSelected"),GL::getTextureByName("Menu1"),NULL,NULL,NULL,NULL))
    current=menu;
}

/**
 *	Draw, behave and handle events of YouWin scene on the screen
**/
void Scenes::drawYouWin(){
  if(!posYouWinCalled){//acontece na primeira vez
      setAllCalledFalseExcept(getUnityVector(posYouWin));//fala que ja chamou essa cena
      vector<int> tmp;
      AL::singleton->stopAllSoundsExcept(tmp);
  }
  AL::singleton->playSoundByName("GG");
  GL::drawTexture(nTRectangle::get(0,GL::defaultSize.y,GL::defaultSize.x,0,-0.9),GL::getTextureByName("Win"));
  GL::drawText(nTPoint::get(345,350,1),"Parabens!! Voce cumpriu sua vida",GL::getColorByName("violet"));
  GL::drawText(nTPoint::get(345,385,1),"academica no DECOM com sucesso.",GL::getColorByName("violet"));
  char buffer[5];
  snprintf(buffer,5,"%.2f",Player::globalCoeficiente);
  string strC(buffer);
  GL::drawText(nTPoint::get(345,420,1),"Coeficiente Global: "+strC,GL::getColorByName("green"));
  if(GL::buttonBehave(nTRectangle::get(460,500,600,425),GL::getColorByName("mouseSelected"),GL::getTextureByName("Menu1"),NULL,NULL,NULL,NULL))
    current=credits;
}