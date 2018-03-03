#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "Util.hpp"

class Constants {
public:
    static const double BLOCKS_ImunityTime;
    static const nTPoint BLOCKS_Size;

    static const double BULLET_BaseDamage;
    static const double BULLET_BaseSpeed;
    static const double BULLET_BaseTimeToShoot;

    static const nTPoint ENEMY_Size;
    static const int ENEMY_BaseLife;

    static const double ENTITY_JumpSpeed;
    static const double ENTITY_HorizontalRecoil;
    static const double ENTITY_VerticalRecoil;
    static const double ENTITY_WalkSpeed;
    static const double ENTITY_RunAcceleration;
    static const double ENTITY_MaxRunSpeed;
    static const double ENTITY_ImunityTime;

    static const int MAP_AmountOfBackgrounds;
    static const int MAP_AmountOfUserMaps;
    static const int MAP_AmountOfMaps;

    static const double MECHANICS_Gravity;

    static const double PLAYER_SwordBaseDamage;
    static const int PLAYER_BaseLife;
    static const nTPoint PLAYER_Size;

    static const double POWERUP_MoveSpeed;

    static const double TEXTURE_DefaultTimeToNext;
};

#endif /* CONSTANTS_H */
