#include "Mechanics.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Blocks.hpp"

Mechanics::Mechanics(){
    forceNormalForce=false;
};

Mechanics::Mechanics(const Mechanics& orig) {
}

Mechanics::~Mechanics() {
}

float Mechanics::gravity=500.0;
vector<void*> Mechanics::players;
vector<void*> Mechanics::enemys;

void Mechanics::applyGravity(){
    if(GL::isPaused||Tutorials::isPaused)
        return;
    Player *pl;
    for(int i=0;i<players.size();i++){
        pl=(Player*)players[i];
        if(!pl->itsInTheWater){
            if(!checkNormalForce(pl->pos,pl->size)){
                pl->vSpeed+=gravity/GL::getFPS();
                pl->canJump=false;
            }else{
                if(pl->vSpeed>0)
                    pl->vSpeed=0;
                if(!pl->spacePressed)pl->canJump=true;
            }
        }else{
            pl->vSpeed+=gravity/GL::getFPS()/3;
            pl->canJump=true;
            if(checkNormalForce(pl->pos,pl->size))
                if(pl->vSpeed>0)
                    pl->vSpeed=0;
        }
    }
    Enemy *en;
    for(int i=0;i<enemys.size();i++){
        en=(Enemy*)enemys[i];
        if(!checkNormalForce(en->pos,en->size)){
            en->vSpeed+=gravity*(1/GL::getFPS());
            en->canJump=false;
        }else{
            if(en->vSpeed>0)
                en->vSpeed=0;
            en->canJump=true;
        }
    }
}

void Mechanics::applyForce(){
    applyGravity();
    if(GL::isPaused||Tutorials::isPaused)
        return;
    Player *pl;
    for(int i=0;i<players.size();i++){
        pl=(Player*)players[i];
        if(pl->hSpeed!=0){
            pl->move(Util::left,pl->hSpeed/GL::getFPS());
        }
        if(pl->vSpeed>0){
            pl->move(Util::down,pl->vSpeed/GL::getFPS());
        }else if(pl->vSpeed<0){
            pl->move(Util::up,pl->vSpeed/GL::getFPS());
        }
    }
    Enemy *en;
    for(int i=0;i<enemys.size();i++){
        en=(Enemy*)enemys[i];
        if(en->hSpeed!=0){
            float tmp=en->hSpeed;
            en->move(Util::left,en->hSpeed/GL::getFPS());
            if(en->hSpeed==0)
                en->hSpeed=-tmp;
        }
        if(en->vSpeed>0){
            en->move(Util::down,en->vSpeed/GL::getFPS());
        }else if(en->vSpeed<0){
            en->move(Util::up,en->vSpeed/GL::getFPS());
        }
    }
}

void Mechanics::move(int dir,float steeps){
    bool needToMoveAgain=false;
    float steepsAgain;
    if(ABS(steeps)>Entity::walkSpeed/GL::getFPS()){
        int signal=1;
        needToMoveAgain=true;
        if(steeps<0)
            signal=-1;
        steepsAgain=signal*(ABS(steeps)-Entity::walkSpeed/GL::getFPS());
        steeps=signal*Entity::walkSpeed/GL::getFPS();
    }
    Blocks *bl;
    int type;
    vector <mapCollision> var;
    bool condition=true;
    if(dir==Util::left||dir==Util::right){
      nTPoint tmp;
      tmp.setPoint(pos.x+steeps,pos.y,pos.z);
      var=Map::checkCollision(tmp,size);
      for(int i=0; i<var.size(); i++){
        type=0;
        if(var[i].blockRef>=Map::staticBlocks.size()&&var[i].blockRef>0){
          bl=(Blocks*)Map::dynamicBlocks[var[i].blockRef-Map::staticBlocks.size()];
          type=bl->type;
        }else if(var[i].blockRef>0){
          bl=(Blocks*)Map::staticBlocks[var[i].blockRef];
          type=bl->type;
        }
        if(var[i].collision.firstObj>=3&&!(type>375&&type<=400)&&!(type==666)&&!(type>4000&&type<4013)&&!(type<-200)){
          condition=false;
        }

      }
      if(condition)
        pos.x+=steeps;
      else
        hSpeed=0;

    }else if(dir==Util::up||dir==Util::down){
      nTPoint tmp;
      tmp.setPoint(pos.x,pos.y+steeps,pos.z);
      var=Map::checkCollision(tmp,size);
      for(int i=0; i<var.size(); i++){
        type=0;
        if(var[i].blockRef>=Map::staticBlocks.size()){
          bl=(Blocks*)Map::dynamicBlocks[var[i].blockRef-Map::staticBlocks.size()];
          type=bl->type;
        }
        if((var[i].collision.firstObj==2 || var[i].collision.firstObj==1)&&!(type>375&&type<=400)&&!(type==666)&&!(type>4000&&type<4013)&&!(type<-200)){
            condition=false;
          }
      }
      if(condition)
        pos.y+=steeps;
      else
        vSpeed=0;
    }
    if(needToMoveAgain)
        move(dir,steepsAgain);
}

