#ifndef TEXTURE_H
#define TEXTURE_H
#include "Util.hpp"
#include "GL.hpp"

class Texture {
public:
    Texture(vector<GLuint> textures);
    Texture(vector<GLuint> textures,double timeToNext);
    Texture(GLuint texture);
    Texture(GLuint texture,double timeToNext);
    Texture(const Texture& orig);
    virtual ~Texture();

    void setTextures(vector<GLuint> textures);
    void setTextures(GLuint texture);
    void setTimeToNext(double timeToNext);
    void resetAnimation();
    int getCurrentIndex();
    int getTexSize();
    GLuint get();
    bool finishedAnimation();
private:
    void process();
    double timeToNext;
    int currentTex;
    double ms;
    bool finished;
    vector<GLuint> textures;
};

#endif /* TEXTURE_H */
