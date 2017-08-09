#include "Map.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Scenes.hpp"
#include "Blocks.hpp"
#include "Tutorials.hpp"
#include "Bullet.hpp"
#include "powerUp.hpp"
#include "AssetsLoader.hpp"
#include "Camera.hpp"

Map::Map() {
}

Map::Map(const Map& orig) {
}

Map::~Map() {
}

vector<vector<int> > Map::currentMap;
vector<void*> Map::staticBlocks;
vector<void*> Map::dynamicBlocks;
GLuint Map::background;
nTPoint Map::size;
int Map::nOfMaps=3;
int Map::nOfBackgrounds=3;
int Map::nOfEnemys=0;
int Map::expetedTime=0;
int Map::totalPowerUps=0;
vector<nTMap> Map::maps;
nTMap Map::usrMap;

/**
 *	Change the current map
 *
 *	@param map the new map
**/
void Map::changeCurrentMap(nTMap map){
    Enemy* en;
    Bullet* bu;
    powerUp* pu;
    for(int i=0;i<Entity::enemys.size();i++){
        en=(Enemy*) Entity::enemys[i];
        delete en;
    }
    for(int i=0;i<Bullet::self.size();i++){
        bu=(Bullet*) Bullet::self[i];
        free(bu);
    }
    for(int i=0;i<powerUp::self.size();i++){
        pu=(powerUp*) powerUp::self[i];
        delete pu;
    }
    Entity::enemys.clear();
    Bullet::self.clear();
    powerUp::self.clear();
    currentMap.clear();

    size.set((map.map[0].size()-1)*Blocks::defaultBlockSize.x,(map.map.size()-1)*Blocks::defaultBlockSize.y,0);
    char buffer[5];
    snprintf(buffer,5,"%d",map.backgroundId);
    string mapID(buffer);
    currentMap=map.map;
    background=GL::getTextureByName("background"+mapID);
    setBlockPos();
}

/**
 *	Delete all blocks and enemys objects and reset map statistics
**/
void Map::deleteAllBlocks(){
    Blocks* bl;
    Enemy* en;
    for(int i=0;i<dynamicBlocks.size();i++){
        bl=(Blocks*)dynamicBlocks[i];
        delete bl;
    }
    for(int i=0;i<staticBlocks.size();i++){
        bl=(Blocks*) staticBlocks[i];
        delete bl;
    }
    for(int i=0;i<Entity::enemys.size();i++){
        en=(Enemy*) Entity::enemys[i];
        delete en;
    }
    dynamicBlocks.clear();
    staticBlocks.clear();
    Entity::enemys.clear();
    nOfEnemys=0;
    expetedTime=0;
    totalPowerUps=0;
}

/**
 *	Instantiate and config all blocks after deleting the older ones according to the current map
**/
void Map::setBlockPos(){
    deleteAllBlocks();
    Blocks *bl;
    for(int i=0;i<currentMap.size();i++){
        for(int j=0;j<currentMap[0].size();j++){
            bl=new Blocks(currentMap[i][j],nTPoint::get(Blocks::defaultBlockSize.x*(j+(1/2))+Blocks::defaultBlockSize.x/2,Blocks::defaultBlockSize.y*(i+(1/2))+Blocks::defaultBlockSize.y/2,Blocks::defaultBlockSize.z),Blocks::defaultBlockSize);
            if(Blocks::checkIfBlocksIsDynamic(currentMap[i][j])){
                if(currentMap[i][j]==1000&&(Player::checkpoint==0||Scenes::freeGameMode)){
                    bl->pos.z=0.9;
                    Player::getPlayerById(0)->spawn(bl->pos,Player::getPlayerById(0)->life);
                }else if(currentMap[i][j]==1000);

                else if(currentMap[i][j]>=2000&&currentMap[i][j]<=3000){
                    bl->pos.z=0.89;
                    nOfEnemys++;
                    new Enemy(currentMap[i][j]-2000,Enemy::defaultLife,bl->pos,Enemy::defaultSize,Entity::getAnimationVector(Enemy::enemyAnim[rand()%Enemy::enemyAnim.size()],Enemy::enemyAnimSize[rand()%Enemy::enemyAnimSize.size()]),0);
                }else if(currentMap[i][j]>5000){
                    bl->pos.z=0.89;
                    new Enemy((currentMap[i][j]-5000)+100,Enemy::bossLife,bl->pos,Enemy::bossSize,Entity::getAnimationVector(Enemy::enemyAnim[0],Enemy::enemyAnimSize[0]),0);
                    nOfEnemys+=3;
                }else{
                    bl->id=dynamicBlocks.size();
                    dynamicBlocks.push_back(bl);
                }
                if((currentMap[i][j]>=301&&currentMap[i][j]<=325)||(currentMap[i][j]>=476&&currentMap[i][j]<=500)){
                    totalPowerUps++;
                }
            }else if(currentMap[i][j]!=0){
                bl->id=staticBlocks.size();
                staticBlocks.push_back(bl);
            }
            if(currentMap[i][j]==4001&&Player::checkpoint==1&&!Scenes::freeGameMode){
                nTPoint pos=bl->pos;
                pos.z=0.9;
                pos.y-=Blocks::defaultBlockSize.y;
                Player::getPlayerById(0)->spawn(pos,Player::getPlayerById(0)->life);
            }else
            if(currentMap[i][j]==4002&&Player::checkpoint==2&&!Scenes::freeGameMode){
                nTPoint pos=bl->pos;
                pos.z=0.9;
                pos.y-=Blocks::defaultBlockSize.y;
                Player::getPlayerById(0)->spawn(pos,Player::getPlayerById(0)->life);
            }
        }
    }
    for(int i=0;i<dynamicBlocks.size();i++){
        bl=(Blocks*)dynamicBlocks[i];
        bl->id+=staticBlocks.size();
    }

    expetedTime=(int)currentMap.size();
}

