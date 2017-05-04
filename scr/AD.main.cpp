#include "importLibs.h"

int main(int argc, char** argv) {
    vector<GLenum> enables;
    enables.push_back(GL_DEPTH_TEST);
    enables.push_back(GL_BLEND);
    enables.push_back(GL_ALPHA_TEST);
    GL *gl=new GL("Adventure Discrete",30,GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH|GLUT_STENCIL|GLUT_ALPHA,
                  800,600,nTColor::Black(),true,enables,argc,argv);
    Enemy::registerNicks();
    mapEdit::definePages();
    new Mechanics();


    if(!AssetsLoader::loadSettings()) if(Util::DEBUG) cout<<"erro ao carregar configs"<<endl;

    if(!AssetsLoader::loadMaps(Map::nOfMaps)) if(Util::DEBUG) cout<<"erro ao carregar mapas"<<endl;

    new AL();

    if(!AssetsLoader::loadSounds()) if(Util::DEBUG) cout<<"erro ao carregar sons"<<endl;

    if(!AssetsLoader::loadTextures()) if(Util::DEBUG) cout<<"erro ao carregar texturas"<<endl;

    if(!AssetsLoader::loadFonts()) if(Util::DEBUG) cout<<"erro ao carregar fontes"<<endl;

    Entity::setSprites();

    new Player(Player::defaultLife,nTPoint::Origin(),Player::defaultPSize,Entity::getAnimationVector(Player::playerAnim[0],Player::playerAnimSize[0]),1);

    gl->start();

    cout<<"dispose objects please\n";
    return 0;
}
