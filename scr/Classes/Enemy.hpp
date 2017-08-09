#ifndef ENEMY_HPP
#define ENEMY_HPP
#include "Util.hpp"
#include "Scenes.hpp"
#include "Entity.hpp"
#include "Bullet.hpp"
#include "Camera.hpp"

class Enemy:public Entity {
public:
    Enemy(int enemyType,float life,nTPoint spawn,nTPoint size,vector<vector<GLuint> > animations,bool isHuman);
    Enemy(const Enemy& orig);
    virtual ~Enemy();

    static void behave();
    GLuint lifeLetter();
    static void registerNicks();

    int type;
    static const int defaultLife;
    static const int bossLife;
    static const nTPoint defaultSize;
    static const nTPoint bossSize;
    static const float imunityTime;
    static vector<vector<string> >enemyAnim;
    static vector<vector<int> >enemyAnimSize;
private:
    void stateControl();
    void makeInvencible(float time);

    string nickname;
    static vector<vector<string> >nicks;
};

#endif /* ENEMY_HPP */