/**
 *	Draw the current map on the screen
**/
void Map::draw(){
    FunctionAnalyser::startFunction("Map::draw");
    Blocks* bl;
    GL::drawTexture(nTRectangle::get(Scenes::camera.x.movedCam,GL::defaultSize.y+Scenes::camera.y.movedCam,GL::defaultSize.x+Scenes::camera.x.movedCam,Scenes::camera.y.movedCam,-0.9),background);
    for(int i=0;i<dynamicBlocks.size();i++){
        bl=(Blocks*)dynamicBlocks[i];
        if(!Blocks::checkIfBlocksIsLiquid(bl->type)||Scenes::current!=Scenes::game)
            bl->draw();
    }
    for(int i=0;i<staticBlocks.size();i++){
        bl=(Blocks*)staticBlocks[i];
        bl->draw();
    }
    FunctionAnalyser::endFunction("Map::draw");
}

/**
 *	Draw the current map transparencies on the screen
**/
void Map::transparencyLayerDraw(){
    FunctionAnalyser::startFunction("Map::transparencyLDraw");
    bool depth=glIsEnabled(GL_DEPTH_TEST);
    glDisable(GL_DEPTH_TEST);
    Blocks* bl;
    for(int i=0;i<dynamicBlocks.size();i++){
        bl=(Blocks*)dynamicBlocks[i];
        if(Blocks::checkIfBlocksIsLiquid(bl->type))
            bl->draw();
    }
    if(depth)glEnable(GL_DEPTH_TEST);
    FunctionAnalyser::endFunction("Map::transparencyLDraw");
}

