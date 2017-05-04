#ifndef NTFONT_H
#define NTFONT_H
#include <iostream>
#include <fstream>
#include <sstream>
#include "GL.hpp"
#include "nTFontChar.hpp"
#include "Util.hpp"

class nTFont {
public:
    nTFont();
    nTFont(const nTFont& orig);
    virtual ~nTFont();

    static nTFont* loadFont(string path);
    void drawText(string text, nTPoint pos, nTColor color);
    void drawCentered_X_Text(string text, nTPoint pos, nTColor color);
    void drawCentered_Y_Text(string text, nTPoint pos, nTColor color);
    void drawCentered_X_Y_Text(string text, nTPoint pos, nTColor color);
    void drawCentered_MultilineX_Text(string text, nTPoint pos, nTColor color);
    void drawCentered_MultilineX_Y_Text(string text, nTPoint pos, nTColor color);
    nTPoint calcBoundaries(string text);

    string name;
    int size;
private:
    static int getIntFromField(string str, int start);
    static string getStrFromField(string str, int start);

    int lineHeight;
    GLuint bitmapTexture;
    int imgSizex,imgSizey;
    vector<void*> chars;

    friend ostream& operator<<(ostream &strm, const nTFont &font);
};

#endif /* NTFONT_H */
