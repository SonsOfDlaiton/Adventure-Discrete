#include "mapEdit.hpp"


mapEdit::mapEdit() {
mapEdit::GLlist=glGenLists(1);
}

mapEdit::mapEdit(const mapEdit& orig) {
}

mapEdit::~mapEdit() {
}

nTPoint mapEdit::scale=Util::nTPointSet(0.8,0.8,1);
nTPoint mapEdit::HUDarea=Util::nTPointSet(650,500,1);
nTPoint mapEdit::size=Util::nTPointSet(0,0,1);
string mapEdit::input;
GLuint mapEdit::GLlist;
int mapEdit::currentBlock=1;
string mapEdit::blankField="_";
int mapEdit::currentBackground=0;
vector<vector<int> > mapEdit::map;
int mapEdit::index;
vector<vector<vector<int> > >mapEdit::blockPages;
vector<string> mapEdit::pageNames;
int mapEdit::pageIndex=0;
short int mapEdit::isCreating=0;
bool mapEdit::isUser=true;

void mapEdit::definePages(){
    blockPages.clear();
    pageNames.clear();
    vector <vector<int> > pageTmp;
    vector < int > rowTmp;

    pageNames.push_back("Solidos");
    for(int i=1;i<7;i++)
        rowTmp.push_back(i);
    pageTmp.push_back(rowTmp);
    rowTmp.clear();
    for(int i=7;i<13;i++)
        rowTmp.push_back(i);
    pageTmp.push_back(rowTmp);
    rowTmp.clear();
    for(int i=13;i<19;i++)
        rowTmp.push_back(i);
    pageTmp.push_back(rowTmp);
    rowTmp.clear();
    for(int i=19;i<25;i++)
        rowTmp.push_back(i);
    pageTmp.push_back(rowTmp);
    rowTmp.clear();
    for(int i=25;i<26;i++)
        rowTmp.push_back(i);
    pageTmp.push_back(rowTmp);
    rowTmp.clear();
   blockPages.push_back(pageTmp);
   pageTmp.clear();
   
   
   pageNames.push_back("Move Horizontal");
   for(int i=201;i<205;i++)
        rowTmp.push_back(i);
    pageTmp.push_back(rowTmp);
    rowTmp.clear();
    blockPages.push_back(pageTmp);
    pageTmp.clear();
   
    
   pageNames.push_back("Move Vertical");
   for(int i=251;i<256;i++)
        rowTmp.push_back(i);
    pageTmp.push_back(rowTmp);
    rowTmp.clear();
    blockPages.push_back(pageTmp);
    pageTmp.clear();
    
    
   pageNames.push_back("Sem colissao");
   for(int i=351;i<357;i++)
        rowTmp.push_back(i);
    pageTmp.push_back(rowTmp);
    rowTmp.clear();
    for(int i=357;i<363;i++)
        rowTmp.push_back(i);
    pageTmp.push_back(rowTmp);
    rowTmp.clear();
    for(int i=363;i<369;i++)
        rowTmp.push_back(i);
    pageTmp.push_back(rowTmp);
    rowTmp.clear();
    for(int i=369;i<370;i++)
        rowTmp.push_back(i);
    pageTmp.push_back(rowTmp);
    rowTmp.clear();
    blockPages.push_back(pageTmp);
    pageTmp.clear();
    
    pageNames.push_back("PowerUps");
    for(int i=476;i<480;i++)
        rowTmp.push_back(i);
    pageTmp.push_back(rowTmp);
    rowTmp.clear();
    for(int i=301;i<305;i++)
        rowTmp.push_back(i);
    pageTmp.push_back(rowTmp);
    rowTmp.clear();
    blockPages.push_back(pageTmp);
    pageTmp.clear();
    
    pageNames.push_back("Destrutivos");
    for(int i=326;i<331;i++)
        rowTmp.push_back(i);
    pageTmp.push_back(rowTmp);
    rowTmp.clear();
    blockPages.push_back(pageTmp);
    pageTmp.clear();
    
    pageNames.push_back("Decorativo");
    for(int i=401;i<407;i++)
        rowTmp.push_back(i);
    pageTmp.push_back(rowTmp);
    rowTmp.clear();
    for(int i=407;i<413;i++)
        rowTmp.push_back(i);
    pageTmp.push_back(rowTmp);
    rowTmp.clear();
    blockPages.push_back(pageTmp);
    pageTmp.clear();
    
    pageNames.push_back("Liquidos");
    rowTmp.push_back(380);
    rowTmp.push_back(378);
    rowTmp.push_back(381);
    rowTmp.push_back(379);
    pageTmp.push_back(rowTmp);
    rowTmp.clear();
    blockPages.push_back(pageTmp);
    pageTmp.clear();
    
    pageNames.push_back("Especiais");
    rowTmp.push_back(101);
    rowTmp.push_back(102);
    rowTmp.push_back(200);
    rowTmp.push_back(501);
    rowTmp.push_back(451);
    rowTmp.push_back(452);
    rowTmp.push_back(453);  
    pageTmp.push_back(rowTmp);
    rowTmp.clear();
    rowTmp.push_back(453);
    rowTmp.push_back(454);
    rowTmp.push_back(4001);
    rowTmp.push_back(4002);
    rowTmp.push_back(666);
    pageTmp.push_back(rowTmp);
    rowTmp.clear();
    blockPages.push_back(pageTmp);
    pageTmp.clear();
    
    pageNames.push_back("Teletransporte");
    for(int i=-1;i>-7;i--)
        rowTmp.push_back(i);
    pageTmp.push_back(rowTmp);
    rowTmp.clear();
    for(int i=-7;i>-13;i--)
        rowTmp.push_back(i);
    pageTmp.push_back(rowTmp);
    rowTmp.clear();
    for(int i=-201;i>-207;i--)
        rowTmp.push_back(i);
    pageTmp.push_back(rowTmp);
    rowTmp.clear();
    for(int i=-207;i>-213;i--)
        rowTmp.push_back(i);
    pageTmp.push_back(rowTmp);
    rowTmp.clear();
    blockPages.push_back(pageTmp);
    pageTmp.clear();
    
    pageNames.push_back("Spawn");
    rowTmp.push_back(1000);
    rowTmp.push_back(0);
    rowTmp.push_back(0);
    rowTmp.push_back(0);
    rowTmp.push_back(0);
    rowTmp.push_back(0);
    pageTmp.push_back(rowTmp);
    rowTmp.clear();
    for(int i=2001;i<2007;i++)
        rowTmp.push_back(i);
    pageTmp.push_back(rowTmp);
    rowTmp.clear();
    for(int i=5001;i<5007;i++)
        rowTmp.push_back(i);
    pageTmp.push_back(rowTmp);
    rowTmp.clear();
    blockPages.push_back(pageTmp);
    pageTmp.clear();
    
}