bool Mechanics::checkNormalForce(nTPoint pos_,nTPoint size_){
    if(player->forceNormalForce&&pos_.x==player->pos.x&&size_.x==player->size.x&&pos_.y==player->pos.y&&size_.y==player->size.y) return true;
    vector <mapCollision> var;
    Blocks *bl;
    int type;
    bool condition=false;
    nTPoint tmp;
    tmp.setPoint(pos_.x,pos_.y+1,pos_.z);
    var=Map::checkCollision(tmp,size_);
    for(int i=0; i<var.size(); i++){
        type=0;
        if(var[i].blockRef>=Map::staticBlocks.size()){
          bl=(Blocks*)Map::dynamicBlocks[var[i].blockRef-Map::staticBlocks.size()];
          type=bl->type;
        }
        if(var[i].collision.firstObj==1&&!(type>375&&type<=400)&&!(type==666)&&!(type>4000&&type<4013)&&!(type<-200)){
          condition=true;
        }
      }
    return condition;
}
float Mechanics::getGravity(){
    return gravity;
}

objCollision Mechanics::getCollision(nTRectangle r1, nTRectangle r2){
  vector<nTPoint> vertexs, var;
  bool c1, c2, c3, c4;
  c1=c2=c3=c4=false;
  float dist, dist2;
  nTPoint pos;
  objCollision adc;
  adc.firstObj=adc.secondObj=0;
  vertexs=Util::getRetangleVertexs(r1);
  var=Util::getRetangleVertexs(r2);
  if((vertexs[0].x>=var[0].x && vertexs[1].x<=var[1].x)) //coordenadas x do block principal dentro do bloco analizado
    c1=true;
  if((vertexs[1].x>=var[0].x && vertexs[0].x<=var[1].x)) //
    c2=true;
  if((vertexs[2].y>=var[2].y && vertexs[1].y<=var[1].y))
    c3=true;
  if((vertexs[1].y>=var[2].y && vertexs[2].y<=var[1].y))
    c4=true;
  if(c1 || c2)
    if(c3 || c4){
      pos.x=r1.p0.x+(r1.p1.x-r1.p0.x)/2;
      pos.y=r1.p0.y-(r1.p0.y-r1.p1.y)/2;
      dist= (var[0].y-Blocks::defaultBlockSize.y/2)-(pos.y) ;
      dist2= pos.x - (var[0].x+Blocks::defaultBlockSize.x/2);
        if(c1==true && c3==true){
            if(ABS(dist)<ABS(dist2)){
                if(dist2>0){
                  adc.firstObj=3;
                  adc.secondObj=4;
                }
                else{
                  adc.firstObj=4;
                  adc.secondObj=3;
                }
              }
            else{
              if(dist>0){
                adc.firstObj=1;
                adc.secondObj=2;
              }
              else{
                adc.firstObj=2;
                adc.secondObj=1;
              }
        }
      }
        else if(c1==true && c4==true){
            if(dist-(r1.p0.y-r1.p1.y)/2<0){
              adc.firstObj=2;
              adc.secondObj=1;
          }
            else{
              adc.firstObj=1;
              adc.secondObj=2;
            }
        }
        else if(c2==true && c3==true){
            if(dist2<=0){
              adc.firstObj=3;
              adc.secondObj=4;
            }
            else{
              adc.firstObj=4;
              adc.secondObj=3;
            }
        }
        else if(c2==true && c4==true){
          if(var[0].y-vertexs[3].y-5<=0 && ABS(dist2)<Blocks::defaultBlockSize.x/2+(r1.p1.x-r1.p0.x)/2-4){
              adc.firstObj=2;
              adc.secondObj=1;
            }
            else if(dist-(r1.p0.y-r1.p1.y)/2-12>=0){
              adc.firstObj=1;
              adc.secondObj=2;
            }
          else if((dist2-Blocks::defaultBlockSize.x/2-(r1.p1.x-r1.p0.x)/2)<=0 && ABS(dist)<=Blocks::defaultBlockSize.y/2+(r1.p0.y-r1.p1.y)/2){
            adc.firstObj=3;
            adc.secondObj=4;
          }
          else if((dist2+Blocks::defaultBlockSize.x/2+(r1.p1.x-r1.p0.x)/2)>=0 && ABS(dist)<=Blocks::defaultBlockSize.y/2+(r1.p0.y-r1.p1.y)/2){
            adc.firstObj=4;
            adc.secondObj=3;
          }
    }
  }
  return adc;
}
