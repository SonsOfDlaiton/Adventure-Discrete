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
    else if(Scenes::current==Scenes::mapEdit)
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
//    float windowAspectRatio,worldAspectRatio,xViewport,yViewport;
//    int WindowWidth,WindowHeight;
    // realHeight=height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(Scenes::camera.x.movedCam, GL::defaultSize.x+Scenes::camera.x.movedCam,GL::defaultSize.y+Scenes::camera.y.movedCam, Scenes::camera.y.movedCam, -1, 1);
    // windowAspectRatio = ((float)width)/height;
    // worldAspectRatio = ((float) GL::defaultSize.x)/ GL::defaultSize.y;
    // xViewport=0;yViewport=0;
    // if (windowAspectRatio < worldAspectRatio) {
        // cout<<"menor\n";
        // xViewport = width / worldAspectRatio;
        // yViewport = (height - xViewport)/2;
        glViewport(0, 0, width, height);
        GL::currentSize.x=width;
        GL::currentSize.y=height;
    // }else if (windowAspectRatio > worldAspectRatio) {
        // yViewport = ((float)height) * worldAspectRatio;
        // xViewport = (width - yViewport)/2;
        // glViewport(xViewport, 0, yViewport, height);
        // GL::currentSize.x=height-xViewport;
        // GL::currentSize.y=yViewport;
    // } else {
        // glViewport(0, 0, width, height);
        // GL::currentSize.x=width;
        // GL::currentSize.y=height;
    // }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/**
 *	FPS checker to evaluate the performance of the game
 *
 *	@param n custom parameter passed
**/
void CallbacksHandler::framesCheck(int n){
    if(!GL::isPaused){
        if(GL::FPSchecker){
            if((GL::framesInGame-previousFramesInGame)<666666)
                cout<<"Frames Per Second: "<<(GL::framesInGame-previousFramesInGame)<<endl;
            previousFramesInGame=GL::framesInGame;
        }
        FunctionAnalyser::printData();
    }
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
    GL::mousePos.x=GL::rawMousePos.x*(float)GL::defaultSize.x/(float)GL::currentSize.x+Scenes::camera.x.movedCam;
    GL::mousePos.y=GL::rawMousePos.y*(float)GL::defaultSize.y/(float)GL::currentSize.y+Scenes::camera.y.movedCam;

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
    glutTimerFunc(GL::getMs(), update, 0);
}
