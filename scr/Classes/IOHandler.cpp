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
  switch(key)
  {
    case GLUT_KEY_UP:
        if(Scenes::current==Scenes::game&&!GL::isPaused){
            Tutorials::pressedKey=GLUT_KEY_UP;
            Player::getPlayerById(0)->atackDirection=Util::direction_up;
            if(Camera::freeCam)
                 Scenes::camera.y.movingCam=-1;
        }else if(Scenes::current==Scenes::mapEdit)
            Scenes::camera.y.movingCam=-1;
    break;

    case GLUT_KEY_DOWN:
        if(Scenes::current==Scenes::game&&!GL::isPaused){
            Tutorials::pressedKey=GLUT_KEY_UP;
            Player::getPlayerById(0)->atackDirection=Util::direction_down;
            Player::getPlayerById(0)->canTp=true;
            if(Camera::freeCam)
                 Scenes::camera.y.movingCam=1;
        }  else if(Scenes::current==Scenes::mapEdit)
            Scenes::camera.y.movingCam=1;

    break;

    case GLUT_KEY_LEFT:
        if(Scenes::current==Scenes::game&&!GL::isPaused){
            if(SpecReleaseKey==-1)
                SpecReleaseKey=0;
            Tutorials::pressedKey=GLUT_KEY_UP;
             Player::getPlayerById(0)->orientation=Util::orientation_left;
             Player::getPlayerById(0)->atackDirection=Util::direction_left;
             if(glutGetModifiers()==GLUT_ACTIVE_SHIFT){
                 if(Player::getPlayerById(0)->canWalk&&!Player::getPlayerById(0)->atacking)Player::getPlayerById(0)->run(Util::direction_left);
             }else{
                if(Player::getPlayerById(0)->canWalk&&!Player::getPlayerById(0)->atacking)Player::getPlayerById(0)->walk(Util::direction_left);
             }
             if(Camera::freeCam)
                    Scenes::camera.x.movingCam=-1;
        }else if(Scenes::current==Scenes::mapEdit)
            Scenes::camera.x.movingCam=-1;
    break;

    case GLUT_KEY_RIGHT:
        if(Scenes::current==Scenes::game&&!GL::isPaused){
            if(SpecReleaseKey==1)
                SpecReleaseKey=0;
            Tutorials::pressedKey=GLUT_KEY_UP;
            Player::getPlayerById(0)->orientation=Util::orientation_right;
            Player::getPlayerById(0)->atackDirection=Util::direction_right;
            if(glutGetModifiers()==GLUT_ACTIVE_SHIFT){
                if(Player::getPlayerById(0)->canWalk&&!Player::getPlayerById(0)->atacking)Player::getPlayerById(0)->run(Util::direction_right);
            }else{
                if(Player::getPlayerById(0)->canWalk&&!Player::getPlayerById(0)->atacking)Player::getPlayerById(0)->walk(Util::direction_right);
            }
            if(Camera::freeCam)
                 Scenes::camera.x.movingCam=1;
        }else if(Scenes::current==Scenes::mapEdit)
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
            Tutorials::pressedKey=' ';
        }
        if(Scenes::current==Scenes::splash)
          Scenes::skipScene=true;
      break;

      case 'Z':
      case 'z':
          if(!GL::isPaused){
            ReleaseZOffSet=false;
            Tutorials::pressedKey='z';
            Player::getPlayerById(0)->atacking=Player::melee;
          }
      break;

      case 'X':
      case 'x':
          if(!GL::isPaused){
            Tutorials::pressedKey='x';
            Player::getPlayerById(0)->atacking=Player::ranged;
          }
      break;

      case 27:
         if(!GL::isPaused){
            Tutorials::pressedKey=27;
         }
        if(Scenes::current==Scenes::game)
            GL::isPaused=!GL::isPaused;
      break;
      default:break;
    }
    if(key>0)
        if(Scenes::current==Scenes::splash)
          Scenes::skipScene=true;
}

