#include "Texture.hpp"

Texture::Texture(vector<GLuint> textures) {
    currentTex=0;
    ms=GL::getGameMs();
    finished=false;
    timeToNext=100;
    setTextures(textures);
};

Texture::Texture(vector<GLuint> textures,double timeToNext) {
    currentTex=0;
    ms=GL::getGameMs();
    finished=false;
    timeToNext=timeToNext;
    setTextures(textures);
};

Texture::Texture(GLuint texture) {
    currentTex=0;
    ms=GL::getGameMs();
    finished=false;
    timeToNext=100;
    setTextures(texture);
};

Texture::Texture(GLuint texture,double timeToNext) {
    currentTex=0;
    ms=GL::getGameMs();
    finished=false;
    timeToNext=timeToNext;
    setTextures(texture);
};

Texture::Texture(const Texture& orig) {
}

Texture::~Texture() {
}


void Texture::setTextures(vector<GLuint> textures){
    this->textures=textures;
}

void Texture::setTextures(GLuint texture){
    textures.clear();
    textures.push_back(texture);
}

void Texture::setTimeToNext(double timeToNext){
    this->timeToNext=timeToNext;
}

void Texture::resetAnimation(){
    currentTex=0;
    ms=GL::getGameMs();
}

int Texture::getCurrentIndex(){
    process();
}

int Texture::getTexSize(){
    return textures.size();
}

GLuint Texture::get(){
    process();
    return textures[currentTex];
}

void Texture::process(){
    double elapsed=GL::getGameMs()-ms;
    if(elapsed>=timeToNext||elapsed<0){
        finished=false;
        ms=GL::getGameMs();
        int step=1;
        if(elapsed>=0){
            step=floor(fmod(elapsed,timeToNext));
        }
        currentTex+=step;
        if(currentTex>=textures.size()){
            currentTex%=textures.size();
            finished=true;
        }
    }
}

 bool Texture::finishedAnimation(){
    return finished=false;
 }
