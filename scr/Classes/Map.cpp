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
int Map::nOfMaps=6;
int Map::nOfBackgrounds=3;
int Map::nOfEnemys=0;
int Map::expetedTime=0;
int Map::totalPowerUps=0;
vector<nTMap> Map::maps;
nTMap Map::usrMap;
nTMap Map::editingMap;

const int Map::lvlTechnical=0;
const int Map::lvlGraduation=1;
const int Map::lvlMasters=2;
const int Map::lvlWork=3;
const int Map::lvlGoodTeacher=4;
const int Map::lvlBadTeacher=5;

/**
 *	Change the current map
 *
 *	@param map the new map
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
    size.set((map.map[0].size()-1)*Blocks::defaultBlockSize.x,(map.map.size()-1)*Blocks::defaultBlockSize.y,0);
    sort(map.backgrounds.begin(), map.backgrounds.end()); // coloca em ordem decrescente os backgrounds a serem desenhados
    actualMap=map;
    bool hasSpawn=false;
    for(int i=0;i<actualMap.map.size();i++){
        for(int j=0;j<actualMap.map[i].size();j++){
            if(Blocks::checkIfBlocksIsPlayerSpawn(actualMap.map[i][j].first)){
                hasSpawn=true;
                break;
            }
        }
    }
    if(!hasSpawn){
        actualMap.map[actualMap.map.size()/2][4].first=1000;
    }
    setBlockPos();
}

/**
 *	Delete all blocks and enemys objects and reset map statistics
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
}

/**
 *	Instantiate and config all blocks after deleting the older ones according to the current map
**/
void Map::setBlockPos(){
    deleteAllBlocks();
    Tutorials::clear();
    Blocks *bl;
    for(int i=0;i<actualMap.map.size();i++){
        vector<int> v;
        staticBlocksArr.push_back(v);
        for(int j=0;j<actualMap.map[0].size();j++){
            staticBlocksArr[i].push_back(-1);
            bl=new Blocks(actualMap.map[i][j].first,nTPoint::get(Blocks::defaultBlockSize.x*(j+(1/2))+Blocks::defaultBlockSize.x/2,Blocks::defaultBlockSize.y*(i+(1/2))+Blocks::defaultBlockSize.y/2,Blocks::defaultBlockSize.z),Blocks::defaultBlockSize,actualMap.map[i][j].second);
            if(Blocks::checkIfBlocksIsDynamic(actualMap.map[i][j].first)){
                if(Blocks::checkIfBlocksIsPlayerSpawn(actualMap.map[i][j].first)&&(Player::checkpoint==0||Scenes::freeGameMode)){
                    nTPoint pos=bl->pos;
                    pos.z=0.9;
                    pos.y-=Blocks::defaultBlockSize.y;
                    if(Scenes::freeGameMode)
                        Player::getPlayerById(0)->spawn(pos,Player::defaultLife);
                    else
                        Player::getPlayerById(0)->spawn(pos,Player::getPlayerById(0)->life);
                }else if(Blocks::checkIfBlocksIsPlayerSpawn(actualMap.map[i][j].first));

                else if(Blocks::checkIfBlocksIsEnemySpawn(actualMap.map[i][j].first)){
                    nTPoint pos=bl->pos;
                    pos.z=0.89;
                    pos.y-=Blocks::defaultBlockSize.y;
                    nOfEnemys++;
                    int random=rand()%Enemy::enemyAnim.size();
                    new Enemy(actualMap.map[i][j].first-2000,Enemy::defaultLife,pos,Enemy::defaultSize,Entity::getAnimationVector(Enemy::enemyAnim[random],Enemy::enemyAnimSize[random]));
                }else if(Blocks::checkIfBlocksIsBossSpawn(actualMap.map[i][j].first)){
                    nTPoint pos=bl->pos;
                    pos.z=0.89;
                    pos.y-=Blocks::defaultBlockSize.y;
                    new Boss(bl->data,bl->pos);
                }else if(Blocks::checkIfBlocksIsTutorial(actualMap.map[i][j].first)){
                    Tutorials::add(j,bl->data,false);
                }else if(Blocks::checkIfBlocksIsTutorialPause(actualMap.map[i][j].first)){
                    Tutorials::add(j,bl->data,true);
                }else{
                    bl->id=dynamicBlocks.size();
                    dynamicBlocks.push_back(bl);
                }
                if(Blocks::checkIfBlocksIsPowerUpChest(actualMap.map[i][j].first)||Blocks::checkIfBlocksIsPowerUpBlock(actualMap.map[i][j].first)){
                    totalPowerUps++;
                }
            }else if(!Blocks::checkIfBlocksIsAir(actualMap.map[i][j].first)){
                bl->id=staticBlocks.size();
                staticBlocksArr[i][j]=bl->id;
                staticBlocks.push_back(bl);
            }
            if(Blocks::checkIfBlocksIsCheckpoint(actualMap.map[i][j].first)){
                if(Blocks::getCheckPointId(actualMap.map[i][j].first)==Player::checkpoint&&!Scenes::freeGameMode){
                    nTPoint pos=bl->pos;
                    pos.z=0.9;
                    pos.y-=Blocks::defaultBlockSize.y;
                    Player::getPlayerById(0)->spawn(pos,Player::getPlayerById(0)->life);
                    Tutorials::checkpoints.push_back(j);
                }
            }
        }
    }
    for(int i=0;i<dynamicBlocks.size();i++){
        bl=(Blocks*)dynamicBlocks[i];
        bl->id+=staticBlocks.size();
    }
    Tutorials::processCheckpoints();
    expetedTime=(int)actualMap.map.size();
}