/**
 *	Check collision of an retangle with the map
 *
 *	@param pos position of the retangle
 *	@param size size of the retangle
 *	@return a vector containg all existing collisions and information about then
**/
vector <mapCollision> Map::checkCollision(nTPoint pos,nTPoint size){
    FunctionAnalyser::startFunction("Map::checkCollision");
    int x32=Blocks::defaultBlockSize.x,x16=x32/2;
    int y32=Blocks::defaultBlockSize.y,y16=y32/2;
    int j_sup,j_inf, i_sup, i_inf, i, j;
    nTRectangle objRec, blockRec;
    nTPoint blockCenter, pointPrincipal;
    mapCollision adc;
    vector <mapCollision> result;
    objRec=nTRectangle::getCollision(pos,size);
    int blockId=getIdByPosition(nTPoint::get(floor(pos.x),ceil(pos.y),pos.z));
    i_inf=floor(objRec.p0.x/Blocks::defaultBlockSize.x);
    i_sup=floor(objRec.p1.x/Blocks::defaultBlockSize.x);
    j_inf=ceil(objRec.p1.y/Blocks::defaultBlockSize.y);
    j_sup=ceil(objRec.p0.y/Blocks::defaultBlockSize.y);
    pointPrincipal.set(i_inf*y32+y16,j_inf*x32+x16,0);
    for(i=i_inf-1; i<=i_sup+1; i++){
      for(j=j_inf-1;j<=j_sup+1; j++){
        if(j>=0 && j<=currentMap.size() && i>=0 && i<=currentMap[0].size()){
            blockCenter.set(i*y32+y16,j*x32-x16,0);
            Blocks* bl=getBlockById(getIdByPosition(blockCenter));
            if(Blocks::checkIfBlocksIsMassive(bl->type)){
                if(bl->id!=blockId&&bl->id){
                    adc.blockId=bl->id;
                    blockRec=nTRectangle::getCollision(blockCenter,Blocks::defaultBlockSize);
                    adc.collision=Mechanics::getCollision(objRec,blockRec);//verifica colisao
                    if(adc.collision.firstObj!=Mechanics::NOCOLLISION){
                        result.push_back(adc);
                  }
                }
            }
        }
      }
    }
    Blocks *bl;
    for(int i=0;i<dynamicBlocks.size();i++){
        bl=(Blocks*)dynamicBlocks[i];
        if((bl->type>250&&bl->type<=300&&Scenes::camera.isInTheXScreen(nTRectangle::getCollision(bl->pos,bl->size)))||(bl->type>200&&bl->type<=250&&Scenes::camera.isInTheYScreen(nTRectangle::getCollision(bl->pos,bl->size)))){
            if(bl->id!=blockId){
                adc.blockId=bl->id;
                blockRec=nTRectangle::getCollision(bl->pos,bl->size);
                adc.collision=Mechanics::getCollision(objRec,blockRec);//verifica colisao
                if(adc.collision.firstObj!=Mechanics::NOCOLLISION)
                    result.push_back(adc);
            }
        }
    }
    FunctionAnalyser::endFunction("Map::checkCollision");
    return result;
}

