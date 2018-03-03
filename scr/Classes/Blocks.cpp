#include "Blocks.hpp"

Blocks::Blocks(int type,nTPoint pos,nTPoint size,string data){
    this->type=type;
    this->pos=pos;
    this->pos.z=0.5;
    this->size=size;
    this->id=-1;
    this->brokeStage=0;
    this->damageState=false;
    this->isVisible=true;
    this->tex=new Texture(getTextureByIndex());
    this->color=nTColor::White();
    this->data=data;
    if(checkIfBlocksIsHalfBlockV(type)){
        if(type==255) // reduz velocidade do cacto (gambiarra)
            type=251;
        moveSpeed=150*((type-250)*0.6);
        this->size.y/=2;
        this->pos.y-=size.y/4;
    }
    if(checkIfBlocksIsHalfBlockH(type)){
        moveSpeed=150*((type-200)*0.6);
        this->size.y/=2;
        this->pos.y-=size.y/4;
    }
    if(checkIfBlocksIsLiquid(type)){
        pos.z=0.9999;
        if(type==AnimatedWater1||type==AnimatedWater2||type==StaticWater){
            color.A=0.6f;
        }else{
            color.A=0.85f;
        }
    }
}

Blocks::Blocks(int type,nTPoint pos,nTPoint size):Blocks(type,pos,size,""){

}

Blocks::Blocks(const Blocks& orig) {
}

Blocks::~Blocks() {
    delete tex;
    if(checkIfBlocksIsDynamic(type)){
        Blocks* bl;
        int idx=id-Map::staticBlocks.size();
        if(idx>=0){
            for(int i=idx+1;i<Map::dynamicBlocks.size();i++){
                bl=(Blocks*)Map::dynamicBlocks[i];
                bl->id--;
            }
            Map::dynamicBlocks.erase(Map::dynamicBlocks.begin()+idx);
        }
    }
}

nTPoint Blocks::defaultBlockSize=nTPoint::get(32,32,0.7);
double Blocks::imunityTime=Constants::BLOCKS_ImunityTime;

const int Blocks::AnimatedWater1=376;
const int Blocks::AnimatedWater2=380;
const int Blocks::AnimatedLava1=377;
const int Blocks::AnimatedLava2=381;
const int Blocks::StaticLava=379;
const int Blocks::StaticWater=378;
const int Blocks::HalfBlockVCactus=255;
const int Blocks::GlassBlock=330;
const int Blocks::EndLevelBlock=666;
const int Blocks::BusShooterBlock=200;
const int Blocks::InvalidPowerUpBlock=325;
const int Blocks::InvalidPowerUpChest=500;
const int Blocks::IceBlock=17;
const int Blocks::IceHHalfBlock=204;
const int Blocks::IceVHalfBlock=254;
const int Blocks::TutorialBlock=527;
const int Blocks::TutorialPauseBlock=528;
const int Blocks::CoinBlock=529;

/**
 *  Gets the texture id of this block
 *
 *  @return id of the texture
**/
vector<GLuint> Blocks::getTextureByIndex(){
    vector<GLuint> out;
    if(type==AnimatedWater1||type==AnimatedWater2){
        out=GL::getTexturesByName("Agua",2);
    }else if(type==AnimatedLava1||type==AnimatedLava2){
        out=GL::getTexturesByName("Lava",2);
    }else
        out.push_back(GL::getTextureByName(getTexNameByIndex(this->type)));
    return out;
}

/**
 *  Draw this block on the screen
**/
void Blocks::draw(){
    if(!this->isVisible){ return;}
    GL::drawTexture(nTRectangle::getCollision(pos,size),color,tex->get());
    if(checkIfBlocksIsDestrutive(type)){
        nTPoint tmp=pos;
        tmp.z=1;
        GL::drawTexture(nTRectangle::getCollision(tmp,size),GL::getTextureByName("Quebrado"+Util::intToStr(brokeStage)));
    }
    if(Mechanics::drawCollisionRec&&checkIfBlocksIsMassive(type))GL::drawCollision(nTRectangle::getCollision(pos, size));
}

