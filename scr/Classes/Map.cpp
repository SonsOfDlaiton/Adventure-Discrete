#include "Map.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Boss.hpp"
#include "Scenes.hpp"
#include "Blocks.hpp"
#include "Tutorials.hpp"
#include "Bullet.hpp"
#include "PowerUp.hpp"
#include "AssetsLoader.hpp"
#include "Camera.hpp"
#include "ADCode.hpp"

Map::Map() {
}

Map::Map(const Map& orig) {
}

Map::~Map() {
}

vector<vector<int> > Map::staticBlocksArr;
vector<void*> Map::staticBlocks;
vector<void*> Map::dynamicBlocks;
nTMap Map::actualMap;
nTPoint Map::size;
int Map::nOfMaps=Constants::MAP_AmountOfMaps;
int Map::nOfUMaps=Constants::MAP_AmountOfUserMaps;
int Map::nOfBackgrounds=Constants::MAP_AmountOfBackgrounds;
int Map::nOfEnemys=0;
int Map::expetedTime=0;
int Map::totalPowerUps=0;
int Map::totalCoins=0;
int Map::currentUsrMap=0;
vector<nTMap> Map::maps;
vector<nTMap> Map::usrMap;
nTMap Map::editingMap;

const int Map::lvlTechnical=0;
const int Map::lvlGraduation=1;
const int Map::lvlMasters=2;
const int Map::lvlWork=3;
const int Map::lvlGoodTeacher=4;
const int Map::lvlBadTeacher=5;

/**
 *  Change the current map
 *
 *  @param map the new map
**/
void Map::changeCurrentMap(nTMap map){
    Enemy* en;
    Boss* bo;
    Bullet* bu;
    PowerUp* pu;
    for(int i=0;i<Entity::enemys.size();i++){
        en=(Enemy*) Entity::enemys[i];
        delete en;
    }
    for(int i=0;i<Entity::bosses.size();i++){
        bo=(Boss*) Entity::bosses[i];
        delete bo;
    }
    for(int i=0;i<Bullet::self.size();i++){
        bu=(Bullet*) Bullet::self[i];
        free(bu);
    }
    for(int i=0;i<PowerUp::self.size();i++){
        pu=(PowerUp*) PowerUp::self[i];
        delete pu;
    }
    Entity::bosses.clear();
    Entity::enemys.clear();
    Bullet::self.clear();
    PowerUp::self.clear();
    actualMap.map.clear();
    actualMap.backgrounds.clear();
    size.set((map.map[0][0].size()-1)*Blocks::defaultBlockSize.x,(map.map[0].size()-1)*Blocks::defaultBlockSize.y,0);
    sort(map.backgrounds.begin(), map.backgrounds.end()); // coloca em ordem decrescente os backgrounds a serem desenhados
    actualMap=map;
    bool hasSpawn=false;
    for(int k=0;k<actualMap.map.size();k++){
        for(int i=0;i<actualMap.map[k].size();i++){
            for(int j=0;j<actualMap.map[k][i].size();j++){
                if(Blocks::checkIfBlocksIsPlayerSpawn(actualMap.map[k][i][j].first)){
                    hasSpawn=true;
                    break;
                }
            }
        }
    }
    if(!hasSpawn){
        actualMap.map[0][actualMap.map[0].size()/2][4].first=1000;
    }
    processADC();
    setBlockPos();
}

/**
 *  Delete all blocks and enemys objects and reset map statistics
**/
void Map::deleteAllBlocks(){
    Blocks* bl;
    Enemy* en;
    Boss* bo;
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
    for(int i=0;i<Entity::bosses.size();i++){
        bo=(Boss*) Entity::bosses[i];
        delete bo;
    }
    dynamicBlocks.clear();
    staticBlocks.clear();
    staticBlocksArr.clear();
    Entity::enemys.clear();
    nOfEnemys=0;
    expetedTime=0;
    totalPowerUps=0;
    totalCoins=0;
}

void Map::processADC(){
    ADCode* adc=new ADCode(actualMap.mapADC,"Map");
    ADCode* adc_cam=adc->getSection("cam");
    Camera::autoCamSpeed=adc_cam->getInt("autospeed",0);
    delete adc_cam;
    Background::loadParalax(adc,actualMap.backgrounds);
    delete adc;
}