void mapEdit::load(int idx){
    map.clear();
    if(idx>=0){
        index=idx;
        map=maps[idx].map;
        currentBackground=maps[idx].backgroundId;
    }else{
        index=-1;
        map=usrMap.map;
        currentBackground=usrMap.backgroundId;
    }
    size.y=map.size();
    size.x=map[0].size();
}

void mapEdit::reset(){
    size=Util::nTPointSet(0,0,1);
    scale=Util::nTPointSet(0.8,0.8,1);
    currentBlock=0;
    currentBackground=0;
    pageIndex=0;
    input="";
    isCreating=0;
    isUser=true;
}

void mapEdit::save(){
    nTMap tmp;
    tmp.backgroundId=currentBackground;
    tmp.map=map;
    if(!isUser){
        if(index>=maps.size()&&!(index<0))
            maps.push_back(tmp);
        else if(index>=0)
            maps[index]=tmp;
        else
            usrMap=tmp;
    }else
        usrMap=tmp;
    
    if(isUser){
        saveMap("Maps/user.map",-1);
    }else{
        if(index<0)
             saveMap("Maps/user.map",-1);
        else{
            char buffer[5];
            snprintf(buffer,5,"%d",index);
            string str(buffer);
            saveMap((char*)("Maps/map"+str+".map").c_str(),index);
        }
    }
}

