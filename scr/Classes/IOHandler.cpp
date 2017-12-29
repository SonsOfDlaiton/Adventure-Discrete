#include "IOHandler.hpp"
#include "Scenes.hpp"

IOHandler::IOHandler() {
};

IOHandler::IOHandler(const IOHandler& orig) {
}

IOHandler::~IOHandler() {
}

int IOHandler::SpecReleaseKey=0;
bool IOHandler::ReleaseMouseOffSet=false;
bool IOHandler::ReleaseMouseROffSet=false;
bool IOHandler::ReleaseZOffSet=false;

/**
 *	Special keys keyboard event GL callback, usually handles directional arrows
 *
 *	@param key the key id witch was pressed
 *	@param x the mouse x position when the key was pressed
 *	@param y the mouse y position when the key was pressed
**/
void IOHandler::specialKeyboard(int key, int x, int y){
  switch(key){
    case GLUT_KEY_UP:
        if(Scenes::current==Scenes::game&&!GL::isPaused){
            Tutorials::pressKey(1);
            Player::getPlayerById(0)->atackDirection=Util::direction_up;
            if(Camera::freeCam)
                 Scenes::camera.y.movingCam=-1;
        }else if(Scenes::current==Scenes::MapEdit&&!GL::hasEditOnFocous())
            Scenes::camera.y.movingCam=-1;
    break;

    case GLUT_KEY_DOWN:
        if(Scenes::current==Scenes::game&&!GL::isPaused){
            Tutorials::pressKey(2);
            Player::getPlayerById(0)->atackDirection=Util::direction_down;
            Player::getPlayerById(0)->canTp=true;
            if(!Player::getPlayerById(0)->atacking&&Player::getPlayerById(0)->vSpeed==0)Player::getPlayerById(0)->lowered=true;
            if(Camera::freeCam)
                 Scenes::camera.y.movingCam=1;
        }  else if(Scenes::current==Scenes::MapEdit&&!GL::hasEditOnFocous())
            Scenes::camera.y.movingCam=1;

    break;

    case GLUT_KEY_LEFT:
        if(Scenes::current==Scenes::game&&!GL::isPaused){
            if(SpecReleaseKey==-1)
                SpecReleaseKey=0;
            Tutorials::pressKey(3);
             Player::getPlayerById(0)->orientation=Util::orientation_left;
             Player::getPlayerById(0)->atackDirection=Util::direction_left;
             if(glutGetModifiers()==GLUT_ACTIVE_SHIFT){
                 if(Player::getPlayerById(0)->canWalk&&!Player::getPlayerById(0)->atacking)Player::getPlayerById(0)->run(Util::direction_left);
             }else{
                if(Player::getPlayerById(0)->canWalk&&!Player::getPlayerById(0)->atacking)Player::getPlayerById(0)->walk(Util::direction_left);
             }
             if(Camera::freeCam)
                    Scenes::camera.x.movingCam=-1;
        }else if(Scenes::current==Scenes::MapEdit&&!GL::hasEditOnFocous())
            Scenes::camera.x.movingCam=-1;
    break;

    case GLUT_KEY_RIGHT:
        if(Scenes::current==Scenes::game&&!GL::isPaused){
            if(SpecReleaseKey==1)
                SpecReleaseKey=0;
            Tutorials::pressKey(4);
            Player::getPlayerById(0)->orientation=Util::orientation_right;
            Player::getPlayerById(0)->atackDirection=Util::direction_right;
            if(glutGetModifiers()==GLUT_ACTIVE_SHIFT){
                if(Player::getPlayerById(0)->canWalk&&!Player::getPlayerById(0)->atacking)Player::getPlayerById(0)->run(Util::direction_right);
            }else{
                if(Player::getPlayerById(0)->canWalk&&!Player::getPlayerById(0)->atacking)Player::getPlayerById(0)->walk(Util::direction_right);
            }
            if(Camera::freeCam)
                 Scenes::camera.x.movingCam=1;
        }else if(Scenes::current==Scenes::MapEdit&&!GL::hasEditOnFocous())
            Scenes::camera.x.movingCam=1;
      break;
    }
}

