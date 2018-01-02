#include "ADCode.hpp"

ADCode::ADCode(string data, string name) {
    this->data=data;
    this->name=name;
    this->pointer=0;
    this->lexicalAnalysis();
};

ADCode::ADCode(const ADCode& orig) {
}

ADCode::~ADCode() {
}

const char ADCode::vectorSeparator=',';
const char ADCode::vectorBegin='{';
const char ADCode::vectorEnd='}';
const char ADCode::stringBegin='\"';
const char ADCode::stringEnd='\"';
const char ADCode::sectionBegin='[';
const char ADCode::sectionEnd=']';

int ADCode::getInt(string field){
    return getInt(field,0);
}

string ADCode::getString(string field){
    return getString(field,"");
}

int ADCode::getInt(string field,int defaultval){
    int value=defaultval;
    size_t found=data.find(field);
	if(found!=string::npos){
        string value_str="";
		bool first=false;
		for(int i=found+1;i<data.size();i++)
			if(isdigit(data[i])){
				first=true;
				value_str+=data[i];
			}else if(first)
				break;
        value=Util::strToInt(value_str);
	}else if(Util::DEBUG) cout<<"ERRORRR - missing \""<<field<<"\" on "<<name<<endl;
	return value;
}

string ADCode::getString(string field, string defaultval){
    string value="";
    size_t found=data.find(field);
	if(found!=string::npos){
		found=data.find(stringBegin,found);
		value=data.substr(found+1);
		value=value.substr(0,value.find(stringEnd));
	}
	if(value==""&&Util::DEBUG){
		cout<<"ERRORRR - missing \""<<field<<"\" on "<<name<<endl;
		return defaultval;
    }
    return value;
}

vector<string> ADCode::getStringVector(string field){
    vector<string> value;
    size_t found=data.find(field);
	if(found!=string::npos){
		found=data.find(vectorBegin,found);
		string value_str=data.substr(found+1);
		value_str=value_str.substr(0,value_str.find(vectorEnd));
		int last=0;
		value_str+=vectorSeparator;
		for(int i=0;i<value_str.size();i++){
			if(value_str[i]==vectorSeparator){
				string str=value_str.substr(last,i-last);
				last=i;
				for(int j=0;j<str.size();j++)
                    if(str[j]==stringBegin){
                        str.erase(str.begin()+j);
                        for(int k=str.size()-1;k>=0;k--)
                            if(str[k]==stringEnd){
                                str.erase(str.begin()+k);
                                break;
                            }
                        break;
                    }
				value.push_back(str);
			}
		}
	}else if(Util::DEBUG) cout<<"ERRORRR - missing \""<<field<<"\" on "<<name<<endl;
	return value;
}

vector<int> ADCode::getIntVector(string field){
    vector<int> value;
    size_t found=data.find(field);
	if(found!=string::npos){
		found=data.find(vectorBegin,found);
		string value_str=data.substr(found+1);
		value_str=value_str.substr(0,value_str.find(vectorEnd));
		int last=0;
		value_str+=vectorSeparator;
		for(int i=0;i<value_str.size();i++){
			if(value_str[i]==vectorSeparator){
				string str=value_str.substr(last,i-last);
				last=i;
				value.push_back(Util::strToInt(str));
			}
		}
	}else if(Util::DEBUG) cout<<"ERRORRR - missing \""<<field<<"\" on "<<name<<endl;
	return value;
}

ADCode* ADCode::getSubSection(string section){
    string value="";
    size_t found=data.find(section);
	if(found!=string::npos){
		found=data.find(sectionBegin,found);
		value=data.substr(found+1);
		value=value.substr(0,value.find(sectionEnd));
	}
	if(value==""&&Util::DEBUG){
		cout<<"ERRORRR - missing section \""<<section<<"\" on "<<name<<endl;
		return new ADCode("",name+"::"+section);
    }
    return new ADCode(value,name+"::"+section);
}

string ADCode::getData(){
    return data;
}

void ADCode::lexicalAnalysis(){
    //TODO verificar campos, verificar abre e fecha parenteses, chaves...
}


float ADCode::getFloat(string field){
    return getFloat(field,0);
}

float ADCode::getFloat(string field,float defaultval){
    //TODO
    return 0;
}

bool ADCode::hasNext(){
//TODO
    return false;
}

bool ADCode::next(int& i){
//TODO
    return false;
}

bool ADCode::next(float& f){
//TODO
    return false;
}

bool ADCode::next(string& s){
//TODO
    return false;
}

bool ADCode::next(vector<string>& vs){
    //TODO
    return false;
}
