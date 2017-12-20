#include "Blocks.hpp"

Blocks::Blocks(int type,nTPoint pos,nTPoint size) {
    this->type=type;
    this->pos=pos;
    this->size=size;
    this->id=-1;
    this->tex=getTextureByIndex();
    this->brokeStage=0;
    this->damageState=false;
    this->isVisible=true;
    this->color=nTColor::White();
	if(type==526){
		this->size.y/=3;
	}
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
    if(checkIfBlocksIsLiquid(type)){
        pos.z=0.9999;
        if(type==376||type==378)
            color.A=0.6f;
        else
            color.A=0.85f;
    }
}

Blocks::Blocks(const Blocks& orig) {
}

Blocks::~Blocks() {
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

nTPoint Blocks::defaultBlockSize=nTPoint::get(32,32);
double Blocks::imunityTime=150;

/**
 *	Gets the texture id of this block
 *
 *	@return id of the texture
**/
GLuint Blocks::getTextureByIndex(){
    return GL::getTextureByName(getTexNameByIndex(this->type));
}

/**
 *	Gets the name of the block using its id
 *
 *	@param BlockType id of the block
 *	@return a string containing the block name, case the id is invalid returns "Unknow"
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

        //outros
        else if(BlockType==526){
            if(Scenes::current==Scenes::mapEdit)
                return string("EnemyCtrl");
            return string("Invisivel");
        }

        else{
            return string("Unknow");
        }
}

/**
 *	Draw this block on the screen
**/
void Blocks::draw(){
    if(!this->isVisible) return;
    nTPoint newSize=size;
    if(type==376||type==380){
        if(round(fmodl(GL::getGameMs(),(int)100))==0){
            if(type==376){
                type=380;
            }else{
                type=376;
            }
        }
        tex=getTextureByIndex();
    }
    if(type==377||type==381){
        if(round(fmodl(GL::getGameMs(),(int)100))==0){
            if(type==377){
                type=381;
            }else{
                type=377;
            }
        }
        tex=getTextureByIndex();
    }
    GL::drawTexture(nTRectangle::getCollision(pos,newSize),color,tex);
    if(type>325&&type<=350){
        nTPoint tmp=pos;
        tmp.z=1;
        char buffer[5];
        snprintf(buffer,5,"%d",brokeStage);
        string bID(buffer);
        GL::drawTexture(nTRectangle::getCollision(tmp,newSize),GL::getTextureByName("Quebrado"+bID));
        if(brokeStage>=10){
            if(type==330)
                AL::singleton->playSoundByName("glassBreak");
            else
                AL::singleton->playSoundByName("BlockBreak");
            this->isVisible=false;
            this->pos.x=-16;
            this->pos.y=-16;
        }
    }
    if(Mechanics::drawCollisionRec&&checkIfBlocksIsMassive(type))GL::drawCollision(nTRectangle::getCollision(pos, size));
}

/**
 *	Override Mechanics::move to move blocks across the map
 *
 *	@param dir direction to where the block will move, could be Util::direction_left, Util::direction_right, Util::direction_up, Util::direction_down
 *	@param steeps amount of steps to move the block
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
    vector <mapCollision> var;
    bool collision=false;
    if(dir==Util::direction_left||dir==Util::direction_right){
        pos.x+=steeps;
        var=Map::checkCollision(pos,size);
        for(int i=0; i<var.size(); i++){
            if(var[i].collision.firstObj==Mechanics::RIGHT||var[i].collision.firstObj==Mechanics::LEFT){
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
            if(type==255){ //cacto
                if(playerCol.firstObj!=Mechanics::NOCOLLISION)
                    Player::getPlayerById(0)->applyDamage(1);
            }else if(playerCol.firstObj==Mechanics::TOP || playerCol.secondObj==Mechanics::BOTTOM){
                Player::getPlayerById(0)->move(dir,steeps);
            }else if(playerCol.firstObj==Mechanics::LEFT || playerCol.firstObj==Mechanics::RIGHT){
                var=Map::checkCollision(Player::getPlayerById(0)->pos,Player::getPlayerById(0)->size);
                collision=false;
                for(int i=0; i<var.size(); i++){
                    if(var[i].blockId!=id){
                        if((var[i].collision.firstObj==Mechanics::RIGHT&&playerCol.secondObj==Mechanics::LEFT)||
                           (var[i].collision.firstObj==Mechanics::LEFT&&playerCol.secondObj==Mechanics::RIGHT)){
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
        var=Map::checkCollision(pos,size);
        for(int i=0; i<var.size(); i++){
            bool anotherCactus=false;
                if(type==255&&var[i].blockId>=Map::staticBlocks.size()){
                    Blocks* bl=(Blocks*)(Map::dynamicBlocks[var[i].blockId-Map::staticBlocks.size()]);
                    if(bl->type==255)
                        anotherCactus=true;
                }
            if((var[i].collision.firstObj==Mechanics::TOP||var[i].collision.firstObj==Mechanics::BOTTOM)&&!anotherCactus){
                pos.y+=steeps;
                moveSpeed*=-1;
                break;
            }
        }
        if(playerCol.firstObj!=Mechanics::NOCOLLISION){
            if(type==255) //cacto
                Player::getPlayerById(0)->applyDamage(1);
            else if(playerCol.secondObj==Mechanics::TOP||playerCol.secondObj==Mechanics::BOTTOM){
                var=Map::checkCollision(Player::getPlayerById(0)->getGroundPos(),Player::getPlayerById(0)->size);
                collision=false;
                for(int i=0; i<var.size(); i++){
                    if(var[i].blockId!=id){
                        if((var[i].collision.firstObj==Mechanics::BOTTOM&&playerCol.secondObj==Mechanics::TOP)||
                           (var[i].collision.firstObj==Mechanics::TOP&&playerCol.secondObj==Mechanics::BOTTOM)){
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
 *	Check if the block is dynamic or static(regular blocks without events or actions)
 *
 *	@param type type of the block
 *	@return true if the block is dynamic, false if the block is static
**/
bool Blocks::checkIfBlocksIsDynamic(int type){
    if(type>100&&type<=350){
        return true;
    }else if(type>=376&&type<=400){
        return true;
    }else if(type==1000||type==501||type==526){
        return true;
    }else if(type>=2000&&type<=3000){
        return true;
    }else if(type>=4000&&type<=4012){
        return true;
    }else if(type>5000||type<0||type==666){
        return true;
    }else if(type>=476&&type<500){
        return true;
    }
    return false;
}

/**
 *	Check if the block is massive(has collision) or not
 *
 *	@param type type of the block
 *	@return true if the block is massive, false if the block isnt massive
**/
bool Blocks::checkIfBlocksIsMassive(int type){
    if((type>1000&&type<4000)||type>5000){
        return false;
    }else if(type==1000||type==0){
        return false;
    }else if(type>=401&&type<=450){
        return false;
    }else if(type>=351&&type<=375){
        return false;
    }
    return true;
}

/**
 *	Check if the block is liquid or not
 *
 *	@param type type of the block
 *	@return true if the block is liquid, false if the block isnt liquid
**/
bool Blocks::checkIfBlocksIsLiquid(int type){
    if(type>=376&&type<=400){
        return true;
    }
    return false;
}

/**
 *	Check if the block is filled(the player cant crosses it) or not
 *
 *	@param type type of the block
 *	@return true if the block is filled, false if the block isnt filled
**/
bool Blocks::checkIfBlocksIsFilled(int type){
    if((type>4000&&type<4013)||type==666){
        return false;
    }
    if(type<=-200&&type>-400){
        return false;
    }
    if(type>=376&&type<=400){
        return false;
    }
    return true;
}


/**
 *	Apply damage to breakeable blocks
 *
 *	@param damage quantity of damage to be applied
**/
void Blocks::applyDamage(double damage){
    if(GL::getGameMs()>=timeToVunerability)
        damageState=false;
    if(damageState)
        return;
    brokeStage+=damage*3.1;
    if(brokeStage<0){
        AL::singleton->playSoundByName("BlockBreak");
        this->isVisible=false;
        this->pos.x=-16;
        this->pos.y=-16;
        delete this;
        return;
    }
    damageState=true;
    timeToVunerability=GL::getGameMs()+imunityTime;
}