/**
 *	Keyboard event GL callback
 *
 *	@param key the key id witch was pressed
 *	@param x the mouse x position when the key was pressed
 *	@param y the mouse y position when the key was pressed
**/
void IOHandler::keyboard(unsigned char key, int x, int y){
    switch(key){
      case ' ':
        if(!GL::isPaused&&Scenes::current==Scenes::game){
            if(!Tutorials::isPaused)Player::getPlayerById(0)->jump();
            Player::getPlayerById(0)->spacePressed=true;
        }
        if(Scenes::current==Scenes::splash)
          Scenes::skipScene=true;
      break;

      case 'Z':
      case 'z':
          if(!GL::isPaused){
            ReleaseZOffSet=false;
            Player::getPlayerById(0)->atacking=Player::melee;
          }
      break;

      case 'X':
      case 'x':
          if(!GL::isPaused){
            Player::getPlayerById(0)->atacking=Player::ranged;
          }
      break;

      case 27:
        if(Scenes::current==Scenes::game){
            GL::isPaused=!GL::isPaused;
            Tutorials::catchPause();
        }
      break;
      default:break;
    }
    if(key>0){
        if(Scenes::current==Scenes::splash)
          Scenes::skipScene=true;
        if(!GL::isPaused){
            Tutorials::pressKey(key);
        }
    }
}

/**
 *	Special keys released keyboard event GL callback, usually handles directional arrows
 *
 *	@param key the key id witch was released
 *	@param x the mouse x position when the key was released
 *	@param y the mouse y position when the key was released
**/
void IOHandler::specialKeyboardUp(int key,int x,int y){
  switch(key){
    case GLUT_KEY_UP:
    if(Scenes::current==Scenes::game){
      if(Player::getPlayerById(0)->orientation>=Util::orientation_right)
            Player::getPlayerById(0)->atackDirection=Util::direction_right;
        else
            Player::getPlayerById(0)->atackDirection=Util::direction_left;
    }
    Scenes::camera.y.movingCam=0;
    GL::moveEditCursor(Util::direction_left);
    break;

    case GLUT_KEY_DOWN:
    if(Scenes::current==Scenes::game){
        Player::getPlayerById(0)->canTp=false;
        Player::getPlayerById(0)->lowered=false;
        if(Player::getPlayerById(0)->orientation>=Util::orientation_right)
            Player::getPlayerById(0)->atackDirection=Util::direction_right;
        else
            Player::getPlayerById(0)->atackDirection=Util::direction_left;
    }
      Scenes::camera.y.movingCam=0;
      GL::moveEditCursor(Util::direction_right);
    break;

    case GLUT_KEY_LEFT:

    if(Scenes::current==Scenes::game){
      if(Player::getPlayerById(0)->orientation>=Util::orientation_right)
            Player::getPlayerById(0)->atackDirection=Util::direction_right;
        else
            Player::getPlayerById(0)->atackDirection=Util::direction_left;
      SpecReleaseKey=-1;
    }
    Scenes::camera.x.movingCam=0;
    GL::moveEditCursor(Util::direction_left);
    break;

    case GLUT_KEY_RIGHT:
    if(Scenes::current==Scenes::game){
      if(Player::getPlayerById(0)->orientation>=Util::orientation_right)
            Player::getPlayerById(0)->atackDirection=Util::direction_right;
        else
            Player::getPlayerById(0)->atackDirection=Util::direction_left;
      SpecReleaseKey=1;
    }
    Scenes::camera.x.movingCam=0;
    GL::moveEditCursor(Util::direction_right);
    break;

    default:
    break;
  }
}

