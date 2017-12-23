#ifndef TUTORIALS_H
#define TUTORIALS_H

#include "Util.hpp"
#include "GL.hpp"

class Tutorials {
public:
    Tutorials(string text,int key);
    Tutorials(const Tutorials& orig);
    virtual ~Tutorials();

    static void draw();
    static void pressKey(char key);
    static void clear();
    static void processCheckpoints();
    static void behave();
    static void add(string data, bool pause_);

    static vector<int> checkpoints;
    static bool isPaused;
private:
    static void drawWhispText(string text);
    static void add(int pos,string text,vector<char> keys_, int duration, bool pause_);
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
