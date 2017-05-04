#include "GL.hpp"
#include "AssetsLoader.hpp"
#include "IOHandler.hpp"
#include "CallbacksHandler.hpp"

GL::GL(string name,float FPS,GLbitfield displayMode,int windowXsize,int windowYsize,nTColor clearcolor,bool blend,vector<GLenum>&enables,int argc, char** argv) {
    this->FPS=FPS;
    srand((unsigned int)time(NULL));
    glutInit(&argc, argv);
    glutInitDisplayMode(displayMode);
    glutInitWindowSize(windowXsize, windowYsize);
    defaultSize.set(windowXsize,windowYsize,0);
    currentSize=defaultSize;
    glutInitWindowPosition(0, 0);
    glutCreateWindow(name.c_str());
    GL::clearColor.set(clearcolor.R, clearcolor.G, clearcolor.B, clearcolor.A);
    glClearColor(clearcolor.R, clearcolor.G, clearcolor.B, clearcolor.A);
    for(GLenum en:enables){
        if(en==GL_ALPHA_TEST)glAlphaFunc(GL_GREATER,0.1);
        glEnable(en);
    }
    if(blend)
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glutSpecialUpFunc(IOHandler::specialKeyboardUp);
    glutKeyboardUpFunc(IOHandler::keyboardUp);
    glutSpecialFunc(IOHandler::specialKeyboard);
    glutDisplayFunc(CallbacksHandler::drawScene);
    glutReshapeFunc(CallbacksHandler::reshape);
    glutKeyboardFunc(IOHandler::keyboard);
    glutTimerFunc(0, CallbacksHandler::update, -1);
    glutTimerFunc(1000, CallbacksHandler::framesCheck, 0);
    glutPassiveMotionFunc(IOHandler::mousePassiveMotion);
    glutMotionFunc(IOHandler::mouseActiveMotion);
    glutMouseFunc(IOHandler::mousePress);
    //glutMouseWheelFunc(IOHandler::mouseWheel);
    Scenes::camera.setCam(10);
};

GL::GL(const GL& orig) {
}

GL::~GL() {
}

float GL::FPS;
nTColor GL::currentColor=nTColor::White();
nTColor GL::clearColor;
int GL::currentFont=0;
vector<void*> GL::fonts;
vector<GLuint> GL::textures;
vector<string> GL::textureNames;
nTPoint GL::defaultSize;
nTPoint GL::currentSize;
bool GL::FPSchecker=true;
bool GL::isPaused=false;
bool GL::leftMouseClicked=false;
bool GL::leftMouseReleased=false;
bool GL::rightMouseClicked=false;
bool GL::rightMouseReleased=false;
unsigned long int GL::framesInGame=0;
nTPoint GL::mousePos;
nTPoint GL::rawMousePos;


/**
 *	Sets the game FPS
 *
 *	@param FPS value of game FPS
**/
void GL::setFPS(float FPS){
    GL::FPS=FPS;
}

/**
 *	Gets the game FPS
 *
 *	@return value of game FPS
**/
float GL::getFPS(){
    return FPS;
}

/**
 *	Gets the delta time in milisecconds
 *
 *	@return delta time in milisecconds
**/
float GL::getMs(){
    return 1000/FPS;
}

/**
 *	Starts the openGL loop
**/
void GL::start(){
    glutMainLoop();
}

