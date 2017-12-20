#ifndef FUNCTIONANALYSER_H
#define FUNCTIONANALYSER_H
#include <chrono>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>

using namespace std;

struct FunctionData{
        string name;
        int nCalls;
        unsigned long delta;
        static bool sort(FunctionData a, FunctionData b){
            return a.name<b.name;
        }
};

class FunctionAnalyser {
public:
    FunctionAnalyser();
    FunctionAnalyser(const FunctionAnalyser& orig);
    virtual ~FunctionAnalyser();

    static void startFrame();
    static void endFrame();
    static void startFunction(string name);
    static void endFunction(string name);
    static void analyseData();
    static void printData();
    static int getIdOfFunction(string name, int startIdx);
    static unsigned long getCurrentTime();

    static const int milliseconds;
    static const int microseconds;
    static const int nanoseconds;
    static int timescale;

    static bool PRINT;
    static bool ANALYSE;
private:
    static vector<FunctionData> data;
    static unsigned long deltaFrame;
    static unsigned long Efunctions;
    static unsigned long frameStartTime;
    static unsigned long frameEndTime;
    static vector<string> functionsName;
    static vector<unsigned long> functionsStartTime;
    static vector<unsigned long> functionsEndTime;
};

#endif /* FUNCTIONANALYSER_H */
