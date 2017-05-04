#include "Map.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "../Libs/Globals.h"
#include "Scenes.hpp"
#include "Blocks.hpp"
#include "Tutorials.hpp"
#include "Bullet.hpp"
#include "powerUp.hpp"

Map::Map() {
}

Map::Map(const Map& orig) {
}

Map::~Map() {
}

vector<vector<int> > Map::currentMap;
vector<void*> Map::staticBlocks;
vector<void*> Map::dynamicBlocks;
int Map::dynamicGLlist;
int Map::staticGLlist;
GLuint Map::background;
nTPoint Map::size;
int Map::nOfMaps=3;
int Map::nOfBackgrounds=3;
int Map::nOfEnemys=0;
int Map::expetedTime=0;
int Map::totalPowerUps=0;

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
    Mechanics::enemys.clear();
    Bullet::self.clear();
    powerUp::self.clear();
    currentMap.clear();

    size.setPoint((map.map[0].size()-1)*Blocks::defaultBlockSize.x,(map.map.size()-1)*Blocks::defaultBlockSize.y,0);
    char buffer[5];
    snprintf(buffer,5,"%d",map.backgroundId);
    string mapID(buffer);
    currentMap=map.map;
    background=GL::getTextureByName("background"+mapID);
    setBlockPos();
    createStaticGLlist();
    createDynamicGLlist();
}

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

void Map::setBlockPos(){
    deleteAllBlocks();
    Blocks *bl;
    Enemy *en;
    for(int i=0;i<currentMap.size();i++){
        for(int j=0;j<currentMap[0].size();j++){
            bl=new Blocks(currentMap[i][j],Util::nTPointSet(Blocks::defaultBlockSize.x*(j+(1/2))+Blocks::defaultBlockSize.x/2,Blocks::defaultBlockSize.y*(i+(1/2))+Blocks::defaultBlockSize.y/2,Blocks::defaultBlockSize.z),Blocks::defaultBlockSize);
            if(Blocks::checkIfBlocksIsDynamic(currentMap[i][j])){
                if(currentMap[i][j]==1000&&(Player::checkpoint==0||Scenes::freeGameMode)){
                    bl->pos.z=0.9;
                    player->spawn(bl->pos,player->life);
                }else if(currentMap[i][j]==1000);

                else if(currentMap[i][j]>=2000&&currentMap[i][j]<=3000){
                    bl->pos.z=0.89;
                    nOfEnemys++;
                    en=new Enemy(currentMap[i][j]-2000,Enemy::defaultLife,bl->pos,Enemy::defaultSize,Entity::getAnimationVector(enemyAnim[rand()%enemyAnim.size()],enemyAnimSize[rand()%enemyAnimSize.size()]),0);
                }else if(currentMap[i][j]>5000){
                    bl->pos.z=0.89;
                    en=new Enemy((currentMap[i][j]-5000)+100,Enemy::bossLife,bl->pos,Enemy::bossSize,Entity::getAnimationVector(enemyAnim[0],enemyAnimSize[0]),0);
                    nOfEnemys+=3;
                }else dynamicBlocks.push_back(bl);
                if((currentMap[i][j]>=301&&currentMap[i][j]<=325)||(currentMap[i][j]>=476&&currentMap[i][j]<=500)){
                    totalPowerUps++;
                }
            }else if(currentMap[i][j]!=0){
                staticBlocks.push_back(bl);
            }
            if(currentMap[i][j]==4001&&Player::checkpoint==1&&!Scenes::freeGameMode){
                nTPoint pos=bl->pos;
                pos.z=0.9;
                pos.y-=Blocks::defaultBlockSize.y;
                player->spawn(pos,player->life);
            }else
            if(currentMap[i][j]==4002&&Player::checkpoint==2&&!Scenes::freeGameMode){
                nTPoint pos=bl->pos;
                pos.z=0.9;
                pos.y-=Blocks::defaultBlockSize.y;
                player->spawn(pos,player->life);
            }
        }
    }
    expetedTime=currentMap.size();
}

void Map::draw(){
    glCallList(dynamicGLlist);
    glCallList(staticGLlist);
}

