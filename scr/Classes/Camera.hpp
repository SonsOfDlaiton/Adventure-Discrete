#ifndef CAMERA_H
#define CAMERA_H

#include "Util.hpp"
#include "GL.hpp"

class Player;

typedef struct scenesP{
    short int movingCam;
    double movedCam;
}scenesPoint;

class Camera {
public:
    Camera();
    Camera(const Camera& orig);
    virtual ~Camera();

    scenesPoint getScenesPointX();
    void setScenesPointX(short int movingCam, double movedCam);
    scenesPoint getScenesPointY();
    void setScenesPointY(short int movingCam, double movedCam);
    double getMovePerFrame();
    void setMoveSpeed(double moveSpeed);
    bool getFreeCam();
    void setFreeCam(double freeCam);
    void setCam(double movePerFrame_);
    void putCameraOnOrigin();
    void lookAt(nTPoint pos);
    void behave(Player* pl);
    bool isInTheScreen(nTRectangle collision);
    bool isInTheXScreen(nTRectangle collision);
    bool isInTheYScreen(nTRectangle collision);
    scenesPoint x;
    scenesPoint y;
    double movePerFrame;
    static bool freeCam;
    static float autoCamSpeed;
};

#endif /* CAMERA_H */
