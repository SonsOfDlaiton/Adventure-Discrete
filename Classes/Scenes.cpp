#include "Scenes.hpp"
#include "Bullet.hpp"
#include "powerUp.hpp"
#include "AL.hpp"

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

bool Scenes::freeCam=false;
Camera Scenes::camera;

vector<int> Scenes::getUnityVector(int value){
    vector<int> out;
    out.push_back(value);
    return out;
}

void Scenes::putCameraOnOrigin(){
    gluLookAt(camera.x.movedCam*-1,camera.y.movedCam*-1,0,camera.x.movedCam*-1,camera.y.movedCam*-1,-1,0,1,0);
    camera.x.movedCam=0;
    camera.y.movedCam=0;
    gluLookAt(0,0,0,0,0,-1,0,1,0);
}

void Scenes::lookAt(nTPoint pos){
    pos.x-=GL::defaultSize.x/2+camera.x.movedCam;
    pos.y-=GL::defaultSize.y/2+camera.y.movedCam;
    gluLookAt(pos.x,pos.y,0,pos.x,pos.y,-1,0,1,0);
    camera.x.movedCam+=pos.x;
    camera.y.movedCam+=pos.y;
    gluLookAt(0,0,0,0,0,-1,0,1,0);
}

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
    putCameraOnOrigin();
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
void Scenes::drawGame(){
    if(!gameCalled){//acontece na primeira vez  
        if(freeGameMode)
            Map::changeCurrentMap(usrMap);
        else{
            if(Player::lives<0){ Map::GG(false); return;}
            if(Player::stage>=0&&Player::stage<maps.size())
                Map::changeCurrentMap(maps[Player::stage]);
            else
                Map::changeCurrentMap(maps[0]);
            if(Player::stage==0&&Player::checkpoint==0)
                zero->refresh(Tutorials::hudHelp,(int)' ');
        }
        setAllCalledFalseExcept(getUnityVector(game));//fala que ja chamou essa cena
        GL::isPaused=false;
        vector<int> tmp;
        al->stopAllSoundsExcept(tmp);
        lookAt(player->pos);
    }
    if(ABS(player->hSpeed)>0)
        Scenes::camera.moveSpeed=ABS(player->hSpeed/GL::getFPS());
    else if(player->forceNormalForce)
        Scenes::camera.moveSpeed=ABS(150/GL::getFPS());
    else
        Scenes::camera.moveSpeed=ABS(Entity::walkSpeed/GL::getFPS());
    
    if((player->pos.x>GL::defaultSize.x/2*1.11+Scenes::camera.x.movedCam))
        Scenes::camera.x.movingCam=1;
    else if((player->pos.x<GL::defaultSize.x/2*0.99+Scenes::camera.x.movedCam)&&(Scenes::camera.x.movedCam>1.11))
        Scenes::camera.x.movingCam=-1;
    else
        Scenes::camera.x.movingCam=0;
    
    if(Map::currentMap.size()>23&&player->pos.y<Map::size.y*1.2){
         if((player->pos.y>GL::defaultSize.y/1.5*1.11+Scenes::camera.y.movedCam))
            Scenes::camera.y.movingCam=1;
         else if((player->pos.y<GL::defaultSize.y/2*0.99+Scenes::camera.y.movedCam)&&(Scenes::camera.y.movedCam>1.11))
            Scenes::camera.y.movingCam=-1;
        else
            Scenes::camera.y.movingCam=0;
    }else if(!Scenes::freeCam)
            Scenes::camera.y.movingCam=0;
    
    
    gluLookAt(camera.moveSpeed*camera.x.movingCam,camera.moveSpeed*camera.y.movingCam,0,camera.moveSpeed*camera.x.movingCam,camera.moveSpeed*camera.y.movingCam,-1,0,1,0);
    if(camera.x.movingCam)
        camera.x.movedCam+=camera.moveSpeed*camera.x.movingCam;
    if(camera.y.movingCam)
        camera.y.movedCam+=camera.moveSpeed*camera.y.movingCam;
    world->applyForce();
    Map::refresh();
    Enemy::behave();
    powerUp::behave();
    Bullet::behave();
    
    if(!Scenes::freeGameMode&&Player::stage==0){
        float jPos=player->pos.x/Blocks::defaultBlockSize.x;
        if(Player::checkpoint==0){
            if(!Tutorials::initHelpC&&jPos>2){
                zero->refresh(Tutorials::initHelp,GLUT_KEY_UP);
                Tutorials::initHelpC=true;
            }

            if(!Tutorials::jumpHelpC&&jPos>5){
                zero->refresh(Tutorials::jumpHelp,(int)' ');
                Tutorials::jumpHelpC=true; 
            }

            if(!Tutorials::atackHelpC&&jPos>10){
                zero->refresh(Tutorials::atackHelp,(int)'z');
                Tutorials::atackHelpC=true;
            }

            if(!Tutorials::rangedAtkHelpC&&jPos>12){
                zero->refresh(Tutorials::rangedAtkHelp,(int)'x');
                Tutorials::rangedAtkHelpC=true;
            }

            if(!Tutorials::powerupHelpC&&jPos>15){
                zero->refresh(Tutorials::powerupHelp,(int)' ');
                player->canJump=false;
                Tutorials::powerupHelpC=true;
            }

            if(!Tutorials::breakHelpC&&jPos>20){
                zero->refresh(Tutorials::breakHelp,(int)'z');
                Tutorials::breakHelpC=true;
            }

            if(!Tutorials::tpHelpC&&jPos>23){
                zero->refresh(Tutorials::tpHelp,(int)' ');
                player->canJump=false;
                Tutorials::tpHelpC=true;
            }

            if(!Tutorials::jumpSpringHelpC&&jPos>30){
                zero->refresh(Tutorials::jumpSpringHelp,(int)' ');
                player->canJump=false;
                Tutorials::jumpSpringHelpC=true;
            }


            if(!Tutorials::shooterHelpC&&jPos>36){
                zero->refresh(Tutorials::shooterHelp,(int)' ');
                player->canJump=false;
                Tutorials::shooterHelpC=true;
            }

            if(!Tutorials::moveHelpC&&jPos>40){
                zero->refresh(Tutorials::moveHelp,(int)' ');
                player->canJump=false;
                Tutorials::moveHelpC=true;
            }

            if(!Tutorials::intercampiHelpC&&jPos>46){
                zero->refresh(Tutorials::intercampiHelp,(int)' ');
                player->canJump=false;
                Tutorials::intercampiHelpC=true;
            }
            
            if(!Tutorials::checkHelpC&&jPos>=50){
                zero->refresh(Tutorials::checkHelp,(int)' ');
                player->canJump=false;
                Tutorials::checkHelpC=true;
            }
        }
        if(!Tutorials::bossHelpC&&jPos>190){
            zero->refresh(Tutorials::bossHelp,(int)' ');
            player->canJump=false;
            Tutorials::bossHelpC=true;
        }  
    }
    
    
    zero->draw();   
    if(player->itsInTheWater)player->draw(GL::getColorByName("blue")); else player->draw(Util::nTColorSet(1,1,1,1));
    if(GL::isPaused)
        GL::drawPause();
    else{
        GL::drawHUD();
        if(freeGameMode){
            al->playSoundByName("userSong");
        }else{
            if(Player::stage==0){
                al->playSoundByName("stage0");
            }else if(Player::stage==1){
                al->playSoundByName("stage1");
            }else if(Player::stage==2){
                al->playSoundByName("stage2");
            }
        }
        al->stopSound(AL::getSoundByName("sinuca"));
    }
}

