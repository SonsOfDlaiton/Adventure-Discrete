#ifndef GLOBALS_H
#define GLOBALS_H
#include "../Classes/Util.hpp"
#include "../Classes/Player.hpp"
#include "../Classes/AL.hpp"
#include "../Classes/GL.hpp"
#include "../Classes/Tutorials.hpp"
extern vector<nTMap> maps;
extern nTMap usrMap;

extern Player *player;
extern Mechanics *world;

extern nTColor clearColor;

extern vector<vector<string> >playerAnim;
extern vector<vector<int> >playerAnimSize;
extern vector<vector<string> >enemyAnim;
extern vector<vector<int> >enemyAnimSize;

extern AL *al;
extern GL *gl;

extern Tutorials *zero;

extern int languageIdx;

#endif	// GLOBALS_H
