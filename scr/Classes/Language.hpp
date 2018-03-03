#ifndef LANGUAGE_H
#define LANGUAGE_H
#include "Util.hpp"
#include "GL.hpp"

class Language {
public:
    Language();
    Language(const Language& orig);
    virtual ~Language();
    static string getWord(string word);
    static GLuint getTexture(string texture);
    static void setLanguage(string);
    static void addLanguage(pair<string,string> language);
    static pair<string,string> learnPortuguese();
    static pair<string,string> learnEnglish();
    static map<string, pair<string,string> > dictionary;
private:
};

#endif /* LANGUAGE_H */
