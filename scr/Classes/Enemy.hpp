#ifndef ENEMY_HPP
#define ENEMY_HPP
#include "Util.hpp"
#include "Scenes.hpp"
#include "Entity.hpp"
#include "Bullet.hpp"
#include "Camera.hpp"

class Enemy:public Entity {
public:
    Enemy(int enemyType,double life,nTPoint spawn,nTPoint size,vector<vector<GLuint> > animations);
    Enemy(const Enemy& orig);
    virtual ~Enemy();

    static void behave();
    GLuint lifeLetter();
    static void registerNicks();

    int type;

    static const int defaultLife;
    static const nTPoint defaultSize;
    static const double imunityTime;
    static vector<vector<string> >enemyAnim;
    static vector<vector<int> >enemyAnimSize;
private:
    static void draw(Enemy* en);
    void stateControl();
    void makeInvencible();
    bool drawLetter;
    string nickname;
    static vector<vector<string> >nicks;
};

#endif /* ENEMY_HPP */

