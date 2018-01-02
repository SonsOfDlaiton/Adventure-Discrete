#ifndef ADC_H
#define ADC_H
#include "Util.hpp"

class ADCode {
public:
    ADCode(string data, string name);
    ADCode(const ADCode& orig);
    virtual ~ADCode();

    int getInt(string field);
    int getInt(string field,int defaultval);
    string getString(string field);
    string getString(string field, string defaultval);
    vector<string> getStringVector(string field);
    vector<int> getIntVector(string field);
    ADCode* getSubSection(string section);
    string getData();
    float getFloat(string field);
    float getFloat(string field,float defaultval);
    bool hasNext();
    bool next(int& i);
    bool next(float& f);
    bool next(string& s);
    bool next(vector<string>& vs);

    int pointer;
private:
    void lexicalAnalysis();
    string data, name;
    static const char vectorSeparator,vectorBegin,vectorEnd,stringBegin,stringEnd,sectionBegin,sectionEnd;
};

#endif /* ADC_H */