/**
 *	Run logic events of the Map and it blocks like move, change directions and textures, spawn bullets, handle block events and draw
**/
void Map::refresh(){
    FunctionAnalyser::startFunction("Map::refresh");
    if(GL::isPaused){
        GL::drawTexture(nTRectangle::get(Scenes::camera.x.movedCam,GL::defaultSize.y+Scenes::camera.y.movedCam,GL::defaultSize.x+Scenes::camera.x.movedCam,Scenes::camera.y.movedCam,-0.9),background);
        FunctionAnalyser::endFunction("Map::refresh");
        return;
    }
    Blocks *bl;
    for(int i=0;i<dynamicBlocks.size();i++){
        bl=(Blocks*)dynamicBlocks[i];
        if(Scenes::camera.isInTheScreen(nTRectangle::getCollision(bl->pos,bl->size))||bl->type==255){
            if(bl->type>250&&bl->type<=300&&Scenes::camera.isInTheXScreen(nTRectangle::getCollision(bl->pos,bl->size)))
                bl->move(Util::direction_up,bl->moveSpeed/GL::getFPS());
            else if(bl->type>200&&bl->type<=250&&Scenes::camera.isInTheYScreen(nTRectangle::getCollision(bl->pos,bl->size)))
                bl->move(Util::direction_left,bl->moveSpeed/GL::getFPS());
            else if(bl->type>=101&&bl->type<200&&Scenes::camera.isInTheXScreen(nTRectangle::getCollision(bl->pos,bl->size))){
                if(round(fmodl(GL::getGameMs(),(int)Bullet::timeToShoot/5))==0&&Player::getPlayerById(0)->life>0){
                    nTPoint tmp=bl->pos;
                    tmp.z=0.9;
                    tmp.x+=Blocks::defaultBlockSize.x*1.2;
                    new Bullet(0,Bullet::baseSpeed,tmp,nTPoint::get(40,20,1));
                    tmp.x-=Blocks::defaultBlockSize.x*1.2*2;
                    new Bullet(0,-Bullet::baseSpeed,tmp,nTPoint::get(40,20,1));
                }
            }else if(bl->type==200){
                if(round(fmodl(GL::getGameMs(),(int)Bullet::timeToShoot))==0&&Player::getPlayerById(0)->life>0){
                    nTPoint tmp=bl->pos;
                    tmp.z=0.9;
                    tmp.y-=Blocks::defaultBlockSize.y/3;
                    if(Player::getPlayerById(0)->pos.x>=bl->pos.x){
                        tmp.x+=Blocks::defaultBlockSize.x*1.2;
                        new Bullet(4,Bullet::baseSpeed*1.3,tmp,nTPoint::get(100,60,1));
                    }else{
                        tmp.x-=Blocks::defaultBlockSize.x*2.4;
                        new Bullet(4,Bullet::baseSpeed*-1.3,tmp,nTPoint::get(-100,60,1));
                    }
                }
            }else if(bl->type==501){
                objCollision var;
                nTPoint point=Player::getPlayerById(0)->pos;
                point.y+=2;
                var=Mechanics::getCollision(nTRectangle::getCollision(point,Player::getPlayerById(0)->size),nTRectangle::getCollision(bl->pos,bl->size));
                if(var.firstObj==Mechanics::BOTTOM){
                    Player::getPlayerById(0)->vSpeed=-Player::getPlayerById(0)->jumpSpeed*1.5;
                    Player::getPlayerById(0)->canJump=false;
                    AL::singleton->playSoundByName("mola");
                }
            }else if(bl->type==4001){
                objCollision var;
                var=Mechanics::getCollision(nTRectangle::getCollision(bl->pos,bl->size),nTRectangle::getCollision(Player::getPlayerById(0)->pos,Player::getPlayerById(0)->size));
                if(var.firstObj!=Mechanics::NOCOLLISION&&Player::checkpoint<1&&!Scenes::freeGameMode){
                    Player::checkpoint=1;
                    AssetsLoader::saveSettings();
                }
            }else if(bl->type==4002){
                objCollision var;
                var=Mechanics::getCollision(nTRectangle::getCollision(bl->pos,bl->size),nTRectangle::getCollision(Player::getPlayerById(0)->pos,Player::getPlayerById(0)->size));
                if(var.firstObj!=Mechanics::NOCOLLISION&&Player::checkpoint<2&&!Scenes::freeGameMode){ ;
                   Player::checkpoint=2;
                    AssetsLoader::saveSettings();
                }
            }else if(bl->type==666){
                objCollision var;
                var=Mechanics::getCollision(nTRectangle::getCollision(bl->pos,bl->size),nTRectangle::getCollision(Player::getPlayerById(0)->pos,Player::getPlayerById(0)->size));
                if(var.firstObj!=Mechanics::NOCOLLISION){
                    if(!Scenes::freeGameMode){
                        Player::checkpoint=0;
                        Player::stage++;
                        if(Player::stage>=Map::nOfMaps){
                            Player::stage=0;
                            Map::GG(true);
                        }else{
                            Map::next();
                        }
                        AssetsLoader::saveSettings();
                    }else
                        Map::next();
                }
            }else if(bl->type>=301&&bl->type<325){
                objCollision var;
                var=Mechanics::getCollision(Player::getPlayerById(0)->swordCollision,nTRectangle::getCollision(bl->pos,bl->size));
                if(var.firstObj==Mechanics::TOP){
                    nTPoint tmp=bl->pos;
                    tmp.y-=Blocks::defaultBlockSize.y*1.5;
                    new powerUp(bl->type-301,tmp,nTPoint::get(20,20),true);
                    bl->type=325;
                    bl->tex=GL::getTextureByName("PowerupOff");
                    AL::singleton->playSoundByName("chestOpen");
                    Player::getPlayerById(0)->powerUpsActiveted++;
                }
            }else if(bl->type>=476&&bl->type<500&&Player::getPlayerById(0)->atacking){
                objCollision var;
                var=Mechanics::getCollision(Player::getPlayerById(0)->swordCollision,nTRectangle::getCollision(bl->pos,bl->size));
                if(var.firstObj!=Mechanics::NOCOLLISION){
                    nTPoint tmp=bl->pos;
                    tmp.y-=Blocks::defaultBlockSize.y*1.5;
                    new powerUp(bl->type-476,tmp,nTPoint::get(20,20),true);
                    bl->type=500;
                    bl->tex=GL::getTextureByName("BauOff");
                }
            }else if(bl->type>=326&&bl->type<=350&&Player::getPlayerById(0)->atacking){
                objCollision var;
                var=Mechanics::getCollision(Player::getPlayerById(0)->swordCollision,nTRectangle::getCollision(bl->pos,bl->size));
                if(var.firstObj!=Mechanics::NOCOLLISION){
                    bl->applyDamage(1);
                }
            }
        }
    }
    draw();
    FunctionAnalyser::endFunction("Map::refresh");
}