/**
 *	Loads an texture given its filepath
 *
 *	@param name name witch the texture will be stored
 *	@param path filepath of the texture
 *	@return id of the texture, case coudnt load the texture returns 0
**/
GLuint GL::loadTexture(string name,string path){
    GLuint temp=SOIL_load_OGL_texture(Util::newPath(path).c_str(),SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    if(temp==0){
        if(Util::DEBUG) cout<<"falha ao carregar textura "<<Util::newPath(path)<<endl;
    }else{
        int i=0;
        for(string n:textureNames)
            if(n==name){
                textures[i]=temp;
                return temp;
            }else
                i++;
        textures.push_back(temp);
        textureNames.push_back(name);
    }
    return temp;
}

/**
 *	Loads multiple textures given its filepath
 *
 *	@param name name witch the texture will be stored
 *	@param nOfTex ammount of texture files
 *	@param path filepath of the textures
 *	@return a vector of texture ids, case coudnt load the texture returns 0 at fail positions
**/
vector<GLuint> GL::loadTextures(string name,int nOfTex,string path){
    vector<GLuint> tex;
    GLuint tmp;
    for(int i=0;i<nOfTex;i++){
        tmp=SOIL_load_OGL_texture(Util::getDinamicPath(path,i,".png").c_str(),SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
        if(tmp){
            tex.push_back(0);
            tex[i]=tmp;
            textures.push_back(tex[i]);
            textureNames.push_back(Util::getDinamicName(name,i));
        }else{if(Util::DEBUG)cout<<"erro em:"<<path<<endl;}
    }
    return tex;
}

/**
 *	Changes the GL current color
 *
 *	@param color the new color
**/
void GL::setColor(nTColor color){
    GL::currentColor=color;
    glColor4f(color.R,color.G,color.B,color.A);
}

/**
 *	Gets the GL current color
 *
 *	@return the GL current color
**/
nTColor GL::getColor(){
    return GL::currentColor;
}

/**
 *	Draw a rectangle on the screen
 *
 *	@param pos the coordinates of the rectangle
 *	@param color the rectangle color
**/
void GL::drawRectangle(nTRectangle pos,nTColor color){
    nTColor tmp=getColor();
    setColor(color);
    glBegin(GL_POLYGON);
        glVertex3f(pos.p0.x, pos.p0.y, pos.p0.z);
        glVertex3f(pos.p1.x, pos.p0.y, pos.p0.z);
        glVertex3f(pos.p1.x, pos.p1.y, pos.p0.z);
        glVertex3f(pos.p0.x, pos.p1.y, pos.p0.z);
    glEnd();
    setColor(tmp);
}

/**
 *	Draw a red line between 2 points, used to draw collision lines
 *
 *	@param p0 the first point
 *	@param p1 the second point
**/
void GL::drawCollisionLine(nTPoint p0,nTPoint p1){
    nTColor tmp=getColor();
    setColor(getColorByName("red"));
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
        glVertex3f(p0.x, p0.y, 1);
        glVertex3f(p1.x, p1.y, 1);
    glEnd();
    glLineWidth(1);
    setColor(tmp);
}

/**
 *	Draw a black rectangle, used to draw collision boundaries
 *
 *	@param pos collision box(the coordinates of the rectangle)
**/
void GL::drawCollision(nTRectangle pos){
    nTColor tmp=getColor();
    setColor(getColorByName("black"));
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
        glVertex3f(pos.p0.x, pos.p0.y, 0.999);
        glVertex3f(pos.p1.x, pos.p0.y, 0.999);
        glVertex3f(pos.p1.x, pos.p1.y, 0.999);
        glVertex3f(pos.p0.x, pos.p1.y, 0.999);
    glEnd();
    glLineWidth(1);
    setColor(tmp);
}

/**
 *	Draw a texture
 *
 *	@param pos the coordinates of the rectangle
 *	@param color the base color to draw the texture
 *	@param tex the texture id
 *	@param Orientation the orientation for the texture coordinates, usually Util::orientation_right, Util::orientation_left or Util::orientation_rotated
**/
void GL::drawTexture(nTRectangle pos,nTColor color,GLuint tex,int Orientation){
    nTColor tmp=getColor();
    setColor(color);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);
    glBegin(GL_QUADS);
    if(Orientation<=Util::orientation_left){
      glTexCoord2f(1, 0); glVertex3f(pos.p0.x, pos.p0.y, pos.p0.z);
      glTexCoord2f(0, 0); glVertex3f(pos.p1.x, pos.p0.y, pos.p0.z);
      glTexCoord2f(0, 1); glVertex3f(pos.p1.x, pos.p1.y, pos.p0.z);
      glTexCoord2f(1, 1); glVertex3f(pos.p0.x, pos.p1.y, pos.p0.z);
    }else if(Orientation==Util::orientation_right){
      glTexCoord2f(0, 0); glVertex3f(pos.p0.x, pos.p0.y, pos.p0.z);
      glTexCoord2f(1, 0); glVertex3f(pos.p1.x, pos.p0.y, pos.p0.z);
      glTexCoord2f(1, 1); glVertex3f(pos.p1.x, pos.p1.y, pos.p0.z);
      glTexCoord2f(0, 1); glVertex3f(pos.p0.x, pos.p1.y, pos.p0.z);
    }else if(Orientation==Util::orientation_rotated){
      glTexCoord2f(0, 0); glVertex3f(pos.p0.x, pos.p0.y, pos.p0.z);
      glTexCoord2f(0, 1); glVertex3f(pos.p1.x, pos.p0.y, pos.p0.z);
      glTexCoord2f(1, 1); glVertex3f(pos.p1.x, pos.p1.y, pos.p0.z);
      glTexCoord2f(1, 0); glVertex3f(pos.p0.x, pos.p1.y, pos.p0.z);
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
    setColor(tmp);
}

/**
 *	Draw a texture centered at a point
 *
 *	@param pos point to center the texture
 *	@param size size of the texture
 *	@param color the base color to draw the texture
 *	@param tex the texture id
 *	@param Orientation the orientation for the texture coordinates, usually Util::orientation_right, Util::orientation_left or Util::orientation_rotated
**/
void GL::drawCenteredTexture(nTPoint pos,nTPoint size,nTColor color,GLuint tex,int Orientation){
    pos.x-=size.x/2;
    pos.y-=size.y/2;
    drawTexture(nTRectangle::get(pos.x,pos.y,pos.x+size.x,pos.y+size.y,pos.z),color,tex,Orientation);
}

/**
 *	Draw a texture
 *
 *	@param pos the coordinates of the rectangle
 *	@param tex the texture id
 *	@param Orientation the orientation for the texture coordinates, usually Util::orientation_right, Util::orientation_left or Util::orientation_rotated
**/
void GL::drawTexture(nTRectangle pos,GLuint tex,int Orientation){
    drawTexture(pos,nTColor::White(),tex,Orientation);
}

/**
 *	Draw a texture centered at a point
 *
 *	@param pos point to center the texture
 *	@param size size of the texture
 *	@param tex the texture id
 *	@param Orientation the orientation for the texture coordinates, usually Util::orientation_right, Util::orientation_left or Util::orientation_rotated
**/
void GL::drawCenteredTexture(nTPoint pos,nTPoint size,GLuint tex,int Orientation){
    drawCenteredTexture(pos,size,nTColor::White(),tex,Orientation);
}

/**
 *	Draw a texture
 *
 *	@param pos the coordinates of the rectangle
 *	@param color the base color to draw the texture
 *	@param tex the texture id
**/
void GL::drawTexture(nTRectangle pos,nTColor color,GLuint tex){
    drawTexture(pos,color,tex,Util::orientation_right);
}

/**
 *	Draw a texture centered at a point
 *
 *	@param pos point to center the texture
 *	@param size size of the texture
 *	@param color the base color to draw the texture
 *	@param tex the texture id
**/
void GL::drawCenteredTexture(nTPoint pos,nTPoint size,nTColor color,GLuint tex){
    drawCenteredTexture(pos,size,color,tex,Util::orientation_right);
}

/**
 *	Draw a texture
 *
 *	@param pos the coordinates of the rectangle
 *	@param tex the texture id
**/
void GL::drawTexture(nTRectangle pos,GLuint tex){
    drawTexture(pos,nTColor::White(),tex,Util::orientation_right);
}

/**
 *	Draw a texture centered at a point
 *
 *	@param pos point to center the texture
 *	@param size size of the texture
 *	@param tex the texture id
**/
void GL::drawCenteredTexture(nTPoint pos,nTPoint size,GLuint tex){
    drawCenteredTexture(pos,size,nTColor::White(),tex,Util::orientation_right);
}

/**
 *	Check if a sound is playing
 *
 *	@param name name of the color
 *	@return the color(ammount of Red, Green, Blue and Alpha)
**/
nTColor GL::getColorByName(string name){
	for(int i=0;name[i];i++)name[i]=tolower(name[i]);
	float R=1,G=1,B=1;
	if(name=="black"){
		R=0;G=0;B=0;
	}else if(name=="red"){
		R=1;G=0;B=0;
	}else if(name=="green"){
		R=0;G=1;B=0;
	}else if(name=="blue"){
		R=0;G=0;B=1;
	}else if(name=="yellow"){
		R=1;G=1;B=0;
	}else if(name=="magenta"){
		R=1;G=0;B=1;
	}else if(name=="cyan"){
		R=0;G=1;B=1;
	}else if(name=="grey"){
		R=0.6;G=0.6;B=0.6;
	}else if(name=="violet"){   //mish 153,112,205 ou 99,70,CD
		R=0.6;G=0.4392156;B=0.80392156;
	}else if(name=="white"){
		R=1;G=1;B=1;
	}else if(name=="mouseSelected"){
		R=0.3;G=0.3;B=0.3;
	}
	return nTColor::get(R,G,B);
}

/**
 *	Load a bitmap font given a filepath
 *
 *	@param path filepath of the bitmap font
 *	@return true if the font was successfully loaded, otherwise false
**/
bool GL::loadFont(string path){
    path=Util::newPath(path);
    nTFont *fnt=nTFont::loadFont(path);
    if(fnt==nullptr) return false;
    fonts.push_back(fnt);
    return true;
}

/**
 *	Changes the GL current font
 *
 *	@param name name of the font
**/
void GL::setFont(string name){
    int font=-1;
    nTFont *fnt;
    for(int i=0;i<fonts.size();i++){
        fnt=(nTFont*) fonts[i];
        if(fnt->name==name){
            font=i;
            break;
        }
    }
    if(font<0){font=0;if(Util::DEBUG)cout<<"Cant find font: "<<name<<endl;}
    currentFont=font;
}

/**
 *	Changes the GL current font
 *
 *	@param name name of the font
 *	@param size size of the font
**/
void GL::setFont(string name,int size){
    int font=-1;
    nTFont *fnt;
    bool founded=false;
    for(int i=0;i<fonts.size();i++){
        fnt=(nTFont*) fonts[i];
        if(fnt->size==size){
            if(font<0) font=i;
            if(fnt->name==name){
                font=i;
                founded=true;
                break;
            }
        }
    }
    if(font<0)font=0;
    if(!founded)if(Util::DEBUG)cout<<"Cant find font: "<<name<<" with size:"<<size<<endl;
    currentFont=font;
}

/**
 *	Draw a text
 *
 *	@param point the start position of the text
 *	@param text the text to be written
 *	@param color the color of the font
**/
void GL::drawText(nTPoint point,string text,nTColor color){
    nTFont *fnt=(nTFont*) fonts[currentFont];
    fnt->drawText(text,point,color);
}
/**
 *	Draw a text centering its x position
 *
 *	@param point the start position of the text
 *	@param text the text to be written
 *	@param color the color of the font
**/
void GL::drawCentered_X_Text(nTPoint point, string text, nTColor color){
    nTFont *fnt=(nTFont*) fonts[currentFont];
    fnt->drawCentered_X_Text(text,point,color);
}

/**
 *	Draw a text centering its y position
 *
 *	@param point the start position of the text
 *	@param text the text to be written
 *	@param color the color of the font
**/
void GL::drawCentered_Y_Text(nTPoint point, string text, nTColor color){
    nTFont *fnt=(nTFont*) fonts[currentFont];
    fnt->drawCentered_Y_Text(text,point,color);
}

/**
 *	Draw a text centering its x and y position
 *
 *	@param point the start position of the text
 *	@param text the text to be written
 *	@param color the color of the font
**/
void GL::drawCentered_X_Y_Text(nTPoint point, string text, nTColor color){
    nTFont *fnt=(nTFont*) fonts[currentFont];
    fnt->drawCentered_X_Y_Text(text,point,color);
}

/**
 *	Draw a text centering its x position for each line
 *
 *	@param point the start position of the text
 *	@param text the text to be written
 *	@param color the color of the font
**/
void GL::drawCentered_MultilineX_Text(nTPoint point, string text, nTColor color){
    nTFont *fnt=(nTFont*) fonts[currentFont];
    fnt->drawCentered_MultilineX_Text(text,point,color);
}

/**
 *	Draw a text centering its x position for each line and its y position
 *
 *	@param point the start position of the text
 *	@param text the text to be written
 *	@param color the color of the font
**/
void GL::drawCentered_MultilineX_Y_Text(nTPoint point, string text, nTColor color){
    nTFont *fnt=(nTFont*) fonts[currentFont];
    fnt->drawCentered_MultilineX_Y_Text(text,point,color);
}

/**
 *	Draw a polygon
 *
 *	@param point the center of the polygon
 *	@param radius the radius of the polygon
 *	@param edges the ammount of edges of the polygon
**/
void GL::drawPolygon(nTPoint point,float radius,int edges){
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(point.x,point.y,point.z);
    for(int i=0;i<=edges;i++){
        glVertex3f(cos(2*M_PI*i/edges)*radius+point.x,sin(2*M_PI*i/edges)*radius+point.y,point.z);
    }
    glEnd();
}

/**
 *	Find a texture by name and return its index on vector textures
 *
 *	@param name name of the texture
 *	@return index of the texture
**/
GLuint GL::getTextureByName(string name){
    int i=0;
    for(string n:textureNames)
        if(n==name){
            return textures[i];
        }else
            i++;
    return 0;
}

/**
 *	Find textures by name and return its indexes on vector textures
 *
 *	@param name base name of the texture
 *	@param nOfTex ammount of textures
 *	@return vector of indexes of the textures
**/
vector<GLuint> GL::getTexturesByName(string name,int nOfTex){
    vector<GLuint> out;
    GLuint tmp;
    for(int i=0;i<nOfTex;i++){
        tmp=getTextureByName(Util::getDinamicName(name,i));
        if(tmp)
            out.push_back(tmp);
    }
    return out;
}

/**
 *	Draw and manage mouse events of a button
 *
 *	@param collision the coordinates of the button
 *	@param pressedColor the draw color of the button when has the mouse over it
 *	@param tex texture id of the button
 *	@param clickFunction the function to be called when the button is left mouse clicked
 *	@param releaseFunction the function to be called when the left mouse clicked button is released
 *	@param RclickFunction the function to be called when the button is right mouse clicked
 *	@param RreleaseFunction the function to be called when the right mouse clicked button is released
 *	@return true if the button is being left mouse clicked
**/
bool GL::buttonBehave(nTRectangle collision,nTColor pressedColor,GLuint tex,void(*clickFunction)(int,int),void(*releaseFunction)(int,int),void(*RclickFunction)(int,int),void(*RreleaseFunction)(int,int)){
    if(GL::mousePos.x>=collision.p0.x&&GL::mousePos.x<=collision.p1.x&&((GL::mousePos.y>=collision.p0.y&&GL::mousePos.y<=collision.p1.y)||(GL::mousePos.y>=collision.p1.y&&GL::mousePos.y<=collision.p0.y))){
        if(tex){
            if(collision.p0.z==1)collision.p0.z=0.99999;
            GL::drawTexture(collision,pressedColor,tex);
        }else{
            collision.p0.z+=0.00001;
            GL::drawRectangle(collision,pressedColor);
        }
        if(GL::leftMouseReleased||GL::rightMouseReleased)
            AL::singleton->playSoundByName("mouse");
        if(GL::leftMouseClicked){
            if(*clickFunction!=NULL){
                clickFunction((int)GL::mousePos.x,(int)GL::mousePos.y);
            }
            return true;
        }else if(GL::leftMouseReleased){
            if(*releaseFunction!=NULL){
                 releaseFunction((int)GL::mousePos.x,(int)GL::mousePos.y);
            }
            GL::leftMouseReleased=0;
            return true;
        }
        if(GL::rightMouseClicked){
            if(*RclickFunction!=NULL){
                RclickFunction((int)GL::mousePos.x,(int)GL::mousePos.y);
            }
            return false;
        }else if(GL::rightMouseReleased){
            if(*RreleaseFunction!=NULL){
                 RreleaseFunction((int)GL::mousePos.x,(int)GL::mousePos.y);
            }
            GL::rightMouseReleased=0;
            return false;
        }
    }else{
        if(tex){
            if(collision.p0.z==1)collision.p0.z=0.99999;
            GL::drawTexture(collision,tex);
        }
    }
    return false;
}

/**
 *	Modify the operator << to print this type of objects
 *	The parameters are passed automatically
 *
 *	@param strm current string stream
 *	@param gl object address
 *	@return the old stream plus the object toString
**/
ostream& operator<<(ostream &strm, const GL &gl) {
    if(Util::DEBUG)
        return strm <<"GL:["<<"FPS("<<gl.FPS<<"),"<<"CurrentColor("<<"R:"<<gl.currentColor.R<<" G:"<<gl.currentColor.G<<" B:"<<gl.currentColor.B<<" A:"<<gl.currentColor.A<<"),"<<
                "Loaded Texutres("<<gl.textures.size()<<"),"<<"ScreenSize("<<"x:"<<gl.currentSize.x<<" y:"<<gl.currentSize.y<<"),"<<"Is paused("<<gl.isPaused<<"),"<<"]\n";
    return strm;
}

/**
 *	Toogles the sound(no loop) playback and save the settings
 *
 *	@param a do nothing
 *	@param b do nothing
**/
void modifySound(int a,int b){
  AL::setSoundState(!AL::getSoundState());
  AssetsLoader::saveSettings();
}

/**
 *	Toogles the music(looped) playback and save the settings
 *
 *	@param a do nothing
 *	@param b do nothing
**/
void modifyMusic(int a,int b){
  AL::setMusicState(!AL::getMusicState());
  AssetsLoader::saveSettings();
}

/**
 *	Draw the pause screen and play the pause music
**/
void GL::drawPause(){
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    AL::singleton->stopAllSoundsExcept(AL::singleton->getSoundByName("sinuca"));
    AL::singleton->playSoundByName("sinuca");
    GL::setFont("BITMAP_TIMES_ROMAN_24");
    string text="Pausado";
    GL::drawTexture((nTRectangle::get(
    Scenes::camera.x.movedCam,
    GL::defaultSize.y+Scenes::camera.y.movedCam,
    GL::defaultSize.x+Scenes::camera.x.movedCam,
    Scenes::camera.y.movedCam,0.95)),
    nTColor::get(1,1,1,0.5),GL::getTextureByName("dce"));
    if(AL::getSoundState()){
        GL::buttonBehave(nTRectangle::get(700+Scenes::camera.x.movedCam,50+Scenes::camera.y.movedCam,740+Scenes::camera.x.movedCam,10+Scenes::camera.y.movedCam,1),nTColor::Black(),GL::getTextureByName("soundOn"),NULL,*modifySound,NULL,NULL);
      }else{
        GL::buttonBehave(nTRectangle::get(700+Scenes::camera.x.movedCam,50+Scenes::camera.y.movedCam,740+Scenes::camera.x.movedCam,10+Scenes::camera.y.movedCam,1),nTColor::Black(),GL::getTextureByName("soundOff"),NULL,*modifySound,NULL,NULL);
      }
      if(AL::getMusicState()){
        GL::buttonBehave(nTRectangle::get(640+Scenes::camera.x.movedCam,50+Scenes::camera.y.movedCam,680+Scenes::camera.x.movedCam,10+Scenes::camera.y.movedCam,1),nTColor::Black(),GL::getTextureByName("musicOn"),NULL,*modifyMusic,NULL,NULL);
      }else{
        GL::buttonBehave(nTRectangle::get(640+Scenes::camera.x.movedCam,50+Scenes::camera.y.movedCam,680+Scenes::camera.x.movedCam,10+Scenes::camera.y.movedCam,1),nTColor::Black(),GL::getTextureByName("musicOff"),NULL,*modifyMusic,NULL,NULL);
      }
    GL::drawCenteredTexture(nTPoint::get(GL::defaultSize.x/2+Scenes::camera.x.movedCam,Scenes::camera.y.movedCam+GL::defaultSize.y/2,0.99),
                            nTPoint::get(200,60),GL::getTextureByName("Ballon"));

    GL::drawCentered_X_Y_Text(nTPoint::get(GL::defaultSize.x/2+Scenes::camera.x.movedCam,Scenes::camera.y.movedCam+GL::defaultSize.y/2,1),text,GL::getColorByName("violet"));
    if(GL::buttonBehave(nTRectangle::get(600+Scenes::camera.x.movedCam,600+Scenes::camera.y.movedCam,700+Scenes::camera.x.movedCam,550+Scenes::camera.y.movedCam,1),GL::getColorByName("mouseSelected"),GL::getTextureByName("quitIcon"),NULL,NULL,NULL,NULL))
        Scenes::current=Scenes::menu;
}

/**
 *	Gets the 4X4 matrix for rotations
 *
 *	@param angle rotation angle in degrees
 *	@return vector of a vector of floats containing the matrix
**/
vector<vector<float> > GL::getRotateMatrix(float angle){
    float rad=Util::angleToRad(angle);
    vector<vector<float> >out;
    out.resize(4,vector<float>(4,0));
    out[0][0]=cos(rad);
    out[0][1]=-sin(rad);
    out[1][1]=cos(rad);
    out[1][0]=sin(rad);
    out[2][2]=1;
    out[3][3]=1;
    return out;
}

/**
 *	Gets the 4X4 matrix for translations
 *
 *	@param point the point to translate
 *	@return vector of a vector of floats containing the matrix
**/
vector<vector<float> > GL::getTranslateMatrix(nTPoint point){
    vector<vector<float> >out;
    out.resize(4,vector<float>(4,0));
    out[0][0]=1;
    out[1][1]=1;
    out[2][2]=1;
    out[3][3]=1;

    out[0][3]=point.x;
    out[1][3]=point.y;
    out[2][3]=point.z;
    return out;
}

/**
 *	Gets the 4X4 matrix for rotations at a specific point
 *
 *	@param point the point to translate
 *	@param center the point of center of the rotation
 *	@param angle rotation angle in degrees
 *	@return vector of a vector of floats containing the matrix
**/
nTPoint GL::rotatePoint(nTPoint point,nTPoint center, float angle){
    vector<vector<float> >Mpoint;
    Mpoint.resize(4,vector<float>(1,0));
    Mpoint[0][0]=point.x;
    Mpoint[1][0]=point.y;
    Mpoint[2][0]=point.z;
    Mpoint[3][0]=1;

    center.set(-center.x,-center.y,-center.z);
    Mpoint=Util::multiplyMatrix(GL::getTranslateMatrix(center),Mpoint);
    Mpoint=Util::multiplyMatrix(GL::getRotateMatrix(angle),Mpoint);
    center.set(-center.x,-center.y,-center.z);
    Mpoint=Util::multiplyMatrix(GL::getTranslateMatrix(center),Mpoint);
    point.set(Mpoint[0][0],Mpoint[1][0],Mpoint[2][0]);

    return point;
}

/**
 *	Transforms a world point to a window point using view port
 *
 *	@param point the point to be transformed
 *	@return the transformed point
**/
nTPoint GL::getModelViewPoint(nTPoint point){
    GLfloat matrixf[16];
    vector<vector<float> >Mpoint;
    vector<vector<float> >out;

    Mpoint.resize(4,vector<float>(1,0));
    out.resize(4,vector<float>(4,0));

    glGetFloatv(GL_MODELVIEW_MATRIX, matrixf);

    Mpoint[0][0]=point.x;
    Mpoint[1][0]=point.y;
    Mpoint[2][0]=point.z;
    Mpoint[3][0]=1;

    out[0][0]=(float)matrixf[0];
    out[0][1]=(float)matrixf[1];
    out[0][2]=(float)matrixf[2];
    out[0][3]=(float)matrixf[3];
    out[1][0]=(float)matrixf[4];
    out[1][1]=(float)matrixf[5];
    out[1][2]=(float)matrixf[6];
    out[1][3]=(float)matrixf[7];
    out[2][0]=(float)matrixf[8];
    out[2][1]=(float)matrixf[9];
    out[2][2]=(float)matrixf[10];
    out[2][3]=(float)matrixf[11];
    out[3][0]=(float)matrixf[12];
    out[3][1]=(float)matrixf[13];
    out[3][2]=(float)matrixf[14];
    out[3][3]=(float)matrixf[15];
    Mpoint=Util::multiplyMatrix(out,Mpoint);

    point.set(Mpoint[0][0],Mpoint[1][0],Mpoint[2][0]);
    return point;
}

/**
 *	Draw the in-game HUD
**/
void GL::drawHUD(){
    GL::setFont("BITMAP_HELVETICA_18");
    nTPoint point;
    char buffer[5];
    point.set(Scenes::camera.x.movedCam+50,Scenes::camera.y.movedCam+18,1);
    for(int i=0;i<Player::getPlayerById(0)->life;i++){
        point.x+=25;
        GL::drawTexture(nTRectangle::getCollision(point,nTPoint::get(20,20,1)),GL::getTextureByName("S2"));
    }
    point.set(Scenes::camera.x.movedCam+10,Scenes::camera.y.movedCam+23,1);
    GL::drawCentered_Y_Text(point,"Vida:",GL::getColorByName("black"));
    point.set(Scenes::camera.x.movedCam+650,Scenes::camera.y.movedCam+23,1);
    snprintf(buffer,5,"%d",(int)GL::getGameMs()/1000);
    string strT(buffer);
    GL::drawCentered_Y_Text(point,"Tempo(s): "+strT,GL::getColorByName("black"));
    if(!Scenes::freeGameMode){
        point.set(Scenes::camera.x.movedCam+168,Scenes::camera.y.movedCam+23,1);
        snprintf(buffer,5,"%d",Player::lives);
        string strL(buffer);
        GL::drawCentered_Y_Text(point,"Vidas: "+strL,GL::getColorByName("black"));
        point.set(Scenes::camera.x.movedCam+280,Scenes::camera.y.movedCam+23,1);
        string strF;
        if(Player::stage==1){
            strF="Graduando";
        }else if(Player::stage==2){
            strF="Pos Grad";
        }else{
            strF="Cefetinho";
        }
        GL::drawCentered_Y_Text(point,"Fase: "+strF,GL::getColorByName("black"));
        point.set(Scenes::camera.x.movedCam+480,Scenes::camera.y.movedCam+23,1);
        snprintf(buffer,5,"%.4f",Player::coeficiente);


        string strC(buffer);
        GL::drawCentered_Y_Text(point,"Coeficiente: "+strC,GL::getColorByName("black"));
    }
}

/**
 *	Gets the ammount of millisecconds passed since the start of the FPS count
 *
 *	@return the time in millisecconds
**/
float GL::getGameMs(){
    return GL::framesInGame*1000/GL::FPS;
}
