#include "Camera.hpp"
#include "Player.hpp"

Camera::Camera(){
}

Camera::Camera(const Camera& orig){
}

Camera::~Camera(){
}

bool Camera::freeCam=false;

scenesPoint Camera::getScenesPointX(){
	return x;
}

scenesPoint Camera::getScenesPointY(){
	return y;
}

double Camera::getMoveSpeed(){
	return moveSpeed;
}

bool Camera::getFreeCam(){
	return freeCam;
}

void Camera::setScenesPointX(short int movingCam, double movedCam){
	x.movingCam = movingCam;
	x.movedCam = movedCam;
}

void Camera::setScenesPointY(short int movingCam, double movedCam){
	y.movingCam = movingCam;
	y.movedCam = movedCam;
}

void Camera::setMoveSpeed(double moveSpeed){
	moveSpeed = moveSpeed;
}

void Camera::setFreeCam(double freeCam){
	Camera::freeCam = freeCam;
}

void Camera::setCam(double moveSpeed_){
    x.movedCam=0;
    x.movingCam=0;
    y.movedCam=0;
    y.movingCam=0;
    moveSpeed=moveSpeed_;
}

/**
 *	Put the camera on the origin position
**/
void Camera::putCameraOnOrigin(){
    gluLookAt(x.movedCam*-1,y.movedCam*-1,0,x.movedCam*-1,y.movedCam*-1,-1,0,1,0);
    x.movedCam=0;
    y.movedCam=0;
    gluLookAt(0,0,0,0,0,-1,0,1,0);
}

/**
 *	Put the camera locking at a specific point
 *
 *	@param pos the point to be looked by the camera
**/
void Camera::lookAt(nTPoint pos){
    pos.x-=GL::defaultSize.x/2+x.movedCam;
    pos.y-=GL::defaultSize.y/2+y.movedCam;
    gluLookAt(pos.x,pos.y,0,pos.x,pos.y,-1,0,1,0);
    x.movedCam+=pos.x;
    y.movedCam+=pos.y;
    gluLookAt(0,0,0,0,0,-1,0,1,0);
}

/**
 *	Check if a rectangle is in the screen
 *
 *	@param collision the coordinates of the rectangle
 *	@return true if is in the screen, otherwise false
**/
bool Camera::isInTheScreen(nTRectangle collision){
    bool out=false;
    nTPoint offSet;
    offSet.set(GL::defaultSize.x*0.2,GL::defaultSize.y*0.3,0);
    offSet.set(0,0,0);
    if(x.movedCam-offSet.x<=collision.p0.x&&collision.p0.x<GL::defaultSize.x+x.movedCam+offSet.x)
        if(y.movedCam-offSet.y<=collision.p0.y&&collision.p0.y<GL::defaultSize.y+y.movedCam+offSet.y)
            out=true;
    if(x.movedCam-offSet.x<=collision.p1.x&&collision.p1.x<GL::defaultSize.x+x.movedCam+offSet.x)
        if(y.movedCam-offSet.y<=collision.p0.y&&collision.p0.y<GL::defaultSize.y+y.movedCam+offSet.y)
            out=true;
    if(x.movedCam-offSet.x<=collision.p1.x&&collision.p1.x<GL::defaultSize.x+x.movedCam+offSet.x)
        if(y.movedCam-offSet.y<=collision.p1.y&&collision.p1.y<GL::defaultSize.y+y.movedCam+offSet.y)
            out=true;
    if(x.movedCam-offSet.x<=collision.p0.x&&collision.p0.x<GL::defaultSize.x+x.movedCam+offSet.x)
        if(y.movedCam-offSet.y<=collision.p1.y&&collision.p1.y<GL::defaultSize.y+y.movedCam+offSet.y)
            out=true;
    return out;
}

/**
 *	Check if a rectangle is in the screen honrizontally
 *
 *	@param collision the coordinates of the rectangle
 *	@return true if is in the screen, otherwise false
**/
bool Camera::isInTheXScreen(nTRectangle collision){
    bool out=false;
    nTPoint offSet;
    offSet.set(GL::defaultSize.x*0.2,GL::defaultSize.y*0.3,0);
    offSet.set(0,0,0);
    if(x.movedCam-offSet.x<=collision.p0.x&&collision.p0.x<GL::defaultSize.x+x.movedCam+offSet.x)
            out=true;
    if(x.movedCam-offSet.x<=collision.p1.x&&collision.p1.x<GL::defaultSize.x+x.movedCam+offSet.x)
            out=true;
    if(x.movedCam-offSet.x<=collision.p1.x&&collision.p1.x<GL::defaultSize.x+x.movedCam+offSet.x)
            out=true;
    if(x.movedCam-offSet.x<=collision.p0.x&&collision.p0.x<GL::defaultSize.x+x.movedCam+offSet.x)
            out=true;
    return out;
}

/**
 *	Check if a rectangle is in the screen vertically
 *
 *	@param collision the coordinates of the rectangle
 *	@return true if is in the screen, otherwise false
**/
bool Camera::isInTheYScreen(nTRectangle collision){
    bool out=false;
    nTPoint offSet;
    offSet.set(GL::defaultSize.x*0.2,GL::defaultSize.y*0.3,0);
    offSet.set(0,0,0);
    if(y.movedCam-offSet.y<=collision.p0.y&&collision.p0.y<GL::defaultSize.y+y.movedCam+offSet.y)
            out=true;
    if(y.movedCam-offSet.y<=collision.p0.y&&collision.p0.y<GL::defaultSize.y+y.movedCam+offSet.y)
            out=true;
    if(y.movedCam-offSet.y<=collision.p1.y&&collision.p1.y<GL::defaultSize.y+y.movedCam+offSet.y)
            out=true;
    if(y.movedCam-offSet.y<=collision.p1.y&&collision.p1.y<GL::defaultSize.y+y.movedCam+offSet.y)
            out=true;
    return out;
}

void Camera::followPlayer(Player* pl){
    if(ABS(pl->hSpeed)>0)
        moveSpeed=ABS(pl->hSpeed/GL::getFPS());
//    else if(Player::getPlayerById(0)->OnMOveBLock)
//        moveSpeed=ABS(150/GL::getFPS());
    else
        moveSpeed=ABS(Entity::walkSpeed/GL::getFPS());

    if((pl->pos.x>GL::defaultSize.x/2*1.11+x.movedCam))
        x.movingCam=1;
    else if((pl->pos.x<GL::defaultSize.x/2*0.99+x.movedCam)&&(x.movedCam>1.11))
        x.movingCam=-1;
    else
        x.movingCam=0;

    if(pl->pos.y<Map::size.y*1.3){
         if((pl->pos.y>GL::defaultSize.y/1.5*1.11+y.movedCam))
            y.movingCam=1;
         else if((pl->pos.y<GL::defaultSize.y/2*0.49+y.movedCam)&&(y.movedCam>1.11))
            y.movingCam=-1;
        else
            y.movingCam=0;
    }else if(!Camera::freeCam)
            y.movingCam=0;


    gluLookAt(moveSpeed*x.movingCam,moveSpeed*y.movingCam,0,moveSpeed*x.movingCam,moveSpeed*y.movingCam,-1,0,1,0);
    if(x.movingCam)
        x.movedCam+=moveSpeed*x.movingCam;
    if(y.movingCam)
        y.movedCam+=moveSpeed*y.movingCam;
}