void mapEdit::drawLines(){  
    nTColor tmp=GL::getColor();
    GL::setColor(GL::getColorByName("white"));
    glLineWidth(1);
    for(int i=0;i<=size.y;i++){
        glBegin(GL_LINE_STRIP);
        for(int j=0;j<=size.x;j++)
            glVertex3f(j*Blocks::defaultBlockSize.x*scale.x,i*Blocks::defaultBlockSize.y*scale.y,-0.5);
        glEnd();
    }
    for(int j=0;j<=size.x;j++){
        glBegin(GL_LINE_STRIP);
        for(int i=0;i<=size.y;i++)
            glVertex3f(j*Blocks::defaultBlockSize.x*scale.x,i*Blocks::defaultBlockSize.y*scale.y,-0.5);
        glEnd();
    }
    GL::setColor(GL::getColorByName("blue"));
    
    glLineWidth(2.5);
    glBegin(GL_LINE_LOOP);
        glVertex3f(0,0,-0.49);
        glVertex3f((size.x)*Blocks::defaultBlockSize.x*scale.x,-0,0.49);
        glVertex3f((size.x)*Blocks::defaultBlockSize.x*scale.x,(size.y)*Blocks::defaultBlockSize.y*scale.y,-0.49);
        glVertex3f(0,(size.y)*Blocks::defaultBlockSize.y*scale.y,-0.49);
    glEnd(); 
    GL::setColor(tmp);
}

void mapEditButton(int x,int y){
    x/=mapEdit::scale.x;
    y/=mapEdit::scale.y;
    if(x>=Scenes::camera.x.movedCam/mapEdit::scale.x&&x<(mapEdit::HUDarea.x+Scenes::camera.x.movedCam)/mapEdit::scale.x&&y>=Scenes::camera.y.movedCam/mapEdit::scale.y&&y<(mapEdit::HUDarea.y+Scenes::camera.y.movedCam)/mapEdit::scale.y){
        int i=round(((y-Blocks::defaultBlockSize.y/2)/Blocks::defaultBlockSize.y)-(1/2));
        int j=round(((x-Blocks::defaultBlockSize.x/2)/Blocks::defaultBlockSize.x)-(1/2));
        if(i<0)
            i=0;
        if(j<0)
            j=0;
        if(i>=mapEdit::map.size())
            i=mapEdit::map.size()-1;
        if(j>=mapEdit::map[0].size())
            j=mapEdit::map[0].size()-1;
        if(mapEdit::currentBlock<60000)
            mapEdit::map[i][j]=mapEdit::currentBlock;
        else
            mapEdit::currentBlock=mapEdit::map[i][j];
    }
}

void mapEraseButton(int x,int y){
    x/=mapEdit::scale.x;
    y/=mapEdit::scale.y;
    if(x>=Scenes::camera.x.movedCam/mapEdit::scale.x&&x<(mapEdit::HUDarea.x+Scenes::camera.x.movedCam)/mapEdit::scale.x&&y>=Scenes::camera.y.movedCam/mapEdit::scale.y&&y<(mapEdit::HUDarea.y+Scenes::camera.y.movedCam)/mapEdit::scale.y){
        int i=round(((y-Blocks::defaultBlockSize.y/2)/Blocks::defaultBlockSize.y)-(1/2));
        int j=round(((x-Blocks::defaultBlockSize.x/2)/Blocks::defaultBlockSize.x)-(1/2));
        if(i<0)
            i=0;
        if(j<0)
            j=0;
        if(i>=mapEdit::map.size())
            i=mapEdit::map.size()-1;
        if(j>=mapEdit::map[0].size())
            j=mapEdit::map[0].size()-1;
        mapEdit::map[i][j]=0;
    }
}

void mapEditSetBlock(int x,int y){
    x-=Scenes::camera.x.movedCam;
    y-=Scenes::camera.y.movedCam;
    if(x>=(mapEdit::HUDarea.x)){
        int i=floor((y-100+16)/25);
        int j=floor((x-663+16)/25);
        if(i<0)
            i=0;
        if(j<0)
            j=0;
        if(i>=mapEdit::blockPages[mapEdit::pageIndex].size())
            i=mapEdit::blockPages[mapEdit::pageIndex].size()-1;
        if(j>=mapEdit::blockPages[mapEdit::pageIndex][0].size())
            j=mapEdit::blockPages[mapEdit::pageIndex][0].size()-1;
        if(mapEdit::blockPages[mapEdit::pageIndex][i][j])
        if(!(mapEdit::blockPages[mapEdit::pageIndex][i][j]>=451&&mapEdit::blockPages[mapEdit::pageIndex][i][j]<=475))
            mapEdit::currentBlock=mapEdit::blockPages[mapEdit::pageIndex][i][j];
    }
}

