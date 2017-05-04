#ifndef BLOCKS_HPP
#define BLOCKS_HPP
#include "Util.hpp"
#include "GL.hpp"
#include "Map.hpp"
#include "Scenes.hpp"

class Blocks {
public:
    Blocks(int type,nTPoint pos,nTPoint size);
    Blocks(const Blocks& orig);
    virtual ~Blocks();
    GLuint getTextureByIndex();
    static string getTexNameByIndex(int BlockType);
    void applyDamage(float damage);
    void draw();
    void move(int dir,float steeps);
    float moveSpeed;
    unsigned long int timeToVunerability;
    static nTPoint defaultBlockSize;
    static bool checkIfBlocksIsDynamic(int id);
    static bool checkIfBlocksIsMassive(int id);
    int type;
    int brokeStage;
    bool damageState;
    static float imunityTime;
    nTPoint pos;
    bool isVisible;
private:
    friend class Map;
    nTPoint size;
    GLuint tex;
};

#endif /* BLOCKS_HPP */

