#include "Util.hpp"
#include "Entity.hpp"
#ifndef BOSSEVENT
#define BOSSEVENT
typedef struct bossevent{
    string event;
    vector <string>params;
    int probability;
    float minimumLife;
}BossEvent;
#endif // BOSSEVENT

#ifndef BOSS_H
#define BOSS_H

class Boss:public Entity {
public:
    Boss(string data,nTPoint spawn);
    Boss(const Boss& orig);
    virtual ~Boss();

    static void behave();
    static int getSpritesId(string name);

    string nickname;
    double imunityTime;
    vector<BossEvent> events;
    static vector<string>bossName;
    static vector<vector<string> >bossAnim;
    static vector<vector<int> >bossAnimSize;

private:
    void draw();
    void stateControl();
    void makeInvencible();
};

#endif /* BOSS_H */
