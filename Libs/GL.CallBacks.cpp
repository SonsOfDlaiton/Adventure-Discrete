#include "GL.Callbacks.h"

bool glCallbacksReleaseMouseOffSet=false;//evita de definir o mouse solto no meio ou final da funcao update e o programa nao fazer a leitura
bool glCallbacksReleaseMouseROffSet=false;//evita de definir o mouse solto no meio ou final da funcao update e o programa nao fazer a leitura
bool glCallbacksReleaseZOffSet=false;

void drawScene(void){
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
}

void reshape(int width, int height){
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

int GLSpecReleaseKey=0;

void specialKeyboard(int key, int x, int y){
  switch(key)
  {
    case GLUT_KEY_UP:
        if(Scenes::current==Scenes::game&&!GL::isPaused){
            Tutorials::pressedKey=GLUT_KEY_UP;
            player->atackDirection=Util::up;//NAO PULA!!!
            if(Scenes::freeCam)
                 Scenes::camera.y.movingCam=-1;
        }else if(Scenes::current==Scenes::mapEdit)
            Scenes::camera.y.movingCam=-1;
    break;

    case GLUT_KEY_DOWN:
        if(Scenes::current==Scenes::game&&!GL::isPaused){
            Tutorials::pressedKey=GLUT_KEY_UP;
            player->atackDirection=Util::down;
            player->canTp=true;
            if(Scenes::freeCam)
                 Scenes::camera.y.movingCam=1;
        }  else if(Scenes::current==Scenes::mapEdit)
            Scenes::camera.y.movingCam=1;  
      
    break;

    case GLUT_KEY_LEFT:
        if(Scenes::current==Scenes::game&&!GL::isPaused){
            if(GLSpecReleaseKey==-1)
                GLSpecReleaseKey=0;
            Tutorials::pressedKey=GLUT_KEY_UP;
             player->orientation=-1;
             player->atackDirection=Util::left;
             if(glutGetModifiers()==GLUT_ACTIVE_SHIFT){
                 if(player->canWalk&&!player->atacking)player->run(Util::left);
             }else{
                if(player->canWalk&&!player->atacking)player->walk(Util::left);
             }
             if(Scenes::freeCam)
                    Scenes::camera.x.movingCam=-1;
        }else if(Scenes::current==Scenes::mapEdit)
            Scenes::camera.x.movingCam=-1;
    break;

    case GLUT_KEY_RIGHT:
        if(Scenes::current==Scenes::game&&!GL::isPaused){
            if(GLSpecReleaseKey==1)
                GLSpecReleaseKey=0;
            Tutorials::pressedKey=GLUT_KEY_UP;
            player->orientation=1;
            player->atackDirection=Util::right;
            if(glutGetModifiers()==GLUT_ACTIVE_SHIFT){
                if(player->canWalk&&!player->atacking)player->run(Util::right);
            }else{
                if(player->canWalk&&!player->atacking)player->walk(Util::right);
            }
            if(Scenes::freeCam)
                 Scenes::camera.x.movingCam=1;
        }else if(Scenes::current==Scenes::mapEdit)
            Scenes::camera.x.movingCam=1;
      break;
}
}
void keyboard(unsigned char key, int x, int y){
    switch(key){
      case ' ':
        if(!GL::isPaused&&Scenes::current==Scenes::game){
            if(!Tutorials::isPaused)player->jump();
            player->spacePressed=true;
            Tutorials::pressedKey=' ';
        }
        if(Scenes::current==Scenes::splash)
          Scenes::skipScene=true;
      break;

      case 'Z':
      case 'z':
          if(!GL::isPaused){
            glCallbacksReleaseZOffSet=false;
            Tutorials::pressedKey='z';
            player->atacking=Player::melee;
          }
      break;

      case 'X':
      case 'x':
          if(!GL::isPaused){
            Tutorials::pressedKey='x';
            player->atacking=Player::ranged;
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

void specialKeyboardUp(int key,int x,int y){
  switch(key)
  {
    case GLUT_KEY_UP:
    if(Scenes::current==Scenes::game){
      if(player->orientation>0)
            player->atackDirection=Util::right;
        else
            player->atackDirection=Util::left;
    }
    Scenes::camera.y.movingCam=0;
    break;

    case GLUT_KEY_DOWN:
    if(Scenes::current==Scenes::game){
        player->canTp=false;
        if(player->orientation>0)
            player->atackDirection=Util::right;
        else
            player->atackDirection=Util::left;
    }
      Scenes::camera.y.movingCam=0;
    break;

    case GLUT_KEY_LEFT:
    if(Scenes::current==Scenes::game){
      if(player->orientation>0)
            player->atackDirection=Util::right;
        else
            player->atackDirection=Util::left;
      GLSpecReleaseKey=-1;
    }
    Scenes::camera.x.movingCam=0;
    break;

    case GLUT_KEY_RIGHT:
    if(Scenes::current==Scenes::game){
      if(player->orientation>0)
            player->atackDirection=Util::right;
        else
            player->atackDirection=Util::left;
      GLSpecReleaseKey=1;
    }
    Scenes::camera.x.movingCam=0;
    break;

    default:
    break;
  }
}
void keyboardUp(unsigned char key,int x,int y){
    switch(key){
        case ' ':
        if(!GL::isPaused&&Scenes::current==Scenes::game){
            player->spacePressed=false;
        }
        break;
        case 'z':
        case 'Z':
            glCallbacksReleaseZOffSet=true;
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
              if(Scenes::current==Scenes::mapEdit&&mapEdit::size.y==0)
                  mapEdit::load(-1);
        break;
        
        case 'C':
        case 'c':
            if(!GL::isPaused&&Scenes::current==Scenes::game){
                player->god=!player->god;
                if(player->god){
                    al->stopSound(AL::getSoundByName("cafeSong"));
                    player->superMan=false;
                }
            }
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
                    if(tmp>=0&&tmp<maps.size()&&(mapEdit::size.y==0||mapEdit::size.x==0)){
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

void update(int n){
    if(GLSpecReleaseKey!=0){
        vector <mapCollision> var;
        bool condition=false;
        Blocks* bl;
        int type;
        nTPoint point;
        point.y=player->pos.y+1;
        point.x=player->pos.x;
        var=Map::checkCollision(point,player->size);
        for(int i=0; i<var.size(); i++){
          if(var[i].collision.firstObj==1){
              if(var[i].blockRef<=Map::staticBlocks.size()){
                bl=(Blocks*)Map::staticBlocks[var[i].blockRef];
                type=bl->type;
                if(type==17){
                      condition=true;
                }
              }else if(var[i].blockRef-Map::staticBlocks.size()<Map::dynamicBlocks.size()){
                  bl=(Blocks*)Map::dynamicBlocks[var[i].blockRef-Map::staticBlocks.size()];
                    type=bl->type;
                  if(type==204||type==254){
                      condition=true;
                      if(player->hSpeed!=0)
                          al->playSoundByName("ice");
                  }
              } 
          }
        }
        if(condition)
            player->reducing=true;
        else
          player->hSpeed=0;
    }
            
    GL::mousePos.x=GL::rawMousePos.x*(float)GL::defaultSize.x/(float)GL::currentSize.x+Scenes::camera.x.movedCam;
    GL::mousePos.y=GL::rawMousePos.y*(float)GL::defaultSize.y/(float)GL::currentSize.y+Scenes::camera.y.movedCam;
    if(glCallbacksReleaseZOffSet){
        glCallbacksReleaseZOffSet=false;
        if(!GL::isPaused){
            Tutorials::pressedKey='z';
            player->atacking=Player::meleeProjectile;
    }}
    if(glCallbacksReleaseMouseOffSet){
        GL::leftMouseReleased=false;
        glCallbacksReleaseMouseOffSet=false;
    }
    if(GL::leftMouseReleased)glCallbacksReleaseMouseOffSet=true;
    if(glCallbacksReleaseMouseROffSet){
        GL::rightMouseReleased=false;
        glCallbacksReleaseMouseROffSet=false;
    }
    if(GL::rightMouseReleased)glCallbacksReleaseMouseROffSet=true;
    glutPostRedisplay();
    glutTimerFunc(GL::getMs(), update, 0);
}
void mousePress(int button,int state,int x,int y){
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
void mousePassiveMotion(int x,int y){
//        GLdouble model[16];
//        GLdouble proj[16]; 
//        GLint view[4];       
//        glGetDoublev(GL_MODELVIEW_MATRIX, model);
//        glGetDoublev(GL_PROJECTION_MATRIX, proj);
//        glGetIntegerv(GL_VIEWPORT, view);
//        double tX=0,tY=0,tZ=0;
//        int newY=view[3]-view[1]-y-view[1];
//        gluProject((double)x, (double)newY, 0 , model, (const GLdouble*)proj, view, &tX,&tY,&tZ);     
//        GL::mousePos.setPoint(tX+Scenes::camera.x.movedCam,tY+Scenes::camera.y.movedCam,tZ);
    GL::rawMousePos.setPoint(x,y,0);
}
void mouseActiveMotion(int x,int y){
    GL::rawMousePos.setPoint(x,y,0);
}

void mouseWheel(int button, int dir, int x, int y){
    if (dir > 0){
        // Zoom in
        if(Scenes::current=Scenes::mapEdit){
            if(mapEdit::scale.x<0.99){
                mapEdit::scale.x+=0.01;
                mapEdit::scale.y+=0.01;
            }
        }            
    }else{
        // Zoom out
        if(Scenes::current=Scenes::mapEdit){
            if(mapEdit::scale.x>0.01){
                mapEdit::scale.x-=0.01;
                mapEdit::scale.y-=0.01;
            }
        }    
    }
}