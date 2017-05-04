#include "FunctionAnalyser.hpp"

FunctionAnalyser::FunctionAnalyser() {
};

FunctionAnalyser::FunctionAnalyser(const FunctionAnalyser& orig) {
}

FunctionAnalyser::~FunctionAnalyser() {
}

bool FunctionAnalyser::PRINT=true;
bool FunctionAnalyser::ANALYSE=true;

unsigned long FunctionAnalyser::deltaFrame=0;
unsigned long FunctionAnalyser::Efunctions=0;
vector<unsigned long> FunctionAnalyser::deltaFunctions;
unsigned long FunctionAnalyser::frameStartTime=0;
unsigned long FunctionAnalyser::frameEndTime=0;
vector<string> FunctionAnalyser::functionsName;
vector<unsigned long> FunctionAnalyser::functionsStartTime;
vector<unsigned long> FunctionAnalyser::functionsEndTime;
vector<string> FunctionAnalyser::functionsCompactName;
vector<int> FunctionAnalyser::functionsCalls;
const int FunctionAnalyser::milliseconds=1000000;
const int FunctionAnalyser::microseconds=1000;
const int FunctionAnalyser::nanoseconds=1;
int FunctionAnalyser::timescale=milliseconds;

unsigned long FunctionAnalyser::getCurrentTime(){
    return std::chrono::high_resolution_clock::now().time_since_epoch().count();
}

void FunctionAnalyser::startFrame(){
    if(!ANALYSE) return;
    functionsName.clear();
    functionsStartTime.clear();
    functionsEndTime.clear();
    deltaFunctions.clear();
    functionsCalls.clear();
    functionsCompactName.clear();
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
    int idxToSearch=0;
    while(1){
        int idx=getIdOfFunction(name,idxToSearch);
        if(idx<0) return;
        if(functionsEndTime[idx]==0){
            functionsEndTime[idx]=endtime;
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
        for(j=0;j<functionsCompactName.size();j++)
        if(functionsName[i]==functionsCompactName[j]){
            alreadyHave=true;
            break;
        }
        if(alreadyHave){
            functionsCalls[j]++;
            deltaFunctions[j]+=(functionsEndTime[i]-functionsStartTime[i]);
        }else{
            functionsCompactName.push_back(functionsName[i]);
            functionsCalls.push_back(1);
            deltaFunctions.push_back(functionsEndTime[i]-functionsStartTime[i]);
        }
        Efunctions+=(functionsEndTime[i]-functionsStartTime[i]);
    }
}
void FunctionAnalyser::printData(){
    if(!PRINT||!ANALYSE) return;
    cout<<"======================================================================================================================"<<endl;
    cout<<"Frame delta time: \t"<<deltaFrame/timescale<<endl;
    cout<<"Functions time: \t"<<Efunctions/timescale<<endl;
    cout<<"----------------------------------------------------------------------------------------------------------------------"<<endl;
    for(int i=0;i<functionsCompactName.size();i++)
        cout<<functionsCompactName[i]<<"("<<functionsCalls[i]<<")"<<setw(42-functionsCompactName[i].size())<<"\t- delta: "<<deltaFunctions[i]/timescale<<"\t- "<<"delta per call: "<<deltaFunctions[i]/functionsCalls[i]/timescale<<"\t- "<<setw(3)<<(100*deltaFunctions[i]/Efunctions)<<"%(functions)"<<"\t- "<<setw(3)<<(100*deltaFunctions[i]/deltaFrame)<<"%(frame)"<<endl;
    cout<<"======================================================================================================================"<<endl;
}
