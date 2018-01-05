#include "Util.hpp"
#include "Entity.hpp"
#ifndef BOSSEVENT
#define BOSSEVENT
typedef struct bossevent{
    string event;
    vector <string>params;
    int probability;
    float minimumLife;
    float maximumLife;
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
    static void setSprites();
    static void rightAnswer();
    static void wrongAnswer();
    static void fallGate(float xpos);
    void applyDamage(double damage);
    double startLife;

    static vector<string>bossName;
    static vector<vector<string> >bossAnim;
    static vector<vector<int> >bossAnimSize;
    static vector<void*> enSummoned;
    static vector<void*> buSummoned;
private:
    string nickname;
    vector<BossEvent> events;
    static void draw(Boss* bo);
    void stateControl();
    void eventHandler();
    void makeInvencible();
    void disableCasts();
    bool canCast;
    double castMs;

    void reincarnation(vector<string> params, int& eid);
    void summon(vector<string> params, int& eid);
    void shield(vector<string> params, int& eid); Texture* shieldTex; float shieldBlock; double shieldMs;
    void questions(vector<string> params, int& eid); bool alreadyHaveQuestions; float questionsDmg; float questionsHex;
    void gate(vector<string> params, int& eid); float damageGate;
};

#endif /* BOSS_H */
