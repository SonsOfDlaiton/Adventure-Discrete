#ifndef MAPEDIT_HPP
#define MAPEDIT_HPP
#include "Util.hpp"
#include "GL.hpp"
#include "Blocks.hpp"
#include "Camera.hpp"
#include "Background.hpp"

class MapEdit {
public:
    MapEdit();
    MapEdit(const MapEdit& orig);
    virtual ~MapEdit();

    static void draw();
    static bool save();
    static void reset();
    static void askForSize();
    static void askForLoad();
    static void setMapSize();
    static void load(int idx);
    static void definePages();

    static bool mapSelected;
    static bool isUser;
    static nTPoint scale;
    static nTPoint HUDarea;
    static nTPoint size;
    static int currentBlock;
    static int index;
    static Background currentBackground;
    static nTMap map;
    static short int isCreating;
    static vector<vector<vector<int> > >blockPages;
    static vector<string> pageNames;
    static int pageIndex;
    static const int editingMap;
    static int editingLayer;
    static bool layersToDraw[nTMap::layers];
private:
    static void drawPanel();
    static void drawLines();
    static string getMoreInfoAboutBlocks(int block);
};

#endif /* MAPEDIT_HPP */

