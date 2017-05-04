#include "Blocks.hpp"

Blocks::Blocks(int type,nTPoint pos,nTPoint size) {
    this->type=type;
    this->pos=pos;
    this->size=size;
    this->tex=getTextureByIndex();
    this->brokeStage=0;
    this->damageState=false;
    this->isVisible=true;
    if(type>250&&type<=300){
        if(type==255)
            type=251;
        moveSpeed=150*((type-250)*0.6);
        this->size.y/=2;
        this->pos.y-=size.y/4;
    }
    if(type>200&&type<=250){
        moveSpeed=150*((type-200)*0.6);
        this->size.y/=2;
        this->pos.y-=size.y/4;
    }  
}

Blocks::Blocks(const Blocks& orig) {
}

Blocks::~Blocks() {
}

nTPoint Blocks::defaultBlockSize=Util::nTPointSet(32,32,0);
float Blocks::imunityTime=150;

GLuint Blocks::getTextureByIndex(){
    return GL::getTextureByName(getTexNameByIndex(this->type));  
}

string Blocks::getTexNameByIndex(int BlockType){
    if(BlockType==1||BlockType==351){
        return string("Pedra");
    }else if(BlockType==2||BlockType==352){
        return string("Terra");
    }else if(BlockType==3){    
        return string("Grama");
    }else if(BlockType==4){
        return string("Tronco Madeira");
    }else if(BlockType==5||BlockType==353){
        return string("Pedra Musgo");
    }else if(BlockType==6||BlockType==354){
        return string("Neve");
    }else if(BlockType==7){
        return string("Grama neve");
    }else if(BlockType==8){
        return string("Tijolo pedra musgo");
    }else if(BlockType==9||BlockType==355){
        return string("Arenito");
    }else if(BlockType==10){
        return string("Ender portal");
    }else if(BlockType==11||BlockType==356){
        return string("Tijolo nether");
    }else if(BlockType==12||BlockType==357){
        return string("Tijolo areia");
    }else if(BlockType==13||BlockType==358){
        return string("Tijolo pedra");
    }else if(BlockType==14||BlockType==359){
        return string("Tijolo");
    }else if(BlockType==15||BlockType==360){
        return string("Madeira");
    }else if(BlockType==16||BlockType==361){
        return string("La ciano");
    }else if(BlockType==17||BlockType==362){
        return string("Gelo");
    }else if(BlockType==19||BlockType==363){
        return string("obsidian");
    }else if(BlockType==20||BlockType==364){
        return string("areia");
    }else if(BlockType==21||BlockType==365){
        return string("La preta");
    }else if(BlockType==22||BlockType==366){
        return string("Cogumelo");
    }else if(BlockType==23||BlockType==367){
        return string("Hoper");
    }else if(BlockType==24||BlockType==368){
        return string("Books");   
    }else if(BlockType==25||BlockType==369){
        return string("Tronco com folhas");   
    }else if(BlockType==18){
        if(Scenes::current==Scenes::mapEdit)
            return string("block");
        return string("Invisivel");
    }else
        
    //meio bloco    
    if(BlockType==201||BlockType==251){
        return string("Meio bloco pedra");
    }else if(BlockType==202||BlockType==252){
        return string("Meio bloco madeira");
    }else if(BlockType==203||BlockType==253){
        return string("Meio bloco arenito");
    }else if(BlockType==204||BlockType==254){
        return string("Meio bloco gelo");
    }else if(BlockType==255){
        return string("Meio bloco Cacto");
    }else
        
    //spawn
    if(BlockType==1000||(BlockType>=2000&&BlockType<=3000)||(BlockType>5000&&BlockType<60000)){
        return string("Spawn");
    }else
        
    //alavanca
//    if(BlockType>=451&&BlockType<=475){
//        return string("Alavanca");
//    }else
        
    //tp
    if(BlockType<0&&BlockType>-200){
        return string("Cano");
    }else
    if(BlockType<0&&BlockType>-400){
        return string("Door down");
    }else
     
       //jumppad
    if(BlockType==501){
        return string("jump spring");
    }else
    
    //atira
    if(BlockType>=101&&BlockType<200){
        return string("Shooter");
    }else if(BlockType==200){
        return string("ShooterBus");
    }else
    
    //liquids
    if(BlockType==376){
        return string("Agua");
    }else if(BlockType==377){
        return string("Lava");
    }else if(BlockType==378){
        return string("AguaF");
    }else if(BlockType==379){
        return string("LavaF");
    }else if(BlockType==380){
        return string("Agua1");
    }else if(BlockType==381){
        return string("Lava1");
    }else
        
    //baus
    if(BlockType>=476&&BlockType<500){
        return string("Bau");
    }else if(BlockType==500){
        return string("BauOff");
    }else if(BlockType>=301&&BlockType<=324){
        return string("Powerup");
    }else if(BlockType==325){
        return string("PowerupOff");
    }else
        
    //mapPoints    
    if(BlockType==666){
        return string("End Game (GG)");
    }else if(BlockType==4001){
        return string("checkpoint0");
    }else if(BlockType==4002){
        return string("checkpoint1");
    }else  
        
    //blocos que quebram    
    if(BlockType==326){
        return string("Tijolo pedra quebrado");
    }else if(BlockType==327){
        return string("Pedregulho");
    }else if(BlockType==328){
        return string("Bedrock");
    }else if(BlockType==329){
        return string("The end");
    }else if(BlockType==330){
        return string("Glass");
    }else   
     
    //decorativos    
    if(BlockType==401){
        return string("Cogumelos azuis");
    }else if(BlockType==402){
        return string("Muda pinheiro");
    }else if(BlockType==403){
        return string("Trigo");
    }else if(BlockType==404){
        return string("Trigo2");
    }else if(BlockType==405){
        return string("Muda seca");
    }else if(BlockType==406){
        return string("Muda neve");
    }else if(BlockType==407){
        return string("Flor Vermelha");
    }else if(BlockType==408){
        return string("Flor Amarela");
    }else if(BlockType==409){
        return string("Cogumelo Nether");
    }else if(BlockType==410){
        return string("Door up");
    }else if(BlockType==411){
        return string("Web");
    }else if(BlockType==412){
        return string("Folha");
    }
    
    
    else{//air
        return string("");
    }
}

