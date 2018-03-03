#include "Background.hpp"
#include "GL.hpp"
#include "Scenes.hpp"

Background::Background(string name, string path, bool move, double zAxis, int x0, int y0, int x1, int y1, int deltaX, double hSpeed, double vSpeed){
	this->move = move;
	this->zAxis = (double) zAxis/10;
	this->local = nTRectangle::get(x0, y0, x1, y1, this->zAxis);
	this->name = name;
	this->path = path;
	this->deltaX = deltaX;
	this->hSpeed = hSpeed;
	this->vSpeed = vSpeed;
	this->initialPos = -256;
	this->pos = this->initialPos;
}

Background::~Background(){
}

bool Background::getMove(){
	return this->move;
}
double Background::getzAxis(){
	return this->zAxis;
}
string Background::getName(){
	return this->name;
}
nTRectangle Background::getLocal(){
	return this->local;
}
string Background::getPath(){
	return this->path;
}
void Background::setMove(bool move){
	this->move = move;
}
void Background::setzAxis(double zAxis){
	this->zAxis = zAxis;
}
void Background::setName(string name){
	this->name=name;
}
void Background::setLocal(nTRectangle local){
	this->local = local;
}
void Background::setPath(string path){
	this->path = path;
}

void Background::drawParalaxBackground(nTPoint size){
	Player* pl = Player::getPlayerById(0);
	double delta = ABS(this->local.p1.x - this->local.p0.x)/100*GL::defaultSize.x;
	if(ABS(pos-initialPos)>=delta+deltaX){
		if(deltaX==0)
			pos=initialPos-delta;
		else
			pos=initialPos;
    }
	if(Scenes::camera.x.movingCam)
		pos+=(pl->hSpeed)/GL::getFPS();
	if(pl->hSpeed!=0)
		pos-=(this->hSpeed*pl->hSpeed)/GL::getFPS();
	for(double x=pos; x<size.x; x+=delta+deltaX){
		GL::drawTexture(nTRectangle::get(
			x + this->local.p0.x/100.*GL::defaultSize.x,
			this->local.p0.y/100.*GL::defaultSize.y,
			x + this->local.p1.x/100.*GL::defaultSize.x,
			this->local.p1.y/100.*GL::defaultSize.y,
			this->zAxis),
		GL::getTextureByName(this->name));
		if(deltaX==0){
			GL::drawTexture(nTRectangle::get(
				x + delta + this->local.p1.x/100.*GL::defaultSize.x,
				this->local.p0.y/100.*GL::defaultSize.y,
				x + this->local.p1.x/100.*GL::defaultSize.x,
				this->local.p1.y/100.*GL::defaultSize.y,
				this->zAxis),
			GL::getTextureByName(this->name));
			x+=delta;
		}
	}
}

// vector<Background> Background::loadADCode(string data){
// 	vector<Background> paralax;
// 	ADCode* adc=new ADCode(data,"Backgrounds");
// 	pair<string,string> background;
// 	while(adc->nextSection(background)){
//     	if(background.first=="background"){
//     		ADCode* adc_back=new ADCode(background.second,"Background");
//     		string name=adc_back->getString("name");
//     		int sprites=adc_back->getInt("nspr");
//     		bool static_back=Util::strToBool(adc_back->getString("static"));
//     		vector<int> p0=adc_back->getIntVector("p0");
//     		vector<int> p1=adc_back->getIntVector("p1");
//     		int deltaX=adc_back->getInt("deltaX");
//     		int Z=adc_back->getInt("depth");
//     		vector<int> speed=adc_back->getIntVector("speed");
//     		if(p0.size()==2&&p1.size()==2&&speed.size()==2){
//     			Background back();
//     			paralax.push_back(back);
//     		}
//     		delete adc_back;
//     	}
//     }
// 	delete adc;	
// 	return paralax;
// }