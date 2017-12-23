#include "mapEdit.hpp"


mapEdit::mapEdit() {
}

mapEdit::mapEdit(const mapEdit& orig) {
}

mapEdit::~mapEdit() {
}

nTPoint mapEdit::scale=nTPoint::get(0.8,0.8,1);
nTPoint mapEdit::HUDarea=nTPoint::get(650,500,1);
nTPoint mapEdit::size=nTPoint::get(0,0,1);
string mapEdit::input;
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

/**
 *	Set the page names and contents of the available blocks on the map editor
**/
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
    rowTmp.push_back(380); // agua animada
    rowTmp.push_back(378); // agua estatica
    rowTmp.push_back(381); // lava animada
    rowTmp.push_back(379); // lava estatica
    pageTmp.push_back(rowTmp);
    rowTmp.clear();
    blockPages.push_back(pageTmp);
    pageTmp.clear();

    pageNames.push_back("Especiais");
    rowTmp.push_back(101); // shooter
    rowTmp.push_back(102); // shooter
    rowTmp.push_back(200); // bus shooter
    rowTmp.push_back(501); // jump boost
    rowTmp.push_back(451); // lever
    rowTmp.push_back(452); // lever
    pageTmp.push_back(rowTmp);
    rowTmp.clear();
    rowTmp.push_back(453); // lever
    rowTmp.push_back(454); // lever
    rowTmp.push_back(4001); // checkpoint
    rowTmp.push_back(4002); // checkpoint
    rowTmp.push_back(666); // end level
    rowTmp.push_back(526); // enemy control
    pageTmp.push_back(rowTmp);
    rowTmp.clear();
    rowTmp.push_back(527); // tutorial
    rowTmp.push_back(528); // tutorial pause
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

/**
 *	Load a preloaded map
 *
 *	@param idx index of the map
**/
void mapEdit::load(int idx){
    map.clear();
    if(idx>=0){
        index=idx;
        map=Map::maps[idx].map;
        //currentBackground=Map::maps[idx].backgroundId;
    }else{
        index=-1;
        map=Map::usrMap.map;
        //currentBackground=Map::usrMap.backgroundId;
    }
    size.y=map.size();
    size.x=map[0].size();
}

/**
 *	Reset the map and eviroment variables
**/
void mapEdit::reset(){
    size=nTPoint::get(0,0,1);
    scale=nTPoint::get(0.8,0.8,1);
    currentBlock=0;
    currentBackground=0;
    pageIndex=0;
    input="";
    isCreating=0;
    isUser=true;
}

/**
 *	Save the map to a file
**/
void mapEdit::save(){
    nTMap tmp;
    //tmp.backgroundId=currentBackground;
    tmp.map=map;
    if(!isUser){
        if(index>=Map::maps.size()&&!(index<0))
            Map::maps.push_back(tmp);
        else if(index>=0)
            Map::maps[index]=tmp;
        else
            Map::usrMap=tmp;
    }else
        Map::usrMap=tmp;

    if(isUser){
        Map::saveMap(Util::newPath("Maps/user.map"),-1);
    }else{
        if(index<0)
             Map::saveMap(Util::newPath("Maps/user.map"),-1);
        else{
            char buffer[5];
            snprintf(buffer,5,"%d",index);
            string str(buffer);
            Map::saveMap(Util::newPath(("Maps/map"+str+".map")),index);
        }
    }
}

/**
 *	Draw map edit grid lines
**/
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

/**
 *	Mouse event callback to Edit Button
 *
 *	@param x mouse x position
 *	@param y mouse y position
**/
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
            i=(int)mapEdit::map.size()-1;
        if(j>=mapEdit::map[0].size())
            j=(int)mapEdit::map[0].size()-1;
        if(mapEdit::currentBlock<60000)
            mapEdit::map[i][j]=mapEdit::currentBlock;
        else
            mapEdit::currentBlock=mapEdit::map[i][j];
    }
}