/**
 *  Override Mechanics::move to move blocks across the map
 *
 *  @param dir direction to where the block will move, could be Util::direction_left, Util::direction_right, Util::direction_up, Util::direction_down
 *  @param steeps amount of steps to move the block
**/
void Blocks::move(int dir,double steeps){
    FunctionAnalyser::startFunction("Blocks::move");
    bool needToMoveAgain=false;
    double steepsAgain=0;
    if(ABS(steeps)>Entity::walkSpeed/GL::getFPS()){
        int signal=1;
        needToMoveAgain=true;
        if(steeps<0)
            signal=-1;
        steepsAgain=signal*(ABS(steeps)-Entity::walkSpeed/GL::getFPS());
        steeps=signal*Entity::walkSpeed/GL::getFPS();
    }
    bool collision=false;
    if(dir==Util::direction_left||dir==Util::direction_right){
        pos.x+=steeps;
        collideWithMap();
        for(int i=0; i<lastMapColl.size(); i++){
            if((lastMapColl[i].collision.firstObj==Mechanics::RIGHT||lastMapColl[i].collision.firstObj==Mechanics::LEFT)&&lastMapColl[i].blockId!=id){
                collision=true;
                break;
            }
        }
        if(collision){
            pos.x-=steeps;
            moveSpeed*=-1;
        }else{
            objCollision playerCol;
            nTRectangle pRec=nTRectangle::getCollision(Player::getPlayerById(0)->getGroundPos(),Player::getPlayerById(0)->size);
            playerCol=Mechanics::getCollision(nTRectangle::getCollision(pos,size),pRec);
            if(type==HalfBlockVCactus){ //cacto
                if(playerCol.firstObj!=Mechanics::NOCOLLISION)
                    Player::getPlayerById(0)->applyDamage(1);
            }else if(playerCol.firstObj==Mechanics::TOP || playerCol.secondObj==Mechanics::BOTTOM){
                Player::getPlayerById(0)->move(dir,steeps);
            }else if(playerCol.firstObj==Mechanics::LEFT || playerCol.firstObj==Mechanics::RIGHT){
                Player* pl=Player::getPlayerById(0);
                pl->collideWithMap();
                collision=false;
                for(int i=0; i<pl->lastMapColl.size(); i++){
                    if(pl->lastMapColl[i].blockId!=id){
                        if((pl->lastMapColl[i].collision.firstObj==Mechanics::RIGHT&&playerCol.secondObj==Mechanics::LEFT)||
                           (pl->lastMapColl[i].collision.firstObj==Mechanics::LEFT&&playerCol.secondObj==Mechanics::RIGHT)){
                            pos.x-=steeps*2;
                            moveSpeed*=-1;
                            collision=true;
                            break;
                        }
                    }
                }
                if((playerCol.secondObj==Mechanics::LEFT||playerCol.secondObj==Mechanics::RIGHT)&&!collision){
                    Player::getPlayerById(0)->move(dir,steeps);
                }
            }
        }
    }else if(dir==Util::direction_up || dir==Util::direction_down){
        objCollision playerCol=Mechanics::getCollision(nTRectangle::getCollision(pos,size),nTRectangle::getCollision(Player::getPlayerById(0)->pos,Player::getPlayerById(0)->size));
        pos.y-=steeps;
        collideWithMap();
        for(int i=0; i<lastMapColl.size(); i++){
            if((lastMapColl[i].collision.firstObj==Mechanics::TOP||lastMapColl[i].collision.firstObj==Mechanics::BOTTOM)&&lastMapColl[i].blockId!=id){
                pos.y+=steeps;
                moveSpeed*=-1;
                break;
            }
        }
        if(playerCol.firstObj!=Mechanics::NOCOLLISION){
            if(type==HalfBlockVCactus) //cacto
                Player::getPlayerById(0)->applyDamage(1);
            else if(playerCol.secondObj==Mechanics::TOP||playerCol.secondObj==Mechanics::BOTTOM){
                Player* pl=Player::getPlayerById(0);
                pl->collideWithMap(pl->getGroundPos());
                collision=false;
                for(int i=0; i<pl->lastMapColl.size(); i++){
                    if(pl->lastMapColl[i].blockId!=id){
                        if((pl->lastMapColl[i].collision.firstObj==Mechanics::BOTTOM&&playerCol.secondObj==Mechanics::TOP)||
                           (pl->lastMapColl[i].collision.firstObj==Mechanics::TOP&&playerCol.secondObj==Mechanics::BOTTOM)){
                            pos.y+=2*steeps;
                            moveSpeed*=-1;
                            collision=true;
                            break;
                        }
                    }
                }
                if(playerCol.secondObj==Mechanics::BOTTOM){
                    Player::getPlayerById(0)->pos.y=pos.y-Player::getPlayerById(0)->size.y/2-size.y/2;
                }else if(playerCol.secondObj==Mechanics::TOP&&!collision){
                    Player::getPlayerById(0)->move(Util::direction_down,steeps*-2);
                    Player::getPlayerById(0)->vSpeed=-(moveSpeed);
                }
            }
        }

    }
    FunctionAnalyser::endFunction("Blocks::move");
    if(needToMoveAgain)
        move(dir,steepsAgain);
}

