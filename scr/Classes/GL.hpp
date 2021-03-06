#ifndef GL_H
#define GL_H
#ifdef __EMSCRIPTEN__
    #include <GL/glut.h>
#else
#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/freeglut.h>
#endif
#endif
#include <time.h>
#include "../Libs/soil/SOIL.h"
#include "../Libs/clip/clip.h"
#ifdef _WIN32
  #include "../Libs/clip/clip_win.h"
#elif __APPLE__
  #include "../Libs/clip/clip_osx.h"
#else
  #include "../Libs/clip/clip_none.h"
#endif
#include "Util.hpp"
#include "Map.hpp"
#include "nTFont.hpp"

class AssetsLoader;

class GL {
public:
    GL(string name,double FPS,GLbitfield displayMode,int windowXsize,int windowYsize,nTColor clearcolor,bool blend,vector<GLenum>&enables,int argc, char** argv);
    GL(const GL& orig);
    virtual ~GL();

    void start();
    static void setFPS(double FPS);
    static double getFPS();
    static double getMs();
    static void setColor(nTColor color);
    static nTColor getColor();
    static bool loadFont(string path);
    static GLuint loadTexture(string name,string path);
    static vector<GLuint> loadTextures(string name,int nOfTex,string path);
    static void drawRectangle(nTRectangle pos,nTColor color);
    static void drawCollision(nTRectangle pos);
    static void drawCollision(nTRectangle pos,float width,nTColor color);
    static void drawCollisionLine(nTPoint p0,nTPoint p1);
    static void drawTexture(nTRectangle pos,nTColor color,GLuint tex,int Orientation);
    static void drawCenteredTexture(nTPoint pos,nTPoint size,nTColor color,GLuint tex,int Orientation);
    static void drawTexture(nTRectangle pos,GLuint tex,int Orientation);
    static void drawCenteredTexture(nTPoint pos,nTPoint size,GLuint tex,int Orientation);
    static void drawTexture(nTRectangle pos,nTColor color,GLuint tex);
    static void drawCenteredTexture(nTPoint pos,nTPoint size,nTColor color,GLuint tex);
    static void drawTexture(nTRectangle pos,GLuint tex);
    static void drawCenteredTexture(nTPoint pos,nTPoint size,GLuint tex);
    static nTColor getColorByName(string name);
    static void setFont(string name,int size);
    static void setFont(string name);
    static nTPoint calcTextBoundaries(string text);
    static void drawText(nTPoint point,string text,nTColor color);
    static void drawCentered_X_Text(nTPoint point, string text, nTColor color);
    static void drawCentered_Y_Text(nTPoint point, string text, nTColor color);
    static void drawCentered_X_Y_Text(nTPoint point, string text, nTColor color);
    static void drawCentered_MultilineX_Text(nTPoint point, string text, nTColor color);
    static void drawCentered_MultilineX_Y_Text(nTPoint point, string text, nTColor color);
    static void drawPolygon(nTPoint point,double radius,int edges);
    static GLuint getTextureByName(string name);
    static vector<GLuint> getTexturesByName(string name,int nOfTex);
    static void drawPause();
    static void drawHUD();
    static double getGameMs();
    static void clearUI();
    static bool hasEditOnfocus();
    static bool hasTexture(string texname);
    static void setfocus(string editName);
    static void typeOnEdit(char c);
    static void moveEditCursor(int direction);
    static void popupBoxBehave(string text,string font,int duration);
    static void drawPopupBox();
    static void editTextBehave(nTRectangle collision,string font,nTColor fontcolor,string initial,string editName,bool numeric,bool setfocus);
    static void editTextBehave(nTRectangle collision,string font,nTColor fontcolor,string editName,bool numeric,bool setfocus);
    static void editTextBehave(nTRectangle collision,string font,string initial,string editName,bool numeric,bool setfocus);
    static void editTextBehave(nTRectangle collision,string font,string editName,bool numeric,bool setfocus);
    static string getEditText(string editName);
    static void setEditText(string editName,string text);
    static bool buttonBehave(nTRectangle collision,nTColor focusColor,GLuint tex,bool holdClick,void(*clickFunction)(int,int),void(*releaseFunction)(int,int),void(*RclickFunction)(int,int),void(*RreleaseFunction)(int,int));
    static bool buttonBehave(nTRectangle collision,nTColor focusColor,GLuint tex);
    static bool textButtonBehave(nTRectangle collision,nTColor focusColor,string text,nTColor textColor,GLuint tex,bool holdClick,void(*clickFunction)(int,int),void(*releaseFunction)(int,int),void(*RclickFunction)(int,int),void(*RreleaseFunction)(int,int));
    static bool textButtonBehave(nTRectangle collision,nTColor focusColor,string text,nTColor textColor,GLuint tex);
    static vector<vector<double> > getRotateMatrix(double angle);
    static vector<vector<double> > getTranslateMatrix(nTPoint point);
    static nTPoint getModelViewPoint(nTPoint point);
    static nTPoint rotatePoint(nTPoint point,nTPoint center,double angle);

    static bool isPaused;
    static nTPoint defaultSize;
    static nTRectangle currentViewPort;
    static bool leftMouseClicked;
    static bool leftMouseReleased;
    static bool rightMouseClicked;
    static bool rightMouseReleased;
    static bool FPSchecker;
    static int currentFPS;
    static nTPoint mousePos;
    static nTPoint rawMousePos;
    static nTColor clearColor;
    static unsigned long int framesInGame;
private:
    static double FPS;
    static nTColor currentColor;
    static int currentFont;
    static vector<string> textureNames;
    static vector<GLuint> textures;
    static vector<void*> fonts;
    static bool editCharCursor;
    static int editOnfocus;
    static int editTextPosition;
    static vector<string> edits;
    static vector<bool> editsNumeric;
    static vector<string> editsText;
    static string popupText;
    static string popupFont;
    static double popupMs;
    static bool popupPaused;
    static int popupDuration;
    friend ostream& operator<<(ostream &strm, const GL &gl);
};
void drawScene(void);
void reshape(int width, int height);
void keyboard(unsigned char key, int x, int y);
void update(int n);
void mousePress(int button,int state,int x,int y);
void mousePassiveMotion(int x,int y);
void mouseActiveMotion(int x,int y);
void modifySound(int a,int b);
void modifyMusic(int a,int b);
#endif /* GL_H */
