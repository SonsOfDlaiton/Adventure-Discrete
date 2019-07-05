#include "Language.hpp"

Language::Language() {
};

Language::Language(const Language& orig) {
}

Language::~Language() {
}

map<string, pair<string,string> > Language::dictionary;

string Language::getWord(string word){
    // TODO not implemented
    return "";
}

void Language::setLanguage(string){
    // TODO not implemented
}

void Language::addLanguage(pair<string,string> language){
    // TODO not implemented
}

pair<string,string> Language::learnPortuguese(){
    // TODO not implemented
    return pair<string, string>();
}

pair<string,string> Language::learnEnglish(){
    // TODO not implemented
    return pair<string, string>();
}

GLuint Language::getTexture(string texture){
    // TODO not implemented
    return 0;
}
