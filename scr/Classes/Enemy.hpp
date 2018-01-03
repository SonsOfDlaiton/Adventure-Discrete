#ifndef ENEMY_HPP
#define ENEMY_HPP
#include "Util.hpp"
#include "Scenes.hpp"
#include "Entity.hpp"
#include "Bullet.hpp"
#include "Camera.hpp"

class Enemy:public Entity {
public:
    Enemy(int enemyType,double life,nTPoint spawn,nTPoint size,string spr_name);
    Enemy(const Enemy& orig);
    virtual ~Enemy();

    static void behave();
    GLuint lifeLetter();
    static void registerNicks();
    static void setSprites();

    int type;

    static const int defaultLife;
    static const nTPoint defaultSize;
    static int getSpritesId(string name);
    static vector<string>enemyName;
    static vector<vector<string> >enemyAnim;
    static vector<vector<int> >enemyAnimSize;

    bool drawLetter;
    string nickname;
private:
    static void draw(Enemy* en);
    void stateControl();
    static vector<vector<string> >nicks;
};

#endif /* ENEMY_HPP */