void mapEdit::draw(){
    glNewList(GLlist,GL_COMPILE_AND_EXECUTE);
    char buffer[5];
    snprintf(buffer,5,"%d",currentBackground);
    string mapID(buffer);
    GL::drawTexture(Util::nTRectangleSet(Scenes::camera.x.movedCam,mapEdit::HUDarea.y+Scenes::camera.y.movedCam,mapEdit::HUDarea.x+Scenes::camera.x.movedCam,Scenes::camera.y.movedCam,-0.9,-0.9),Util::nTColorSet(1,1,1,1),GL::getTextureByName("background"+mapID),1);
    drawPanel();
    drawLines();
    Blocks *bl;
    nTPoint tmp,tmp1;
    for(int i=0;i<size.y;i++){
        for(int j=0;j<size.x;j++){
                tmp=Util::nTPointSet(Blocks::defaultBlockSize.x*(j+(1/2))+Blocks::defaultBlockSize.x/2,Blocks::defaultBlockSize.y*(i+(1/2))+Blocks::defaultBlockSize.y/2,Blocks::defaultBlockSize.z);
                tmp1=Blocks::defaultBlockSize;
                tmp.x*=scale.x;
                tmp.y*=scale.y;
                tmp1.x*=scale.x;
                tmp1.y*=scale.y;
                GL::buttonBehave(Util::getCollisionRectangle(tmp,tmp1),Util::nTColorSet(0.38,0.38,0.38,0.8),0,*mapEditButton,NULL,*mapEraseButton,NULL);
            if(map[i][j]){
                tmp.z-=0.1;
                tmp1.z-=0.1;
                bl = new Blocks(map[i][j],tmp,tmp1);
                bl->draw();
                delete bl;
            }
        }
    }
    glEndList();
}

void mapEditSetAir(int x,int y){
    mapEdit::currentBlock=0;
}

void mapEditGetBlock(int x,int y){
    mapEdit::currentBlock=60000;
}

void mapEditZoomIn(int x,int y){
    if(mapEdit::scale.x<1.99){
        mapEdit::scale.x+=0.01;
        mapEdit::scale.y+=0.01;
    }
}

void mapEditZoomOut(int x,int y){
    if(mapEdit::scale.x>0.01){
        mapEdit::scale.x-=0.01;
        mapEdit::scale.y-=0.01;
    }
}

void mapEditBackgroundUp(int x,int y){
    mapEdit::currentBackground++;
    if(mapEdit::currentBackground>=Map::nOfBackgrounds)
         mapEdit::currentBackground=Map::nOfBackgrounds-1;
}

void mapEditBackgroundDown(int x,int y){
    mapEdit::currentBackground--;
    if(mapEdit::currentBackground<0)
         mapEdit::currentBackground=0;
}

void mapEditCamera(int x,int y){
    Scenes::putCameraOnOrigin();
}

void mapEditPageUp(int x,int y){
    mapEdit::pageIndex--;
    if(mapEdit::pageIndex<0)
        mapEdit::pageIndex=mapEdit::blockPages.size()-1;
}

void mapEditPageDown(int x,int y){
    mapEdit::pageIndex++;
    if(mapEdit::pageIndex>=mapEdit::blockPages.size())
        mapEdit::pageIndex=0;
}

void mapEditSave(int x,int y){
    mapEdit::save();
}

void mapEditLeave(int x,int y){
    Scenes::current=Scenes::menu;
}