/**
 *	Gets the id of the block based on its position
 *
 *	@param pos position of the block
 *	@return id of the block on the vectors
**/
int Map::getIdByPosition(nTPoint pos){
  Blocks* bl;
  for(int i=0; i<staticBlocks.size(); i++){
    bl=(Blocks*)staticBlocks[i];
    if(bl->pos.x==pos.x && bl->pos.y==pos.y){
      return bl->id;
    }
  }
  for(int i=0; i<dynamicBlocks.size(); i++){
    bl=(Blocks*)dynamicBlocks[i];
    if(bl->pos.x==pos.x && bl->pos.y==pos.y){
      return bl->id;
    }
  }
  return 0;
}

/**
 *	Handle gameover events
 *
 *	@param isWinner false if the player lose the game
**/
void Map::GG(bool isWinner){
    if(Scenes::freeGameMode){
        Scenes::current=Scenes::menu;
        return;
    }
    Player::refreshGlobalcoeficiente();
    Player::lives=3;
    Player::checkpoint=0;
    Player::stage=0;
    Player::checkpoint=0;

    if(isWinner){
        Scenes::current=Scenes::posYouWin;
    }else{
        Player::globalCoeficiente=0;
        Scenes::current=Scenes::posGameEnd;
    }
    AssetsLoader::saveSettings();
}

/**
 *	Handle collision with end level block events
**/
void Map::next(){
    if(Scenes::freeGameMode){
        Scenes::current=Scenes::posGame;
    }else{
        if(Player::lives>=0)
            Scenes::current=Scenes::posGame;
        else
            GG(false);
    }
}

/**
 *	Loads a map
 *
 *  @param path filepath to the map
 *  @return true if the map was successfully loaded, otherwise false
**/
bool Map::loadMap(string path){
    nTMap tmp;
    ifstream mapFILE(path);
    if(mapFILE.is_open()){
        string tmp2;
        vector<int> tmp3;
        getline(mapFILE,tmp2);
        istringstream (tmp2) >> tmp.backgroundId;
        while(mapFILE.good()){//!mapFILE.eof()
            while(getline(mapFILE,tmp2)){
                int tmp4;
                string tmp5;
                for(int i=0;i<tmp2.size();i++){
                    if(tmp2[i]!='\''){
                        tmp5+=tmp2[i];
                    }else{
                        istringstream (tmp5) >> tmp4;
                        tmp3.push_back(tmp4);
                        tmp5.clear();
                    }
                }
                tmp.map.push_back(tmp3);
                tmp3.clear();
            }
        }
        string strT(path);
        if(strT!=Util::newPath("Maps/user.map"))
            maps.push_back(tmp);
        else
            usrMap=tmp;
        mapFILE.close();
        return true;
    }else{
        if(Util::DEBUG)
        cout<<"Erro ao carregar o arquivo "<<path<<endl;
        return false;
  }
}

/**
 *	Saves a map
 *
 *  @param path filepath to save the map
 *  @param idx index of to map to be saved
 *  @return true if the map was successfully saved, otherwise false
**/
bool Map::saveMap(string path,int idx){
    nTMap save;
    if(idx>=maps.size()&&!(idx<0)){
        cout<<"Erro no index "<<idx<<"do map"<<endl;
        return false;
    }else if(idx>=0){
        save=maps[idx];
    }else{
        save=usrMap;
    }
    ofstream mapFILE(path);
    if(mapFILE.is_open()){
        mapFILE<<save.backgroundId<<endl;
        for(int i=0;i<save.map.size();i++){
            for(int j=0;j<save.map[i].size();j++)
                mapFILE<<save.map[i][j]<<'\'';
            mapFILE<<endl;
        }
        mapFILE.close();
        return true;
    }else{
        if(Util::DEBUG)
        cout<<"Erro ao salvar o arquivo "<<path<<endl;
        return false;
  }
}

/**
 *	Gets the block object from its id
 *
 *	@param id the id of the block
 *  @return the address of the block object
**/
Blocks* Map::getBlockById(int id){
    if(id>=Map::staticBlocks.size()){
        int idx=id-Map::staticBlocks.size();
        if(idx<Map::dynamicBlocks.size()){
            return (Blocks*)Map::dynamicBlocks[idx];
        }else{
            return nullptr;
        }
    }else{
        return (Blocks*)Map::staticBlocks[id];
    }
    return nullptr;
}
