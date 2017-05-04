#ifndef LOADFILES_H
#define LOADFILES_H
#include "Globals.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "../Classes/Util.hpp"
#include "../Classes/AL.hpp"
#include "../Classes/GL.hpp"

typedef struct settings{
  bool sound=true,music=true;
  int lives=0,checkpoint=0,currentStage=0,sword=0;
  float CR=0;
}SETTINGS;

bool loadMaps(int nOfMaps);
bool loadTextures();
bool loadStrings();
bool loadMap(char* path);
bool loadSounds();
bool saveMap(char* path,int idx);
bool loadSettings();
bool saveSettings();
void setSprites();

#endif	// LOADFILES_H