void mapEdit::drawPanel(){
    GL::drawRectangle(Util::nTRectangleSet(Scenes::camera.x.movedCam,GL::defaultSize.y+Scenes::camera.y.movedCam,GL::defaultSize.x+Scenes::camera.x.movedCam,HUDarea.y+Scenes::camera.y.movedCam,0,0),GL::getColorByName("grey"));
    GL::drawRectangle(Util::nTRectangleSet(HUDarea.x+Scenes::camera.x.movedCam,GL::defaultSize.y+Scenes::camera.y.movedCam,GL::defaultSize.x+Scenes::camera.x.movedCam,Scenes::camera.y.movedCam,0,0),GL::getColorByName("grey"));
    
    GL::buttonBehave(Util::nTRectangleSet(20+Scenes::camera.x.movedCam,570+Scenes::camera.y.movedCam,50+Scenes::camera.x.movedCam,533+Scenes::camera.y.movedCam,1,1),GL::getColorByName("mouseSelected"),GL::getTextureByName("Ar"),*mapEditSetAir,NULL,NULL,NULL);
    GL::buttonBehave(Util::nTRectangleSet(70+Scenes::camera.x.movedCam,570+Scenes::camera.y.movedCam,100+Scenes::camera.x.movedCam,533+Scenes::camera.y.movedCam,1,1),GL::getColorByName("mouseSelected"),GL::getTextureByName("Zoom-"),*mapEditZoomOut,NULL,NULL,NULL);
    GL::buttonBehave(Util::nTRectangleSet(110+Scenes::camera.x.movedCam,570+Scenes::camera.y.movedCam,140+Scenes::camera.x.movedCam,533+Scenes::camera.y.movedCam,1,1),GL::getColorByName("mouseSelected"),GL::getTextureByName("Zoom+"),*mapEditZoomIn,NULL,NULL,NULL);
    GL::buttonBehave(Util::nTRectangleSet(150+Scenes::camera.x.movedCam,570+Scenes::camera.y.movedCam,180+Scenes::camera.x.movedCam,533+Scenes::camera.y.movedCam,1,1),GL::getColorByName("mouseSelected"),GL::getTextureByName("Background-"),NULL,*mapEditBackgroundDown,NULL,NULL);
    GL::buttonBehave(Util::nTRectangleSet(190+Scenes::camera.x.movedCam,570+Scenes::camera.y.movedCam,220+Scenes::camera.x.movedCam,533+Scenes::camera.y.movedCam,1,1),GL::getColorByName("mouseSelected"),GL::getTextureByName("Background+"),NULL,*mapEditBackgroundUp,NULL,NULL);
    GL::buttonBehave(Util::nTRectangleSet(230+Scenes::camera.x.movedCam,570+Scenes::camera.y.movedCam,280+Scenes::camera.x.movedCam,533+Scenes::camera.y.movedCam,1,1),GL::getColorByName("mouseSelected"),GL::getTextureByName("CameraDefault"),*mapEditCamera,NULL,NULL,NULL);
    GL::buttonBehave(Util::nTRectangleSet(290+Scenes::camera.x.movedCam,570+Scenes::camera.y.movedCam,320+Scenes::camera.x.movedCam,533+Scenes::camera.y.movedCam,1,1),GL::getColorByName("mouseSelected"),GL::getTextureByName("Get Block"),*mapEditGetBlock,NULL,NULL,NULL);
    GL::buttonBehave(Util::nTRectangleSet(330+Scenes::camera.x.movedCam,570+Scenes::camera.y.movedCam,360+Scenes::camera.x.movedCam,533+Scenes::camera.y.movedCam,1,1),GL::getColorByName("mouseSelected"),GL::getTextureByName("Save"),*mapEditSave,NULL,NULL,NULL);
    GL::buttonBehave(Util::nTRectangleSet(485+Scenes::camera.x.movedCam,570+Scenes::camera.y.movedCam,515+Scenes::camera.x.movedCam,533+Scenes::camera.y.movedCam,1,1),GL::getColorByName("mouseSelected"),GL::getTextureByName("Back"),*mapEditLeave,NULL,NULL,NULL);
    GL::buttonBehave(Util::nTRectangleSet(770+Scenes::camera.x.movedCam,60+Scenes::camera.y.movedCam,790+Scenes::camera.x.movedCam,40+Scenes::camera.y.movedCam,1,1),GL::getColorByName("mouseSelected"),GL::getTextureByName("-"),NULL,*mapEditPageDown,NULL,NULL);
    GL::buttonBehave(Util::nTRectangleSet(660+Scenes::camera.x.movedCam,60+Scenes::camera.y.movedCam,680+Scenes::camera.x.movedCam,40+Scenes::camera.y.movedCam,1,1),GL::getColorByName("mouseSelected"),GL::getTextureByName("+"),NULL,*mapEditPageUp,NULL,NULL);
    GL::setFontByIndex(5);
    GL::drawText(Util::nTPointSet(550+Scenes::camera.x.movedCam,530+Scenes::camera.y.movedCam,1),"Bloco Atual",GL::getColorByName("black"));
    GL::drawText(Util::nTPointSet(550+Scenes::camera.x.movedCam,582+Scenes::camera.y.movedCam,1),(char*)(Blocks::getTexNameByIndex(currentBlock)+getMoreInfoAboutBlocks(currentBlock)).c_str(),GL::getColorByName("black"));
    if(currentBlock<60000&&currentBlock!=0)
        GL::drawTexture(Util::nTRectangleSet(550+Scenes::camera.x.movedCam,570+Scenes::camera.y.movedCam,580+Scenes::camera.x.movedCam,533+Scenes::camera.y.movedCam,1,1),GL::getColorByName("white"),GL::getTextureByName(Blocks::getTexNameByIndex(currentBlock)),1);
    else if(currentBlock==0)
        GL::drawTexture(Util::nTRectangleSet(550+Scenes::camera.x.movedCam,570+Scenes::camera.y.movedCam,580+Scenes::camera.x.movedCam,533+Scenes::camera.y.movedCam,1,1),GL::getColorByName("white"),GL::getTextureByName("Ar"),1);
    else
        GL::drawTexture(Util::nTRectangleSet(550+Scenes::camera.x.movedCam,570+Scenes::camera.y.movedCam,580+Scenes::camera.x.movedCam,533+Scenes::camera.y.movedCam,1,1),GL::getColorByName("white"),GL::getTextureByName("Get Block"),1);

    GL::setFontByIndex(2);
    int dx=GL::mousePos.x-Scenes::camera.x.movedCam;
    int dy=GL::mousePos.y-Scenes::camera.y.movedCam;
    int blockID=0;
    if(dx>=(mapEdit::HUDarea.x)){
        int di=floor((dy-100+16)/25);
        int dj=floor((dx-663+16)/25);
        if(di>=0&&di<blockPages[mapEdit::pageIndex].size())
        if(dj>=0&&dj<blockPages[mapEdit::pageIndex][0].size())
            blockID=mapEdit::blockPages[mapEdit::pageIndex][di][dj];
    }
    string strT=Blocks::getTexNameByIndex(blockID);
    if(strT!=""){
        strT+=getMoreInfoAboutBlocks(blockID);
        nTPoint poinT=GL::mousePos;
        poinT.x-=10*strT.size();
        poinT.z=1;
        GL::drawText(poinT,(char*)strT.c_str(),GL::getColorByName("black")); 
    }
    GL::setFontByIndex(0);
    if(!isUser)
        GL::drawText(Util::nTPointSet(500+Scenes::camera.x.movedCam,590+Scenes::camera.y.movedCam,1),"MODO DESENVOLVEDOR",GL::getColorByName("violet"));
    GL::setFontByIndex(3);
    GL::drawText(Util::nTPointSet(686+Scenes::camera.x.movedCam,55+Scenes::camera.y.movedCam,1),(char*)pageNames[pageIndex].c_str(),GL::getColorByName("yellow"));
        for(int i=0;i<blockPages[pageIndex].size();i++){
        for(int j=0;j<blockPages[pageIndex][i].size();j++){
            nTRectangle tmp;
            if(blockPages[pageIndex][i][j]>=201&&blockPages[pageIndex][i][j]<=300)
                tmp=Util::getCollisionRectangle(Util::nTPointSet(663+25*j,100+25*i,1),Util::nTPointSet(16,8,0));
            else
                tmp=Util::getCollisionRectangle(Util::nTPointSet(663+25*j,100+25*i,1),Util::nTPointSet(16,16,0));
            tmp.p0.x+=Scenes::camera.x.movedCam;tmp.p1.x+=Scenes::camera.x.movedCam;
            tmp.p0.y+=Scenes::camera.y.movedCam;tmp.p1.y+=Scenes::camera.y.movedCam;
            GL::buttonBehave(tmp,GL::getColorByName("mouseSelected"),GL::getTextureByName(Blocks::getTexNameByIndex(blockPages[pageIndex][i][j])),*mapEditSetBlock,(void(*)(int, int))NULL,NULL,NULL);           
        }
    }

}