/**
 *  Apply damage to breakeable blocks
 *
 *  @param damage quantity of damage to be applied
**/
void Blocks::applyDamage(double damage){
    if(GL::getGameMs()>=timeToVunerability)
        damageState=false;
    if(damageState)
        return;
    brokeStage+=damage*3.1;
    if(brokeStage>=10){
        if(type==GlassBlock)
            AL::singleton->playSoundByName("glassBreak");
        else
            AL::singleton->playSoundByName("BlockBreak");
        this->isVisible=false;
        delete this;
        return;
    }
    damageState=true;
    timeToVunerability=GL::getGameMs()+imunityTime;
}

/**
 *  Check if the block is dynamic or static(regular blocks without events or actions)
 *
 *  @param type type of the block
 *  @return true if the block is dynamic, false if the block is static
**/
bool Blocks::checkIfBlocksIsDynamic(int type){
    return checkIfBlocksIsShooter(type)||checkIfBlocksIsHalfBlockH(type)||checkIfBlocksIsHalfBlockV(type)
        ||checkIfBlocksIsPowerUpBlock(type)||checkIfBlocksIsPowerUpChest(type)||checkIfBlocksIsDestrutive(type)
        ||checkIfBlocksIsLiquid(type)||checkIfBlocksIsLever(type)||checkIfBlocksIsJumpBoost(type)
        ||checkIfBlocksIsBossSpawn(type)||checkIfBlocksIsPlayerSpawn(type)||checkIfBlocksIsEnemySpawn(type)
        ||checkIfBlocksIsCheckpoint(type)||checkIfBlocksIsTeleportDoor(type)||checkIfBlocksIsTeleportPipe(type)
        ||checkIfBlocksIsEndLevel(type)||checkIfBlocksIsEnemyCollider(type)||checkIfBlocksIsTutorial(type)
        ||checkIfBlocksIsTutorialPause(type)||(type==CoinBlock);
}

/**
 *  Check if the block is massive(has collision) or not
 *
 *  @param type type of the block
 *  @return true if the block is massive, false if the block isnt massive
**/
bool Blocks::checkIfBlocksIsMassive(int type){
    return !(checkIfBlocksIsBossSpawn(type)||checkIfBlocksIsPlayerSpawn(type)||checkIfBlocksIsEnemySpawn(type)
        ||checkIfBlocksIsAir(type)||checkIfBlocksIsDecorative(type)||checkIfBlocksIsNoCollision(type));
}


/**
 *  Check if the block is filled(the player cant crosses it) or not
 *
 *  @param type type of the block
 *  @return true if the block is filled, false if the block isnt filled
**/
bool Blocks::checkIfBlocksIsFilled(int type){
    return !(checkIfBlocksIsCheckpoint(type)||checkIfBlocksIsEndLevel(type)||checkIfBlocksIsTeleportDoor(type)
        ||checkIfBlocksIsLiquid(type));
}



