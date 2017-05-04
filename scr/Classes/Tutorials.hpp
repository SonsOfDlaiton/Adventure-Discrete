#ifndef TUTORIALS_H
#define TUTORIALS_H

#include "Util.hpp"
#include "GL.hpp"

class Tutorials {
public:
    Tutorials(string text,int key);
    Tutorials(const Tutorials& orig);
    virtual ~Tutorials();

    void draw();
    void refresh(string text,int key);

    static int pressedKey;
    static bool initHelpC;
    static bool atackHelpC;
    static bool jumpHelpC;
    static bool powerupHelpC;
    static bool rangedAtkHelpC;
    static bool bossHelpC;
    static bool breakHelpC;
    static bool moveHelpC;
    static bool tpHelpC;
    static bool intercampiHelpC;
    static bool jumpSpringHelpC;
    static bool shooterHelpC;
    static bool checkHelpC;
    static string initHelp;
    static string atackHelp;
    static string jumpHelp;
    static string powerupHelp;
    static string rangedAtkHelp;
    static string bossHelp;
    static string breakHelp;
    static string moveHelp;
    static string tpHelp;
    static string intercampiHelp;
    static string jumpSpringHelp;
    static string shooterHelp;
    static string checkHelp;
    static string hudHelp;
    static bool isPaused;
    static Tutorials* singleton;
private:
    void drawWhispText(string text);

    bool isCalled;
    string text;
    int key;
    static bool showTutorials;
};

#endif /* TUTORIALS_H */
