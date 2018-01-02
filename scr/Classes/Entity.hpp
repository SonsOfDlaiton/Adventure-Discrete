#ifndef ENTITY_H
#define ENTITY_H
#include "Util.hpp"
#include "GL.hpp"
#include "Mechanics.hpp"

class Entity:public Mechanics{
public:
    Entity();
    Entity(const Entity& orig);
    virtual ~Entity();

    virtual void draw(nTColor color);
    virtual void draw();
    virtual void applyDamage(double damage);
    virtual void jump();
    virtual void walk(int dir);
    virtual void run(int dir);
    static void behave();
    static vector<vector<GLuint> >  getAnimationVector(vector<string> animations, vector<int>size);
    static void setSprites();
	static double getSpriteMs();

    int id;
    bool imuneToDamage;
    bool damageState;
    int atacking;
    bool canJump;
    double life;
    bool itsInTheWater;
    bool lowered;
    bool reducing;
    int orientation;
    static const double walkSpeed;
    static const double imunityTime;
	static vector<void*> enemys;
	static vector<void*> bosses;
    static const int state_Idle,state_Walking,state_Running,state_Jumping,state_Atacking,state_SpecialAtacking,state_TakingDamage,state_Dying,state_Spawning,state_Lowering,state_Holding,state_MeleeAirAtackingDown,state_MeleeAirAtackingUp,state_MeleeAirAtackingHorizontal,state_MeleeGroundAtackingUp,state_MeleeGroundAtackingHorizontal,state_RangedAirAtackingDown,state_RangedAirAtackingUp,state_RangedAirAtackingHorizontal,state_RangedGroundAtackingUp,state_RangedGroundAtackingHorizontal;
private:
    static const double SpritesPerSecond;
protected:
    friend class Mechanics;
    friend void playerChangeDamageState(int id);

    virtual void stateControl();
    virtual void execAnimation();
    virtual void reduceSpeed(int dir);
    void setAnimations(vector<vector<GLuint> > animations);
    virtual void especificDraw();
    virtual void makeInvencible();
    virtual void makeInvencible(double time);

    GLuint currentTex;
    int currentIndex;
    int currentState;
    int nextState;
    bool defaultOrientation;
    bool isHuman;
    bool isVisible;
    unsigned long timeToVunerability;
    vector<vector<GLuint> > animations;
    static const double jumpSpeed;
    static const double verticalRecoil;
    static const double horizontalRecoil;
    static const double runAceleration;
    static const double maxRunSpeed;
    static vector<void*> players;
};

#endif /* ENTITY_H */
