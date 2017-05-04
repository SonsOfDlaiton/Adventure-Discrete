#ifndef BLOCKS_HPP
#define BLOCKS_HPP
#include "Util.hpp"
#include "Scenes.hpp"
#include "AL.hpp"
#include "Player.hpp"

class GL;

class Blocks {
public:
    Blocks(int type,nTPoint pos,nTPoint size);
    Blocks(const Blocks& orig);
    virtual ~Blocks();

    void draw();
    static bool checkIfBlocksIsDynamic(int type);
    static bool checkIfBlocksIsMassive(int type);
    static bool checkIfBlocksIsLiquid(int type);
    static bool checkIfBlocksIsFilled(int type);
    static string getTexNameByIndex(int BlockType);

    int type;
    nTPoint pos;
    int id;
    bool isVisible;
    int brokeStage;
    static nTPoint defaultBlockSize;
private:
    friend class Map;

    GLuint getTextureByIndex();
    void applyDamage(float damage);
    void move(int dir,float steeps);

    float moveSpeed;
    unsigned long int timeToVunerability;
    bool damageState;
    static float imunityTime;
    nTPoint size;
    GLuint tex;
    nTColor color;
};

#endif /* BLOCKS_HPP */