/**
 *	Special keys released keyboard event GL callback, usually handles directional arrows
 *
 *	@param key the key id witch was released
 *	@param x the mouse x position when the key was released
 *	@param y the mouse y position when the key was released
**/
void IOHandler::specialKeyboardUp(int key,int x,int y){
  switch(key)
  {
    case GLUT_KEY_UP:
    if(Scenes::current==Scenes::game){
      if(Player::getPlayerById(0)->orientation>=Util::orientation_right)
            Player::getPlayerById(0)->atackDirection=Util::direction_right;
        else
            Player::getPlayerById(0)->atackDirection=Util::direction_left;
    }
    Scenes::camera.y.movingCam=0;
    break;

    case GLUT_KEY_DOWN:
    if(Scenes::current==Scenes::game){
        Player::getPlayerById(0)->canTp=false;
        if(Player::getPlayerById(0)->orientation>=Util::orientation_right)
            Player::getPlayerById(0)->atackDirection=Util::direction_right;
        else
            Player::getPlayerById(0)->atackDirection=Util::direction_left;
    }
      Scenes::camera.y.movingCam=0;
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

        case '0':
              if(Scenes::current==Scenes::mapEdit)
                  mapEdit::input+=key;
        break;
        case '1':
              if(Scenes::current==Scenes::mapEdit)
                  mapEdit::input+=key;
        break;
        case '2':
              if(Scenes::current==Scenes::mapEdit)
                  mapEdit::input+=key;
        break;
        case '3':
              if(Scenes::current==Scenes::mapEdit)
                  mapEdit::input+=key;
        break;
        case '4':
              if(Scenes::current==Scenes::mapEdit)
                  mapEdit::input+=key;
        break;
        case '5':
              if(Scenes::current==Scenes::mapEdit)
                  mapEdit::input+=key;
        break;
        case '6':
              if(Scenes::current==Scenes::mapEdit)
                  mapEdit::input+=key;
        break;
        case '7':
              if(Scenes::current==Scenes::mapEdit)
                  mapEdit::input+=key;
        break;
        case '8':
              if(Scenes::current==Scenes::mapEdit)
                  mapEdit::input+=key;
        break;
        case '9':
              if(Scenes::current==Scenes::mapEdit)
                  mapEdit::input+=key;
        break;

        case 'D':
        case 'd':
              if(Scenes::current==Scenes::mapEdit)
                  mapEdit::isUser=!mapEdit::isUser;
        break;

        case 'U':
        case 'u':
              if(Scenes::current==Scenes::mapEdit&&mapEdit::size.y==0&&mapEdit::isCreating==-1)
                  mapEdit::load(-1);
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
            if(Scenes::current==Scenes::game||Scenes::current==Scenes::mapEdit)
                Mechanics::drawCollisionRec=!Mechanics::drawCollisionRec;
            break;

        case 13://enter
            if(Scenes::current==Scenes::mapEdit&&mapEdit::input!=""){
                float tmp;
                istringstream (mapEdit::input) >> tmp;
                mapEdit::input="";

                if(mapEdit::isCreating==1){
                    if(tmp>=19&&tmp<65000&&(mapEdit::size.y==0||mapEdit::size.x==0)){
                        if(mapEdit::size.y==0){
                            mapEdit::size.y=tmp;
                        }else{
                            mapEdit::size.x=tmp;
                            mapEdit::setMapSize();
                        }
                    }
                }else if(mapEdit::isCreating==-1){
                    if(tmp>=0&&tmp<Map::maps.size()&&(mapEdit::size.y==0||mapEdit::size.x==0)){
                        mapEdit::load(tmp);
                    }
                }
            }
        break;

        case 8://backSpace
            if(Scenes::current==Scenes::mapEdit)
                if(mapEdit::input.size())
                    mapEdit::input.erase(mapEdit::input.begin()+mapEdit::input.size()-1);
        break;
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
          if(Scenes::current==Scenes::mapEdit)
              mapEdit::currentBlock=60000;
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
//        GLdouble model[16];
//        GLdouble proj[16];
//        GLint view[4];
//        glGetDoublev(GL_MODELVIEW_MATRIX, model);
//        glGetDoublev(GL_PROJECTION_MATRIX, proj);
//        glGetIntegerv(GL_VIEWPORT, view);
//        double tX=0,tY=0,tZ=0;
//        int newY=view[3]-view[1]-y-view[1];
//        gluProject((double)x, (double)newY, 0 , model, (const GLdouble*)proj, view, &tX,&tY,&tZ);
//        GL::mousePos.set(tX+Scenes::camera.x.movedCam,tY+Scenes::camera.y.movedCam,tZ);
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
    if (dir > 0){
        // Zoom in
        if(Scenes::current==Scenes::mapEdit){
            if(mapEdit::scale.x<0.99){
                mapEdit::scale.x+=0.01;
                mapEdit::scale.y+=0.01;
            }
        }
    }else{
        // Zoom out
        if(Scenes::current==Scenes::mapEdit){
            if(mapEdit::scale.x>0.01){
                mapEdit::scale.x-=0.01;
                mapEdit::scale.y-=0.01;
            }
        }
    }
}

