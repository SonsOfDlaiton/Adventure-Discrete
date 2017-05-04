#ifndef MAPEDIT_HPP
#define MAPEDIT_HPP
#include "Util.hpp"
#include "GL.hpp"
#include "Blocks.hpp"
#include "../Libs/loadFiles.h"

class mapEdit {
public:
    mapEdit();
    mapEdit(const mapEdit& orig);
    virtual ~mapEdit();
    static void drawPanel();
    static void drawLines();
    static void draw();
    static void askForSize();
    static void askForLoad();
    static void reset();
    static void setMapSize();
    static void save();
    static void load(int idx);
    static void definePages();
    static string getMoreInfoAboutBlocks(int block);
    static bool isUser;
    static string input;
    static string blankField;
    static nTPoint scale;
    static nTPoint HUDarea;
    static nTPoint size;
    static int currentBlock;
    static int currentBackground;
    static int index;
    static vector<vector<int> > map;
    static short int isCreating;
    static vector<vector<vector<int> > >blockPages;
    static vector<string> pageNames;
    static int pageIndex;
private:
    static GLuint GLlist;
};

#endif /* MAPEDIT_HPP */

