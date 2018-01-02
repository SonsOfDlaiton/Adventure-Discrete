#ifndef PLAYER_H
#define PLAYER_H
#include "Util.hpp"
#include "Entity.hpp"
#include "GL.hpp"
#include "Mechanics.hpp"
#include "Enemy.hpp"
#include "Bullet.hpp"
#include "Blocks.hpp"
#include "IOHandler.hpp"
#include "Camera.hpp"


class Player:public Entity {
public:
    Player(double life,nTPoint spawn,nTPoint size);
    Player(const Player& orig);
    virtual ~Player();

    static void setSprites();
    static void behave();
    void makeInvencible(double time);
    void atack(int type);
    void spawn(nTPoint spawn,double life);
    nTPoint getGroundPos();
    static Player* getPlayerById(int id);

    bool god;
    int sword;
    bool canTp;
    bool canWalk;
    bool spacePressed;
    int atackDirection;
    static bool beatGame;
    static int stage;
    static int lives;
    static int checkpoint;
    static int enemysKilled;
    static int PowerUpsActiveted;
    static int loadedLife;
    static double coeficiente;
    static const int defaultLife;
    static double globalCoeficiente;
    static const int ranged;
    static const int melee;
    static const int meleeProjectile;
    static const nTPoint defaultPSize;
    static vector<vector<string> >playerAnim;
    static vector<vector<int> >playerAnimSize;
private:
    friend class Map;
    friend class Bullet;

    void stateControl();
    void especificDraw();
    static void refreshCoeficiente();
    static void refreshGlobalcoeficiente();

    bool haveBulletSword;
    bool haveBulletSpec;
    bool alReadyAtacked;
    nTPoint swordSize;
    nTPoint swordPos;
    nTRectangle swordCollision;
    static const double swordBaseDamage;
    static const double imunityTime;
    friend ostream& operator<<(ostream &strm, const Player &player);
};

#endif /* PLAYER_H */