/**
 *	Mouse event callback to Erase Button
 *
 *	@param x mouse x position
 *	@param y mouse y position
**/
void mapEraseButton(int x,int y){
    x/=mapEdit::scale.x;
    y/=mapEdit::scale.y;
    if(x>=Scenes::camera.x.movedCam/mapEdit::scale.x&&x<(mapEdit::HUDarea.x+Scenes::camera.x.movedCam)/mapEdit::scale.x&&y>=Scenes::camera.y.movedCam/mapEdit::scale.y&&y<(mapEdit::HUDarea.y+Scenes::camera.y.movedCam)/mapEdit::scale.y){
        long i=round(((y-Blocks::defaultBlockSize.y/2)/Blocks::defaultBlockSize.y)-(1/2));
        long j=round(((x-Blocks::defaultBlockSize.x/2)/Blocks::defaultBlockSize.x)-(1/2));
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

/**
 *	Mouse event callback to SetBlock Button
 *
 *	@param x mouse x position
 *	@param y mouse y position
**/
void mapEditSetBlock(int x,int y){
    x-=Scenes::camera.x.movedCam;
    y-=Scenes::camera.y.movedCam;
    if(x>=(mapEdit::HUDarea.x)){
        long i=floor((y-100+16)/25);
        long j=floor((x-663+16)/25);
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
/**
 *	Draw everything on the screen
**/
void mapEdit::draw(){
    char buffer[5];
    snprintf(buffer,5,"%d",currentBackground);
    string mapID(buffer);
    GL::drawTexture(nTRectangle::get(Scenes::camera.x.movedCam,mapEdit::HUDarea.y+Scenes::camera.y.movedCam,mapEdit::HUDarea.x+Scenes::camera.x.movedCam,Scenes::camera.y.movedCam,-0.9),GL::getTextureByName("background"+mapID));
    drawPanel();
    drawLines();
    Blocks *bl;
    nTPoint tmp,tmp1;
    for(int i=0;i<size.y;i++){
        for(int j=0;j<size.x;j++){
                tmp=nTPoint::get(Blocks::defaultBlockSize.x*(j+(1/2))+Blocks::defaultBlockSize.x/2,Blocks::defaultBlockSize.y*(i+(1/2))+Blocks::defaultBlockSize.y/2,Blocks::defaultBlockSize.z);
                tmp1=Blocks::defaultBlockSize;
                tmp.x*=scale.x;
                tmp.y*=scale.y;
                tmp1.x*=scale.x;
                tmp1.y*=scale.y;
                GL::buttonBehave(nTRectangle::getCollision(tmp,tmp1),nTColor::get(0.38,0.38,0.38,0.8),0,true,*mapEditButton,NULL,*mapEraseButton,NULL);
            if(map[i][j]){
                tmp.z-=0.1;
                tmp1.z-=0.1;
                bl = new Blocks(map[i][j],tmp,tmp1);
                bl->draw();
                delete bl;
            }
        }
    }
}

/**
 *	Mouse event callback to Air Button
 *
 *	@param x mouse x position
 *	@param y mouse y position
**/
void mapEditSetAir(int x,int y){
    mapEdit::currentBlock=0;
}

/**
 *	Mouse event callback to GetBlock Button
 *
 *	@param x mouse x position
 *	@param y mouse y position
**/
void mapEditGetBlock(int x,int y){
    mapEdit::currentBlock=60000;
}

/**
 *	Mouse event callback to ZoomIn Button
 *
 *	@param x mouse x position
 *	@param y mouse y position
**/
void mapEditZoomIn(int x,int y){
    if(mapEdit::scale.x<1.99){
        mapEdit::scale.x+=0.01;
        mapEdit::scale.y+=0.01;
    }
}

/**
 *	Mouse event callback to ZoomOut Button
 *
 *	@param x mouse x position
 *	@param y mouse y position
**/
void mapEditZoomOut(int x,int y){
    if(mapEdit::scale.x>0.01){
        mapEdit::scale.x-=0.01;
        mapEdit::scale.y-=0.01;
    }
}

/**
 *	Mouse event callback to BackgroundUp Button
 *
 *	@param x mouse x position
 *	@param y mouse y position
**/
void mapEditBackgroundUp(int x,int y){
    mapEdit::currentBackground++;
    if(mapEdit::currentBackground>=Map::nOfBackgrounds)
         mapEdit::currentBackground=Map::nOfBackgrounds-1;
}

/**
 *	Mouse event callback to BackgroundDown Button
 *
 *	@param x mouse x position
 *	@param y mouse y position
**/
void mapEditBackgroundDown(int x,int y){
    mapEdit::currentBackground--;
    if(mapEdit::currentBackground<0)
         mapEdit::currentBackground=0;
}

/**
 *	Mouse event callback to Camera Button
 *
 *	@param x mouse x position
 *	@param y mouse y position
**/
void mapEditCamera(int x,int y){
    Scenes::camera.putCameraOnOrigin();
}

/**
 *	Mouse event callback to PageUp Button
 *
 *	@param x mouse x position
 *	@param y mouse y position
**/
void mapEditPageUp(int x,int y){
    mapEdit::pageIndex--;
    if(mapEdit::pageIndex<0)
        mapEdit::pageIndex=(int)mapEdit::blockPages.size()-1;
}

/**
 *	Mouse event callback to PageDown Button
 *
 *	@param x mouse x position
 *	@param y mouse y position
**/
void mapEditPageDown(int x,int y){
    mapEdit::pageIndex++;
    if(mapEdit::pageIndex>=mapEdit::blockPages.size())
        mapEdit::pageIndex=0;
}

/**
 *	Mouse event callback to Save Button
 *
 *	@param x mouse x position
 *	@param y mouse y position
**/
void mapEditSave(int x,int y){
    mapEdit::save();
}

/**
 *	Mouse event callback to Leave Button
 *
 *	@param x mouse x position
 *	@param y mouse y position
**/
void mapEditLeave(int x,int y){
    Scenes::current=Scenes::menu;
}

/**
 *	Draw HUD panel on the screen
**/
void mapEdit::drawPanel(){
    GL::drawRectangle(nTRectangle::get(Scenes::camera.x.movedCam,GL::defaultSize.y+Scenes::camera.y.movedCam,GL::defaultSize.x+Scenes::camera.x.movedCam,HUDarea.y+Scenes::camera.y.movedCam,0.8),GL::getColorByName("grey"));
    GL::drawRectangle(nTRectangle::get(HUDarea.x+Scenes::camera.x.movedCam,GL::defaultSize.y+Scenes::camera.y.movedCam,GL::defaultSize.x+Scenes::camera.x.movedCam,Scenes::camera.y.movedCam,0.8),GL::getColorByName("grey"));

    GL::buttonBehave(nTRectangle::get(20+Scenes::camera.x.movedCam,570+Scenes::camera.y.movedCam,50+Scenes::camera.x.movedCam,533+Scenes::camera.y.movedCam,1),GL::getColorByName("mouseSelected"),GL::getTextureByName("Ar"),false,*mapEditSetAir,NULL,NULL,NULL);
    GL::buttonBehave(nTRectangle::get(70+Scenes::camera.x.movedCam,570+Scenes::camera.y.movedCam,100+Scenes::camera.x.movedCam,533+Scenes::camera.y.movedCam,1),GL::getColorByName("mouseSelected"),GL::getTextureByName("Zoom-"),false,*mapEditZoomOut,NULL,NULL,NULL);
    GL::buttonBehave(nTRectangle::get(110+Scenes::camera.x.movedCam,570+Scenes::camera.y.movedCam,140+Scenes::camera.x.movedCam,533+Scenes::camera.y.movedCam,1),GL::getColorByName("mouseSelected"),GL::getTextureByName("Zoom+"),false,*mapEditZoomIn,NULL,NULL,NULL);
    GL::buttonBehave(nTRectangle::get(150+Scenes::camera.x.movedCam,570+Scenes::camera.y.movedCam,180+Scenes::camera.x.movedCam,533+Scenes::camera.y.movedCam,1),GL::getColorByName("mouseSelected"),GL::getTextureByName("Background-"),false,NULL,*mapEditBackgroundDown,NULL,NULL);
    GL::buttonBehave(nTRectangle::get(190+Scenes::camera.x.movedCam,570+Scenes::camera.y.movedCam,220+Scenes::camera.x.movedCam,533+Scenes::camera.y.movedCam,1),GL::getColorByName("mouseSelected"),GL::getTextureByName("Background+"),false,NULL,*mapEditBackgroundUp,NULL,NULL);
    GL::buttonBehave(nTRectangle::get(230+Scenes::camera.x.movedCam,570+Scenes::camera.y.movedCam,280+Scenes::camera.x.movedCam,533+Scenes::camera.y.movedCam,1),GL::getColorByName("mouseSelected"),GL::getTextureByName("CameraDefault"),false,*mapEditCamera,NULL,NULL,NULL);
    GL::buttonBehave(nTRectangle::get(290+Scenes::camera.x.movedCam,570+Scenes::camera.y.movedCam,320+Scenes::camera.x.movedCam,533+Scenes::camera.y.movedCam,1),GL::getColorByName("mouseSelected"),GL::getTextureByName("Get Block"),false,*mapEditGetBlock,NULL,NULL,NULL);
    GL::buttonBehave(nTRectangle::get(330+Scenes::camera.x.movedCam,570+Scenes::camera.y.movedCam,360+Scenes::camera.x.movedCam,533+Scenes::camera.y.movedCam,1),GL::getColorByName("mouseSelected"),GL::getTextureByName("Save"),false,*mapEditSave,NULL,NULL,NULL);
    GL::buttonBehave(nTRectangle::get(485+Scenes::camera.x.movedCam,570+Scenes::camera.y.movedCam,515+Scenes::camera.x.movedCam,533+Scenes::camera.y.movedCam,1),GL::getColorByName("mouseSelected"),GL::getTextureByName("Back"),false,*mapEditLeave,NULL,NULL,NULL);
    GL::buttonBehave(nTRectangle::get(770+Scenes::camera.x.movedCam,60+Scenes::camera.y.movedCam,790+Scenes::camera.x.movedCam,40+Scenes::camera.y.movedCam,1),GL::getColorByName("mouseSelected"),GL::getTextureByName("-"),false,NULL,*mapEditPageDown,NULL,NULL);
    GL::buttonBehave(nTRectangle::get(660+Scenes::camera.x.movedCam,60+Scenes::camera.y.movedCam,680+Scenes::camera.x.movedCam,40+Scenes::camera.y.movedCam,1),GL::getColorByName("mouseSelected"),GL::getTextureByName("+"),false,NULL,*mapEditPageUp,NULL,NULL);
    GL::setFont("BITMAP_HELVETICA_12");
    GL::drawText(nTPoint::get(550+Scenes::camera.x.movedCam,536+Scenes::camera.y.movedCam,1),"Bloco Atual",GL::getColorByName("black"));
    GL::drawText(nTPoint::get(550+Scenes::camera.x.movedCam,588+Scenes::camera.y.movedCam,1),Blocks::getTexNameByIndex(currentBlock)+getMoreInfoAboutBlocks(currentBlock),GL::getColorByName("black"));
    if(currentBlock<60000&&currentBlock!=0)
        GL::drawTexture(nTRectangle::get(550+Scenes::camera.x.movedCam,570+Scenes::camera.y.movedCam,580+Scenes::camera.x.movedCam,533+Scenes::camera.y.movedCam,1),GL::getTextureByName(Blocks::getTexNameByIndex(currentBlock)));
    else if(currentBlock==0)
        GL::drawTexture(nTRectangle::get(550+Scenes::camera.x.movedCam,570+Scenes::camera.y.movedCam,580+Scenes::camera.x.movedCam,533+Scenes::camera.y.movedCam,1),GL::getTextureByName("Ar"));
    else
        GL::drawTexture(nTRectangle::get(550+Scenes::camera.x.movedCam,570+Scenes::camera.y.movedCam,580+Scenes::camera.x.movedCam,533+Scenes::camera.y.movedCam,1),GL::getTextureByName("Get Block"));

    GL::setFont("BITMAP_9_BY_15");
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
        GL::drawText(poinT,strT,GL::getColorByName("black"));
        GL::drawRectangle(nTRectangle::get(GL::mousePos.x-5,GL::mousePos.y-2,poinT.x,GL::mousePos.y-19,1),GL::getColorByName("white"));
    }
    GL::setFont("BITMAP_TIMES_ROMAN_24");
    if(!isUser)
        GL::drawText(nTPoint::get(500+Scenes::camera.x.movedCam,590+Scenes::camera.y.movedCam,1),"MODO DESENVOLVEDOR",GL::getColorByName("violet"));
    GL::setFont("BITMAP_TIMES_ROMAN_10");
    GL::drawText(nTPoint::get(686+Scenes::camera.x.movedCam,55+Scenes::camera.y.movedCam,1),pageNames[pageIndex],GL::getColorByName("yellow"));
        for(int i=0;i<blockPages[pageIndex].size();i++){
        for(int j=0;j<blockPages[pageIndex][i].size();j++){
            nTRectangle tmp;
            if(blockPages[pageIndex][i][j]>=201&&blockPages[pageIndex][i][j]<=300)
                tmp=nTRectangle::getCollision(nTPoint::get(663+25*j,100+25*i,1),nTPoint::get(16,8));
            else
                tmp=nTRectangle::getCollision(nTPoint::get(663+25*j,100+25*i,1),nTPoint::get(16,16));
            tmp.p0.x+=Scenes::camera.x.movedCam;tmp.p1.x+=Scenes::camera.x.movedCam;
            tmp.p0.y+=Scenes::camera.y.movedCam;tmp.p1.y+=Scenes::camera.y.movedCam;
            GL::buttonBehave(tmp,GL::getColorByName("mouseSelected"),GL::getTextureByName(Blocks::getTexNameByIndex(blockPages[pageIndex][i][j])),false,*mapEditSetBlock,(void(*)(int, int))NULL,NULL,NULL);
        }
    }

    GL::setFont("BITMAP_HELVETICA_18");
    GL::drawCentered_MultilineX_Text(nTPoint::get(725+Scenes::camera.x.movedCam,272+Scenes::camera.y.movedCam,1),"Selected block\n data:",GL::getColorByName("green"));
    GL::editTextBehave(nTRectangle::get(660+Scenes::camera.x.movedCam,290+Scenes::camera.y.movedCam,790+Scenes::camera.x.movedCam,499+Scenes::camera.y.movedCam,1),"BITMAP_TIMES_ROMAN_10","mapEdit::blockData",false);

}

/**
 *	Draw, behave and handle events of NewMap sub scene on the screen
**/
void mapEdit::askForSize(){
    GL::setFont("BITMAP_TIMES_ROMAN_24");
    GL::drawText(nTPoint::get(200,150,1),"Digite os valores e apos digitar precione enter:",GL::getColorByName("red"));
    GL::setFont("BITMAP_HELVETICA_18");
    GL::drawText(nTPoint::get(200,400,1),"Seu mapa fica salvo em ./Maps/user.map",GL::getColorByName("red"));
    GL::drawText(nTPoint::get(200,200,1),"Digite o numero de linhas do mapa(19):",GL::getColorByName("red"));
    GL::drawText(nTPoint::get(200,250,1),"Digite o numero de colunas do mapa:",GL::getColorByName("red"));
    if(size.y==0){
        if(input=="")
            GL::drawText(nTPoint::get(520,200,1),blankField,GL::getColorByName("blue"));
        else
            GL::drawText(nTPoint::get(520,200,1),input,GL::getColorByName("blue"));
    }else{
        char buffer[10];
        snprintf(buffer,10,"%f",size.y);
        GL::drawText(nTPoint::get(520,200,1),buffer,GL::getColorByName("red"));
        if(input=="")
            GL::drawText(nTPoint::get(520,250,1),blankField,GL::getColorByName("blue"));
        else
            GL::drawText(nTPoint::get(520,250,1),input,GL::getColorByName("blue"));
    }
}

/**
 *	Draw, behave and handle events of Load sub scene on the screen
**/
void mapEdit::askForLoad(){
    GL::setFont("BITMAP_TIMES_ROMAN_24");
    GL::drawText(nTPoint::get(200,150,1),"Digite os valores e apos digitar precione enter:",GL::getColorByName("red"));
    GL::setFont("BITMAP_HELVETICA_18");
    GL::drawText(nTPoint::get(200,400,1),"Seu mapa fica salvo em ./Maps/user.map",GL::getColorByName("red"));
        char buffer[10];
        snprintf(buffer,10,"%d",(Map::maps.size()-1));
        string maxMap(buffer);
    GL::drawText(nTPoint::get(200,200,1),"Digite qual mapa quer carregar(0-"+maxMap+")(u):",GL::getColorByName("red"));
        if(input=="")
            GL::drawText(nTPoint::get(530,200,1),blankField,GL::getColorByName("blue"));
        else
            GL::drawText(nTPoint::get(530,200,1),input,GL::getColorByName("blue"));
}

/**
 *	Reset and resize map vector
**/
void mapEdit::setMapSize(){
    index=(int)Map::maps.size();
    map.clear();
    vector<int> tmp;
    for(int i=0;i<size.y;i++){
        for(int j=0;j<size.x;j++)
            tmp.push_back(0);
        map.push_back(tmp);
        tmp.clear();
    }
}

/**
 *	Gets extra information about the block
 *
 *	@param block the type of the block
 *	@return a string containing the extra information
**/
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
