#ifndef BLOCKS_HPP
#define BLOCKS_HPP
#include "Util.hpp"
#include "Scenes.hpp"
#include "AL.hpp"
#include "Player.hpp"

class GL;

class Blocks:public Mechanics {
public:
    Blocks(int type,nTPoint pos,nTPoint size,string data);
    Blocks(int type,nTPoint pos,nTPoint size);
    Blocks(const Blocks& orig);
    virtual ~Blocks();

    void draw();
    static bool checkIfBlocksIsDynamic(int type);
    static bool checkIfBlocksIsMassive(int type);
    static bool checkIfBlocksIsLiquid(int type);
    static bool checkIfBlocksIsFilled(int type);
    static bool checkIfBlocksIsTeleportDoor(int type);
    static bool checkIfBlocksIsTeleportPipe(int type);
    static bool checkIfBlocksIsAir(int type);
    static bool checkIfBlocksIsSolid(int type);
    static bool checkIfBlocksIsShooter(int type);
    static bool checkIfBlocksIsHalfBlockH(int type);
    static bool checkIfBlocksIsHalfBlockV(int type);
    static bool checkIfBlocksIsPowerUpBlock(int type);
    static bool checkIfBlocksIsDestrutive(int type);
    static bool checkIfBlocksIsNoCollision(int type);
    static bool checkIfBlocksIsDecorative(int type);
    static bool checkIfBlocksIsLever(int type);
    static bool checkIfBlocksIsPowerUpChest(int type);
    static bool checkIfBlocksIsJumpBoost(int type);
    static bool checkIfBlocksIsEnemyCollider(int type);
    static bool checkIfBlocksIsTutorial(int type);
    static bool checkIfBlocksIsTutorialPause(int type);
    static bool checkIfBlocksIsEndLevel(int type);
    static bool checkIfBlocksIsPlayerSpawn(int type);
    static bool checkIfBlocksIsEnemySpawn(int type);
    static bool checkIfBlocksIsCheckpoint(int type);
    static bool checkIfBlocksIsBossSpawn(int type);
    static int getCheckPointId(int checkType);
    static int getPowerUpChestId(int puType);
    static int getPowerUpBlockId(int puType);
    static string getTexNameByIndex(int BlockType);

    int type;
    int id;
    bool isVisible;
    int brokeStage;
    string data;
    static const int AnimatedWater1;
    static const int AnimatedWater2;
    static const int AnimatedLava1;
    static const int AnimatedLava2;
    static const int StaticLava;
    static const int StaticWater;
    static const int HalfBlockVCactus;
    static const int GlassBlock;
    static const int EndLevelBlock;
    static const int BusShooterBlock;
    static const int InvalidPowerUpBlock;
    static const int InvalidPowerUpChest;
    static const int IceBlock;
    static const int IceHHalfBlock;
    static const int IceVHalfBlock;
    static const int TutorialBlock;
    static const int TutorialPauseBlock;
    static nTPoint defaultBlockSize;
private:
    friend class Map;

    GLuint getTextureByIndex();
    void applyDamage(double damage);
    void move(int dir,double steeps);

    double moveSpeed;
    unsigned long int timeToVunerability;
    bool damageState;
    static double imunityTime;
    GLuint tex;
    nTColor color;
};

#endif /* BLOCKS_HPP */