/**
 *	Keyboard released event GL callback
 *
 *	@param key the key id witch was released
 *	@param x the mouse x position when the key was released
 *	@param y the mouse y position when the key was released
**/
void IOHandler::keyboardUp(unsigned char key,int x,int y){
    switch(key){
        case ' ':
        if(!GL::isPaused&&Scenes::current==Scenes::game){
            Player::getPlayerById(0)->spacePressed=false;
        }
        break;
        case 'z':
        case 'Z':
            ReleaseZOffSet=true;
        break;

        case 'D':
        case 'd':
              if(Scenes::current==Scenes::MapEdit&&!GL::hasEditOnFocous())
                  MapEdit::isUser=!MapEdit::isUser;
        break;

        case 'C':
        case 'c':
            if(!GL::isPaused&&Scenes::current==Scenes::game){
                if(Player::getPlayerById(0)->god){
                    AL::singleton->stopSound(AL::getSoundByName("cafeSong"));
                    Player::getPlayerById(0)->imuneToDamage=false;
                }
                Player::getPlayerById(0)->god=!Player::getPlayerById(0)->god;
            }
        break;

        case 'S':
        case 's':
            if((Scenes::current==Scenes::game||Scenes::current==Scenes::MapEdit)&&!GL::hasEditOnFocous())
                Mechanics::drawCollisionRec=!Mechanics::drawCollisionRec;
            break;
    }
    if(key>0){
      GL::typeOnEdit(key);
    }
}

/**
 *	Mouse clicked event GL callback
 *
 *	@param button the mouse button id witch was clicked
 *	@param state the state of the button in question, could be GLUT_DOWN or GLUT_UP
 *	@param x the mouse x position when the button was clicked
 *	@param y the mouse y position when the button was clicked
**/
void IOHandler::mousePress(int button,int state,int x,int y){
    if(button==GLUT_LEFT_BUTTON){
        if(state==GLUT_DOWN){
            GL::leftMouseClicked=true;
            GL::leftMouseReleased=false;
          }
      if(state==GLUT_UP){
            GL::leftMouseClicked=false;
            GL::leftMouseReleased=true;
        }
    }
    if(button==GLUT_RIGHT_BUTTON){
        if(state==GLUT_DOWN){
            GL::rightMouseClicked=true;
            GL::rightMouseReleased=false;
          }
      if(state==GLUT_UP){
            GL::rightMouseClicked=false;
            GL::rightMouseReleased=true;
        }
    }
    if(button==GLUT_MIDDLE_BUTTON){
      if(state==GLUT_DOWN){
      }
      if(state==GLUT_UP){
        if(Scenes::current==Scenes::MapEdit)
          MapEdit::currentBlock=60000;
      }
    }
}

/**
 *	Mouse movement when not clicking event GL callback
 *
 *	@param x the mouse x position
 *	@param y the mouse y position
**/
void IOHandler::mousePassiveMotion(int x,int y){
    GL::rawMousePos.set(x,y,0);
}

/**
 *	Mouse movement when clicking event GL callback
 *
 *	@param x the mouse x position
 *	@param y the mouse y position
**/
void IOHandler::mouseActiveMotion(int x,int y){
    GL::rawMousePos.set(x,y,0);
}

/**
 *	Mouse wheel event GL callback
 *
 *	@param button the mouse button id
 *	@param dir direction of the mouse wheel, dir>0=UP, dir<0=DOWN
 *	@param x the mouse x position
 *	@param y the mouse y position
**/
void IOHandler::mouseWheel(int button, int dir, int x, int y){
    if (dir>0){
        // Zoom in
        if(Scenes::current==Scenes::MapEdit){
            if(MapEdit::scale.x<0.99){
                MapEdit::scale.x+=0.01;
                MapEdit::scale.y+=0.01;
            }
        }
    }else{
        // Zoom out
        if(Scenes::current==Scenes::MapEdit){
            if(MapEdit::scale.x>0.01){
                MapEdit::scale.x-=0.01;
                MapEdit::scale.y-=0.01;
            }
        }
    }
}

