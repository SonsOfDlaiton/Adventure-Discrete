#include "Background.hpp"
#include "GL.hpp"
#include "Scenes.hpp"
#include "ADCode.hpp"
#include "Texture.hpp"

Background::Background(string name, bool move, double zAxis, int x0, int y0, int x1, int y1, int deltaX, int deltaY, double hSpeed, double vSpeed, int nTex){
	move = move;
	zAxis = (double) zAxis/10;
	local = nTRectangle::get(x0, y0, x1, y1, zAxis);
	name = name;
	deltaX = deltaX;
	deltaY = deltaY;
	hSpeed = hSpeed;
	vSpeed = vSpeed;
	initialPos = -500;
	pos = initialPos;

	tex = new Texture();
	if(nTex>1)
		tex->setTextures(GL::getTexturesByName(name, nTex));
	else
		tex->setTextures(GL::getTextureByName(name));

}

Background::~Background(){
	//delete tex;
}

const int Background::verticalSensibility = 5000; // TODO -> Move to Constants

bool Background::getMove(){
	return move;
}
double Background::getzAxis(){
	return zAxis;
}
string Background::getName(){
	return name;
}
nTRectangle Background::getLocal(){
	return local;
}
void Background::setMove(bool move){
	move = move;
}
void Background::setzAxis(double zAxis){
	zAxis = zAxis;
}
void Background::setName(string name){
	name=name;
}
void Background::setLocal(nTRectangle local){
	local = local;
}
void Background::setPos(double pos){
	pos = pos;
}

void Background::setRandomPositions(nTPoint size){
	if (deltaY<0 || deltaX<0){
		int x=0, y=0, maxDistance=ABS(deltaX);
		for (int pos=initialPos; pos<size.x; pos+=rand()%(2*maxDistance)/1.3+(local.p1.x - local.p0.x)/131*GL::defaultSize.x){
			x=pos;
			if (deltaY!=0)
				y=ABS(rand()%(2*deltaY))-deltaY;
			pair<int, int> tmp(x, y);
			if(deltaX>=0)
				tmp.first=0;
			if(deltaY>=0)
				tmp.second=0;

			randomPositions.push_back(tmp);
		}
	}
}

void Background::drawBackgrounds(vector<Background> &backgrounds){
	for(int i=0; i<backgrounds.size(); i++){
        if(!backgrounds[i].getMove())
            GL::drawTexture(nTRectangle::get(Scenes::camera.x.movedCam,GL::defaultSize.y+Scenes::camera.y.movedCam,GL::defaultSize.x+Scenes::camera.x.movedCam,Scenes::camera.y.movedCam,backgrounds[i].getzAxis()-0.9),backgrounds[i].tex->get());
        else{
        	if(backgrounds[i].randomPositions.size()==0)
        		backgrounds[i].setRandomPositions(Map::size);
            backgrounds[i].drawParalaxBackground(Map::size);
        }
    }
}

void Background::drawParalaxBackground(nTPoint size){
	Player* pl = Player::getPlayerById(0);
	int backgroundWidth = (local.p1.x - local.p0.x)/100*GL::defaultSize.x;

	if(Scenes::camera.x.movingCam)
		pos-=(pl->hSpeed*hSpeed)/GL::getFPS()-(pl->hSpeed)/GL::getFPS();

	double percentYOfPlayer = 0;
	double offSetMoveScreenY = 0;

	percentYOfPlayer = 1-(pl->pos.y)/GL::defaultSize.y;
	if (Scenes::camera.y.movingCam&&pl->vSpeed<0)
		offSetMoveScreenY = Scenes::camera.movePerFrame;
	else if (Scenes::camera.y.movingCam&&pl->vSpeed>0)
		offSetMoveScreenY = -Scenes::camera.movePerFrame;


	int randomIterator=0;
	int randOnY=0;
	for (int x=pos; x< pl->pos.x + GL::defaultSize.x*1.5; ){
		GL::drawTexture(nTRectangle::get(
			x + local.p0.x/100.*GL::defaultSize.x,
			(100-local.p0.y)/100.*Map::size.y + (vSpeed*percentYOfPlayer*verticalSensibility)/GL::getFPS() - offSetMoveScreenY + randOnY*10,
			x + local.p1.x/100.*GL::defaultSize.x,
			(100-local.p1.y)/100.*Map::size.y + (vSpeed*percentYOfPlayer*verticalSensibility)/GL::getFPS() - offSetMoveScreenY + randOnY*10,
			zAxis),
			GL::getTextureByName(name));
		if (deltaX==0){
			GL::drawTexture(nTRectangle::get(
				x + backgroundWidth + local.p1.x/100.*GL::defaultSize.x,
				(100-local.p0.y)/100.*Map::size.y + (vSpeed*percentYOfPlayer*verticalSensibility)/GL::getFPS() - offSetMoveScreenY,
				x + local.p1.x/100.*GL::defaultSize.x,
				(100-local.p1.y)/100.*Map::size.y + (vSpeed*percentYOfPlayer*verticalSensibility)/GL::getFPS() - offSetMoveScreenY,
				zAxis),
				GL::getTextureByName(name));
			x+=backgroundWidth;
		}
		if (deltaY<0)
			randOnY = randomPositions.at(randomIterator).second;
		if (deltaX>=0)
			x+=backgroundWidth+deltaX;
		else{
			x-=(randomPositions.at(randomIterator).first - randomPositions.at(++randomIterator).first);
			if (randomIterator==randomPositions.size())
				break;
		}

	}
}

void Background::loadParalax(ADCode* adc, vector<Background>& paralax){
	paralax.clear();
	ADCode* adc_back=nullptr;
    while(adc->nextSection(adc_back)){
        if(adc_back->getName()=="Map::background"){
            string name=adc_back->getString("name");
            int sprites=adc_back->getInt("nspr",1);
            bool static_back=Util::strToBool(adc_back->getString("static"));
            if(!static_back){
                vector<int> p0=adc_back->getIntVector("p0");
                vector<int> p1=adc_back->getIntVector("p1");
                int DeltaX=adc_back->getInt("deltaX");
                int DeltaY=adc_back->getInt("deltaY");
                int Z=adc_back->getInt("depth");
                vector<int> speed=adc_back->getIntVector("speed");
                if(p0.size()==2&&p1.size()==2&&speed.size()==2){
                    Background back(name, !static_back, Z, p0[0], p0[1], p1[0], p0[1], DeltaX, DeltaY, speed[0], speed[1], sprites);
                    paralax.push_back(back);
                }
            } else{
            	Background back(name, !static_back, 0, 0, 0, 0, 0, 0, 0, 0, 0, sprites);
            	paralax.push_back(back);
            }
        }
    }
    if(adc_back==nullptr)
        delete adc_back;
}