vector <mapCollision> Map::checkCollision(nTPoint pos,nTPoint size){
    int x32=Blocks::defaultBlockSize.x,x16=x32/2;
    int y32=Blocks::defaultBlockSize.y,y16=y32/2;
    bool c1, c2, c3, c4;
    c1=c2=c3=c4=false;
    int j_sup,j_inf, i_sup, i_inf, i, j;
    nTRectangle tmp, rec;
    nTPoint point, pointPrincipal;
    mapCollision adc;
    vector <nTPoint> vertexs;
    vector <mapCollision> result;
    adc.collision.firstObj=adc.collision.secondObj=adc.blockRef=0;
    result.push_back(adc);
    tmp=Util::getCollisionRectangle(pos,size);
    vertexs=Util::getRetangleVertexs(tmp);
    pos.x=floor(pos.x);
    pos.y=ceil(pos.y);
    i_inf=floor(vertexs[0].x/Blocks::defaultBlockSize.x);
    i_sup=floor(vertexs[1].x/Blocks::defaultBlockSize.x);
    j_inf=ceil(vertexs[2].y/Blocks::defaultBlockSize.y);
    j_sup=ceil(vertexs[1].y/Blocks::defaultBlockSize.y);
    pointPrincipal.setPoint(i_inf*y32+y16,j_inf*x32+x16,0);
    for(i=i_inf-2; i<=i_sup+2; i++){
      for(j=j_inf-2;j<=j_sup+2; j++){
        if(j>=0 && j<currentMap.size() && i>=0 && i<currentMap[0].size()){
          point.setPoint(i*y32+y16,j*x32-x16,0);
          if(getBlockReference(point)!=getBlockReference(pos)){
          rec=Util::getCollisionRectangle(point,Blocks::defaultBlockSize);
        if(Blocks::checkIfBlocksIsMassive(getIdByPosition(point))){
          adc.collision=Mechanics::getCollision(tmp,rec);
          adc.blockRef=getBlockReference(point);
          if(result[0].collision.firstObj==0)
            result.clear();
          if(adc.collision.firstObj!=0 && adc.collision.secondObj!=0){
            result.push_back(adc);
          }
        }
          }
        }
      }
    }
    int a, b;
    tmp=Util::getCollisionRectangle(player->pos,player->size);
    vertexs=Util::getRetangleVertexs(tmp);
    a=floor(vertexs[0].x/32);
    b=ceil(vertexs[0].y/32);
    point.setPoint(a*y32+y16,b*x32-x16,0);
    int type=getIdByPosition(point);
    if(Blocks::checkIfBlocksIsMassive(type)&&!(type>375&&type<=400)&&!(type==666)&&!(type>4000&&type<4013)&&!(type<-200)){
      player->pos.y=player->pos.y-16;
    }
      return result;
}

void Map::refresh(){
    if(GL::isPaused){
        GL::drawTexture(Util::nTRectangleSet(Scenes::camera.x.movedCam,GL::defaultSize.y+Scenes::camera.y.movedCam,GL::defaultSize.x+Scenes::camera.x.movedCam,Scenes::camera.y.movedCam,-0.9,-0.9),Util::nTColorSet(1,1,1,1),background,1);
        return;
    }
    Blocks *bl;
    Bullet *bu;
    for(int i=0;i<dynamicBlocks.size();i++){
        bl=(Blocks*)dynamicBlocks[i];
        if(Scenes::isInTheScreen(Util::getCollisionRectangle(bl->pos,bl->size))||bl->type==255){
        if(bl->type>250&&bl->type<=300)
            bl->move(Util::up,bl->moveSpeed/GL::getFPS());
        else if(bl->type>200&&bl->type<=250)
            bl->move(Util::left,bl->moveSpeed/GL::getFPS());
        else if(bl->type>=101&&bl->type<200){
            if(round(fmodl(GL::getGameMs(),(int)Bullet::timeToShoot/5))==0&&player->life>0){
                nTPoint tmp=bl->pos;
                tmp.z=0.9;
                tmp.x+=Blocks::defaultBlockSize.x*1.2;
                bu=new Bullet(0,Bullet::baseSpeed,tmp,Util::nTPointSet(40,20,1));
                tmp.x-=Blocks::defaultBlockSize.x*1.2*2;
                bu=new Bullet(0,-Bullet::baseSpeed,tmp,Util::nTPointSet(40,20,1));
            }
        }
        else if(bl->type==200){
            if(round(fmodl(GL::getGameMs(),(int)Bullet::timeToShoot))==0&&player->life>0){
                nTPoint tmp=bl->pos;
                tmp.z=0.9;
                tmp.y-=Blocks::defaultBlockSize.y/3;
                if(player->pos.x>=bl->pos.x){
                    tmp.x+=Blocks::defaultBlockSize.x*1.2;
                    bu=new Bullet(4,Bullet::baseSpeed*1.3,tmp,Util::nTPointSet(100,60,1));
                }else{
                    tmp.x-=Blocks::defaultBlockSize.x*2.4;
                    bu=new Bullet(4,Bullet::baseSpeed*-1.3,tmp,Util::nTPointSet(-100,60,1));
                }
            }
        }
        else if(bl->type==501){
            objCollision var;
            nTPoint point=player->pos;
            point.y+=2;
            var=Mechanics::getCollision(Util::getCollisionRectangle(point,player->size),Util::getCollisionRectangle(bl->pos,bl->size));
            if(var.firstObj==1&&var.secondObj==2){
                player->vSpeed=-player->jumpSpeed*1.5;
                player->canJump=false;
                al->playSoundByName("mola");
            }
        }
        else if(bl->type==4001){
            objCollision var;
            var=Mechanics::getCollision(Util::getCollisionRectangle(bl->pos,bl->size),Util::getCollisionRectangle(player->pos,player->size));
            if(var.firstObj&&Player::checkpoint<1&&!Scenes::freeGameMode){
                Player::checkpoint=1;
                saveSettings();
            }
        }
        else if(bl->type==4002){
            objCollision var;
            var=Mechanics::getCollision(Util::getCollisionRectangle(bl->pos,bl->size),Util::getCollisionRectangle(player->pos,player->size));
            if(var.firstObj&&Player::checkpoint<2&&!Scenes::freeGameMode){ ;
               Player::checkpoint=2;
                saveSettings();
            }
        }
        else if(bl->type==666){
            objCollision var;
            var=Mechanics::getCollision(Util::getCollisionRectangle(bl->pos,bl->size),Util::getCollisionRectangle(player->pos,player->size));
            if(var.firstObj){
                if(!Scenes::freeGameMode){
                    Player::checkpoint=0;
                    Player::stage++;
                    if(Player::stage>=Map::nOfMaps){
                        Player::stage=0;
                        Map::GG(true);
                    }else{
                        Map::next();
                    }
                    saveSettings();
                }else
                    Map::next();
            }
        }
        else if(bl->type>=301&&bl->type<325){
            objCollision var;
            var=Mechanics::getCollision(player->swordCollision,Util::getCollisionRectangle(bl->pos,bl->size));
            if(var.firstObj==2&&var.secondObj==1){
                nTPoint tmp=bl->pos;
                tmp.y-=Blocks::defaultBlockSize.y*1.5;
                powerUp *pu=new powerUp(bl->type-301,tmp,Util::nTPointSet(20,20,0),true);
                bl->type=325;
                bl->tex=GL::getTextureByName("PowerupOff");
                al->playSoundByName("chestOpen");
                player->powerUpsActiveted++;
            }
        }
        else if(bl->type>=476&&bl->type<500&&player->atacking){
            objCollision var;
            var=Mechanics::getCollision(player->swordCollision,Util::getCollisionRectangle(bl->pos,bl->size));
            if(var.firstObj){
                nTPoint tmp=bl->pos;
                tmp.y-=Blocks::defaultBlockSize.y*1.5;
                powerUp *pu=new powerUp(bl->type-476,tmp,Util::nTPointSet(20,20,0),true);
                bl->type=500;
                bl->tex=GL::getTextureByName("BauOff");
            }
        }else if(bl->type>=326&&bl->type<=350&&player->atacking){
            objCollision var;
            var=Mechanics::getCollision(player->swordCollision,Util::getCollisionRectangle(bl->pos,bl->size));
            if(var.firstObj){
                bl->applyDamage(1);
            }
        }
        }

    }
    createDynamicGLlist();
    draw();
}

