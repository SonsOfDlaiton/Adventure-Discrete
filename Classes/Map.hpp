#ifndef MAP_HPP
#define MAP_HPP
#include "Util.hpp"
#include "GL.hpp"

class Map {
public:
    Map();
    Map(const Map& orig);
    virtual ~Map();
    static void changeCurrentMap(nTMap map); 
    static vector<mapCollision> checkCollision(nTPoint pos,nTPoint size);
    static void draw();
    static void deleteAllBlocks();
    static void setBlockPos();
    static void refresh();
    static void GG(bool isWinner);
    static void next();
    static void createDynamicGLlist();
    static void createStaticGLlist();
    static int getBlockReference(nTPoint pos);
    static int getIdByPosition(nTPoint pos);
    static vector<vector<int> > currentMap;
    static nTPoint size;
    static int nOfMaps;
    static vector<void*> staticBlocks;
    static vector<void*> dynamicBlocks;
    static int nOfEnemys;
    static int totalPowerUps;
    static int expetedTime;
    static int nOfBackgrounds;
private:
    friend class GL;
    static int dynamicGLlist;
    static int staticGLlist;
    static GLuint background;
};

#endif /* MAP_HPP */

