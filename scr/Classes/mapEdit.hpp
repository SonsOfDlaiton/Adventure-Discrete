#ifndef MAPEDIT_HPP
#define MAPEDIT_HPP
#include "Util.hpp"
#include "GL.hpp"
#include "Blocks.hpp"

class mapEdit {
public:
    mapEdit();
    mapEdit(const mapEdit& orig);
    virtual ~mapEdit();

    static void draw();
    static void save();
    static void reset();
    static void askForSize();
    static void askForLoad();
    static void setMapSize();
    static void load(int idx);
    static void definePages();

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
    static void drawPanel();
    static void drawLines();
    static string getMoreInfoAboutBlocks(int block);
};

#endif /* MAPEDIT_HPP */