void Scenes::drawMapEdit(){
    if(!mapEditCalled){//acontece na primeira vez
        setAllCalledFalseExcept(getUnityVector(Scenes::mapEdit));//fala que ja chamou essa cena
        mapEdit::reset();
        Scenes::camera.moveSpeed=10;
        vector<int> tmp;
        tmp.push_back(AL::getSoundByName("menuSong"));
        al->stopAllSoundsExcept(tmp);
    }    
    al->playSoundByName("menuSong");
    if(mapEdit::size.x==0){
        GL::drawTexture(Util::nTRectangleSet(0,GL::defaultSize.y,GL::defaultSize.x,0,-0.9,-0.9),Util::nTColorSet(0.4,0.4,0.4,1),GL::getTextureByName("background0"),1);
        if(fmodl(GL::framesInGame,(int)GL::getFPS()/3)==0)
        if(mapEdit::blankField=="")
            mapEdit::blankField="_";
        else
            mapEdit::blankField="";
        
         if(mapEdit::isCreating==0){
            GL::setFontByIndex(0);
            GL::drawText(Util::nTPointSet(100,75,1),"Selecione o modo de edicao:",GL::getColorByName("violet"));
            GL::drawText(Util::nTPointSet(75,400,1),"Editar um mapa existente.",GL::getColorByName("violet"));
            GL::drawText(Util::nTPointSet(450,400,1),"Criar um novo mapa.",GL::getColorByName("violet"));
            if(GL::buttonBehave(Util::nTRectangleSet(100,350,300,150,0,0),GL::getColorByName("mouseSelected"),GL::getTextureByName("editMapIcon"),NULL,NULL,NULL,NULL))
                mapEdit::isCreating=-1;
            if(GL::buttonBehave(Util::nTRectangleSet(450,350,650,150,0,0),GL::getColorByName("mouseSelected"),GL::getTextureByName("newMapIcon"),NULL,NULL,NULL,NULL))
                mapEdit::isCreating=1;
            if(GL::buttonBehave(Util::nTRectangleSet(200,550,340,500,0,0),GL::getColorByName("mouseSelected"),GL::getTextureByName("backIcon"),NULL,NULL,NULL,NULL))
            current=menu;
        }else if(mapEdit::isCreating==1){
            mapEdit::askForSize();
            if(GL::buttonBehave(Util::nTRectangleSet(400,550,540,500,0,0),GL::getColorByName("mouseSelected"),GL::getTextureByName("backIcon"),NULL,NULL,NULL,NULL))
                mapEdit::isCreating=0;
        }else if(mapEdit::isCreating==-1){
            mapEdit::askForLoad();
            if(GL::buttonBehave(Util::nTRectangleSet(400,550,540,500,0,0),GL::getColorByName("mouseSelected"),GL::getTextureByName("backIcon"),NULL,NULL,NULL,NULL))
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

void Scenes::drawMenu(){
    if(!menuCalled){//acontece na primeira vez
        setAllCalledFalseExcept(getUnityVector(menu));//fala que ja chamou essa cena
        vector<int> tmp;
        tmp.push_back(AL::getSoundByName("menuSong"));
        al->stopAllSoundsExcept(tmp);
    }
    al->playSoundByName("menuSong");
    GL::drawTexture(Util::nTRectangleSet(0,GL::defaultSize.y,GL::defaultSize.x,0,-0.9,-0.9),Util::nTColorSet(0.4,0.4,0.4,1),GL::getTextureByName("background0"),1);
    if(AL::getSoundState()){
      GL::buttonBehave(Util::nTRectangleSet(700,50,740,10,0.6,0.6),Util::nTColorSet(0,0,0,1),GL::getTextureByName("soundOn"),NULL,*modifySound,NULL,NULL);
    }else{
      GL::buttonBehave(Util::nTRectangleSet(700,50,740,10,0.6,0.6),Util::nTColorSet(0,0,0,1),GL::getTextureByName("soundOff"),NULL,*modifySound,NULL,NULL);
    }
    if(AL::getMusicState()){
      GL::buttonBehave(Util::nTRectangleSet(640,50,680,10,0.6,0.6),Util::nTColorSet(0,0,0,1),GL::getTextureByName("musicOn"),NULL,*modifyMusic,NULL,NULL);
    }else{
      GL::buttonBehave(Util::nTRectangleSet(640,50,680,10,0.6,0.6),Util::nTColorSet(0,0,0,1),GL::getTextureByName("musicOff"),NULL,*modifyMusic,NULL,NULL);
    }
    if(GL::buttonBehave(Util::nTRectangleSet(350,362+10,450,330,0.4,0.4),Util::nTColorSet(0.4,0.4,0.4,1),GL::getTextureByName("playIcon"),NULL,NULL,NULL,NULL))
      Scenes::current=preGame;
    if(GL::buttonBehave(Util::nTRectangleSet(250,404+10,550,372,0.4,0.4),Util::nTColorSet(0.4,0.4,0.4,1),GL::getTextureByName("mapEditIcon"),NULL,NULL,NULL,NULL))
      Scenes::current=mapEdit;
//    if(GL::buttonBehave(Util::nTRectangleSet(340,446+10,460,414,0.4,0.4),Util::nTColorSet(0.4,0.4,0.4,1),GL::getTextureByName("optionsIcon"),NULL,NULL,NULL,NULL))
//      Scenes::current=menu;
    if(GL::buttonBehave(Util::nTRectangleSet(360,446+10,440,414,0.4,0.4),Util::nTColorSet(0.4,0.4,0.4,1),GL::getTextureByName("quitIcon"),NULL,NULL,NULL,NULL))
      exit(0);
        if(GL::buttonBehave(Util::nTRectangleSet(57,557+23,2*57+23,500,0.4,0.4),Util::nTColorSet(0.4,0.4,0.4,1),GL::getTextureByName("cefetLogo"),NULL,NULL,NULL,NULL))
        Scenes::current=credits;
        if(GL::buttonBehave(Util::nTRectangleSet(3*57+190,557+23,4*57+23+190,500,0.4,0.4),Util::nTColorSet(0.4,0.4,0.4,1),GL::getTextureByName("decomLogo"),NULL,NULL,NULL,NULL))
          Scenes::current=credits;
          if(GL::buttonBehave(Util::nTRectangleSet(5*57+400,557+23,6*57+23+400,500,0.4,0.4),Util::nTColorSet(0.4,0.4,0.4,1),GL::getTextureByName("theCOM"),NULL,NULL,NULL,NULL))
            Scenes::current=credits;
    GL::drawTexture(Util::nTRectangleSet(100,300,700,0,0.4,0.4),GL::getColorByName("white"),GL::getTextureByName("legendsLogo"),1);
}

void Scenes::drawOptions(){

}

void Scenes::drawCredits(){
  if(!creditsCalled){
      setAllCalledFalseExcept(getUnityVector(credits));
      vector<int> tmp;
      al->stopAllSoundsExcept(tmp);
  }
  GL::drawTexture((Util::nTRectangleSet(0,600,800,0,0,0)),Util::nTColorSet(1,1,1,1),GL::getTextureByName("credits"),1);
  if(GL::buttonBehave(Util::nTRectangleSet(57,500,187,450,0.4,0.4),Util::nTColorSet(0.4,0.4,0.4,1),GL::getTextureByName("backIcon"),NULL,NULL,NULL,NULL))
    Scenes::current=menu;

}

void Scenes::drawSplash(){
    if(!splashCalled){
        setAllCalledFalseExcept(getUnityVector(splash));
        vector<int> tmp;
        al->stopAllSoundsExcept(tmp);
    }
  float x=(GL::framesInGame/GL::getFPS())*300;
  if(skipScene&&x>1000){
    skipScene=false;
    current=menu;}
  if(x<500)
    GL::drawTexture((Util::nTRectangleSet(0,600,800,0,0,0)),Util::nTColorSet(1,1,1,1),GL::getTextureByName("legalNote"),1);
  if(x>=500&&x<1000){
    x-=500;
    GL::drawTexture((Util::nTRectangleSet(0,600,800,0,0,0)),Util::nTColorSet(1,1,1,1-x/500),GL::getTextureByName("legalNote"),1);

  }
  if(x>=1000&&x<1500){
    x-=1000;
    GL::drawTexture((Util::nTRectangleSet(50,450,750,150,0,0)),Util::nTColorSet(1,1,1,1),GL::getTextureByName("decomCelebrations"),1);
  }
  if(x>=1500&&x<2000){
    x-=1500;
    GL::drawTexture((Util::nTRectangleSet(50,450,750,150,0,0)),Util::nTColorSet(1,1,1,1-x/500),GL::getTextureByName("decomCelebrations"),1);
  }
  if(x>=2000&&x<2500){
    x-=2000;
    GL::drawTexture((Util::nTRectangleSet(57,580,637-x,x,0.1,0.1)),GL::getColorByName("white"),GL::getTextureByName("cefetLogo"),1);
  }
  if(x>=2500&&x<3000){
    x-=2500;
    GL::drawTexture((Util::nTRectangleSet(57,580,137,500,0.1,0.1)),GL::getColorByName("white"),GL::getTextureByName("cefetLogo"),1);
    GL::drawTexture((Util::nTRectangleSet(111+x/2,580,691-x/2,x,0.2,0.2)),GL::getColorByName("white"),GL::getTextureByName("decomLogo"),1);
  }
  if(x>=3000&&x<3500){
    x-=3000;
    GL::drawTexture((Util::nTRectangleSet(57,580,137,500,0.1,0.1)),GL::getColorByName("white"),GL::getTextureByName("cefetLogo"),1);
    GL::drawTexture((Util::nTRectangleSet(361,580,441,500,0.2,0.2)),GL::getColorByName("white"),GL::getTextureByName("decomLogo"),1);
    GL::drawTexture((Util::nTRectangleSet(185+x,580,756,x,0.3,0.3)),GL::getColorByName("white"),GL::getTextureByName("theCOM"),1);
  }
  if(x>=3500&&x<4000){
    x-=3500;
    GL::drawTexture((Util::nTRectangleSet(57,580,137,500,0.1,0.1)),GL::getColorByName("white"),GL::getTextureByName("cefetLogo"),1);
    GL::drawTexture((Util::nTRectangleSet(361,580,441,500,0.2,0.2)),GL::getColorByName("white"),GL::getTextureByName("decomLogo"),1);
    GL::drawTexture((Util::nTRectangleSet(685,580,756,500,0.3,0.3)),GL::getColorByName("white"),GL::getTextureByName("theCOM"),1);
    GL::drawTexture((Util::nTRectangleSet(100,300,700,0,0.4,0.4)),Util::nTColorSet(1,1,1,x/500),GL::getTextureByName("legendsLogo"),1);

  }
  if(x>4000)
    Scenes::current=menu;
}

bool Scenes::isInTheScreen(nTRectangle collision){
    bool out=false;
    nTPoint offSet;
    offSet.setPoint(GL::defaultSize.x*0.2,GL::defaultSize.y*0.3,0);
    offSet.setPoint(0,0,0);
    if(camera.x.movedCam-offSet.x<=collision.p0.x&&collision.p0.x<GL::defaultSize.x+camera.x.movedCam+offSet.x)
        if(camera.y.movedCam-offSet.y<=collision.p0.y&&collision.p0.y<GL::defaultSize.y+camera.y.movedCam+offSet.y)
            out=true;
    if(camera.x.movedCam-offSet.x<=collision.p1.x&&collision.p1.x<GL::defaultSize.x+camera.x.movedCam+offSet.x)
        if(camera.y.movedCam-offSet.y<=collision.p0.y&&collision.p0.y<GL::defaultSize.y+camera.y.movedCam+offSet.y)
            out=true;
    if(camera.x.movedCam-offSet.x<=collision.p1.x&&collision.p1.x<GL::defaultSize.x+camera.x.movedCam+offSet.x)
        if(camera.y.movedCam-offSet.y<=collision.p1.y&&collision.p1.y<GL::defaultSize.y+camera.y.movedCam+offSet.y)
            out=true;
    if(camera.x.movedCam-offSet.x<=collision.p0.x&&collision.p0.x<GL::defaultSize.x+camera.x.movedCam+offSet.x)
        if(camera.y.movedCam-offSet.y<=collision.p1.y&&collision.p1.y<GL::defaultSize.y+camera.y.movedCam+offSet.y)
            out=true;
    return out;
}

void Scenes::drawPreCampaign(){
  if(!preCampaignCalled){//acontece na primeira vez
      setAllCalledFalseExcept(getUnityVector(preCampaign));//fala que ja chamou essa cena
      freeGameMode=false;
      vector<int> tmp;
        al->stopAllSoundsExcept(tmp);
  }
  // if(GL::buttonBehave(Util::nTRectangleSet(340,550,480,500,0,0),GL::getColorByName("mouseSelected"),GL::getTextureByName("backIcon"),NULL,NULL,NULL,NULL))
    // current=preGame;
  float x=(GL::framesInGame/GL::getFPS())*300;
  GL::setFontByIndex(0);
  if(x<500)
    GL::drawText(Util::nTPointSet(250,200,1),"Realizando prova do CEFET",GL::getColorByName("violet"));
  if(x>=500&&x<1000){
    x-=500;
    GL::drawText(Util::nTPointSet(250,200,1),"Realizando prova do CEFET",GL::getColorByName("violet"));
    GL::drawRectangle(Util::nTRectangleSet(240,320,280,280,0,0),Util::nTColorSet(1,1,1,x/500));
  }
  if(x>=1000&&x<1500){
    x-=1000;
    GL::drawText(Util::nTPointSet(250,200,1),"Realizando prova do CEFET",GL::getColorByName("violet"));
    GL::drawRectangle(Util::nTRectangleSet(240,320,280,280,0,0),Util::nTColorSet(1,1,1,1));
    GL::drawRectangle(Util::nTRectangleSet(360,320,400,280,0,0),Util::nTColorSet(1,1,1,x/500));
  }
  if(x>=1500&&x<2000){
    x-=1500;
    GL::drawText(Util::nTPointSet(250,200,1),"Realizando prova do CEFET",GL::getColorByName("violet"));
    GL::drawRectangle(Util::nTRectangleSet(240,320,280,280,0,0),Util::nTColorSet(1,1,1,1));
    GL::drawRectangle(Util::nTRectangleSet(360,320,400,280,0,0),Util::nTColorSet(1,1,1,1));
    GL::drawRectangle(Util::nTRectangleSet(480,320,520,280,0,0),Util::nTColorSet(1,1,1,x/500));
  }
  if(x>=2000&&x<2500){
    x-=2000;
    GL::drawRectangle(Util::nTRectangleSet(360,200,400,350,0,0),Util::nTColorSet(1,1,1,1));
    GL::drawRectangle(Util::nTRectangleSet(360,390,400,430,0,0),Util::nTColorSet(1,1,1,1));
  }
  if(x>=2500&&x<3000){
    x-=2500;
    GL::drawTexture(Util::nTRectangleSet(0,GL::defaultSize.y,GL::defaultSize.x,0,-0.9,-0.9),Util::nTColorSet(1,1,1,x/500),GL::getTextureByName("aprovado"),1);
    GL::drawRectangle(Util::nTRectangleSet(360,200,400,350,0,0),Util::nTColorSet(1,1,1,1-x/500));
    GL::drawRectangle(Util::nTRectangleSet(360,390,400,430,0,0),Util::nTColorSet(1,1,1,1-x/500));
  }
  if(x>=3000&&x<3500){
    x-=3000;
    GL::drawTexture(Util::nTRectangleSet(0,GL::defaultSize.y,GL::defaultSize.x,0,-0.9,-0.9),Util::nTColorSet(1,1,1,1),GL::getTextureByName("aprovado"),1);
  }
  if(x>=3500&&x<4000){
    x-=3500;
    GL::drawTexture(Util::nTRectangleSet(0,GL::defaultSize.y,GL::defaultSize.x,0,-0.9,-0.9),Util::nTColorSet(1,1,1,1-x/500),GL::getTextureByName("aprovado"),1);
  }
  if(x>=4000&&x<4500);
  if(x>=4500||Player::stage!=0||Player::checkpoint!=0)
    current=game;
  else
      al->playSoundByName("makeTest");


}
void Scenes::drawPreFreeMode(){
  if(!preFreeModeCalled){//acontece na primeira vez
      setAllCalledFalseExcept(getUnityVector(preFreeMode));//fala que ja chamou essa cena
      freeGameMode=true;
      vector<int> tmp;
      tmp.push_back(AL::getSoundByName("menuSong"));
      al->stopAllSoundsExcept(tmp);
  }

//if(GL::buttonBehave(Util::nTRectangleSet(350,362+10,450,330,0.4,0.4),Util::nTColorSet(0.4,0.4,0.4,1),GL::getTextureByName("playIcon"),NULL,NULL,NULL,NULL))
  Scenes::current=game;
  if(GL::buttonBehave(Util::nTRectangleSet(340,550,480,500,0,0),GL::getColorByName("mouseSelected"),GL::getTextureByName("backIcon"),NULL,NULL,NULL,NULL))
    current=preGame;
  //TODO implementar modo de escolher mapa
}

void Scenes::drawPreGame(){
  if(!preGameCalled){//acontece na primeira vez
      setAllCalledFalseExcept(getUnityVector(preGame));//fala que ja chamou essa cena
      vector<int> tmp;
      tmp.push_back(AL::getSoundByName("menuSong"));
      al->stopAllSoundsExcept(tmp);
  }
  al->playSoundByName("menuSong");
  GL::setFontByIndex(0);
  GL::drawTexture(Util::nTRectangleSet(0,GL::defaultSize.y,GL::defaultSize.x,0,-0.9,-0.9),Util::nTColorSet(0.2,0.2,0.2,1),GL::getTextureByName("background0"),1);
  GL::drawText(Util::nTPointSet(250,75,1),"Selecione o modo de Jogo:",GL::getColorByName("violet"));
  GL::drawText(Util::nTPointSet(110,350,1),"Modo Campanha.",GL::getColorByName("violet"));
  GL::drawText(Util::nTPointSet(500,350,1),"Modo livre.",GL::getColorByName("violet"));
  GL::setFontByIndex(6);
  char buffer[5];
  snprintf(buffer,5,"%.2f",Player::globalCoeficiente);
  string strC(buffer);
  GL::drawText(Util::nTPointSet(95,325,1),(char*)("Coeficiente Global: "+strC).c_str(),GL::getColorByName("blue"));
  if(GL::buttonBehave(Util::nTRectangleSet(200,550,340,500,0,0),GL::getColorByName("mouseSelected"),GL::getTextureByName("backIcon"),NULL,NULL,NULL,NULL))
    current=menu;
  if(GL::buttonBehave(Util::nTRectangleSet(50,300,350,100,0,0),GL::getColorByName("mouseSelected"),GL::getTextureByName("campaignIcon"),NULL,NULL,NULL,NULL))
    current=preCampaign;
  if(GL::buttonBehave(Util::nTRectangleSet(400,300,700,100,0,0),GL::getColorByName("mouseSelected"),GL::getTextureByName("freeModeIcon"),NULL,NULL,NULL,NULL))
    current=preFreeMode;
}

void Scenes::drawPosGame(){
  if(!posGameCalled){//acontece na primeira vez
      setAllCalledFalseExcept(getUnityVector(posGame));//fala que ja chamou essa cena
      vector<int> tmp;
      al->stopAllSoundsExcept(tmp);
      al->playSoundByName("diePlayer");
  }
  GL::drawTexture(Util::nTRectangleSet(camera.x.movedCam,GL::defaultSize.y+camera.y.movedCam,GL::defaultSize.x+camera.x.movedCam,camera.y.movedCam,-0.9,-0.9),Util::nTColorSet(1,1,1,1),GL::getTextureByName("Cefet"),1);
  GL::drawTexture(Util::nTRectangleSet(100+camera.x.movedCam,GL::defaultSize.y-200+camera.y.movedCam,GL::defaultSize.x-100+camera.x.movedCam,200+camera.x.movedCam,-0.8,-0.8),Util::nTColorSet(1,0,0,1),GL::getTextureByName("GameOver"),1);

  if(GL::buttonBehave(Util::nTRectangleSet(150+camera.x.movedCam,550+camera.y.movedCam,290+camera.x.movedCam,500+camera.y.movedCam,0,0),GL::getColorByName("mouseSelected"),GL::getTextureByName("Menu1"),NULL,NULL,NULL,NULL))
    current=menu;
  if(GL::buttonBehave(Util::nTRectangleSet(450+camera.x.movedCam,550+camera.y.movedCam,690+camera.x.movedCam,500+camera.y.movedCam,0,0),GL::getColorByName("mouseSelected"),GL::getTextureByName("Continuar1"),NULL,NULL,NULL,NULL))
    current=game;
}

void Scenes::drawEndGame(){
  if(!posGameEndCalled){//acontece na primeira vez
      setAllCalledFalseExcept(getUnityVector(posGameEnd));//fala que ja chamou essa cena
      vector<int> tmp;
      al->stopAllSoundsExcept(tmp);
  }
  al->playSoundByName("loseSong");
  GL::drawTexture(Util::nTRectangleSet(0,GL::defaultSize.y,GL::defaultSize.x,0,-0.9,-0.9),Util::nTColorSet(1,1,1,1),GL::getTextureByName("Cefet"),1);
  GL::drawTexture(Util::nTRectangleSet(0,GL::defaultSize.y-100,GL::defaultSize.x,0,-0.8,-0.8),Util::nTColorSet(1,0,0,1),GL::getTextureByName("Jubilado"),1);

  if(GL::buttonBehave(Util::nTRectangleSet(300,500,440,450,0,0),GL::getColorByName("mouseSelected"),GL::getTextureByName("Menu1"),NULL,NULL,NULL,NULL))
    current=menu;
}

void Scenes::drawYouWin(){
  if(!posYouWinCalled){//acontece na primeira vez
      setAllCalledFalseExcept(getUnityVector(posYouWin));//fala que ja chamou essa cena
      vector<int> tmp;
      al->stopAllSoundsExcept(tmp);
  }
  al->playSoundByName("GG");
  GL::drawTexture(Util::nTRectangleSet(0,GL::defaultSize.y,GL::defaultSize.x,0,-0.9,-0.9),Util::nTColorSet(1,1,1,1),GL::getTextureByName("Win"),1);
  GL::drawText(Util::nTPointSet(345,350,1),"Parabens!! Voce cumpriu sua vida",GL::getColorByName("violet"));
  GL::drawText(Util::nTPointSet(345,385,1),"academica no DECOM com sucesso.",GL::getColorByName("violet"));
  char buffer[5];
  snprintf(buffer,5,"%.2f",Player::globalCoeficiente);
  string strC(buffer);
  GL::drawText(Util::nTPointSet(345,420,1),(char*)("Coeficiente Global: "+strC).c_str(),GL::getColorByName("green"));
  if(GL::buttonBehave(Util::nTRectangleSet(460,500,600,425,0,0),GL::getColorByName("mouseSelected"),GL::getTextureByName("Menu1"),NULL,NULL,NULL,NULL))
    current=credits;
}