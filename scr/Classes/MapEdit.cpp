#include "MapEdit.hpp"


MapEdit::MapEdit() {
}

MapEdit::MapEdit(const MapEdit& orig) {
}

MapEdit::~MapEdit() {
}

nTPoint MapEdit::scale=nTPoint::get(0.8,0.8,1);
nTPoint MapEdit::HUDarea=nTPoint::get(650,500,1);
nTPoint MapEdit::size=nTPoint::get(0,0,1);
int MapEdit::currentBlock=1;
nTMap MapEdit::map;
int MapEdit::index;
vector<vector<vector<int> > >MapEdit::blockPages;
vector<string> MapEdit::pageNames;
int MapEdit::pageIndex=0;
short int MapEdit::isCreating=0;
bool MapEdit::isUser=true;
const int MapEdit::editingMap=-6000;

/**
 *	Set the page names and contents of the available blocks on the map editor
**/
void MapEdit::definePages(){
    blockPages.clear();
    pageNames.clear();
    vector<vector<int> > pageTmp;
    vector<int> rowTmp;

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
void MapEdit::load(int idx){
    map.map.clear();
    if(idx>=0){
        index=idx;
        map=Map::maps[idx];
    }else if(idx==-1){
        index=-1;
        map=Map::usrMap;
        //currentBackground=Map::usrMap.backgroundId;
    }else if(idx==MapEdit::editingMap){
        index=-1;
        map=Map::editingMap;
    }
    size.y=map.map.size();
    size.x=map.map[0].size();
}

/**
 *	Reset the map and eviroment variables
**/
void MapEdit::reset(){
    size=nTPoint::get(0,0,1);
    scale=nTPoint::get(0.8,0.8,1);
    currentBlock=0;
    pageIndex=0;
    isCreating=0;
    isUser=true;
}

/**
 *	Save the map to a file
**/
bool MapEdit::save(){
    nTMap tmp;
    //tmp.backgroundId=currentBackground;
    tmp=map;
    if(!isUser){
        if(index>=Map::maps.size()&&!(index<0))
            Map::maps.push_back(tmp);
        else if(index>=0)
            Map::maps[index]=tmp;
        else
            Map::usrMap=tmp;
    }else
        Map::usrMap=tmp;
    Map::editingMap=tmp;
    if(isUser){
        return Map::saveMap(Util::newPath("Maps/user.map"),-1);
    }else{
        if(index<0){
            return Map::saveMap(Util::newPath("Maps/user.map"),-1);
        }else{
            return Map::saveMap(Util::newPath(("Maps/map"+Util::intToStr(index)+".map")),index);
        }
    }
    return false;
}

/**
 *	Draw map edit grid lines
**/
void MapEdit::drawLines(){
    nTColor tmp=GL::getColor();
    GL::setColor(nTColor::White());
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
void MapEditButton(int x,int y){
    x/=MapEdit::scale.x;
    y/=MapEdit::scale.y;
    if(x>=Scenes::camera.x.movedCam/MapEdit::scale.x&&x<(MapEdit::HUDarea.x+Scenes::camera.x.movedCam)/MapEdit::scale.x&&y>=Scenes::camera.y.movedCam/MapEdit::scale.y&&y<(MapEdit::HUDarea.y+Scenes::camera.y.movedCam)/MapEdit::scale.y){
        int i=round(((y-Blocks::defaultBlockSize.y/2)/Blocks::defaultBlockSize.y)-(1/2));
        int j=round(((x-Blocks::defaultBlockSize.x/2)/Blocks::defaultBlockSize.x)-(1/2));
        if(i<0)
            i=0;
        if(j<0)
            j=0;
        if(i>=MapEdit::map.map.size())
            i=(int)MapEdit::map.map.size()-1;
        if(j>=MapEdit::map.map[0].size())
            j=(int)MapEdit::map.map[0].size()-1;
        if(MapEdit::currentBlock!=60000){
            MapEdit::map.map[i][j].first=MapEdit::currentBlock;
            MapEdit::map.map[i][j].second=GL::getEditText("MapEdit::blockData");
        }else{
            MapEdit::currentBlock=MapEdit::map.map[i][j].first;
            GL::setEditText("MapEdit::blockData",MapEdit::map.map[i][j].second);
        }
    }
}

/**
 *	Mouse event callback to Erase Button
 *
 *	@param x mouse x position
 *	@param y mouse y position
**/
void mapEraseButton(int x,int y){
    x/=MapEdit::scale.x;
    y/=MapEdit::scale.y;
    if(x>=Scenes::camera.x.movedCam/MapEdit::scale.x&&x<(MapEdit::HUDarea.x+Scenes::camera.x.movedCam)/MapEdit::scale.x&&y>=Scenes::camera.y.movedCam/MapEdit::scale.y&&y<(MapEdit::HUDarea.y+Scenes::camera.y.movedCam)/MapEdit::scale.y){
        long i=round(((y-Blocks::defaultBlockSize.y/2)/Blocks::defaultBlockSize.y)-(1/2));
        long j=round(((x-Blocks::defaultBlockSize.x/2)/Blocks::defaultBlockSize.x)-(1/2));
        if(i<0)
            i=0;
        if(j<0)
            j=0;
        if(i>=MapEdit::map.map.size())
            i=MapEdit::map.map.size()-1;
        if(j>=MapEdit::map.map[0].size())
            j=MapEdit::map.map[0].size()-1;
        MapEdit::map.map[i][j].first=0;
    }
}

/**
 *	Mouse event callback to SetBlock Button
 *
 *	@param x mouse x position
 *	@param y mouse y position
**/
void MapEditSetBlock(int x,int y){
    x-=Scenes::camera.x.movedCam;
    y-=Scenes::camera.y.movedCam;
    if(x>=(MapEdit::HUDarea.x)){
        long i=floor((y-100+16)/25);
        long j=floor((x-663+16)/25);
        if(i<0)
            i=0;
        if(j<0)
            j=0;
        if(i>=MapEdit::blockPages[MapEdit::pageIndex].size())
            i=MapEdit::blockPages[MapEdit::pageIndex].size()-1;
        if(j>=MapEdit::blockPages[MapEdit::pageIndex][0].size())
            j=MapEdit::blockPages[MapEdit::pageIndex][0].size()-1;
        if(MapEdit::blockPages[MapEdit::pageIndex][i][j])
        if(!(MapEdit::blockPages[MapEdit::pageIndex][i][j]>=451&&MapEdit::blockPages[MapEdit::pageIndex][i][j]<=475)){
            MapEdit::currentBlock=MapEdit::blockPages[MapEdit::pageIndex][i][j];
            GL::setEditText("MapEdit::blockData","");
        }
    }
}
/**
 *	Draw everything on the screen
**/
void MapEdit::draw(){
    for(int i=0; i<map.backgrounds.size(); i++){
        if(!map.backgrounds[i].getMove())
            GL::drawTexture(nTRectangle::get(Scenes::camera.x.movedCam,GL::defaultSize.y+Scenes::camera.y.movedCam,GL::defaultSize.x+Scenes::camera.x.movedCam,Scenes::camera.y.movedCam,map.backgrounds[i].getzAxis()-0.9),GL::getTextureByName(map.backgrounds[i].getName()));
        else
            map.backgrounds[i].drawParalaxBackground(size);
    }
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
            GL::buttonBehave(nTRectangle::getCollision(tmp,tmp1),nTColor::get(0.38,0.38,0.38,0.8),0,true,*MapEditButton,NULL,*mapEraseButton,NULL);
            if(map.map[i][j].first){
                tmp.z-=0.1;
                tmp1.z-=0.1;
                bl = new Blocks(map.map[i][j].first,tmp,tmp1);
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
void MapEditSetAir(int x,int y){
    MapEdit::currentBlock=0;
}

/**
 *	Mouse event callback to GetBlock Button
 *
 *	@param x mouse x position
 *	@param y mouse y position
**/
void MapEditGetBlock(int x,int y){
    MapEdit::currentBlock=60000;
}

/**
 *	Mouse event callback to ZoomIn Button
 *
 *	@param x mouse x position
 *	@param y mouse y position
**/
void MapEditZoomIn(int x,int y){
    if(MapEdit::scale.x<1.99){
        MapEdit::scale.x+=0.01;
        MapEdit::scale.y+=0.01;
    }
}

/**
 *	Mouse event callback to ZoomOut Button
 *
 *	@param x mouse x position
 *	@param y mouse y position
**/
void MapEditZoomOut(int x,int y){
    if(MapEdit::scale.x>0.01){
        MapEdit::scale.x-=0.01;
        MapEdit::scale.y-=0.01;
    }
}

/**
 *	Mouse event callback to BackgroundUp Button
 *
 *	@param x mouse x position
 *	@param y mouse y position
**/
void MapEditBackgroundUp(int x,int y){

}

/**
 *	Mouse event callback to BackgroundDown Button
 *
 *	@param x mouse x position
 *	@param y mouse y position
**/
void MapEditBackgroundDown(int x,int y){

}

/**
 *	Mouse event callback to Camera Button
 *
 *	@param x mouse x position
 *	@param y mouse y position
**/
void MapEditCamera(int x,int y){
    Scenes::camera.putCameraOnOrigin();
}

/**
 *	Mouse event callback to PageUp Button
 *
 *	@param x mouse x position
 *	@param y mouse y position
**/
void MapEditPageUp(int x,int y){
    MapEdit::pageIndex--;
    if(MapEdit::pageIndex<0)
        MapEdit::pageIndex=(int)MapEdit::blockPages.size()-1;
}

/**
 *	Mouse event callback to PageDown Button
 *
 *	@param x mouse x position
 *	@param y mouse y position
**/
void MapEditPageDown(int x,int y){
    MapEdit::pageIndex++;
    if(MapEdit::pageIndex>=MapEdit::blockPages.size())
        MapEdit::pageIndex=0;
}

/**
 *  Mouse event callback to Save Button
 *
 *  @param x mouse x position
 *  @param y mouse y position
**/
void MapEditSave(int x,int y){
    if(MapEdit::save())
        GL::popupBoxBehave("Você salvou as alterações com sucesso!","BITMAP_HELVETICA_12",1000);
}

/**
 *  Mouse event callback to Play Button
 *
 *  @param x mouse x position
 *  @param y mouse y position
**/
void MapEditPlay(int x,int y){
    nTMap tmp;
    //tmp.backgroundId=currentBackground;
    tmp=MapEdit::map;
    Map::editingMap=tmp;
    bool hasSpawn=false;
    for(int i=0;i<MapEdit::map.map.size();i++){
        for(int j=0;j<MapEdit::map.map[i].size();j++){
            if(Blocks::checkIfBlocksIsPlayerSpawn(MapEdit::map.map[i][j].first)){
                hasSpawn=true;
                break;
            }
        }
    }
    if(!hasSpawn){
        Map::editingMap.map[MapEdit::map.map.size()/2][4].first=1000;
    }
    Scenes::testGameMode=true;
    Scenes::current=Scenes::game;
}

/**
 *	Mouse event callback to Leave Button
 *
 *	@param x mouse x position
 *	@param y mouse y position
**/
void MapEditLeave(int x,int y){
    Scenes::current=Scenes::menu;
}

/**
 *	Draw HUD panel on the screen
**/
void MapEdit::drawPanel(){
    GL::drawRectangle(nTRectangle::get(Scenes::camera.x.movedCam,GL::defaultSize.y+Scenes::camera.y.movedCam,GL::defaultSize.x+Scenes::camera.x.movedCam,HUDarea.y+Scenes::camera.y.movedCam,0.8),GL::getColorByName("grey"));
    GL::drawRectangle(nTRectangle::get(HUDarea.x+Scenes::camera.x.movedCam,GL::defaultSize.y+Scenes::camera.y.movedCam,GL::defaultSize.x+Scenes::camera.x.movedCam,Scenes::camera.y.movedCam,0.8),GL::getColorByName("grey"));

    GL::buttonBehave(nTRectangle::get(20+Scenes::camera.x.movedCam,570+Scenes::camera.y.movedCam,50+Scenes::camera.x.movedCam,533+Scenes::camera.y.movedCam,1),GL::getColorByName("mouseSelected"),GL::getTextureByName("Ar"),false,*MapEditSetAir,NULL,NULL,NULL);
    GL::buttonBehave(nTRectangle::get(70+Scenes::camera.x.movedCam,570+Scenes::camera.y.movedCam,100+Scenes::camera.x.movedCam,533+Scenes::camera.y.movedCam,1),GL::getColorByName("mouseSelected"),GL::getTextureByName("Zoom-"),false,*MapEditZoomOut,NULL,NULL,NULL);
    GL::buttonBehave(nTRectangle::get(110+Scenes::camera.x.movedCam,570+Scenes::camera.y.movedCam,140+Scenes::camera.x.movedCam,533+Scenes::camera.y.movedCam,1),GL::getColorByName("mouseSelected"),GL::getTextureByName("Zoom+"),false,*MapEditZoomIn,NULL,NULL,NULL);
    GL::buttonBehave(nTRectangle::get(150+Scenes::camera.x.movedCam,570+Scenes::camera.y.movedCam,180+Scenes::camera.x.movedCam,533+Scenes::camera.y.movedCam,1),GL::getColorByName("mouseSelected"),GL::getTextureByName("Background-"),false,NULL,*MapEditBackgroundDown,NULL,NULL);
    GL::buttonBehave(nTRectangle::get(190+Scenes::camera.x.movedCam,570+Scenes::camera.y.movedCam,220+Scenes::camera.x.movedCam,533+Scenes::camera.y.movedCam,1),GL::getColorByName("mouseSelected"),GL::getTextureByName("Background+"),false,NULL,*MapEditBackgroundUp,NULL,NULL);
    GL::buttonBehave(nTRectangle::get(230+Scenes::camera.x.movedCam,570+Scenes::camera.y.movedCam,280+Scenes::camera.x.movedCam,533+Scenes::camera.y.movedCam,1),GL::getColorByName("mouseSelected"),GL::getTextureByName("CameraDefault"),false,*MapEditCamera,NULL,NULL,NULL);
    GL::buttonBehave(nTRectangle::get(290+Scenes::camera.x.movedCam,570+Scenes::camera.y.movedCam,320+Scenes::camera.x.movedCam,533+Scenes::camera.y.movedCam,1),GL::getColorByName("mouseSelected"),GL::getTextureByName("Get Block"),false,*MapEditGetBlock,NULL,NULL,NULL);
    GL::buttonBehave(nTRectangle::get(330+Scenes::camera.x.movedCam,570+Scenes::camera.y.movedCam,360+Scenes::camera.x.movedCam,533+Scenes::camera.y.movedCam,1),GL::getColorByName("mouseSelected"),GL::getTextureByName("Save"),false,*MapEditSave,NULL,NULL,NULL);
    GL::buttonBehave(nTRectangle::get(370+Scenes::camera.x.movedCam,570+Scenes::camera.y.movedCam,400+Scenes::camera.x.movedCam,533+Scenes::camera.y.movedCam,1),GL::getColorByName("mouseSelected"),GL::getTextureByName("PlayCircle"),false,*MapEditPlay,NULL,NULL,NULL);
    GL::buttonBehave(nTRectangle::get(485+Scenes::camera.x.movedCam,570+Scenes::camera.y.movedCam,515+Scenes::camera.x.movedCam,533+Scenes::camera.y.movedCam,1),GL::getColorByName("mouseSelected"),GL::getTextureByName("Back"),false,*MapEditLeave,NULL,NULL,NULL);
    GL::buttonBehave(nTRectangle::get(770+Scenes::camera.x.movedCam,60+Scenes::camera.y.movedCam,790+Scenes::camera.x.movedCam,40+Scenes::camera.y.movedCam,1),GL::getColorByName("mouseSelected"),GL::getTextureByName("-"),false,NULL,*MapEditPageDown,NULL,NULL);
    GL::buttonBehave(nTRectangle::get(660+Scenes::camera.x.movedCam,60+Scenes::camera.y.movedCam,680+Scenes::camera.x.movedCam,40+Scenes::camera.y.movedCam,1),GL::getColorByName("mouseSelected"),GL::getTextureByName("+"),false,NULL,*MapEditPageUp,NULL,NULL);
    GL::setFont("BITMAP_HELVETICA_12");
    GL::drawText(nTPoint::get(550+Scenes::camera.x.movedCam,536+Scenes::camera.y.movedCam,1),"Bloco Atual",nTColor::Black());
    GL::drawText(nTPoint::get(550+Scenes::camera.x.movedCam,588+Scenes::camera.y.movedCam,1),Blocks::getTexNameByIndex(currentBlock)+getMoreInfoAboutBlocks(currentBlock),nTColor::Black());
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
    if(dx>=(MapEdit::HUDarea.x)){
        int di=floor((dy-100+16)/25);
        int dj=floor((dx-663+16)/25);
        if(di>=0&&di<blockPages[MapEdit::pageIndex].size())
        if(dj>=0&&dj<blockPages[MapEdit::pageIndex][0].size())
            blockID=MapEdit::blockPages[MapEdit::pageIndex][di][dj];
    }
    string strT=Blocks::getTexNameByIndex(blockID);
    if(strT!=""){
        strT+=getMoreInfoAboutBlocks(blockID);
        nTPoint poinT=GL::mousePos;
        poinT.x-=10*strT.size();
        poinT.z=1;
        GL::drawText(poinT,strT,nTColor::Black());
        GL::drawRectangle(nTRectangle::get(GL::mousePos.x-5,GL::mousePos.y-2,poinT.x,GL::mousePos.y-19,1),nTColor::White());
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
            GL::buttonBehave(tmp,GL::getColorByName("mouseSelected"),GL::getTextureByName(Blocks::getTexNameByIndex(blockPages[pageIndex][i][j])),false,*MapEditSetBlock,(void(*)(int, int))NULL,NULL,NULL);
        }
    }

    GL::setFont("BITMAP_HELVETICA_18");
    GL::drawCentered_MultilineX_Text(nTPoint::get(725+Scenes::camera.x.movedCam,272+Scenes::camera.y.movedCam,1),"Selected block\n data:",GL::getColorByName("green"));
    GL::editTextBehave(nTRectangle::get(660+Scenes::camera.x.movedCam,290+Scenes::camera.y.movedCam,790+Scenes::camera.x.movedCam,449+Scenes::camera.y.movedCam,1),"BITMAP_TIMES_ROMAN_10","MapEdit::blockData",false,false);
    GL::setFont("BITMAP_TIMES_ROMAN_10");
    if(GL::textButtonBehave(nTRectangle::get(660+Scenes::camera.x.movedCam,452+Scenes::camera.y.movedCam,725+Scenes::camera.x.movedCam,474+Scenes::camera.y.movedCam,1),GL::getColorByName("mouseSelected"),"Paste from file",nTColor::Black(),GL::getTextureByName("btnSkin1"))){
        // if (clip::has(clip::text_format())) {
        //     string value;
        //     clip::get_text(value);
        //     GL::setEditText("MapEdit::blockData",value);
        // }
        GL::setEditText("MapEdit::blockData",Util::getFromFile("tmp_clipboard.txt"));
        GL::setfocus("MapEdit::blockData");
    }
    if(GL::textButtonBehave(nTRectangle::get(725+Scenes::camera.x.movedCam,452+Scenes::camera.y.movedCam,790+Scenes::camera.x.movedCam,474+Scenes::camera.y.movedCam,1),GL::getColorByName("mouseSelected"),"Copy to file",nTColor::Black(),GL::getTextureByName("btnSkin1"))){
        // if (clip::has(clip::text_format())) {
        //     clip::set_text(GL::getEditText("MapEdit::blockData"));
        // }
        Util::setToFile("tmp_clipboard.txt",GL::getEditText("MapEdit::blockData"));
        GL::setfocus("MapEdit::blockData");
    }
    if(GL::textButtonBehave(nTRectangle::get(692.5+Scenes::camera.x.movedCam,476+Scenes::camera.y.movedCam,757.5+Scenes::camera.x.movedCam,494+Scenes::camera.y.movedCam,1),GL::getColorByName("mouseSelected"),"Clear",nTColor::Black(),GL::getTextureByName("btnSkin1"))){
        GL::setEditText("MapEdit::blockData","");
        GL::setfocus("MapEdit::blockData");
    }
}

/**
 *	Draw, behave and handle events of NewMap sub scene on the screen
**/
void MapEdit::askForSize(){
    GL::setFont("BITMAP_TIMES_ROMAN_24");
    GL::drawText(nTPoint::get(200,150,1),"Digite os valores e apos digitar pressione ok:",GL::getColorByName("red"));
    GL::setFont("BITMAP_HELVETICA_18");
    GL::drawText(nTPoint::get(200,400,1),"Seu mapa fica salvo em ./Maps/user.map",GL::getColorByName("red"));
    GL::drawText(nTPoint::get(200,200,1),"Digite o numero de linhas do mapa(19):",GL::getColorByName("red"));
    GL::drawText(nTPoint::get(200,250,1),"Digite o numero de colunas do mapa(19):",GL::getColorByName("red"));
    GL::editTextBehave(nTRectangle::get(550,175,640,200,1),"BITMAP_HELVETICA_18",GL::getColorByName("blue"),"19","MapEdit::askForSizeY",true,true);
    GL::editTextBehave(nTRectangle::get(550,225,640,250,1),"BITMAP_HELVETICA_18",GL::getColorByName("blue"),"19","MapEdit::askForSizeX",true,false);

    if(GL::textButtonBehave(nTRectangle::get(610,275,660,300,1),GL::getColorByName("mouseSelected"),"ok",nTColor::Black(),GL::getTextureByName("btnSkin1"))){
        float X,Y;
        Y=Util::strToFloat(GL::getEditText("MapEdit::askForSizeY"));
        X=Util::strToFloat(GL::getEditText("MapEdit::askForSizeX"));
        if(Y>=19&&Y<65000&&X>=19&&X<65000){
            MapEdit::size.y=Y;
            MapEdit::size.x=X;
            MapEdit::setMapSize();
        }
    }
}

/**
 *	Draw, behave and handle events of Load sub scene on the screen
**/
void MapEdit::askForLoad(){
    GL::setFont("BITMAP_TIMES_ROMAN_24");
    GL::drawText(nTPoint::get(200,150,1),"Digite os valores e apos digitar pressione ok:",GL::getColorByName("red"));
    GL::setFont("BITMAP_HELVETICA_18");
    GL::drawText(nTPoint::get(200,400,1),"Seu mapa fica salvo em ./Maps/user.map",GL::getColorByName("red"));
    GL::drawText(nTPoint::get(200,200,1),"Digite qual mapa quer carregar(0-"+Util::intToStr((int)(Map::maps.size()-1))+")(u):",GL::getColorByName("red"));
    GL::editTextBehave(nTRectangle::get(530,175,600,200,1),"BITMAP_HELVETICA_18",GL::getColorByName("blue"),"MapEdit::askForLoad",false,true);
    string input=GL::getEditText("MapEdit::askForLoad");
    for(int i=0;i<input.size();i++)
        if(isalpha(input[i])&&tolower(input[i])!='u'){
            input=input.substr(0,i)+input.substr(i+1,input.size()-i);
            i--;
            GL::setEditText("MapEdit::askForLoad",input);
        }
    if(GL::textButtonBehave(nTRectangle::get(610,175,660,200,1),GL::getColorByName("mouseSelected"),"ok",nTColor::Black(),GL::getTextureByName("btnSkin1"))){
        if(tolower(input[0])=='u'){
            MapEdit::load(-1);
        }else{
            int tmp=Util::strToInt(input);
            if(tmp>=0&&tmp<Map::maps.size()){
                MapEdit::load(tmp);
            }
        }
    }
}

/**
 *	Reset and resize map vector
**/
void MapEdit::setMapSize(){
    index=(int)Map::maps.size();
    map.map.clear();
    pair<int,string> p;
    p.first=0;
    p.second="";
    vector<pair<int,string> > tmp;
    for(int i=0;i<size.y;i++){
        for(int j=0;j<size.x;j++)
            tmp.push_back(p);
        map.map.push_back(tmp);
        tmp.clear();
    }
}

/**
 *	Gets extra information about the block
 *
 *	@param block the type of the block
 *	@return a string containing the extra information
**/
string MapEdit::getMoreInfoAboutBlocks(int block){
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
