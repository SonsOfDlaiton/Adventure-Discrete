#include "FunctionAnalyser.hpp"

FunctionAnalyser::FunctionAnalyser() {
};

FunctionAnalyser::FunctionAnalyser(const FunctionAnalyser& orig) {
}

FunctionAnalyser::~FunctionAnalyser() {
}

bool FunctionAnalyser::PRINT=false;
bool FunctionAnalyser::ANALYSE=PRINT;
bool FunctionAnalyser::SORTBYCONSUMPTION=true;

vector<FunctionData> FunctionAnalyser::data;
unsigned long FunctionAnalyser::deltaFrame=0;
unsigned long FunctionAnalyser::Efunctions=0;
unsigned long FunctionAnalyser::frameStartTime=0;
unsigned long FunctionAnalyser::frameEndTime=0;
vector<string> FunctionAnalyser::functionsName;
vector<string> FunctionAnalyser::activeFunctions;
vector<unsigned long> FunctionAnalyser::functionsStartTime;
vector<unsigned long> FunctionAnalyser::functionsEndTime;
const int FunctionAnalyser::milliseconds=1000000;
const int FunctionAnalyser::microseconds=1000;
const int FunctionAnalyser::nanoseconds=1;
int FunctionAnalyser::timescale=microseconds;

unsigned long FunctionAnalyser::getCurrentTime(){
    return std::chrono::high_resolution_clock::now().time_since_epoch().count();
}

void FunctionAnalyser::startFrame(){
    if(!ANALYSE) return;
    activeFunctions.clear();
    functionsName.clear();
    functionsStartTime.clear();
    functionsEndTime.clear();
    data.clear();
    frameEndTime=0;
    Efunctions=0;
    deltaFrame=0;
    frameStartTime=getCurrentTime();
}
void FunctionAnalyser::endFrame(){
    if(!ANALYSE) return;
    frameEndTime=getCurrentTime();
    analyseData();
}

int FunctionAnalyser::getIdOfFunction(string name, int startIdx){
    for(int i=startIdx;i<functionsName.size();i++)
        if(name==functionsName[i])
            return i;
    return -1;
}
void FunctionAnalyser::startFunction(string name){
    if(!ANALYSE) return;
    int id=getIdOfFunction(name,0);
    activeFunctions.push_back(name);
    if(id<0){
        functionsName.push_back(name);
        functionsEndTime.push_back(0);
        functionsStartTime.push_back(getCurrentTime());
    }else if(functionsEndTime[id]!=0){
        functionsName.push_back(name);
        functionsEndTime.push_back(0);
        functionsStartTime.push_back(getCurrentTime());
    }
}
void FunctionAnalyser::endFunction(string name){
    if(!ANALYSE) return;
    unsigned long endtime=getCurrentTime();
    activeFunctions.pop_back();
    int idxToSearch=0;
    while(1){
        int idx=getIdOfFunction(name,idxToSearch);
        if(idx<0) return;
        if(functionsEndTime[idx]==0){
            functionsEndTime[idx]=endtime;
            if(activeFunctions.size()>0&&activeFunctions[activeFunctions.size()-1]!=name){
                functionsStartTime[getIdOfFunction(activeFunctions[activeFunctions.size()-1],0)]+=endtime-functionsStartTime[idx];
            }
            return;
        }
        idxToSearch=idx+1;
    }
}

void FunctionAnalyser::analyseData(){
    if(!ANALYSE) return;
    deltaFrame=frameEndTime-frameStartTime;
    int j;
    for(int i=0;i<functionsName.size();i++){
        bool alreadyHave=false;
        for(j=0;j<data.size();j++)
        if(functionsName[i]==data[j].name){
            alreadyHave=true;
            break;
        }
        if(alreadyHave){
            data[j].nCalls++;
            data[j].delta+=(functionsEndTime[i]-functionsStartTime[i]);
        }else{
            FunctionData fd;
            fd.name=functionsName[i];
            fd.nCalls=1;
            fd.delta=functionsEndTime[i]-functionsStartTime[i];
            data.push_back(fd);
        }
        Efunctions+=(functionsEndTime[i]-functionsStartTime[i]);
    }
    if(SORTBYCONSUMPTION)
        sort(data.begin(), data.end(), FunctionData::sortDelta);
    else
        sort(data.begin(), data.end(), FunctionData::sortName);
}
void FunctionAnalyser::printData(){
    if(!PRINT||!ANALYSE) return;
    cout<<"======================================================================================================================"<<endl;
    cout<<"Frame delta time: \t"<<deltaFrame/timescale<<endl;
    cout<<"Functions time: \t"<<Efunctions/timescale<<endl;
    cout<<"----------------------------------------------------------------------------------------------------------------------"<<endl;
    for(int i=0;i<data.size();i++)
        cout<<data[i].name<<"("<<data[i].nCalls<<")"<<setw(42-data[i].name.size())<<"\t- delta: "<<data[i].delta/timescale<<"\t- "<<"delta per call: "<<data[i].delta/data[i].nCalls/timescale<<"\t- "<<setw(3)<<(100*data[i].delta/Efunctions)<<"%(functions)"<<"\t- "<<setw(3)<<(100*data[i].delta/deltaFrame)<<"%(frame)"<<endl;
    cout<<"======================================================================================================================"<<endl;
}
