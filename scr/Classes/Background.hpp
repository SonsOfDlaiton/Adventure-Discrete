#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "Util.hpp"

class ADCode;
class Texture;

class Background{
public:
	Background(string name, bool move, double zAxis, int x0, int y0, int x1, int y1, int deltaX, int deltaY, double hSpeed, double vSpeed, int nTex);
	~Background();
	bool operator<(const Background &b1) const { return zAxis > b1.zAxis; }
	void drawParalaxBackground(nTPoint size);
	void setMove(bool move);
	void setzAxis(double zAxis);
	void setLocal(nTRectangle local);
	void setPos(double pos);
	bool getMove();
	void setRandomPositions(nTPoint size);
	double getzAxis();
	Texture *tex;
	nTRectangle getLocal();
	static void loadParalax(ADCode* adc, vector<Background*>& paralax);
	static void drawBackgrounds(vector<Background*> &backgrounds);
	static const int verticalSensibility; // TODO -> Move to Constants
private:
	bool move;
	int deltaX, deltaY;
	double zAxis, hSpeed, vSpeed, initialPos, pos;
	vector<pair<int, int> > randomPositions;
	nTRectangle local;
};

typedef struct mapstru{
  vector<Background*> backgrounds;
  vector<vector<vector<pair<int,string> > > > map;
  string mapADC;
  static const short layers=2;
}nTMap;

#endif /*BACKGROUND_H*/
