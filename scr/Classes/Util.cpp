#include "Util.hpp"
Util::Util() {
};

Util::Util(const Util& orig) {
}

Util::~Util() {
}

bool Util::DEBUG=true;
#ifdef GDB
string Util::resourcesFolder="../res/";
#else
string Util::resourcesFolder="res/";
#endif
const int Util::direction_left=13;
const int Util::direction_right=45;
const int Util::direction_up=9849526;
const int Util::direction_down=552626;
const int Util::orientation_left=-1;
const int Util::orientation_right=1;
const int Util::orientation_rotated=2;

/**
 *	Add the resources folder path to a filepath
 *
 *	@param path the resource filepath
 *	@return the full resource filepath
**/
string Util::newPath(string path){
    return resourcesFolder+path;
}

/**
 *	Get the name for a specific multiresource
 *
 *	@param p1 the multiresource name
 *	@param idx the multiresource index
 *	@return the specific resource name
**/
string Util::getDinamicName(string p1,int idx){
    char buffer[5];
    snprintf(buffer,5,"%d",idx);
    string str1(buffer);
    return p1+str1;
}

/**
 *	Get the path of a specific multiresource
 *
 *	@param p1 the multiresource filepath
 *	@param idx the multiresource index
 *	@param p3 the multiresource extension
 *	@return the specific resource filepath
**/
string Util::getDinamicPath(string p1,int idx, string p3){
    p1=newPath(p1);
    char buffer[5];
    snprintf(buffer,5,"%d",idx);
    string out(p1);
    string str1(buffer);
    string str2(p3);
    out+=str1+str2;
    return out;
}

/**
 *	Split a compact rectangle into a four point rectangle
 *
 *	@param rectangle the compact rectangle
 *	@return an array containing the four rectangle points
**/
vector <nTPoint> Util::getRetangleVertexs(nTRectangle rectangle){
  int sizex;
  nTPoint sup_left, inf_right, inf_left, sup_right;
  vector <nTPoint> points; // inferior esquerdo, inferior direito, superior direito e superior esquerdo,;

  inf_left=rectangle.p0;
  sup_right=rectangle.p1;

  sizex = sup_right.x - inf_left.x;

  inf_right.set(inf_left.x + sizex, inf_left.y, inf_left.z);
  sup_left.set(sup_right.x - sizex, sup_right.y , sup_right.z); //conferir se z será o mesmo sempre
  points.push_back(inf_left);
  points.push_back(inf_right);
  points.push_back(sup_right);
  points.push_back(sup_left);
  return points;
}

/**
 *	Multiply two vector matrixes
 *
 *	@param m1 the first matrix
 *	@param m2 the second matrix
 *	@return the result matrix
**/
vector<vector<float> >Util::multiplyMatrix(vector<vector<float> >m1,vector<vector<float> >m2){
    nTPoint outSize;
    outSize.set(m1.size(),m2[0].size(),0);
    vector<vector<float> >out;
    out.resize(outSize.x,vector<float>(outSize.y ,0));
    if(m1[0].size() != m2.size()) return out; //n se aplica
    for(int i=0;i<outSize.x;i++)
        for(int j=0;j<outSize.y;j++)
            for(int k=0;k<m1[0].size();k++)
                out[i][j]+=m1[i][k]*m2[k][j];
    return out;
}

/**
 *	Convert a degrees angle into a rad angle
 *
 *	@param RAD the angle in degrees
 *	@return the angle in rad
**/
float Util::angleToRad(float RAD){
    return RAD*M_PI/180;
}

/**
 *	Convert a rad angle into a degrees angle
 *
 *	@param angle the angle in rad
 *	@return the angle in degrees
**/
float Util::radToAngle(float angle){
    return angle*180/M_PI;
}