void Blocks::draw(){
    if(!this->isVisible) return;
    nTPoint newSize=size;
    if(type==376||type==380){
        if(round(fmodl(GL::getGameMs(),(int)100))==0)
            if(type==376){
                type=380;
            }else{
                type=376;
            }
        tex=getTextureByIndex();
    }
    if(type==377||type==381){
        if(round(fmodl(GL::getGameMs(),(int)100))==0)
            if(type==377){
                type=381;
            }else{
                type=377;
            }
        tex=getTextureByIndex();
    }
    GL::drawTexture(Util::getCollisionRectangle(pos,newSize),Util::nTColorSet(1,1,1,1),tex,1);
    if(type>325&&type<=350){
        nTPoint tmp=pos;
        tmp.z=1;
        char buffer[5];
        snprintf(buffer,5,"%d",brokeStage);
        string bID(buffer);
        GL::drawTexture(Util::getCollisionRectangle(tmp,newSize),Util::nTColorSet(1,1,1,1),GL::getTextureByName("Quebrado"+bID),1);
        if(brokeStage>=10){
            if(type==330)
                al->playSoundByName("glassBreak");
            else
                al->playSoundByName("BlockBreak");
          this->isVisible=false;
          Blocks* bl=(Blocks*)this;
         this->pos.x=-16;
         this->pos.y=-16;
         //delete bl;
        }
    }
}