/**
 *  Instantiate and config all blocks after deleting the older ones according to the current map
**/
void Map::setBlockPos(){
    deleteAllBlocks();
    Tutorials::clear();
    Blocks *bl;
    for(int i=0;i<actualMap.map[0].size();i++){
        vector<int> v;
        staticBlocksArr.push_back(v);
        for(int j=0;j<actualMap.map[0][0].size();j++)
            staticBlocksArr[i].push_back(-1);
    }
    for(int k=actualMap.map.size()-1;k>=0;k--){
        for(int i=0;i<actualMap.map[k].size();i++){
            for(int j=0;j<actualMap.map[k][0].size();j++){
                bl=new Blocks(actualMap.map[k][i][j].first,nTPoint::get(Blocks::defaultBlockSize.x*(j+(1/2))+Blocks::defaultBlockSize.x/2,Blocks::defaultBlockSize.y*(i+(1/2))+Blocks::defaultBlockSize.y/2,Blocks::defaultBlockSize.z-k*0.0001f),Blocks::defaultBlockSize,actualMap.map[k][i][j].second);
                if(Blocks::checkIfBlocksIsDynamic(actualMap.map[k][i][j].first)){
                    if(Blocks::checkIfBlocksIsPlayerSpawn(actualMap.map[k][i][j].first)&&(Player::checkpoint==0||Scenes::freeGameMode)){
                        nTPoint pos=bl->pos;
                        pos.z=0.9;
                        pos.y-=Blocks::defaultBlockSize.y;
                        if(Scenes::freeGameMode)
                            Player::getPlayerById(0)->spawn(pos,Player::defaultLife);
                        else
                            Player::getPlayerById(0)->spawn(pos,Player::getPlayerById(0)->life);
                    }else if(Blocks::checkIfBlocksIsPlayerSpawn(actualMap.map[k][i][j].first));

                    else if(Blocks::checkIfBlocksIsEnemySpawn(actualMap.map[k][i][j].first)){
                        nTPoint pos=bl->pos;
                        pos.z=0.89;
                        pos.y-=Blocks::defaultBlockSize.y;
                        nOfEnemys++;
                        new Enemy(actualMap.map[k][i][j].first-2000,Enemy::defaultLife,pos,Enemy::defaultSize,"%random%");
                    }else if(Blocks::checkIfBlocksIsBossSpawn(actualMap.map[k][i][j].first)){
                        nTPoint pos=bl->pos;
                        pos.z=0.89;
                        pos.y-=Blocks::defaultBlockSize.y;
                        new Boss(bl->data,bl->pos);
                    }else if(Blocks::checkIfBlocksIsTutorial(actualMap.map[k][i][j].first)){
                        Tutorials::add(j,bl->data,false);
                    }else if(Blocks::checkIfBlocksIsTutorialPause(actualMap.map[k][i][j].first)){
                        Tutorials::add(j,bl->data,true);
                    }else if(actualMap.map[k][i][j].first==Blocks::CoinBlock){
                        nTPoint pos=bl->pos;
                        pos.z=0.89;
                        new Bullet(Bullet::coinBullet,0,pos,nTPoint::get(16,16,1));
                        totalCoins++;
                    }else{
                        bl->id=dynamicBlocks.size();
                        dynamicBlocks.push_back(bl);
                    }
                    if(Blocks::checkIfBlocksIsPowerUpChest(actualMap.map[k][i][j].first)||Blocks::checkIfBlocksIsPowerUpBlock(actualMap.map[k][i][j].first)){
                        totalPowerUps++;
                    }
                }else if(!Blocks::checkIfBlocksIsAir(actualMap.map[k][i][j].first)){
                    bl->id=staticBlocks.size();
                    staticBlocksArr[i][j]=bl->id;
                    staticBlocks.push_back(bl);
                }
                if(Blocks::checkIfBlocksIsCheckpoint(actualMap.map[k][i][j].first)){
                    if(Blocks::getCheckPointId(actualMap.map[k][i][j].first)==Player::checkpoint&&!Scenes::freeGameMode){
                        nTPoint pos=bl->pos;
                        pos.z=0.9;
                        pos.y-=Blocks::defaultBlockSize.y;
                        Player::getPlayerById(0)->spawn(pos,Player::getPlayerById(0)->life);
                        Tutorials::checkpoints.push_back(j);
                    }
                }
            }
        }
    }
    for(int i=0;i<dynamicBlocks.size();i++){
        bl=(Blocks*)dynamicBlocks[i];
        bl->id+=staticBlocks.size();
    }
    Tutorials::processCheckpoints();
    expetedTime=(int)actualMap.map[0].size()*2.666f;
}

