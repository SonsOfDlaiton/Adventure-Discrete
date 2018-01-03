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
    float getNumber(string field);
    float getNumber(string field,float defaultval);


    bool nextInt(int& i);
    bool nextFloat(float& f);
    bool next(int& i);
    bool next(float& f);
    bool next(string& s);
    bool next(vector<string>& vs);
    bool next(pair<string,int>& i);
    bool next(pair<string,float>& f);
    bool next(pair<string,string>& s);
    bool next(pair<string,vector<string> >& vs);
    void resetPointers();
    static vector<string> strToStrVector(string input);
    static vector<int> strToIntVector(string input);

private:
    void removeComments();
    void decode();
    void includeFiles();
    void printInterpretedData();
    string data, name;
    int vstr_pointer,strs_pointer,numbers_pointer,ints_pointer,flts_pointer,sections_pointer;
    vector<pair<string,vector<string> > > vstr;
    vector<pair<string,string> > strs;
    vector<pair<string,int> > numbers;
    vector<pair<string,int> > ints;
    vector<pair<string,float> > flts;
    vector<pair<string,string> > sections;
    static const char vectorSeparator,vectorBegin,vectorEnd,stringBegin,stringEnd,sectionBegin,sectionEnd,assignment,escape;
    static const string includeFile,lineComment,sectionCommentBegin,sectionCommentEnd;
};

#endif /* ADC_H */
