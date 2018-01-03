#include "ADCode.hpp"

ADCode::ADCode(string data, string name) {
    this->data=data;
    this->name=name;
    this->resetPointers();
    try{
        this->decode();
    }catch(exception& e){
        cout<<"ADCode ERROR:\n------------\n"<<data<<"\n------------\n";
    }
    this->printInterpretedData();
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
const char ADCode::assignment='=';
const char ADCode::escape='\\';
const string ADCode::includeFile="#include";
const string ADCode::lineComment="//";
const string ADCode::sectionCommentBegin="/*";
const string ADCode::sectionCommentEnd="*/";

int ADCode::getInt(string field){
    return getInt(field,0);
}

string ADCode::getString(string field){
    return getString(field,"");
}

int ADCode::getInt(string field,int defaultval){
    for(pair<string,int> n:ints)
        if(n.first==field)
            return n.second;
    if(Util::DEBUG) cout<<"ERRORRR - missing \""<<field<<"\" on "<<name<<endl;
    return defaultval;
}

string ADCode::getString(string field, string defaultval){
    for(pair<string,string> n:strs)
        if(n.first==field)
            return n.second;
    if(Util::DEBUG) cout<<"ERRORRR - missing \""<<field<<"\" on "<<name<<endl;
    return defaultval;
}

vector<string> ADCode::getStringVector(string field){
    vector<string> value;
    for(pair<string,vector<string> > n:vstr)
        if(n.first==field)
            return n.second;
    if(Util::DEBUG) cout<<"ERRORRR - missing \""<<field<<"\" on "<<name<<endl;
    return value;
}

vector<int> ADCode::getIntVector(string field){
    vector<int> value;
    for(pair<string,vector<string> > n:vstr)
        if(n.first==field){
            for(string s:n.second)
                value.push_back(Util::strToInt(s));
            return value;
        }
    if(Util::DEBUG) cout<<"ERRORRR - missing \""<<field<<"\" on "<<name<<endl;
	return value;
}

ADCode* ADCode::getSubSection(string section){
    for(pair<string,string> n:sections)
        if(n.first==section)
            return new ADCode(n.second,name+"::"+section);
    if(Util::DEBUG) cout<<"ERRORRR - missing section \""<<section<<"\" on "<<name<<endl;
    return new ADCode("",name+"::"+section);
}

string ADCode::getData(){
    return data;
}

void ADCode::includeFiles(){
    size_t found=data.find(includeFile);
	while(found!=string::npos){
        size_t start=found;
		found=data.find(stringBegin,found);
		if(found!=string::npos){
            string include=data.substr(found+1);
            found=include.find(stringEnd);
            if(found!=string::npos){
                include=include.substr(0,found);
                data=data.substr(0,start)+"\n"+Util::getFromFile(Util::newPath(include))+"\n"+data.substr(data.find(stringEnd,data.find(stringBegin,start)+1)+1);
            }else{
                cout<<"ADCode ERROR: Expected File String to include\n";
            }
        }else{
            cout<<"ADCode ERROR: Expected File String to include\n";
        }
        found=data.find(includeFile);
	}
}

void ADCode::decode(){
    includeFiles();
    removeComments();
    int last=0;
    int currentSection=0;
    vector<pair<string,string> > lex_stack;
	for(int i=0;i<data.size();i++){
        if(currentSection==0){
            switch(data[i]){
                case sectionBegin:
                    if(lex_stack.size()>0)if(lex_stack[lex_stack.size()-1].second=="string") break;
                    lex_stack.push_back(pair<string,string>(data.substr(last,i-last),"section"));
                    Util::replaceAllOccurrences(lex_stack[lex_stack.size()-1].first,"\n","");
                    Util::replaceAllOccurrences(lex_stack[lex_stack.size()-1].first," ","");
                    Util::replaceAllOccurrences(lex_stack[lex_stack.size()-1].first,"\t","");
                    last=i+1;
                    currentSection=i+1;
                    break;

                case sectionEnd:
                    if(lex_stack.size()>0)if(lex_stack[lex_stack.size()-1].second=="string") break;
                    cout<<"ADCode ERROR: Expected Section Begin\n";
                    break;

                case escape:
                    i++;
                    break;

                case assignment:
                    if(lex_stack.size()>0)if(lex_stack[lex_stack.size()-1].second=="string") break;
                    lex_stack.push_back(pair<string,string>(data.substr(last,i-last),"assign"));
                    Util::replaceAllOccurrences(lex_stack[lex_stack.size()-1].first,"\n","");
                    Util::replaceAllOccurrences(lex_stack[lex_stack.size()-1].first," ","");
                    Util::replaceAllOccurrences(lex_stack[lex_stack.size()-1].first,"\t","");
                    last=i+1;
                    for(int j=i+1;j<data.size();j++){
                        if(data[j]==stringBegin||data[j]==vectorBegin)
                            break;
                        if(isdigit(data[j])||data[j]=='-'||data[j]=='.'){
                            string value_str="";
                            bool flo=false;
                            for(int k=j;k<data.size();k++){
                                if(isdigit(data[k])||data[k]=='-'||data[k]=='.'){
                                    value_str+=data[k];
                                    last=k+1;
                                    i=k;
                                    if(data[k]=='.')
                                        flo=true;
                                }else break;
                            }
                            string field_name=lex_stack[lex_stack.size()-1].first;
                            if(flo){
                                flts.push_back(pair<string,float>(field_name,Util::strToFloat(value_str)));
                                numbers.push_back(pair<string,int>(field_name,-flts.size()));
                            }else{
                                ints.push_back(pair<string,int>(field_name,Util::strToInt(value_str)));
                                numbers.push_back(pair<string,int>(field_name,+ints.size()));
                            }
                            lex_stack.pop_back();
                            break;
                        }
                    }
                    break;

                case stringBegin://case stringEnd:
                    if(lex_stack.size()>0&&lex_stack[lex_stack.size()-1].second=="string"){
                        string str=data.substr(last,i-last);
                        lex_stack.pop_back();
                        if(lex_stack[lex_stack.size()-1].second=="assign"){
                            strs.push_back(pair<string,string>(lex_stack[lex_stack.size()-1].first,str));
                            lex_stack.pop_back();
                        }else if(lex_stack[lex_stack.size()-1].second=="vector"||lex_stack[lex_stack.size()-1].second=="separator")
                            lex_stack.push_back(pair<string,string>(str,"separator"));
                        else
                            cout<<"ADCode ERROR: Expected Destiny to string\n";
                        last=i+1;
                    }else{
                        lex_stack.push_back(pair<string,string>("","string"));
                        last=i+1;
                    }
                    break;

                case vectorBegin:
                    if(lex_stack.size()>0)if(lex_stack[lex_stack.size()-1].second=="string") break;
                    lex_stack.push_back(pair<string,string>("","vector"));
                    last=i+1;
                    break;

                case vectorSeparator:
                    if(lex_stack.size()>0)if(lex_stack[lex_stack.size()-1].second=="string") break;
                    if(data.substr(last,i-last)!="")
                        lex_stack.push_back(pair<string,string>(data.substr(last,i-last),"separator"));
                    last=i+1;
                    break;

                case vectorEnd:
                    if(lex_stack.size()>0)if(lex_stack[lex_stack.size()-1].second=="string") break;
                    lex_stack.push_back(pair<string,string>(data.substr(last,i-last),"separator"));
                    last=i+1;
                    string str_vec="";
                    while(lex_stack.size()>0&&lex_stack[lex_stack.size()-1].second=="separator"){
                        if(lex_stack[lex_stack.size()-1].first!="")
                            str_vec+=lex_stack[lex_stack.size()-1].first+vectorSeparator;
                        lex_stack.pop_back();
                    }
                    if(lex_stack.size()>0){
                        if(lex_stack[lex_stack.size()-1].second=="vector"){
                            lex_stack.pop_back();
                        }else{
                            cout<<"ADCode ERROR: Expected Vector Begin before Separators\n";
                        }
                        if(lex_stack[lex_stack.size()-1].second=="assign"){
                            vstr.push_back(pair<string,vector<string> >(lex_stack[lex_stack.size()-1].first,strToStrVector(str_vec)));
                            lex_stack.pop_back();
                        }else if(lex_stack[lex_stack.size()-1].second=="vector"||lex_stack[lex_stack.size()-1].second=="separator")
                            lex_stack.push_back(pair<string,string>((char)(11)+str_vec+(char)(4),"separator"));
                        else
                            cout<<"ADCode ERROR: Expected Assign Vector or Separator\n";
                    }else
                        cout<<"ADCode ERROR: Expected Vector Begin or any token before Separators\n";
                    last=i+1;
                    break;
            }
        }else{
            switch(data[i]){
                case escape:
                    i++;
                    break;

                case stringBegin://case stringEnd:
                    if(lex_stack.size()>0&&lex_stack[lex_stack.size()-1].second=="string"){
                        lex_stack.pop_back();
                    }else{
                        lex_stack.push_back(pair<string,string>("","string"));
                    }
                    break;

                case vectorBegin:
                    if(lex_stack.size()>0)if(lex_stack[lex_stack.size()-1].second=="string") break;
                    lex_stack.push_back(pair<string,string>("","vector"));
                    break;

                case vectorEnd:
                    if(lex_stack.size()>0)if(lex_stack[lex_stack.size()-1].second=="string") break;
                    if(lex_stack[lex_stack.size()-1].second=="vector"){
                        lex_stack.pop_back();
                    }else{
                        cout<<"ADCode ERROR: Expected Vector Begin\n";
                    }
                    break;

                case sectionBegin:
                    if(lex_stack.size()>0)if(lex_stack[lex_stack.size()-1].second=="string") break;
                    lex_stack.push_back(pair<string,string>("","section"));
                    break;

                case sectionEnd:
                    if(lex_stack.size()>0)if(lex_stack[lex_stack.size()-1].second=="string") break;
                    if(lex_stack[lex_stack.size()-1].second=="section"){
                        string section_name=lex_stack[lex_stack.size()-1].first;
                        lex_stack.pop_back();
                        bool haveSection=false;
                        for(int j=0;j<lex_stack.size();j++)
                            if(lex_stack[lex_stack.size()-1].second=="section"){
                                haveSection=true;
                                break;
                            }
                        if(!haveSection){
                            sections.push_back(pair<string,string>(section_name,data.substr(currentSection,i-currentSection)));
                            currentSection=0;
                            last=i+1;
                        }
                    }else{
                         cout<<"ADCode ERROR: Expected Section Begin\n";
                    }
                    break;
            }
        }
	}
}

vector<int> ADCode::strToIntVector(string input){
    vector<string> str_vec=strToStrVector(input);
    vector<int> val;
    for(string s:str_vec)
        val.push_back(Util::strToInt(s));
    return val;
}

vector<string> ADCode::strToStrVector(string input){
    vector<string> value;
    int lock=0;
    int last=0;
    for(int i=0;i<input.size();i++){
        if(input[i]==(char)11){
            lock++;
        }
        if(input[i]==(char)4){
            lock--;
        }
        if(lock==0&&input[i]==vectorSeparator){
            string str=input.substr(last,i-last);
            last=i+1;
            for(int j=0;j<str.size();j++){
                if(str[j]==escape)
                    j++;
                else if(str[j]==stringBegin||str[j]==stringEnd||str[j]==(char)4||str[j]==(char)11)
                    str.erase(str.begin()+j);
            }
            if(str!="")
                value.push_back(str);
        }
    }
    reverse(value.begin(),value.end());
    return value;
}


float ADCode::getFloat(string field){
    return getFloat(field,0);
}

float ADCode::getFloat(string field,float defaultval){
    for(pair<string,float> n:flts)
        if(n.first==field)
            return n.second;
    if(Util::DEBUG) cout<<"ERRORRR - missing \""<<field<<"\" on "<<name<<endl;
    return defaultval;
}

bool ADCode::next(int& i){
    pair<string,int> p;
    bool returnval=next(p);
    i=p.second;
    return returnval;
}

bool ADCode::next(float& f){
    pair<string,float> p;
    bool returnval=next(p);
    f=p.second;
    return returnval;
}

bool ADCode::next(string& s){
    pair<string,string> p;
    bool returnval=next(p);
    s=p.second;
    return returnval;
}

bool ADCode::next(vector<string>& vs){
    pair<string,vector<string> > p;
    bool returnval=next(p);
    vs=p.second;
    return returnval;
}

bool ADCode::next(pair<string,int>& i){
    pair<string,float> f;
    bool returnval=next(f);
    i.first=f.first;
    i.second=(int)f.second;
    return returnval;
}

bool ADCode::next(pair<string,float>& f){
    if(numbers_pointer>=numbers.size())
        return false;
    int id=numbers[numbers_pointer++].second;
    if(id<0){
        id=-id-1;
        if(id>=flts.size())
            return false;
        f.first=flts[id].first;
        f.second=(float)flts[id].second;
    }else{
        id=id-1;
        if(id>=ints.size())
            return false;
        f.first=ints[id].first;
        f.second=(float)ints[id].second;
    }
    return true;
}

bool ADCode::next(pair<string,string>& s){
     if(strs_pointer>=strs.size())
        return false;
    s.first=strs[strs_pointer].first;
    s.second=strs[strs_pointer++].second;
    return true;
}

bool ADCode::next(pair<string,vector<string> >& vs){
    if(vstr_pointer>=vstr.size())
        return false;
    vs.first=vstr[vstr_pointer].first;
    vs.second=vstr[vstr_pointer++].second;
    return true;
}

bool ADCode::nextInt(int& i){
    if(ints_pointer>=ints.size())
        return false;
    i=ints[ints_pointer++].second;
    return true;
}

bool ADCode::nextFloat(float& f){
    if(flts_pointer>=flts.size())
        return false;
    f=flts[flts_pointer++].second;
    return true;
}

void ADCode::resetPointers(){
    vstr_pointer=0;
    strs_pointer=0;
    numbers_pointer=0;
    ints_pointer=0;
    flts_pointer=0;
    sections_pointer=0;
}

void ADCode::removeComments(){
    size_t found=data.find(lineComment);
	while(found!=string::npos){
        size_t endOfComment=data.find("\n",found);
        data.erase(found,endOfComment-found);
        found=data.find(lineComment);
	}
	found=data.find(sectionCommentBegin);
	while(found!=string::npos){
        size_t endOfComment=data.find(sectionCommentEnd,found);
        data.erase(found,endOfComment-found);
        found=data.find(lineComment);
	}
}

float ADCode::getNumber(string field){
    return getNumber(field,0);
}

float ADCode::getNumber(string field,float defaultval){
    for(pair<string,int> n:ints)
        if(n.first==field)
            return n.second;
    for(pair<string,float> n:flts)
        if(n.first==field)
            return n.second;
    if(Util::DEBUG) cout<<"ERRORRR - missing \""<<field<<"\" on "<<name<<endl;
    return defaultval;
}

void ADCode::printInterpretedData(){
    cout<<"interpretados:\n------------\n";
    cout<<"int("<<ints.size()<<"):\n";
    if(ints.size())
    for(pair<string,int> p:ints)
        cout<<p.first<<": "<<p.second<<endl;
    cout<<"float("<<flts.size()<<"):\n";
    if(flts.size())
    for(pair<string,float> p:flts)
        cout<<p.first<<": "<<p.second<<endl;
    cout<<"string("<<strs.size()<<"):\n";
    if(strs.size())
    for(pair<string,string> p:strs)
        cout<<p.first<<": "<<p.second<<endl;
    cout<<"vector("<<vstr.size()<<"):\n";
    if(vstr.size())
    for(pair<string,vector<string> > p:vstr){
        cout<<p.first<<": {";
        for(string n:p.second){
            if(n.find((char)11)!=string::npos){
                Util::replaceAllOccurrences(n,""+(char)11,"");
                Util::replaceAllOccurrences(n,""+(char)4,"");
                cout<<"{"<<n<<"},";
            }else{
                cout<<n<<",";
            }
        }
        cout<<"}\n";
    }
    cout<<"sections("<<sections.size()<<"):\n";
    if(sections.size())
    for(pair<string,string> p:sections)
        cout<<p.first<<": "<<p.second<<endl;
    cout<<"\n------------\n";
}
