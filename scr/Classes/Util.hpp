#ifndef NTCOLOR
#define NTCOLOR
typedef struct color{
        double R,G,B,A;
        void set(double R_,double G_, double B_, double A_){
            R=R_;G=G_;B=B_;A=A_;
        };
        void set(double R_,double G_, double B_){
            R=R_;G=G_;B=B_;A=1;
        };
        static struct color White(){
            struct color colo;
            colo.set(1,1,1);
            return colo;
        };
        static struct color Black(){
            struct color colo;
            colo.set(0,0,0);
            return colo;
        };
        static struct color get(double R_,double G_, double B_, double A_){
            struct color clor;
            clor.set(R_,G_,B_,A_);
            return clor;
        };
        static struct color get(double R_,double G_, double B_){
            struct color clor;
            clor.set(R_,G_,B_,1);
            return clor;
        };
}nTColor;
#endif // NTCOLOR
#ifndef NTPOINT
#define NTPOINT
typedef struct point{
        double x,y,z;
        void set(double x_,double y_,double z_){
            x=x_;y=y_;z=z_;
        };
        void set(double x_,double y_){
            x=x_;y=y_;z=0;
        };
        static struct point Origin(){
            struct point ori;
            ori.set(0,0,0);
            return ori;
        };
        static struct point get(double x_,double y_,double z_){
            struct point pnt;
            pnt.set(x_,y_,z_);
            return pnt;
        };
        static struct point get(double x_,double y_){
            struct point pnt;
            pnt.set(x_,y_);
            return pnt;
        };
}nTPoint;
#endif // NTPOINT

#ifndef NTRECTANGLE
#define NTRECTANGLE
typedef struct rectangle{
        nTPoint p0,p1;
        void set(nTPoint p0_,nTPoint p1_){
            p0=p0_;p1=p1_;
        };
        void set(double x0_,double y0_,double x1_,double y1_,double z0_,double z1_){
            p0.x=x0_;p0.y=y0_;p1.x=x1_;p1.y=y1_;
            p0.z=z0_;p1.z=z1_;
        };
        void set(double x0_,double y0_,double x1_,double y1_,double z_){
            p0.x=x0_;p0.y=y0_;p1.x=x1_;p1.y=y1_;
            p0.z=z_;p1.z=z_;
        };
        void set(double x0_,double y0_,double x1_,double y1_){
            p0.x=x0_;p0.y=y0_;p1.x=x1_;p1.y=y1_;
            p0.z=0;p1.z=0;
        };
        static struct rectangle get(nTPoint p0_,nTPoint p1_){
            struct rectangle rect;
            rect.set(p0_,p1_);
            return rect;
        };
        static struct rectangle get(double x0_,double y0_,double x1_,double y1_,double z0_,double z1_){
            struct rectangle rect;
            rect.set(x0_,y0_,x1_,y1_,z0_,z1_);
            return rect;
        };
        static struct rectangle get(double x0_,double y0_,double x1_,double y1_,double z_){
            struct rectangle rect;
            rect.set(x0_,y0_,x1_,y1_,z_);
            return rect;
        };
        static struct rectangle get(double x0_,double y0_,double x1_,double y1_){
            struct rectangle rect;
            rect.set(x0_,y0_,x1_,y1_);
            return rect;
        };
        static struct rectangle getCollision(nTPoint pos,nTPoint size){
            nTPoint inf_left, up_right;
            struct rectangle collision;
            inf_left.set(pos.x-(size.x/2) ,pos.y+(size.y/2), pos.z-(size.z/2));
            up_right.set(pos.x+(size.x/2) ,pos.y-(size.y/2), pos.z+(size.z/2));
            collision.set(inf_left, up_right);
            return collision;
        }
}nTRectangle;
#endif // NTRECTANGLE

#ifndef UTIL_H
#define UTIL_H
#define ABS(x) ((x)<0?-(x):(x))
#define M_PI 3.14159265359
#include "FunctionAnalyser.hpp"
#include <vector>
#include <string>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <math.h>
#include <map>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>

using namespace std;

typedef struct auxcollision{
  int firstObj;
  int secondObj;
}objCollision;

typedef struct collision{
    objCollision collision;
    int blockId;
}mapCollision;

typedef struct settings{
    const char version[50] = "Adventure Discrete - Beta 1.0";
    double records[10];
    double recordsU[10];
    bool sound=true,music=true,beatGame=false;
    int lives=0,checkpoint=0,currentStage=0,sword=0;
    double CR=0;
}SETTINGS;

class Util {
public:
    Util();
    Util(const Util& orig);
    virtual ~Util();

    static string getFromFile(string path);
    static void setToFile(string path,string data);
    static string newPath(string path);
    static string getDinamicPath(string p1,int idx, string p3);
    static string getDinamicName(string p1,int idx);
    static vector<vector<double> >multiplyMatrix(vector<vector<double> >m1,vector<vector<double> >m2);
    static vector <nTPoint> getRetangleVertexs(nTRectangle rectangle);
    static double angleToRad(double angle);
    static double radToAngle(double RAD);
    static void replaceAllOccurrences(std::string& subject,const std::string& search,const std::string& replace);
    static bool timerWithInterval(double interval);
    static bool isInsideBox(nTRectangle collider, nTPoint point);
    static string intToStr(int i);
    static string floatToStr(float f);
    static int strToInt(string str);
    static float strToFloat(string str);
    static bool strToBool(string str);

    static bool DEBUG;
    static const int direction_left;
    static const int direction_right;
    static const int direction_up;
    static const int direction_down;
    static const int orientation_left;
    static const int orientation_right;
    static const int orientation_rotated;
private:
    static string resourcesFolder;
};

#endif /* UTIL_H */
