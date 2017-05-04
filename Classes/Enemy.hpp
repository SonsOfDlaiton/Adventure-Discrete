#ifndef ENEMY_HPP
#define ENEMY_HPP
//#include "Util.hpp"
#include "Mechanics.hpp"
#include "Entity.hpp"
#include "Player.hpp"

class Enemy:public Entity {
public:
    Enemy(int enemyType,float life,nTPoint spawn,nTPoint size,vector<vector<GLuint> > animations,bool isHuman);
    Enemy(const Enemy& orig);
    virtual ~Enemy();
    void stateControl();
    static void behave();
    void makeInvencible(float time);
    static nTPoint defaultSize;
    static nTPoint bossSize;
    static int bossLife;
    static int defaultLife;
    string nickname;
    static void registerNicks();
    GLuint lifeLetter();
    static vector<vector<string> >nicks;
    int type;
private:
    static float imunityTime;
};

#endif /* ENEMY_HPP */

