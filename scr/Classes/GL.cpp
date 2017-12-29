#include "GL.hpp"
#include "AssetsLoader.hpp"
#include "IOHandler.hpp"
#include "CallbacksHandler.hpp"

GL::GL(string name,double FPS,GLbitfield displayMode,int windowXsize,int windowYsize,nTColor clearcolor,bool blend,vector<GLenum>&enables,int argc, char** argv) {
    this->FPS=FPS;
    srand((unsigned int)time(NULL));
    glutInit(&argc, argv);
    glutInitDisplayMode(displayMode);
    glutInitWindowSize(windowXsize, windowYsize);
    defaultSize.set(windowXsize,windowYsize,0);
    currentViewPort=nTRectangle::get(0,0,defaultSize.x,defaultSize.y);
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

double GL::FPS;
nTColor GL::currentColor=nTColor::White();
nTColor GL::clearColor;
int GL::currentFont=0;
vector<void*> GL::fonts;
vector<GLuint> GL::textures;
vector<string> GL::textureNames;
nTPoint GL::defaultSize;
nTRectangle GL::currentViewPort;
bool GL::FPSchecker=true;
bool GL::isPaused=false;
bool GL::leftMouseClicked=false;
bool GL::leftMouseReleased=false;
bool GL::rightMouseClicked=false;
bool GL::rightMouseReleased=false;
unsigned long int GL::framesInGame=0;
nTPoint GL::mousePos;
nTPoint GL::rawMousePos;
int GL::editOnFocous=-1;
int GL::editTextPosition=-1;
vector<string> GL::edits;
vector<bool> GL::editsNumeric;
vector<string> GL::editsText;
string GL::popupText="";
string GL::popupFont="";
double GL::popupMs;
bool GL::popupPaused=false;
int GL::popupDuration=0;
bool GL::editCharCursor;


/**
 *	Sets the game FPS
 *
 *	@param FPS value of game FPS
**/
void GL::setFPS(double FPS){
    GL::FPS=FPS;
}

/**
 *	Gets the game FPS
 *
 *	@return value of game FPS
**/
double GL::getFPS(){
    return FPS;
}

/**
 *	Gets the delta time in milisecconds
 *
 *	@return delta time in milisecconds
**/
double GL::getMs(){
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
 *  Draw a black rectangle, used to draw collision boundaries
 *
 *  @param pos collision box(the coordinates of the rectangle)
**/
void GL::drawCollision(nTRectangle pos){
    drawCollision(pos,2,nTColor::Black());
}

/**
 *  Draw a black rectangle, used to draw collision boundaries
 *
 *  @param pos collision box(the coordinates of the rectangle)
**/
void GL::drawCollision(nTRectangle pos,float width,nTColor color){
    nTColor tmp=getColor();
    setColor(color);
    glLineWidth(width);
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
		R=0.6f;G=0.6f;B=0.6f;
	}else if(name=="violet"){   //mish 153,112,205 ou 99,70,CD
		R=0.6f;G=0.4392156f;B=0.80392156f;
	}else if(name=="white"){
		R=1;G=1;B=1;
	}else if(name=="mouseselected"){
		R=0.2f;G=0.2f;B=0.2f;
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


nTPoint GL::calcTextBoundaries(string text){
    nTFont *fnt=(nTFont*) fonts[currentFont];
    return fnt->calcBoundaries(text);
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
void GL::drawPolygon(nTPoint point,double radius,int edges){
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
 *  Draw and manage mouse events of a button
 *
 *  @param collision the coordinates of the button
 *  @param pressedColor the draw color of the button when has the mouse over it
 *  @param tex texture id of the button
 *  @param holdClick if false only compute one click
 *  @param clickFunction the function to be called when the button is left mouse clicked
 *  @param releaseFunction the function to be called when the left mouse clicked button is released
 *  @param RclickFunction the function to be called when the button is right mouse clicked
 *  @param RreleaseFunction the function to be called when the right mouse clicked button is released
 *  @return true if the button is being left mouse clicked
**/
bool GL::buttonBehave(nTRectangle collision,nTColor pressedColor,GLuint tex,bool holdClick,void(*clickFunction)(int,int),void(*releaseFunction)(int,int),void(*RclickFunction)(int,int),void(*RreleaseFunction)(int,int)){
    if(Util::isInsideBox(collision,GL::mousePos)){
        if(tex){
            GL::drawTexture(collision,pressedColor,tex);
        }else{
            GL::drawRectangle(collision,pressedColor);
        }
        if(GL::leftMouseClicked||GL::rightMouseClicked)
            AL::singleton->playSoundByName("mouse");
        if(GL::leftMouseClicked){
            if(*clickFunction!=NULL){
                clickFunction((int)GL::mousePos.x,(int)GL::mousePos.y);
            }
            if(!holdClick)GL::leftMouseClicked=0;
            return true;
        }else if(GL::leftMouseReleased){
            if(*releaseFunction!=NULL){
                 releaseFunction((int)GL::mousePos.x,(int)GL::mousePos.y);
            }
            GL::leftMouseReleased=0;
            return false;
        }
        if(GL::rightMouseClicked){
            if(*RclickFunction!=NULL){
                RclickFunction((int)GL::mousePos.x,(int)GL::mousePos.y);
            }
             if(!holdClick)GL::rightMouseClicked=0;
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
            GL::drawTexture(collision,tex);
        }
    }
    return false;
}

/**
 *  Draw and manage mouse events of a button
 *
 *  @param collision the coordinates of the button
 *  @param pressedColor the draw color of the button when has the mouse over it
 *  @param tex texture id of the button
 *  @return true if the button is being left mouse clicked
**/
bool GL::buttonBehave(nTRectangle collision,nTColor pressedColor,GLuint tex){
    return GL::buttonBehave(collision,pressedColor,tex,false,NULL,NULL,NULL,NULL);
}

/**
 *  Draw and manage mouse events of a button
 *
 *  @param collision the coordinates of the button
 *  @param pressedColor the draw color of the button when has the mouse over it
 *  @param text text of the button
 *  @param textColor the text color
 *  @param tex texture id of the button
 *  @param holdClick if false only compute one click
 *  @param clickFunction the function to be called when the button is left mouse clicked
 *  @param releaseFunction the function to be called when the left mouse clicked button is released
 *  @param RclickFunction the function to be called when the button is right mouse clicked
 *  @param RreleaseFunction the function to be called when the right mouse clicked button is released
 *  @return true if the button is being left mouse clicked
**/
bool GL::textButtonBehave(nTRectangle collision,nTColor pressedColor,string text,nTColor textColor,GLuint tex,bool holdClick,void(*clickFunction)(int,int),void(*releaseFunction)(int,int),void(*RclickFunction)(int,int),void(*RreleaseFunction)(int,int)){
    collision.p0.z-=0.00001;
    if(Util::isInsideBox(collision,GL::mousePos)){
        if(tex){
            GL::drawTexture(collision,pressedColor,tex);
        }else{
            GL::drawRectangle(collision,pressedColor);
        }
        if(GL::leftMouseClicked||GL::rightMouseClicked)
            AL::singleton->playSoundByName("mouse");
        if(GL::leftMouseClicked){
            if(*clickFunction!=NULL){
                clickFunction((int)GL::mousePos.x,(int)GL::mousePos.y);
            }
            if(!holdClick)GL::leftMouseClicked=0;
            return true;
        }else if(GL::leftMouseReleased){
            if(*releaseFunction!=NULL){
                 releaseFunction((int)GL::mousePos.x,(int)GL::mousePos.y);
            }
            GL::leftMouseReleased=0;
            return false;
        }
        if(GL::rightMouseClicked){
            if(*RclickFunction!=NULL){
                RclickFunction((int)GL::mousePos.x,(int)GL::mousePos.y);
            }
             if(!holdClick)GL::rightMouseClicked=0;
            return false;
        }else if(GL::rightMouseReleased){
            if(*RreleaseFunction!=NULL){
                 RreleaseFunction((int)GL::mousePos.x,(int)GL::mousePos.y);
            }
            GL::rightMouseReleased=0;
            return false;
        }
    }else{
        GL::drawTexture(collision,tex);
    }
    collision.p0.z+=0.00001;
    GL::drawCentered_MultilineX_Y_Text(nTPoint::get((collision.p0.x+collision.p1.x)/2,(collision.p0.y+collision.p1.y)/2+3,collision.p0.z),text,textColor);
    return false;
}

/**
 *  Draw and manage mouse events of a button
 *
 *  @param collision the coordinates of the button
 *  @param pressedColor the draw color of the button when has the mouse over it
 *  @param text text of the button
 *  @param tex texture id of the button
 *  @return true if the button is being left mouse clicked
**/
bool GL::textButtonBehave(nTRectangle collision,nTColor pressedColor,string text,nTColor textColor,GLuint tex){
    return GL::textButtonBehave(collision,pressedColor,text,textColor,tex,false,NULL,NULL,NULL,NULL);
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
                "Loaded Texutres("<<gl.textures.size()<<"),"<<"ScreenSize("<<"x:"<<gl.currentViewPort.p1.x<<" y:"<<gl.currentViewPort.p1.y<<"),"<<"Is paused("<<gl.isPaused<<"),"<<"]\n";
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
    if(AL::getSoundState())
        GL::buttonBehave(nTRectangle::get(100+Scenes::camera.x.movedCam,550+Scenes::camera.y.movedCam,140+Scenes::camera.x.movedCam,510+Scenes::camera.y.movedCam,0.99),nTColor::Black(),GL::getTextureByName("soundOn"),false,NULL,*modifySound,NULL,NULL);
    else
        GL::buttonBehave(nTRectangle::get(100+Scenes::camera.x.movedCam,550+Scenes::camera.y.movedCam,140+Scenes::camera.x.movedCam,510+Scenes::camera.y.movedCam,0.99),nTColor::Black(),GL::getTextureByName("soundOff"),false,NULL,*modifySound,NULL,NULL);

    if(AL::getMusicState())
        GL::buttonBehave(nTRectangle::get(40+Scenes::camera.x.movedCam,550+Scenes::camera.y.movedCam,80+Scenes::camera.x.movedCam,510+Scenes::camera.y.movedCam,0.99),nTColor::Black(),GL::getTextureByName("musicOn"),false,NULL,*modifyMusic,NULL,NULL);
    else
        GL::buttonBehave(nTRectangle::get(40+Scenes::camera.x.movedCam,550+Scenes::camera.y.movedCam,80+Scenes::camera.x.movedCam,510+Scenes::camera.y.movedCam,0.99),nTColor::Black(),GL::getTextureByName("musicOff"),false,NULL,*modifyMusic,NULL,NULL);

    GL::drawCenteredTexture(nTPoint::get(GL::defaultSize.x/2+Scenes::camera.x.movedCam,Scenes::camera.y.movedCam+GL::defaultSize.y/2,0.96),
                            nTPoint::get(200,60),GL::getTextureByName("Ballon"));

    GL::drawCentered_X_Y_Text(nTPoint::get(GL::defaultSize.x/2+Scenes::camera.x.movedCam,Scenes::camera.y.movedCam+GL::defaultSize.y/2,1),text,GL::getColorByName("violet"));
    if(GL::buttonBehave(nTRectangle::get(600+Scenes::camera.x.movedCam,600+Scenes::camera.y.movedCam,700+Scenes::camera.x.movedCam,550+Scenes::camera.y.movedCam,0.99),GL::getColorByName("mouseSelected"),GL::getTextureByName("quitIcon"))){
        if(Scenes::testGameMode){
            Scenes::current=Scenes::MapEdit;
            MapEdit::load(MapEdit::editingMap);
        }else
            Scenes::current=Scenes::menu;
    }
    Tutorials::draw(Tutorials::getSinucaTut());
}

/**
 *	Gets the 4X4 matrix for rotations
 *
 *	@param angle rotation angle in degrees
 *	@return vector of a vector of doubles containing the matrix
**/
vector<vector<double> > GL::getRotateMatrix(double angle){
    double rad=Util::angleToRad(angle);
    vector<vector<double> >out;
    out.resize(4,vector<double>(4,0));
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
 *	@return vector of a vector of doubles containing the matrix
**/
vector<vector<double> > GL::getTranslateMatrix(nTPoint point){
    vector<vector<double> >out;
    out.resize(4,vector<double>(4,0));
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
 *	@return vector of a vector of doubles containing the matrix
**/
nTPoint GL::rotatePoint(nTPoint point,nTPoint center, double angle){
    vector<vector<double> >Mpoint;
    Mpoint.resize(4,vector<double>(1,0));
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
    vector<vector<double> >Mpoint;
    vector<vector<double> >out;

    Mpoint.resize(4,vector<double>(1,0));
    out.resize(4,vector<double>(4,0));

    glGetFloatv(GL_MODELVIEW_MATRIX, matrixf);

    Mpoint[0][0]=point.x;
    Mpoint[1][0]=point.y;
    Mpoint[2][0]=point.z;
    Mpoint[3][0]=1;

    out[0][0]=(double)matrixf[0];
    out[0][1]=(double)matrixf[1];
    out[0][2]=(double)matrixf[2];
    out[0][3]=(double)matrixf[3];
    out[1][0]=(double)matrixf[4];
    out[1][1]=(double)matrixf[5];
    out[1][2]=(double)matrixf[6];
    out[1][3]=(double)matrixf[7];
    out[2][0]=(double)matrixf[8];
    out[2][1]=(double)matrixf[9];
    out[2][2]=(double)matrixf[10];
    out[2][3]=(double)matrixf[11];
    out[3][0]=(double)matrixf[12];
    out[3][1]=(double)matrixf[13];
    out[3][2]=(double)matrixf[14];
    out[3][3]=(double)matrixf[15];
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
    point.set(Scenes::camera.x.movedCam+42,Scenes::camera.y.movedCam+20,1);
    GL::drawRectangle(nTRectangle::get(Scenes::camera.x.movedCam,Scenes::camera.y.movedCam+8,Scenes::camera.x.movedCam+GL::defaultSize.x,Scenes::camera.y.movedCam+34,0.9),nTColor::get(0.8,0.7,0.9,0.7));
    for(int i=0;i<Player::getPlayerById(0)->life;i++){
        point.x+=25;
        nTRectangle r=nTRectangle::getCollision(point,nTPoint::get(20,20,1));
        r.p0.z=1;
        GL::drawTexture(r,GL::getTextureByName("S2"));
    }
    point.set(Scenes::camera.x.movedCam+10,Scenes::camera.y.movedCam+23,1);
    GL::drawCentered_Y_Text(point,"Vida:",nTColor::Black());
    point.set(Scenes::camera.x.movedCam+650,Scenes::camera.y.movedCam+23,1);
    snprintf(buffer,5,"%d",(int)GL::getGameMs()/1000);
    string strT(buffer);
    GL::drawCentered_Y_Text(point,"Tempo(s): "+strT,nTColor::Black());
    if(!Scenes::freeGameMode){
        point.set(Scenes::camera.x.movedCam+186,Scenes::camera.y.movedCam+23,1);
        snprintf(buffer,5,"%d",Player::lives);
        string strL(buffer);
        GL::drawCentered_Y_Text(point,"Vidas: "+strL,nTColor::Black());
        point.set(Scenes::camera.x.movedCam+280,Scenes::camera.y.movedCam+23,1);
        string strF;
        if(Player::stage==1){
            strF="Graduando";
        }else if(Player::stage==2){
            strF="Pos Grad";
        }else if(Player::stage==3){
            strF="Trabalho";
        }else if(Player::stage==4){
            strF="Bom Prof";
        }else if(Player::stage==5){
            strF="Mau Prof";
        }else{
            strF="Cefetinho";
        }
        GL::drawCentered_Y_Text(point,"Fase: "+strF,nTColor::Black());
        point.set(Scenes::camera.x.movedCam+480,Scenes::camera.y.movedCam+23,1);
        snprintf(buffer,5,"%.4f",Player::coeficiente);


        string strC(buffer);
        GL::drawCentered_Y_Text(point,"Coeficiente: "+strC,nTColor::Black());
    }
}

/**
 *	Gets the ammount of millisecconds passed since the start of the FPS count
 *
 *	@return the time in millisecconds
**/
double GL::getGameMs(){
    return GL::framesInGame*1000/GL::FPS;
}


void GL::clearUI(){
    editOnFocous=-1;
    edits.clear();
    editsNumeric.clear();
    editsText.clear();
    popupText="";
    popupMs=0;
    popupPaused=false;
}

string GL::getEditText(string editName){
    for(int i=0;i<edits.size();i++)
        if(editName==edits[i])
            return editsText[i];
    return "";
}

void GL::setEditText(string editName,string text){
    for(int i=0;i<edits.size();i++)
        if(edits[i]==editName){
            editsText[i]=text;
            editTextPosition=text.size();
            return;
        }
}

void GL::editTextBehave(nTRectangle collision,string font,string editName,bool numeric,bool setFocous){
    editTextBehave(collision,font,nTColor::get(0,0,0),"",editName,numeric,setFocous);
}

void GL::editTextBehave(nTRectangle collision,string font,string initial,string editName,bool numeric,bool setFocous){
    editTextBehave(collision,font,nTColor::get(0,0,0),initial,editName,numeric,setFocous);
}

void GL::editTextBehave(nTRectangle collision,string font,nTColor fontcolor,string editName,bool numeric,bool setFocous){
    editTextBehave(collision,font,fontcolor,"",editName,numeric,setFocous);
}

void GL::editTextBehave(nTRectangle collision,string font,nTColor fontcolor,string initial,string editName,bool numeric,bool setFocous){
    int editId=-1;
    for(int i=0;i<edits.size();i++){
        if(edits[i]==editName){
            if(editId<0)
                editId=i;
            else{
                if(Util::DEBUG) cout<<"ERROR: two edits with the same id("<<editName<<")\n\n\n\nERROR: two edits with the same id\n";
            }
        }
    }
    if(editId<0){
        editId=edits.size();
        edits.push_back(editName);
        editsNumeric.push_back(numeric);
        editsText.push_back(initial);
        if(setFocous)
            editOnFocous=editId;
    }

    if(GL::leftMouseClicked){
        if(Util::isInsideBox(collision,GL::mousePos)){
            AL::singleton->playSoundByName("mouse");
            GL::leftMouseClicked=0;
            editOnFocous=editId;
            editTextPosition=editsText[editOnFocous].size();
        }else{
            editOnFocous=-1;
        }
    }


    GL::drawRectangle(nTRectangle::get(collision.p0.x,collision.p0.y,collision.p1.x,collision.p1.y,collision.p0.z-0.001),nTColor::White());
    GL::drawRectangle(nTRectangle::get(collision.p0.x-1,collision.p0.y-1,collision.p1.x+1,collision.p1.y+1,collision.p0.z-0.002),nTColor::Black());

    nTPoint boxSize=nTPoint::get(ABS(collision.p1.x-collision.p0.x),ABS(collision.p1.y-collision.p0.y));
    int fontBKP=currentFont;
    GL::setFont(font);
    nTFont *fnt=(nTFont*) fonts[currentFont];
    string textToDraw=editsText[editId];
    int cursor=editTextPosition;
    if(cursor<0) cursor=textToDraw.size();
    if(cursor>textToDraw.size()) cursor=textToDraw.size();
    if(editOnFocous==editId){ //draw text plus | (blinking pipe)
        if(Util::timerWithInterval(500)){
            if(editCharCursor)
                textToDraw.insert(cursor,"|");
            else
                textToDraw.insert(cursor," ");
            editCharCursor=!editCharCursor;
        }
    }
    float lineWidth=fnt->calcBoundaries("_").x;
    nTPoint boundaries = fnt->calcBoundaries(textToDraw);
    if(boundaries.x>boxSize.x){
        int maxChars=floor((float)boxSize.x/(float)lineWidth);
        for(int i=maxChars-1;i<textToDraw.size();i+=maxChars){
            textToDraw.insert(i,"\n");
            cursor++;
        }
    }
    float lineHeight=fnt->calcBoundaries("|").y;
    boundaries = fnt->calcBoundaries(textToDraw);
    int maxLines=ABS(floor(boxSize.y/lineHeight));
    int lines=1;
    int centerLine=0;
    int last=0;
    vector<string> splited;
    for(int i=0;i<textToDraw.size();i++){
        if(i==cursor)
            centerLine=lines;
        if(textToDraw[i]=='\n'){
            splited.push_back(textToDraw.substr(last,i-last));
            last=i+1;
            lines++;
        }
    }
    splited.push_back(textToDraw.substr(last,textToDraw.size()-last));
    if(lines>=maxLines){
        textToDraw="";
        int newTextLines=0;
        int first=-1;
        for(int i=-maxLines/2;i<maxLines/2;i++){
            if(centerLine+i<splited.size()&&centerLine+i>=0){
                if(first<0)
                    first=centerLine+i;
                textToDraw+=splited[centerLine+i]+"\n";
                newTextLines++;
            }
        }
        if(newTextLines<maxLines){
            if(centerLine-maxLines/2<=0){
                for(int i=newTextLines;i<maxLines-1;i++){
                    textToDraw+=splited[i]+"\n";
                }
            }else{
                for(int i=1;i<=maxLines-newTextLines-1;i++){
                    textToDraw=splited[first-i]+"\n"+textToDraw;
                }
            }
        }
    }
    drawText(nTPoint::get(collision.p0.x+lineWidth/2,collision.p0.y-((float)lineHeight*1.1f),collision.p0.z),textToDraw,nTColor::Black());
    currentFont=fontBKP;
}

void GL::typeOnEdit(char c){
    if(edits.size()>0&&editOnFocous>=0){
        int cursor=editTextPosition;
        if(cursor<0) cursor=editsText[editOnFocous].size();
        if(cursor>editsText[editOnFocous].size()) cursor=editsText[editOnFocous].size();
        switch(c){
            case ' ':
                editsText[editOnFocous].insert(cursor," ");
                editTextPosition++;
                break;

            case 13: // enter
                editsText[editOnFocous].insert(cursor,"\n");
                editTextPosition++;
                break;

            case 8: // backspace
                if(cursor!=0){
                    editsText[editOnFocous].erase(editsText[editOnFocous].begin()+cursor-1);
                    editTextPosition--;
                }
                break;

            case 9: // tab
                editOnFocous++;
                if(editOnFocous>=edits.size()||editOnFocous<0)
                    editOnFocous=0;
                break;

            case 127: // delete
                if(cursor!=editsText[editOnFocous].size())
                    editsText[editOnFocous].erase(editsText[editOnFocous].begin()+cursor);
                break;

            default:
                if((editsNumeric[editOnFocous]&&isdigit(c))||(!editsNumeric[editOnFocous]&&isgraph(c))){
                    stringstream ss;
                    string s;
                    ss<<c;
                    ss>>s;
                    editsText[editOnFocous].insert(cursor,s);
                    editTextPosition++;
                }
                break;
        }
    }
}

bool GL::hasEditOnFocous(){
    return edits.size()>0&&editOnFocous>=0;
}

void GL::moveEditCursor(int direction){
    if(edits.size()>0&&editOnFocous>=0){
        if(direction==Util::direction_left){
            editTextPosition--;
            if(editTextPosition<0)
                editTextPosition=0;
        }else if(direction==Util::direction_right){
            editTextPosition++;
            if(editTextPosition>editsText[editOnFocous].size())
                editTextPosition=editsText[editOnFocous].size();
        }
    }
}

void GL::setFocous(string editName){
    for(int i=0;i<edits.size();i++)
        if(editName==edits[i]){
            editOnFocous=i;
            return;
        }
}

void GL::popupBoxBehave(string text,string font,int duration){
    popupText=text;
    popupFont=font;
    popupMs=-GL::getGameMs();
    popupDuration=duration;
    popupPaused=false;
}

void GL::drawPopupBox(){
    if(popupText!=""){
        if(!popupPaused&&isPaused){
            popupPaused=true;
        }
        isPaused=true;
        int fontBKP=currentFont;
        GL::setFont(popupFont);
        nTFont *fnt=(nTFont*) fonts[currentFont];
        nTPoint titleSize=fnt->calcBoundaries("Pop-up Box");
        nTPoint txtSize=fnt->calcBoundaries(popupText);
        if(txtSize.x<titleSize.x)
            titleSize.x=txtSize.x;
        nTPoint size=nTPoint::get(txtSize.x*1.2,txtSize.y*1.2+titleSize.x*1.6+20,0.98);
        nTRectangle collision=nTRectangle::getCollision(nTPoint::get(GL::defaultSize.x/2+Scenes::camera.x.movedCam,GL::defaultSize.y/2+Scenes::camera.y.movedCam,0.98),size);
        GL::drawRectangle(collision,nTColor::get(1,0.9,0.9));
        GL::drawText(nTPoint::get(collision.p0.x+20,collision.p1.y+20,0.98),"Pop-up Box",nTColor::get(0,0,0));
        GL::drawCentered_MultilineX_Y_Text(nTPoint::get(GL::defaultSize.x/2+Scenes::camera.x.movedCam,GL::defaultSize.y/2+10+titleSize.y+Scenes::camera.y.movedCam,0.98),popupText,nTColor::get(0,0,0));

        if((popupMs+GL::getGameMs()>=popupDuration&&popupDuration)||
            textButtonBehave(nTRectangle::getCollision(nTPoint::get(GL::defaultSize.x/2+Scenes::camera.x.movedCam,collision.p0.y-22+Scenes::camera.y.movedCam,0.99),nTPoint::get(40,20,0.99)),GL::getColorByName("mouseSelected"),"Ok",nTColor::get(0,0,0),GL::getTextureByName("btnSkin1"))){
            popupText="";
            isPaused=false;
        }
        currentFont=fontBKP;
    }
}