/**
 *	Draw the current map on the screen
**/
void Map::draw(){
    FunctionAnalyser::startFunction("Map::draw");
    Blocks* bl;
    drawMapBackground();
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
    for(i=i_inf;i<=i_sup;i++){
        for(j=j_inf;j<=j_sup;j++){
            if(j>=0 && j<=actualMap.map.size() && i>=0 && i<=actualMap.map[0].size()){
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
 *	Run logic events of the Map and it blocks like move, change directions and textures, spawn bullets, handle block events and draw
**/
void Map::refresh(){
    FunctionAnalyser::startFunction("Map::refresh");
    if(GL::isPaused){
        drawMapBackground();
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
                        Player::stage++;
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
                    bl->tex=GL::getTextureByName("PowerUpOff");
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
                    bl->tex=GL::getTextureByName("BauOff");
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
 *	Gets the id of the block based on its position
 *
 *	@param pos position of the block
 *	@return id of the block on the vectors
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
    if(isWinner){
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
        vector<pair<int,string> > tmp3;
        getline(mapFILE,tmp2);//read version
        getline(mapFILE,tmp2);
        int loop=Util::strToInt(tmp2);
        vector<int> b;
        int c;
        string a;
        for(int i=0; i<loop; i++){
            getline(mapFILE,tmp2);
            for(int j=0;j<tmp2.size();j++){
                if(tmp2[j]!='\''){
                    a+=tmp2[j];
                }else{
                    c=Util::strToInt(a);
                    b.push_back(c);
                    a.clear();
                }
            }
            string name, path;
            getline(mapFILE, name);
            getline(mapFILE, path);
            name.erase(remove(name.begin(),name.end(),13),name.end());
            path.erase(remove(path.begin(),path.end(),13),path.end());
            Background back(name, path, b[0]==1, b[1]/100.0, b[2], b[3], b[4], b[5], b[6], b[7]/100.0, b[8]/100);
            GL::loadTexture(back.getName(),back.getPath());
            tmp.backgrounds.push_back(back);
            b.clear();
        }
        while(mapFILE.good()){//!mapFILE.eof()
            while(getline(mapFILE,tmp2)){
                pair<int,string> tmp4;
                string tmp5;
                for(int i=0;i<tmp2.size();i++){
                    if(tmp2[i]!='\''){
                        tmp5+=tmp2[i];
                    }else{
                        size_t found = tmp5.find_first_of("\\");
                        if(found!=string::npos){
                            tmp4.second=tmp5.substr(found+1,tmp5.size()-found);
                            Util::replaceAllOccurrences(tmp4.second,"\"-ad-Xchar-13-\"","\n");
                            Util::replaceAllOccurrences(tmp4.second,"\"-ad-Xchar-92-\"","\\");
                            Util::replaceAllOccurrences(tmp4.second,"\"-ad-Xchar-39-\"","\'");
                            tmp5=tmp5.substr(0,found);
                        }else{
                            tmp4.second="";
                        }
                        tmp4.first=Util::strToInt(tmp5);
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
        SETTINGS s;
        mapFILE<<s.version<<endl;
        mapFILE<<save.backgrounds.size()<<endl;
        for(int i=0; i<save.backgrounds.size(); i++){
            if(save.backgrounds[i].getMove())
                mapFILE<<1<<'\'';
            else
                mapFILE<<0<<'\'';
            mapFILE<<save.backgrounds[i].getzAxis()<<"\'"<<save.backgrounds[i].getLocal().p0.x<<"\'"<<save.backgrounds[i].getLocal().p0.y<<"\'"<<save.backgrounds[i].getLocal().p1.x<<"\'"<<save.backgrounds[i].getLocal().p1.y<<endl;
            mapFILE<<save.backgrounds[i].getName()<<endl;
            mapFILE<<save.backgrounds[i].getPath()<<endl;
        }
        for(int i=0;i<save.map.size();i++){
            for(int j=0;j<save.map[i].size();j++){
                Util::replaceAllOccurrences(save.map[i][j].second,"\n","\"-ad-Xchar-13-\"");
                Util::replaceAllOccurrences(save.map[i][j].second,"\'","\"-ad-Xchar-39-\"");
                Util::replaceAllOccurrences(save.map[i][j].second,"\\","\"-ad-Xchar-92-\"");
                mapFILE<<save.map[i][j].first<<'\\'<<save.map[i][j].second<<'\'';
            }
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
    }else if(id>=0){
        return (Blocks*)Map::staticBlocks[id];
    }
    return nullptr;
}

void Map::drawMapBackground(){
    for(int i=0; i<actualMap.backgrounds.size(); i++){
        if(!actualMap.backgrounds[i].getMove())
            GL::drawTexture(nTRectangle::get(Scenes::camera.x.movedCam,GL::defaultSize.y+Scenes::camera.y.movedCam,GL::defaultSize.x+Scenes::camera.x.movedCam,Scenes::camera.y.movedCam,actualMap.backgrounds[i].getzAxis()),GL::getTextureByName(actualMap.backgrounds[i].getName()));
        else
            actualMap.backgrounds[i].drawParalaxBackground(size);
    }
}
