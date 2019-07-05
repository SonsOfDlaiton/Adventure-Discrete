#include "Texture.hpp"

Texture::Texture() {
    currentTex=0;
    ms=GL::getGameMs();
    finished=false;
    timeToNext=defaultTimeToNext;
};

Texture::Texture(vector<GLuint> textures) {
    currentTex=0;
    ms=GL::getGameMs();
    finished=false;
    timeToNext=defaultTimeToNext;
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
    timeToNext=defaultTimeToNext;
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

double Texture::defaultTimeToNext=Constants::TEXTURE_DefaultTimeToNext;

void Texture::setTextures(vector<GLuint> textures){
    this->textures=textures;
    currentTex=0;
    finished=false;
    ms=GL::getGameMs();
}

void Texture::setTextures(GLuint texture){
    textures.clear();
    textures.push_back(texture);
    currentTex=0;
    finished=false;
    ms=GL::getGameMs();
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
    return currentTex;
}

int Texture::getTexSize(){
    return textures.size();
}

GLuint Texture::get(){
    process();
    if(textures.size()==0) return GL::getTextureByName("Unknow");
    return textures[currentTex];
}

void Texture::process(){
    if(textures.size()==1){
        if(!finished&&(GL::getGameMs()-ms>=timeToNext||GL::getGameMs()-ms<0))
            finished=true;
        return;
    }
    double elapsed=GL::getGameMs()-ms;
    if(elapsed>=timeToNext||elapsed<0){
        ms=GL::getGameMs();
        int step=1;
        if(elapsed>=0){
            step=floor(elapsed/timeToNext);
        }
        currentTex+=step;
        if(currentTex>=textures.size()&&textures.size()){
            currentTex=currentTex%textures.size();
            finished=true;
        }
    }
}

 bool Texture::finishedAnimation(){
    process();
    if(finished){
        finished=false;
        return true;
    }
    return false;
 }
