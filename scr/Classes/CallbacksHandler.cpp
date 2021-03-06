#include "CallbacksHandler.hpp"
#include "GL.hpp"
#include "Scenes.hpp"
#include "IOHandler.hpp"

CallbacksHandler::CallbacksHandler() {
};

CallbacksHandler::CallbacksHandler(const CallbacksHandler& orig) {
}

CallbacksHandler::~CallbacksHandler() {
}

unsigned long int CallbacksHandler::previousFramesInGame=0;

/**
 *	Draw screen and call the current scene. Its a GL callback
**/
void CallbacksHandler::drawScene(void){
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    GL::framesInGame++;
    if(GL::framesInGame>=100000000000)
            GL::framesInGame=0;
    if(Scenes::current==Scenes::game)
        Scenes::drawGame();
    else if(Scenes::current==Scenes::MapEdit)
        Scenes::drawMapEdit();
    else if(Scenes::current==Scenes::menu)
        Scenes::drawMenu();
    else if(Scenes::current==Scenes::credits)
        Scenes::drawCredits();
    else if(Scenes::current==Scenes::splash)
        Scenes::drawSplash();
    else if(Scenes::current==Scenes::options)
        Scenes::drawOptions();
    else if(Scenes::current==Scenes::preGame)
        Scenes::drawPreGame();
    else if(Scenes::current==Scenes::preCampaign)
        Scenes::drawPreCampaign();
    else if(Scenes::current==Scenes::preFreeMode)
        Scenes::drawPreFreeMode();
    else if(Scenes::current==Scenes::posGame)
        Scenes::drawPosGame();
    else if(Scenes::current==Scenes::posGameEnd)
        Scenes::drawEndGame();
    else if(Scenes::current==Scenes::posYouWin)
        Scenes::drawYouWin();
    GL::drawPopupBox();
    glutSwapBuffers();
    FunctionAnalyser::endFrame();
}


/**
 *	Window reshape GL callback, responsible for maintaining the aspect ratio
 *
 *	@param width the new width of the window
 *	@param height the new height of the window
**/
void CallbacksHandler::reshape(int width, int height){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(Scenes::camera.x.movedCam,GL::defaultSize.x+Scenes::camera.x.movedCam,GL::defaultSize.y+Scenes::camera.y.movedCam,Scenes::camera.y.movedCam,-1,1);
    float newAspectRatio=(float)width/(float)height;
    float oldAspectRatio=(float)GL::defaultSize.x/(float)GL::defaultSize.y;
    float scale=1;
    nTPoint crop=nTPoint::get(0,0);
    if(newAspectRatio>oldAspectRatio){
        scale=(float)height/(float)GL::defaultSize.y;
        crop.x= (width-GL::defaultSize.x*scale)/2;
    }else if(newAspectRatio<oldAspectRatio){
        scale=(float)width/(float)GL::defaultSize.x;
        crop.y=(height-GL::defaultSize.y*scale)/2;
    }else{
        scale=(float)width/(float)GL::defaultSize.x;
    }
    float w=(float)GL::defaultSize.x*scale;
    float h=(float)GL::defaultSize.y*scale;
    GL::currentViewPort=nTRectangle::get(crop.x,crop.y,w,h);
    glViewport(crop.x,crop.y,w,h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/**
 *	FPS checker to evaluate the performance of the game
 *
 *	@param n custom parameter passed
**/
void CallbacksHandler::framesCheck(int n){
    if(GL::FPSchecker){
        GL::currentFPS=(int)(GL::framesInGame-previousFramesInGame);
        previousFramesInGame=GL::framesInGame;
    }
    if(!GL::isPaused)
        FunctionAnalyser::printData();
    if(FunctionAnalyser::PRINT||GL::FPSchecker)
        glutTimerFunc(1000, framesCheck, 0);
}

/**
 *	Time event GL callback, called to each frame
 *
 *	@param n custom parameter passed
**/
void CallbacksHandler::update(int n){
    FunctionAnalyser::startFrame();
    GL::mousePos.x=(GL::rawMousePos.x-GL::currentViewPort.p0.x)*(double)GL::defaultSize.x/(double)GL::currentViewPort.p1.x+Scenes::camera.x.movedCam;
    GL::mousePos.y=(GL::rawMousePos.y-GL::currentViewPort.p0.y)*(double)GL::defaultSize.y/(double)GL::currentViewPort.p1.y+Scenes::camera.y.movedCam;
    if(IOHandler::ReleaseMouseOffSet){
        GL::leftMouseReleased=false;
        IOHandler::ReleaseMouseOffSet=false;
    }
    if(GL::leftMouseReleased)IOHandler::ReleaseMouseOffSet=true;
    if(IOHandler::ReleaseMouseROffSet){
        GL::rightMouseReleased=false;
        IOHandler::ReleaseMouseROffSet=false;
    }
    if(GL::rightMouseReleased)IOHandler::ReleaseMouseROffSet=true;
    glutPostRedisplay();
    glutTimerFunc(GL::getMs(),update,0);
}
