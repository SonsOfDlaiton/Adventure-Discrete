#ifndef CAMERA_H
#define CAMERA_H
#include "Util.hpp"
#include "GL.hpp"

typedef struct scenesP{
    short int movingCam;
    float movedCam;
}scenesPoint;

class Camera {
public:
    Camera();
    Camera(const Camera& orig);
    virtual ~Camera();

    scenesPoint getScenesPointX();
    void setScenesPointX(short int movingCam, float movedCam);
    scenesPoint getScenesPointY();
    void setScenesPointY(short int movingCam, float movedCam);
    float getMoveSpeed();
    void setMoveSpeed(float moveSpeed);
    bool getFreeCam();
    void setFreeCam(float freeCam);
    void setCam(float moveSpeed_);
    void putCameraOnOrigin();
    void lookAt(nTPoint pos);
    bool isInTheScreen(nTRectangle collision);
    bool isInTheXScreen(nTRectangle collision);
    bool isInTheYScreen(nTRectangle collision);
    scenesPoint x;
    scenesPoint y;
    float moveSpeed;
    static bool freeCam;
};

#endif /* AL_H */