/**
 *  Draw the current map on the screen
**/
void Map::draw(){
    FunctionAnalyser::startFunction("Map::draw");
    Blocks* bl;
    Background::drawBackgrounds(actualMap.backgrounds);
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
 *  Draw the current map transparencies on the screen
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
 *  Check collision of an retangle with the map
 *
 *  @param pos position of the retangle
 *  @param size size of the retangle
 *  @return a vector containg all existing collisions and information about then
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
    for(i=i_inf;i<=i_sup;i++){
        for(j=j_inf;j<=j_sup;j++){
            if(j>=0 && j<=actualMap.map[0].size() && i>=0 && i<=actualMap.map[0][0].size()){
                blockCenter.set(i*y32+y16,j*x32-x16,0);
                Blocks* bl=getBlockById(getIdByPosition(blockCenter));
                if(bl!=nullptr){
                    if(Blocks::checkIfBlocksIsMassive(bl->type)){
                        if(bl->id!=blockId&&bl->id){
                            adc.blockId=bl->id;
                            blockRec=nTRectangle::getCollision(blockCenter,Blocks::defaultBlockSize);
                            adc.collision=Mechanics::getCollision(objRec,blockRec);//verifica colisao
                            if(adc.collision.firstObj!=Mechanics::NOCOLLISION){
                                result.push_back(adc);
                                if(Mechanics::drawCollisionRec) GL::drawCollision(blockRec,3,nTColor::get(0,1,0));
                            }else if(Mechanics::drawCollisionRec)GL::drawCollision(nTRectangle::getCollision(blockCenter,Blocks::defaultBlockSize),3,nTColor::get(0,0,1));
                        }else if(Mechanics::drawCollisionRec)GL::drawCollision(nTRectangle::getCollision(blockCenter,Blocks::defaultBlockSize),3,nTColor::get(0,0,1));
                    }else if(Mechanics::drawCollisionRec)GL::drawCollision(nTRectangle::getCollision(blockCenter,Blocks::defaultBlockSize),3,nTColor::get(0,0,1));
                }
            }
        }
    }
    Blocks *bl;
    for(int i=0;i<dynamicBlocks.size();i++){
        bl=(Blocks*)dynamicBlocks[i];
        blockRec=nTRectangle::getCollision(bl->pos,bl->size);
        if((Blocks::checkIfBlocksIsHalfBlockV(bl->type)&&Scenes::camera.isInTheXScreen(blockRec))||(Blocks::checkIfBlocksIsHalfBlockH(bl->type)&&Scenes::camera.isInTheYScreen(blockRec))){
            if(bl->id!=blockId){
                adc.blockId=bl->id;
                adc.collision=Mechanics::getCollision(objRec,blockRec);//verifica colisao
                if(adc.collision.firstObj!=Mechanics::NOCOLLISION){
                    result.push_back(adc);
                    if(Mechanics::drawCollisionRec) GL::drawCollision(blockRec,3,nTColor::get(0,1,0));
                }
            }
        }
    }
    FunctionAnalyser::endFunction("Map::checkCollision");
    return result;
}