void mapEdit::askForSize(){
    glNewList(GLlist,GL_COMPILE_AND_EXECUTE);
    GL::setFontByIndex(0);
    GL::drawText(Util::nTPointSet(200,150,1),"Digite os valores e apos digitar precione enter:",GL::getColorByName("red"));
    GL::setFontByIndex(6);
    GL::drawText(Util::nTPointSet(200,400,1),"Seu mapa fica salvo em ./Maps/user.map",GL::getColorByName("red"));
    GL::drawText(Util::nTPointSet(200,200,1),"Digite o numero de linhas do mapa(19):",GL::getColorByName("red"));
    GL::drawText(Util::nTPointSet(200,250,1),"Digite o numero de colunas do mapa:",GL::getColorByName("red"));
    if(size.y==0){
        if(input=="")
            GL::drawText(Util::nTPointSet(520,200,1),(char*)blankField.c_str(),GL::getColorByName("blue"));
        else
            GL::drawText(Util::nTPointSet(520,200,1),(char*)input.c_str(),GL::getColorByName("blue"));
    }else{
        char buffer[10];
        snprintf(buffer,10,"%f",size.y);
        GL::drawText(Util::nTPointSet(520,200,1),buffer,GL::getColorByName("red"));
        if(input=="")
            GL::drawText(Util::nTPointSet(520,250,1),(char*)blankField.c_str(),GL::getColorByName("blue"));
        else
            GL::drawText(Util::nTPointSet(520,250,1),(char*)input.c_str(),GL::getColorByName("blue"));
    }
    glEndList();
}

