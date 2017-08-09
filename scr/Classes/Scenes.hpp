#ifndef SCENES_HPP
#define SCENES_HPP
#include "Util.hpp"
#include "Mechanics.hpp"
#include "Tutorials.hpp"
#include "mapEdit.hpp"
#include "Enemy.hpp"
#include "Camera.hpp"

class Scenes {
public:
    Scenes();
    Scenes(const Scenes& orig);
    virtual ~Scenes();

    static void drawGame();
    static void drawMapEdit();
    static void drawMenu();
    static void drawSplash();
    static void drawOptions();
    static void drawCredits();
    static void drawPreGame();
    static void drawPreCampaign();
    static void drawPreFreeMode();
    static void drawPosGame();
    static void drawEndGame();
    static void drawYouWin();

    static int current;
    static const int game;
    static const int menu;
    static const int mapEdit;
    static const int splash;
    static const int preGame;
    static const int credits;
    static const int pregame;
    static const int preFreeMode;
    static const int preCampaign;
    static const int options;
    static const int posGame;
    static const int posGameEnd;
    static const int posYouWin;
    static Camera camera;
    static bool skipScene;
    static bool freeGameMode;
private:
    friend class Map;
    friend class Player;
    friend class Entity;
    static bool gameCalled;
    static bool menuCalled;
    static bool optionsCalled;
    static bool splashCalled;
    static bool creditsCalled;
    static bool mapEditCalled;
    static bool preGameCalled;
    static bool preCampaignCalled;
    static bool preFreeModeCalled;
    static bool posGameCalled;
    static bool posGameEndCalled;
    static bool posYouWinCalled;
    static void setAllCalledFalseExcept(vector<int> except);
    static vector<int> getUnityVector(int value);
};

#endif /* SCENES_HPP */

