#ifndef TUTORIALS_H
#define TUTORIALS_H

#include "Util.hpp"
#include "GL.hpp"

class ADCode;

class Tutorials {
public:
    Tutorials();
    Tutorials(const Tutorials& orig);
    virtual ~Tutorials();

    static void draw(string text);
    static void pressKey(char key);
    static void clear();
    static void processCheckpoints();
    static void behave();
    static void catchPause();
    static void add(int pos,string data, bool pause_);
    static void add(int pos,ADCode* adc, bool pause_);
    static string getSinucaTut();
    static void registerSinucaTuts();

    static vector<int> checkpoints;
    static bool isPaused;
private:
    static void drawWhispText(string text);
    static void add(int pos,string text,vector<char> keys_, int duration, bool pause_);
    static vector<string> sinucaTuts;
    static int sinucaCurrent;
    static double sinucaMs;
    static vector<int> tutorials;
    static vector<string> texts;
    static vector<vector<char> > keys;
    static vector<int> time;
    static vector<bool> pause;
    static vector<bool> called;
    static double startTutMs;
    static int activeTut;
    static bool showTutorials;
};

#endif /* TUTORIALS_H */
