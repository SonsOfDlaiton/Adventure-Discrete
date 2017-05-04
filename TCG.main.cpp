#include "importLibs.h"

int main(int argc, char** argv) {
    vector<GLenum> enables;
    clearColor.setColor(0,0,0,0);
    enables.push_back(GL_DEPTH_TEST);
    enables.push_back(GL_BLEND);
    enables.push_back(GL_ALPHA_TEST);
    GL *gl=new GL("The.COM Games",30,GLUT_DOUBLE | GLUT_RGBA|GLUT_DEPTH|GLUT_STENCIL|GLUT_ALPHA,
                  800,600,clearColor,true,enables,argc,argv);
    Enemy::registerNicks();
    mapEdit::definePages();
    
    
    
    if(!loadSettings()) if(Util::DEBUG) cout<<"erro ao carregar configs"<<endl;
    
    if(!loadMaps(Map::nOfMaps)) if(Util::DEBUG) cout<<"erro ao carregar mapas"<<endl;
    
    al=new AL();
    
    if(!loadSounds()) if(Util::DEBUG) cout<<"erro ao carregar sons"<<endl;
    
    if(!loadTextures()) if(Util::DEBUG) cout<<"erro ao carregar texturas"<<endl;
    
    setSprites();
    
    player=new Player(3,Util::nTPointSet(0,0,0),Player::defaultPSize,Entity::getAnimationVector(playerAnim[0],playerAnimSize[0]),1);
    
    gl->start();
    return 0;
}