void mapEdit::askForLoad(){
    glNewList(GLlist,GL_COMPILE_AND_EXECUTE);
    GL::setFontByIndex(0);
    GL::drawText(Util::nTPointSet(200,150,1),"Digite os valores e apos digitar precione enter:",GL::getColorByName("red"));
    GL::setFontByIndex(6);
    GL::drawText(Util::nTPointSet(200,400,1),"Seu mapa fica salvo em ./Maps/user.map",GL::getColorByName("red"));
        char buffer[10];
        snprintf(buffer,10,"%ld",maps.size()-1);
        string maxMap(buffer);
    GL::drawText(Util::nTPointSet(200,200,1),(char*)("Digite qual mapa quer carregar(0-"+maxMap+")(u):").c_str(),GL::getColorByName("red"));
        if(input=="")
            GL::drawText(Util::nTPointSet(530,200,1),(char*)blankField.c_str(),GL::getColorByName("blue"));
        else
            GL::drawText(Util::nTPointSet(530,200,1),(char*)input.c_str(),GL::getColorByName("blue"));
    glEndList();
}
    
void mapEdit::setMapSize(){
    index=maps.size();
    map.clear();
    vector<int> tmp;
    for(int i=0;i<size.y;i++){
        for(int j=0;j<size.x;j++)
            tmp.push_back(0);
        map.push_back(tmp);
        tmp.clear();
    }
} 

string mapEdit::getMoreInfoAboutBlocks(int block){
    string out="";
    switch(block){
        case 18: out=" invisivel";break;
        case 476:case 301: out="(Upgrade Espada)";break;
        case 477:case 302: out="(Vida Extra)";break;
        case 478:case 303: out="(Cafe)";break;
        case 479:case 304: out="(Debuff)";break;
        case -1:case-201: out="(1)";break;
        case -2:case-202: out="(2)";break;
        case -3:case-203: out="(3)";break;
        case -4:case-204: out="(4)";break;
        case -5:case-205: out="(5)";break;
        case -6:case-206: out="(6)";break;
        case -7:case-207: out="(7)";break;
        case -8:case-208: out="(8)";break;
        case -9:case-209: out="(9)";break;
        case -10:case-210: out="(10)";break;
        case -11:case-211: out="(11)";break;
        case -12:case-212: out="(12)";break;
        case 1000: out="(Jogador)";break;
    }
    if(block>2000&&block<3000)
        out="(Inimigo)";
    if(block>5000&&block<60000)
        out="(Boss)";
    return out;
}