bool Blocks::checkIfBlocksIsTeleportDoor(int type){
    return (type>-400&&type<=-200);
}

bool Blocks::checkIfBlocksIsTeleportPipe(int type){
    return (type>-200&&type<0);
}

bool Blocks::checkIfBlocksIsAir(int type){
    return type==0;
}


bool Blocks::checkIfBlocksIsSolid(int type){
    return (type>=1&&type<=100);
}

bool Blocks::checkIfBlocksIsShooter(int type){
    return (type>=101&&type<=200);
}

bool Blocks::checkIfBlocksIsHalfBlockH(int type){
    return (type>=201&&type<=250);
}

bool Blocks::checkIfBlocksIsHalfBlockV(int type){
    return (type>=251&&type<=300);
}

bool Blocks::checkIfBlocksIsPowerUpBlock(int type){
    return (type>=301&&type<325);
}

bool Blocks::checkIfBlocksIsDestrutive(int type){
    return (type>=326&&type<=350);
}

bool Blocks::checkIfBlocksIsNoCollision(int type){
    return (type>=351&&type<=375);
}

/**
 *  Check if the block is liquid or not
 *
 *  @param type type of the block
 *  @return true if the block is liquid, false if the block isnt liquid
**/
bool Blocks::checkIfBlocksIsLiquid(int type){
    return (type>=376&&type<=400);
}
bool Blocks::checkIfBlocksIsDecorative(int type){
    return (type>=401&&type<=450);
}

bool Blocks::checkIfBlocksIsLever(int type){
    return (type>=451&&type<=475);
}

bool Blocks::checkIfBlocksIsPowerUpChest(int type){
    return (type>=476&&type<500);
}

bool Blocks::checkIfBlocksIsJumpBoost(int type){
    return (type>=501&&type<=525);
}

bool Blocks::checkIfBlocksIsEnemyCollider(int type){
    return type==526;
}

bool Blocks::checkIfBlocksIsTutorial(int type){
    return type==527;
}

bool Blocks::checkIfBlocksIsTutorialPause(int type){
    return type==528;
}

bool Blocks::checkIfBlocksIsEndLevel(int type){
    return type==666;
}

bool Blocks::checkIfBlocksIsPlayerSpawn(int type){
    return type==1000;
}

bool Blocks::checkIfBlocksIsEnemySpawn(int type){
    return (type>=2001&&type<=3000);
}

bool Blocks::checkIfBlocksIsCheckpoint(int type){
    return (type>=4001&&type<=4012);
}

bool Blocks::checkIfBlocksIsBossSpawn(int type){
    return (type>=5001&&type<=8000);
}

int Blocks::getCheckPointId(int checkType){
    return checkType-4000;
}

int Blocks::getPowerUpChestId(int puType){
    return puType-476;
}

int Blocks::getPowerUpBlockId(int puType){
    return puType-301;
}

/**
 *  Gets the name of the block using its id
 *
 *  @param BlockType id of the block
 *  @return a string containing the block name, case the id is invalid returns "Unknow"
**/
string Blocks::getTexNameByIndex(int BlockType){
    if(BlockType==0){//air
        return string("");
    }else if(BlockType==1||BlockType==351){
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
        if(Scenes::current==Scenes::MapEdit)
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
    }else if(BlockType<0&&BlockType>-400){
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
    return string("Agua0");
    }else if(BlockType==377){
    return string("Lava0");
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
    return string("PowerUp");
    }else if(BlockType==325){
    return string("PowerUpOff");
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

    //outros
    else if(BlockType==526){
        if(Scenes::current==Scenes::MapEdit)
            return string("EnemyCtrl");
        return string("Invisivel");
    }else if(BlockType==527){
        if(Scenes::current==Scenes::MapEdit)
            return string("TutorialBlock");
        return string("Invisivel");
    }else if(BlockType==528){
        if(Scenes::current==Scenes::MapEdit)
            return string("TutorialPauseBlock");
        return string("Invisivel");
    }else if(BlockType==529){
        return string("Coin0");
    }

    else{
        return string("Unknow");
    }
}