void Map::createDynamicGLlist(){
    glNewList(dynamicGLlist,GL_COMPILE);
    Blocks* bl;
    GL::drawTexture(Util::nTRectangleSet(Scenes::camera.x.movedCam,GL::defaultSize.y+Scenes::camera.y.movedCam,GL::defaultSize.x+Scenes::camera.x.movedCam,Scenes::camera.y.movedCam,-0.9,-0.9),Util::nTColorSet(1,1,1,1),background,1);
    for(int i=0;i<dynamicBlocks.size();i++){
        bl=(Blocks*)dynamicBlocks[i];
        bl->draw();
    }
    glEndList();
}

void Map::createStaticGLlist(){
    glNewList(staticGLlist,GL_COMPILE);
    Blocks* bl;
    for(int i=0;i<staticBlocks.size();i++){
        bl=(Blocks*)staticBlocks[i];
        bl->draw();
    }
    glEndList();
}

int Map::getBlockReference(nTPoint pos){
  Blocks* bl;
  for(int i=0; i<staticBlocks.size(); i++){
    bl=(Blocks*)staticBlocks[i];
    if(bl->pos.x==pos.x && bl->pos.y==pos.y)
      return i;
  }
  for(int i=0; i<dynamicBlocks.size(); i++){
    bl=(Blocks*)dynamicBlocks[i];
    if(bl->pos.x==pos.x && bl->pos.y==pos.y)
      return i+staticBlocks.size();
  }
  return -1;
}

int Map::getIdByPosition(nTPoint pos){
  Blocks* bl;
  for(int i=0; i<staticBlocks.size(); i++){
    bl=(Blocks*)staticBlocks[i];
    if(bl->pos.x==pos.x && bl->pos.y==pos.y){
      return bl->type;
    }
  }
  for(int i=0; i<dynamicBlocks.size(); i++){
    bl=(Blocks*)dynamicBlocks[i];
    if(bl->pos.x==pos.x && bl->pos.y==pos.y){
      return bl->type;
    }
  }
  return 0;
}

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
    saveSettings();
}
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