void Blocks::move(int dir,float steeps){
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
    vector <mapCollision> var;
    bool condition=true;
    if(dir==Util::left||dir==Util::right){
      pos.setPoint(pos.x+steeps,pos.y,pos.z);
      var=Map::checkCollision(pos,size);
        for(int i=0; i<var.size(); i++){
          if(var[i].collision.firstObj>=3)
            condition=false;
        }
        if(!condition){
          pos.x-=steeps;
          moveSpeed*=-1;
          }else {
        objCollision teste;
        vector <mapCollision> teste2;
        condition=false;
        nTRectangle pRec=Util::getCollisionRectangle(player->pos,player->size);
        pRec.p1.y+=player->size.y/2;
        teste=Mechanics::getCollision(Util::getCollisionRectangle(pos,size),pRec);
        if(type==255){
            if(teste.firstObj)
            player->applyDamage(1);
        }else
      if(teste.firstObj){
        player->forceNormalForce=true;  
        nTPoint point;
        point.x=player->pos.x;
        point.y=player->pos.y-1;
        teste2=Map::checkCollision(point,player->size);
        for(int i=0; i<teste2.size(); i++){
          if(teste2[i].collision.firstObj==2){
            condition=true;
          }
        }
        if(condition){
          pos.x-=steeps;
          moveSpeed*=-1;
        }
          player->pos.y=pos.y-player->size.y/2-defaultBlockSize.y/4;
          player->move(dir,steeps);
          player->canJump=true;
      }else{
          player->forceNormalForce=false;
      }
    }

    }else if(dir==Util::up || dir==Util::down){
      pos.setPoint(pos.x,pos.y-steeps,pos.z);
      var=Map::checkCollision(pos,size);
      for(int i=0; i<var.size(); i++){
        if((var[i].collision.firstObj==2 && steeps>0 &&var[i].blockRef!=-1) || (var[i].collision.firstObj==1 && steeps<0&&var[i].blockRef!=-1)){
          condition=false;
        }
      }
      if(!condition){
        pos.y+=steeps;
        moveSpeed*=-1;
      }
      else {
        objCollision teste;
        vector <mapCollision> teste2;
        condition=false;
        nTRectangle pRec=Util::getCollisionRectangle(player->pos,player->size);
        pRec.p1.y+=player->size.y/2;
        teste=Mechanics::getCollision(Util::getCollisionRectangle(pos,size),pRec);
        if(type==255){
            if(teste.firstObj)
            player->applyDamage(1);
        }else
      if(teste.firstObj){
        player->forceNormalForce=true;  
        nTPoint point;
        point.x=player->pos.x;
        point.y=player->pos.y-1;
        teste2=Map::checkCollision(point,player->size);
        for(int i=0; i<teste2.size(); i++){
          if(teste2[i].collision.firstObj==2){
            condition=true;
          }
        }
        if(condition){
          pos.y+=steeps+5;
          moveSpeed*=-1;
        }
          player->pos.y=pos.y-player->size.y/2-defaultBlockSize.y/8;
          player->canJump=true;
      }else if(teste.firstObj==1){
        player->forceNormalForce=false;
        if(type!=255){
            pos.y+=steeps;
            moveSpeed*=-1;
          }
      }else{
          player->forceNormalForce=false;
      }
    }
  }
    if(needToMoveAgain)
        move(dir,steepsAgain);
}

bool Blocks::checkIfBlocksIsDynamic(int id){
    bool out=false;
    if(id>100&&id<=350){
        out=true;
    }else if(id>376&&id<=400){
        out=true;
    }else if(id==1000||id==501){
        out=true;
    }else if(id>=2000&&id<=3000){
        out=true;
    }else if(id>=4000&&id<=4012){
        out=true;
    }else if(id>5000||id<0||id==666){
        out=true;
    }else if(id>=476&&id<500){
          out=true;
    }
    return out;
}

bool Blocks::checkIfBlocksIsMassive(int id){//ve se o bloco tem massa
    bool out=true;
    if(id>1000&&id<4000||id>5000){
        out=false;
    }else if(id==1000||id==0){
        out=false;
    }else if(id>=401&&id<=450){
        out=false;
    }else if(id>=351&&id<=375){
        out=false;
    }
    return out;
}

void Blocks::applyDamage(float damage){
    if(GL::getGameMs()>=timeToVunerability)
        damageState=false;
    if(damageState)
        return;
    brokeStage+=damage*3.1;
      if(brokeStage<0){
        al->playSoundByName("BlockBreak");
        this->isVisible=false;
        Blocks* bl=(Blocks*)this;
         this->pos.x=-16;
         this->pos.y=-16;
         //delete bl;
    }
    damageState=true;
    timeToVunerability=GL::getGameMs()+imunityTime;   
}
