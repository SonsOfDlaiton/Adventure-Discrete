#ifndef MAP_HPP
#define MAP_HPP
#include "Util.hpp"
#include "GL.hpp"
#include "Background.hpp"

class Blocks;

class Map {
public:
    Map();
    Map(const Map& orig);
    virtual ~Map();

    static void refresh();
    static void next();
    static void GG(bool isWinner);
    static vector<mapCollision> checkCollision(nTPoint pos,nTPoint size);
    static bool loadMap(string path);
    static bool saveMap(string path,int idx);
    static Blocks* getBlockById(int id);
    static void changeCurrentMap(nTMap map);
    static void transparencyLayerDraw();

    static int nOfMaps;
    static nTPoint size;
    static int nOfEnemys;
    static int nOfBackgrounds;
    static int totalPowerUps;
    static int expetedTime;
    static vector<void*> staticBlocks;
    static vector<void*> dynamicBlocks;
    static vector<nTMap> maps;
    static nTMap usrMap;
private:
    friend class GL;

    static void draw();
    static void deleteAllBlocks();
    static void setBlockPos();
    static int getIdByPosition(nTPoint pos);
    static void drawMapBackground();

    static nTMap actualMap;//static GLuint background;
};
#endif /* MAP_HPP */