/**
 *  Run logic events of the Map and it blocks like move, change directions and textures, spawn bullets, handle block events and draw
**/
void Map::refresh(){
    FunctionAnalyser::startFunction("Map::refresh");
    if(GL::isPaused){
        Background::drawBackgrounds(actualMap.backgrounds);
        FunctionAnalyser::endFunction("Map::refresh");
        return;
    }
    Blocks *bl;
    for(int i=0;i<dynamicBlocks.size();i++){
        bl=(Blocks*)dynamicBlocks[i];
        if(Scenes::camera.isInTheScreen(nTRectangle::getCollision(bl->pos,bl->size))||bl->type==Blocks::HalfBlockVCactus){
            if(Blocks::checkIfBlocksIsHalfBlockV(bl->type)&&Scenes::camera.isInTheXScreen(nTRectangle::getCollision(bl->pos,bl->size)))
                bl->move(Util::direction_up,bl->moveSpeed/GL::getFPS());
            else if(Blocks::checkIfBlocksIsHalfBlockH(bl->type)&&Scenes::camera.isInTheYScreen(nTRectangle::getCollision(bl->pos,bl->size)))
                bl->move(Util::direction_left,bl->moveSpeed/GL::getFPS());
            else if(Blocks::checkIfBlocksIsShooter(bl->type)&&bl->type!=Blocks::BusShooterBlock&&Scenes::camera.isInTheXScreen(nTRectangle::getCollision(bl->pos,bl->size))){
                if(Util::timerWithInterval(Bullet::timeToShoot/2.5)&&Player::getPlayerById(0)->life>0){
                    nTPoint tmp=bl->pos;
                    tmp.z=0.9;
                    tmp.x+=Blocks::defaultBlockSize.x*1.2;
                    new Bullet(Bullet::errorBlockBullet,Bullet::baseSpeed,tmp,nTPoint::get(40,20,1));
                    tmp.x-=Blocks::defaultBlockSize.x*1.2*2;
                    new Bullet(Bullet::errorBlockBullet,-Bullet::baseSpeed,tmp,nTPoint::get(40,20,1));
                }
            }else if(bl->type==Blocks::BusShooterBlock){
                if(Util::timerWithInterval(Bullet::timeToShoot)&&Player::getPlayerById(0)->life>0&&Scenes::camera.isInTheXScreen(nTRectangle::getCollision(bl->pos,bl->size))){
                    nTPoint tmp=bl->pos;
                    tmp.z=0.9;
                    tmp.y-=Blocks::defaultBlockSize.y/3;
                    if(Player::getPlayerById(0)->pos.x>=bl->pos.x){
                        tmp.x+=Blocks::defaultBlockSize.x*1.2;
                        new Bullet(Bullet::busBullet,Bullet::baseSpeed*1.3,tmp,nTPoint::get(100,60,1));
                    }else{
                        tmp.x-=Blocks::defaultBlockSize.x*2.4;
                        new Bullet(Bullet::busBullet,Bullet::baseSpeed*-1.3,tmp,nTPoint::get(-100,60,1));
                    }
                }
            }else if(Blocks::checkIfBlocksIsJumpBoost(bl->type)){
                objCollision var=Mechanics::getCollision(nTRectangle::getCollision(Player::getPlayerById(0)->getGroundPos(),Player::getPlayerById(0)->size),nTRectangle::getCollision(bl->pos,bl->size));
                if(var.firstObj==Mechanics::BOTTOM){
                    Player::getPlayerById(0)->vSpeed=-Player::getPlayerById(0)->jumpSpeed*1.5;
                    Player::getPlayerById(0)->canJump=false;
                    AL::singleton->playSoundByName("mola");
                }
            }else if(Blocks::checkIfBlocksIsCheckpoint(bl->type)){
                objCollision var=Mechanics::getCollision(nTRectangle::getCollision(bl->pos,bl->size),nTRectangle::getCollision(Player::getPlayerById(0)->pos,Player::getPlayerById(0)->size));
                if(var.firstObj!=Mechanics::NOCOLLISION&&Player::checkpoint<Blocks::getCheckPointId(bl->type)&&!Scenes::freeGameMode){
                    Player::checkpoint=Blocks::getCheckPointId(bl->type);
                    AssetsLoader::saveSettings();
                }
            }else if(bl->type==Blocks::EndLevelBlock){
                objCollision var=Mechanics::getCollision(nTRectangle::getCollision(bl->pos,bl->size),nTRectangle::getCollision(Player::getPlayerById(0)->pos,Player::getPlayerById(0)->size));
                if(var.firstObj!=Mechanics::NOCOLLISION){
                    if(!Scenes::freeGameMode){
                        Player::checkpoint=0;
                        if(Player::stage==lvlGoodTeacher||Player::stage==lvlBadTeacher){
                            Player::stage=0;
                            Map::GG(true);
                        }else{
                            Map::next();
                        }
                        AssetsLoader::saveSettings();
                    }else
                        Map::next();
                }
            }else if(Blocks::checkIfBlocksIsPowerUpBlock(bl->type)){
                objCollision var=Mechanics::getCollision(Player::getPlayerById(0)->swordCollision,nTRectangle::getCollision(bl->pos,bl->size));
                if((var.secondObj==Mechanics::BOTTOM||var.secondObj==Mechanics::LEFT||var.secondObj==Mechanics::RIGHT)&&Player::getPlayerById(0)->atackDirection==Util::direction_up){
                    nTPoint tmp=bl->pos;
                    tmp.y-=Blocks::defaultBlockSize.y*1.5;
                    new PowerUp(Blocks::getPowerUpBlockId(bl->type),tmp,nTPoint::get(20,20));
                    bl->type=Blocks::InvalidPowerUpBlock;
                    bl->tex->setTextures(GL::getTextureByName("PowerUpOff"));
                    Player::getPlayerById(0)->PowerUpsActiveted++;
                }
            }else if(Blocks::checkIfBlocksIsPowerUpChest(bl->type)&&Player::getPlayerById(0)->atacking){
                objCollision var=Mechanics::getCollision(Player::getPlayerById(0)->swordCollision,nTRectangle::getCollision(bl->pos,bl->size));
                if(var.firstObj!=Mechanics::NOCOLLISION){
                    nTPoint tmp=bl->pos;
                    tmp.y-=Blocks::defaultBlockSize.y*1.5;
                    new PowerUp(Blocks::getPowerUpChestId(bl->type),tmp,nTPoint::get(20,20));
                    bl->type=Blocks::InvalidPowerUpChest;
                    AL::singleton->playSoundByName("chestOpen");
                    bl->tex->setTextures(GL::getTextureByName("BauOff"));
                    Player::getPlayerById(0)->PowerUpsActiveted++;
                }
            }else if(Blocks::checkIfBlocksIsDestrutive(bl->type)&&Player::getPlayerById(0)->atacking){
                objCollision var=Mechanics::getCollision(Player::getPlayerById(0)->swordCollision,nTRectangle::getCollision(bl->pos,bl->size));
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
 *  Gets the id of the block based on its position
 *
 *  @param pos position of the block
 *  @return id of the block on the vectors
**/
int Map::getIdByPosition(nTPoint pos){
    FunctionAnalyser::startFunction("Map::getIdByPosition");
    Blocks* bl;
    int y=(int)floor(pos.y/Blocks::defaultBlockSize.y);
    if(y<staticBlocksArr.size()){
        int x=(int)floor(pos.x/Blocks::defaultBlockSize.x);
        if(x<staticBlocksArr[y].size()){
            if(staticBlocksArr[y][x]>=0){
                FunctionAnalyser::endFunction("Map::getIdByPosition");
                return staticBlocksArr[y][x];
            }
        }
    }
    for(int i=0; i<dynamicBlocks.size(); i++){
        bl=(Blocks*)dynamicBlocks[i];
        if(bl->pos.x==pos.x && bl->pos.y==pos.y){
            FunctionAnalyser::endFunction("Map::getIdByPosition");
            return bl->id;
        }
    }
    FunctionAnalyser::endFunction("Map::getIdByPosition");
    return 0;
}

/**
 *  Handle gameover events
 *
 *  @param isWinner false if the player lose the game
**/
void Map::GG(bool isWinner){
    if(Scenes::freeGameMode){
        Scenes::current=Scenes::menu;
        return;
    }
    if(isWinner){
        Player::refreshGlobalcoeficiente();
        Player::lives++;
        Player::beatGame=true;
        Scenes::current=Scenes::posYouWin;
    }else{
        Player::stage=0;
        Player::checkpoint=0;
        Player::lives=3;
        Player::globalCoeficiente=0;
        Scenes::current=Scenes::posGameEnd;
    }
    AssetsLoader::saveSettings();
}

/**
 *  Handle collision with end level block events
**/
void Map::next(){
    Player::refreshGlobalcoeficiente();
    if(Scenes::freeGameMode){
        Scenes::current=Scenes::posGame;
    }else{
        if(Player::lives>=0){
            if(Player::getPlayerById(0)->life>0)
                Player::stage++;
            Scenes::current=Scenes::posGame;
        }else
            GG(false);
    }
}

/**
 *  Loads a map
 *
 *  @param path filepath to the map
 *  @return true if the map was successfully loaded, otherwise false
**/
bool Map::loadMap(string path){
    nTMap structOfMap;
    ifstream mapFILE(path);
    if(mapFILE.is_open()){
        string lineSelected;
        getline(mapFILE,lineSelected);//read version
        getline(mapFILE,lineSelected);
        structOfMap.mapADC=lineSelected;

        vector<int> listOfReadNumbers;
        int readNumberInINT;
        string readNumberInString;

        int layer=0;
        vector<vector<pair<int,string> > > tmpMap2D;
        vector<pair<int,string> > tmpMap1D;
        structOfMap.map.push_back(tmpMap2D); // aciona ao map uma linha nula (que logo será preenchida)
        while(mapFILE.good()){//!mapFILE.eof()
            while(getline(mapFILE,lineSelected)){
                if(lineSelected=="nTMap---newLayer---nTMap"){ // quando mudar de camada adiciona o map a linha criada na ultima iteracao o loop
                    layer++;
                    structOfMap.map.push_back(tmpMap2D);
                }else{
                    pair<int,string> tmp;
                    for(int i=0;i<lineSelected.size();i++){
                        if(lineSelected[i]!='\''){
                            readNumberInString+=lineSelected[i];
                        }else{
                            size_t found = readNumberInString.find_first_of("\\");
                            if(found!=string::npos){ // achou '\'
                                tmp.second=readNumberInString.substr(found+1,readNumberInString.size()-found); // guarda adCode
                                Util::replaceAllOccurrences(tmp.second,"\"-ad-Xchar-13-\"","\n");
                                Util::replaceAllOccurrences(tmp.second,"\"-ad-Xchar-92-\"","\\");
                                Util::replaceAllOccurrences(tmp.second,"\"-ad-Xchar-39-\"","\'");
                                readNumberInString=readNumberInString.substr(0,found);
                            }else{ // nao achou (nao tem adCode)
                                tmp.second="";
                            }
                            tmp.first=Util::strToInt(readNumberInString); // pega Id do bloco
                            tmpMap1D.push_back(tmp);
                            readNumberInString.clear();
                        }
                    }
                    structOfMap.map[layer].push_back(tmpMap1D);
                    tmpMap1D.clear();
                }
            }
        }
        string strT(path);
        if(strT.find("Maps/user")==string::npos)
            maps.push_back(structOfMap);
        else
            usrMap.push_back(structOfMap);
        mapFILE.close();
        return true;
    }else{
        if(Util::DEBUG)
        cout<<"Erro ao carregar o arquivo "<<path<<endl;
        return false;
  }
}

/**
 *  Saves a map
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
        save=usrMap[currentUsrMap];
    }
    ofstream mapFILE(path);
    if(mapFILE.is_open()){
        SETTINGS s;
        mapFILE<<s.version<<endl;
        string back=save.mapADC;
        Util::replaceAllOccurrences(back,"\n","\"-ad-Xchar-13-\"");
        Util::replaceAllOccurrences(back,"\'","\"-ad-Xchar-39-\"");
        Util::replaceAllOccurrences(back,"\\","\"-ad-Xchar-92-\"");
        mapFILE<<back<<endl;
        for(int k=0;k<save.map.size();k++){
            for(int i=0;i<save.map[k].size();i++){
                for(int j=0;j<save.map[k][i].size();j++){
                    Util::replaceAllOccurrences(save.map[k][i][j].second,"\n","\"-ad-Xchar-13-\"");
                    Util::replaceAllOccurrences(save.map[k][i][j].second,"\'","\"-ad-Xchar-39-\"");
                    Util::replaceAllOccurrences(save.map[k][i][j].second,"\\","\"-ad-Xchar-92-\"");
                    mapFILE<<save.map[k][i][j].first<<'\\'<<save.map[k][i][j].second<<'\'';
                }
                mapFILE<<endl;
            }
            mapFILE<<"nTMap---newLayer---nTMap"<<endl;
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
 *  Gets the block object from its id
 *
 *  @param id the id of the block
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
    }else if(id>=0){
        return (Blocks*)Map::staticBlocks[id];
    }
    return nullptr;
}
