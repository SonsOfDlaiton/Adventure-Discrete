#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "Util.hpp"

class Background{
public:
	Background(string name, string path, bool move, double zAxis, int x0, int y0, int x1, int y1, int deltaX, double hSpeed, double vSpeed);
	~Background();
	bool operator<(const Background &b1) const { return zAxis > b1.zAxis; }
	void drawParalaxBackground(nTPoint size);
	void setMove(bool move);
	void setzAxis(double zAxis);
	void setName(string name);
	void setLocal(nTRectangle local);
	void setPath(string path);
	bool getMove();
	double getzAxis();
	string getName();
	nTRectangle getLocal();
	string getPath();
private:
	string name, path;
	bool move;
	int deltaX;
	double zAxis, hSpeed, vSpeed, initialPos, pos;
	nTRectangle local;
};

typedef struct mapstru{
  vector<Background> backgrounds;
  vector<vector<vector<pair<int,string> > > > map;
  string mapADC;
  static const short layers=2;
}nTMap;

#endif /*BACKGROUND_H*/
