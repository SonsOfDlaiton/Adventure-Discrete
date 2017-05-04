#include "Globals.h"
vector<nTMap> maps;
nTMap usrMap;

Mechanics *world=new Mechanics();
Player *player;

nTColor clearColor;

vector<vector<string> >playerAnim;
vector<vector<int> >playerAnimSize;
vector<vector<string> >enemyAnim;
vector<vector<int> >enemyAnimSize;

AL *al;

Tutorials *zero=new Tutorials("",0);

int languageIdx=0;