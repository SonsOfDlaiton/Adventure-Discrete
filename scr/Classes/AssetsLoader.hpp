#ifndef ASSETSLOADER_H
#define ASSETSLOADER_H
#include "Util.hpp"
#include "GL.hpp"
#include "AL.hpp"
#include "Player.hpp"
class GL;


class AssetsLoader {
public:
    AssetsLoader();
    AssetsLoader(const AssetsLoader& orig);
    virtual ~AssetsLoader();

    static bool saveSettings();
    static bool loadSettings();
    static bool loadSounds();
    static bool loadMaps(int nOfMaps);
    static bool loadTextures();
    static bool loadFonts();
private:

};

#endif /* ASSETSLOADER_H */
