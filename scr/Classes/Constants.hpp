#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "Util.hpp"

class Constants {
public:
    static const double BLOCKS_ImunityTime;
    static const double BLOCKS_BaseDamage;
    static const double BLOCKS_SizeX;
    static const double BLOCKS_SizeY;
    static const double BLOCKS_Depth;

    static const int GAME_FPS;

    static const double BOSS_BaseDamage;

    static const double BULLET_BaseDamage;
    static const double BULLET_BaseBusDamage;
    static const double BULLET_BaseSpeed;
    static const double BULLET_BaseTimeToShoot;

    static const double ENEMY_SizeX;
    static const double ENEMY_SizeY;
    static const int ENEMY_BaseLife;
    static const double ENEMY_BaseDamage;

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
    static const double PLAYER_SizeX;
    static const double PLAYER_SizeY;

    static const double POWERUP_MoveSpeed;

    static const double TEXTURE_DefaultTimeToNext;
};

#endif /* CONSTANTS_H */
