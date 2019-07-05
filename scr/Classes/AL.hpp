#ifndef AL_H
#define AL_H

#ifdef __APPLE__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif
#include "Util.hpp"

class AL {
public:
    AL();
    AL(const AL& orig);
    virtual ~AL();

    static bool checkIfIsPlaying(int sound);
    static void stopAllSoundsExcept(vector<int>&soundss);
    static void stopAllSoundsExcept(int sounds);
    static bool playSoundByName(string name);
    static int  getSoundByName(string name);
    static bool playSound(int sound);
    static bool stopSound(int sound);
    static bool loadSound(string path,string name,float vol,bool Loop);
    static void setMusicState(bool musicOn);
    static void setSoundState(bool soundOn);
    static bool getSoundState();
    static bool getMusicState();

    static AL* singleton;
private:
    static vector<string> soundName;
    static vector<ALuint> sounds;
    static vector<ALuint> buffers;
    static vector<bool> isMusic;
    static bool soundOn;
    static bool musicOn;
    static const ALfloat ListenerPos[3];
    static const ALfloat ListenerVel[3];
    static const ALfloat ListenerOri[6];
    static const ALfloat SourcePos[3];
    static const ALfloat SourceVel[3];
    friend ostream& operator<<(ostream &strm, const AL &al);
};

#endif /* AL_H */
