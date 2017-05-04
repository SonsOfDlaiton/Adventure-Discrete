#include "loadFiles.h"

bool loadTextures(){
    bool out = true;
    GL::loadTexture("paraboloide hiperbolico<3","Textures/paraboloideHiperbolico.png");//n precisa de if
    //HUD
    if(!GL::loadTexture("Ballon","Textures/HUD/defaultBallon.png")) out=false;
    if(!GL::loadTexture("Whisp","Textures/HUD/Tutorials/whisp.png")) out=false;
    if(!GL::loadTexture("-","Textures/HUD/Buttons/arrowMinus.png")) out=false;
    if(!GL::loadTexture("+","Textures/HUD/Buttons/arrowPlus.png")) out=false;
    if(!GL::loadTexture("CameraDefault","Textures/HUD/Buttons/camera.png")) out=false;
    if(!GL::loadTexture("Back","Textures/HUD/Buttons/back.png")) out=false;
    if(!GL::loadTexture("Zoom-","Textures/HUD/Buttons/lupa-.png")) out=false;
    if(!GL::loadTexture("Zoom+","Textures/HUD/Buttons/lupa+.png")) out=false;
    if(!GL::loadTexture("Background-","Textures/HUD/Buttons/background-.png")) out=false;
    if(!GL::loadTexture("Background+","Textures/HUD/Buttons/background+.png")) out=false;
    if(!GL::loadTexture("Save","Textures/HUD/Buttons/save.png")) out=false;    
    if(!GL::loadTexture("Get Block","Textures/HUD/Buttons/getBlock.png")) out=false;
    if(!GL::loadTexture("decomCelebrations","Textures/logo/GameLogo.png")) out=false;
    if(!GL::loadTexture("theCOM","Textures/logo/thecom.png")) out=false;
    if(!GL::loadTexture("cefetLogo","Textures/logo/cefet.png")) out=false;
    if(!GL::loadTexture("decomLogo","Textures/logo/decom.png")) out=false;
    if(!GL::loadTexture("legendsLogo","Textures/logo/mainLogo.png")) out=false;
    if(!GL::loadTexture("optionsIcon","Textures/HUD/Buttons/options.png")) out=false;
    if(!GL::loadTexture("quitIcon","Textures/HUD/Buttons/quit.png")) out=false;
    if(!GL::loadTexture("playIcon","Textures/HUD/Buttons/play.png")) out=false;
    if(!GL::loadTexture("mapEditIcon","Textures/HUD/Buttons/mapEdit.png")) out=false;
    if(!GL::loadTexture("optionsIcon","Textures/HUD/Buttons/options.png")) out=false;
    if(!GL::loadTexture("S2","Textures/HUD/heart.png")) out=false;
    if(!GL::loadTexture("legalNote","Textures/logo/legalNote.png")) out=false;
    if(!GL::loadTexture("credits","Textures/logo/credits.png")) out=false;
    if(!GL::loadTexture("backIcon","Textures/HUD/Buttons/backIcon.png")) out=false;
    if(!GL::loadTexture("soundOn","Textures/HUD/Buttons/soundOn.png")) out=false;
    if(!GL::loadTexture("soundOff","Textures/HUD/Buttons/soundOff.png")) out=false;
    if(!GL::loadTexture("musicOn","Textures/HUD/Buttons/musicOn.png")) out=false;
    if(!GL::loadTexture("musicOff","Textures/HUD/Buttons/musicOff.png")) out=false;
    if(!GL::loadTexture("newMapIcon","Textures/HUD/Buttons/newMapIcon.png")) out=false;
    if(!GL::loadTexture("freeModeIcon","Textures/HUD/Buttons/freeMode.png")) out=false;
    if(!GL::loadTexture("campaignIcon","Textures/HUD/Buttons/campain.png")) out=false;
    if(!GL::loadTexture("editMapIcon","Textures/HUD/Buttons/editMapIcon.png")) out=false;
    if((GL::loadTextures("paper",2,"Textures/enemys/prova/blank/walk/")).size()!=2)out=false;
    // if((GL::loadTextures("letter",6,"Textures/enemys/letters/")).size()!=6)out=false;
    if(!GL::loadTexture("A","Textures/enemys/letters/A.png")) out=false;
    if(!GL::loadTexture("B","Textures/enemys/letters/B.png")) out=false;
    if(!GL::loadTexture("C","Textures/enemys/letters/C.png")) out=false;
    if(!GL::loadTexture("D","Textures/enemys/letters/D.png")) out=false;
    if(!GL::loadTexture("E","Textures/enemys/letters/E.png")) out=false;
    if(!GL::loadTexture("F","Textures/enemys/letters/F.png")) out=false;
    if(!GL::loadTexture("GameOver","Textures/HUD/Buttons//texturagameover.png")) out=false;
    if(!GL::loadTexture("Menu1","Textures/HUD/Buttons//menu.png")) out=false;
    if(!GL::loadTexture("Continuar1","Textures/HUD/Buttons//continuar.png")) out=false;
    if(!GL::loadTexture("Jubilado","Textures/HUD/Buttons//jubilado.png")) out=false;

    //powerups
    if(!GL::loadTexture("cogumelo0","Textures/powerups/cogumelo.png")) out=false;
    if(!GL::loadTexture("cogumelo1","Textures/powerups/estrela.png")) out=false;
    if(!GL::loadTexture("gcc","Textures/powerups/gcc.png")) out=false;
    if(!GL::loadTexture("cafe","Textures/powerups/coffe.png")) out=false;
    if(!GL::loadTexture("win95","Textures/powerups/win95.png")) out=false;
    
    
    //Espada
    if(!GL::loadTexture("Sword0","Textures/characters/swords/0.png")) out=false;
    if(!GL::loadTexture("Sword1","Textures/characters/swords/1.png")) out=false;
    if(!GL::loadTexture("Sword2","Textures/characters/swords/2.png")) out=false;
    if((GL::loadTextures("StrongAtk",4,"Textures/characters/specialAtack/strong/")).size()!=4)out=false;
    if((GL::loadTextures("WeakAtk",3,"Textures/characters/specialAtack/weak/")).size()!=3)out=false;
    
    //Bullets
    if(!GL::loadTexture("WinBullet","Textures/enemys/bullets/windowsShot.png")) out=false;
    if(!GL::loadTexture("Intercampi","Textures/enemys/bullets/intercamp0.png")) out=false;
    if(!GL::loadTexture("SwordBullet0","Textures/characters/swords/swordProjectile/0.png")) out=false;
    if(!GL::loadTexture("SwordBullet1","Textures/characters/swords/swordProjectile/1.png")) out=false;
    
    //blocos fisicos
    if(!GL::loadTexture("Pedra","Textures/blocks/2.png")) out=false;
    if(!GL::loadTexture("Terra","Textures/blocks/3.png"))out=false;
    if(!GL::loadTexture("Grama","Textures/blocks/4.png"))out=false;
    if(!GL::loadTexture("Tronco Madeira","Textures/blocks/21.png"))out=false;
    if(!GL::loadTexture("Pedra Musgo","Textures/blocks/37.png"))out=false;
    if(!GL::loadTexture("Neve","Textures/blocks/66.png"))out=false;
    if(!GL::loadTexture("Grama neve","Textures/blocks/68.png"))out=false;
    if(!GL::loadTexture("Tijolo pedra musgo","Textures/blocks/100.png"))out=false;
    if(!GL::loadTexture("Arenito","Textures/blocks/179.png"))out=false;
    if(!GL::loadTexture("Ender portal","Textures/blocks/180.png"))out=false;
    if(!GL::loadTexture("Tijolo nether","Textures/blocks/185.png"))out=false;
    if(!GL::loadTexture("Tijolo areia","Textures/blocks/190.png"))out=false;
    if(!GL::loadTexture("Tijolo pedra","Textures/blocks/54.png"))out=false;
    if(!GL::loadTexture("Tijolo","Textures/blocks/8.png"))out=false;
    if(!GL::loadTexture("Madeira","Textures/blocks/5.png"))out=false;
    if(!GL::loadTexture("La ciano","Textures/blocks/169.png"))out=false;
    if(!GL::loadTexture("Gelo","Textures/blocks/67.png"))out=false;
    if(!GL::loadTexture("Invisivel","Textures/blocks/-1.png"))out=false;
    if(!GL::loadTexture("block","Textures/blocks/215.png"))out=false;
    if(!GL::loadTexture("obsidian","Textures/blocks/38.png"))out=false;
    if(!GL::loadTexture("areia","Textures/blocks/19.png"))out=false;
    if(!GL::loadTexture("La preta","Textures/blocks/113.png"))out=false;
    if(!GL::loadTexture("Cogumelo","Textures/blocks/126.png"))out=false;
    if(!GL::loadTexture("Hoper","Textures/blocks/139.png"))out=false;    
    if(!GL::loadTexture("Books","Textures/blocks/36.png"))out=false;
    if(!GL::loadTexture("Tronco com folhas","Textures/blocks/226.png"))out=false;    

    //blocos que movem   
    if(!GL::loadTexture("Meio bloco pedra","Textures/blocks/211.png"))out=false;
    if(!GL::loadTexture("Meio bloco madeira","Textures/blocks/212.png"))out=false;
    if(!GL::loadTexture("Meio bloco arenito","Textures/blocks/213.png"))out=false;
    if(!GL::loadTexture("Meio bloco gelo","Textures/blocks/214.png"))out=false;
    if(!GL::loadTexture("Meio bloco Cacto","Textures/blocks/222.png"))out=false;
    
    //blocos que quebram
    if((GL::loadTextures("Quebrado",10,"Textures/blocks/blockbroke")).size()!=10)out=false;
    if(!GL::loadTexture("Tijolo pedra quebrado","Textures/blocks/101.png"))out=false;
    if(!GL::loadTexture("Pedregulho","Textures/blocks/17.png"))out=false;
    if(!GL::loadTexture("Bedrock","Textures/blocks/18.png"))out=false;
    if(!GL::loadTexture("The end","Textures/blocks/166.png"))out=false;
    if(!GL::loadTexture("Glass","Textures/blocks/49.png"))out=false;    
    
   //blocos especiais
    if(!GL::loadTexture("Spawn","Textures/blocks/65.png"))out=false;
    if(!GL::loadTexture("Alavanca","Textures/blocks/96.png"))out=false;
    if(!GL::loadTexture("Ar","Textures/blocks/216.png"))out=false;
    if(!GL::loadTexture("Cano","Textures/blocks/217.png"))out=false;
    if(!GL::loadTexture("Lava","Textures/blocks/218.png"))out=false;
    if(!GL::loadTexture("Agua","Textures/blocks/219.png"))out=false;
    if(!GL::loadTexture("Lava1","Textures/blocks/218-b.png"))out=false;
    if(!GL::loadTexture("Agua1","Textures/blocks/219-b.png"))out=false;
    if(!GL::loadTexture("LavaF","Textures/blocks/lavafull.png"))out=false;
    if(!GL::loadTexture("AguaF","Textures/blocks/aguafull.png"))out=false;
    if(!GL::loadTexture("Powerup","Textures/blocks/220.png"))out=false;
    if(!GL::loadTexture("PowerupOff","Textures/blocks/220-b.png"))out=false;
    if(!GL::loadTexture("Shooter","Textures/blocks/221.png"))out=false;
    if(!GL::loadTexture("ShooterBus","Textures/blocks/224.png"))out=false;
    if(!GL::loadTexture("jump spring","Textures/blocks/jumpPad.png"))out=false;
    if(!GL::loadTexture("Door down","Textures/blocks/97.png"))out=false;    
    if(!GL::loadTexture("AlavancaOFF","Textures/blocks/223.png"))out=false;
    if(!GL::loadTexture("checkpoint0","Textures/CheckPoints/Checkpoint0.png"))out=false;
    if(!GL::loadTexture("checkpoint1","Textures/CheckPoints/Checkpoint1.png"))out=false;    
    if(!GL::loadTexture("End Game (GG)","Textures/CheckPoints/GG.png"))out=false;
    if(!GL::loadTexture("Bau","Textures/blocks/28.png"))out=false;
    if(!GL::loadTexture("BauOff","Textures/blocks/28-b.png"))out=false;
    
   //blocos decorativos
    if(!GL::loadTexture("Cogumelos azuis","Textures/blocks/29.png"))out=false;
    if(!GL::loadTexture("Muda pinheiro","Textures/blocks/63.png"))out=false;
    if(!GL::loadTexture("Trigo","Textures/blocks/94.png"))out=false;
    if(!GL::loadTexture("Trigo2","Textures/blocks/95.png"))out=false;
    if(!GL::loadTexture("Muda seca","Textures/blocks/55.png"))out=false;
    if(!GL::loadTexture("Muda neve","Textures/blocks/111.png"))out=false;
    if(!GL::loadTexture("Flor Vermelha","Textures/blocks/13.png"))out=false;
    if(!GL::loadTexture("Flor Amarela","Textures/blocks/14.png"))out=false;
    if(!GL::loadTexture("Cogumelo Nether","Textures/blocks/189.png"))out=false;
    if(!GL::loadTexture("Door up","Textures/blocks/81.png"))out=false;    
    if(!GL::loadTexture("Web","Textures/blocks/12.png"))out=false; 
    if(!GL::loadTexture("Folha","Textures/blocks/225.png"))out=false; 

    //backgrounds
    if((GL::loadTextures("background",Map::nOfBackgrounds,"Textures/backgrounds/")).size()!=Map::nOfBackgrounds)out=false;
    if(!GL::loadTexture("dce","Textures/backgrounds/dce.png")) out=false;
    if(!GL::loadTexture("aprovado","Textures/backgrounds/aprovado.png")) out=false;
    if(!GL::loadTexture("Cefet","Textures/backgrounds/fecet.png")) out=false;
    if(!GL::loadTexture("Win","Textures/backgrounds/win.png")) out=false;

    //Enemy
    if((GL::loadTextures("paperWalk",2,"Textures/enemys/prova/blank/walk/")).size()!=2)out=false;
    if((GL::loadTextures("bookIdle",1,"Textures/enemys/livro/idle/")).size()!=1)out=false;
    if((GL::loadTextures("bookWalk",5,"Textures/enemys/livro/walk/")).size()!=5)out=false;
    if((GL::loadTextures("calcIdle",1,"Textures/enemys/cassio/idle/")).size()!=1)out=false;
    if((GL::loadTextures("calcWalk",4,"Textures/enemys/cassio/walk/")).size()!=4)out=false;
    
    //Player
    //Air
        if((GL::loadTextures("cefetinhoAtkMeleeAirDown",1,"Textures/characters/cefetinho/atack/melee/air/down/")).size()!=1)out=false;
        if((GL::loadTextures("cefetinhoAtkMeleeAirFront",1,"Textures/characters/cefetinho/atack/melee/air/front/")).size()!=1)out=false;
        if((GL::loadTextures("cefetinhoAtkMeleeAirUp",1,"Textures/characters/cefetinho/atack/melee/air/up/")).size()!=1)out=false;
      //ground
        if((GL::loadTextures("cefetinhoAtkMeleeGroundFront",1,"Textures/characters/cefetinho/atack/melee/ground/front/")).size()!=1)out=false;
        if((GL::loadTextures("cefetinhoAtkMeleeGroundUp",1,"Textures/characters/cefetinho/atack/melee/ground/up/")).size()!=1)out=false;
    //ranged
      //air
        if((GL::loadTextures("cefetinhoAtkRangedAirDown",1,"Textures/characters/cefetinho/atack/ranged/air/down/")).size()!=1)out=false;
        if((GL::loadTextures("cefetinhoAtkRangedAirFront",1,"Textures/characters/cefetinho/atack/ranged/air/front/")).size()!=1)out=false;
        if((GL::loadTextures("cefetinhoAtkRangedAirUp",1,"Textures/characters/cefetinho/atack/ranged/air/up/")).size()!=1)out=false;
      //ground
        if((GL::loadTextures("cefetinhoAtkRangedGroundFront",4,"Textures/characters/cefetinho/atack/ranged/ground/front/")).size()!=4)out=false;
        if((GL::loadTextures("cefetinhoAtkRangedGroundUp",1,"Textures/characters/cefetinho/atack/ranged/ground/up/")).size()!=1)out=false;
          //damage
            if((GL::loadTextures("cefetinhoDmgAir",1,"Textures/characters/cefetinho/damage/air/")).size()!=1)out=false;
            if((GL::loadTextures("cefetinhoDmgGround",1,"Textures/characters/cefetinho/damage/ground/")).size()!=1)out=false;
          if((GL::loadTextures("cefetinhoDie",4,"Textures/characters/cefetinho/die/")).size()!=4)out=false;
          if((GL::loadTextures("cefetinhoIdle",5,"Textures/characters/cefetinho/idle/")).size()!=5)out=false;
          if((GL::loadTextures("cefetinhoJump",1,"Textures/characters/cefetinho/jump/")).size()!=1)out=false;
          //move
            if((GL::loadTextures("cefetinhoMoveRun",4,"Textures/characters/cefetinho/move/run/")).size()!=4)out=false;
            if((GL::loadTextures("cefetinhoMoveWalk",4,"Textures/characters/cefetinho/move/walk/")).size()!=4)out=false;
          if((GL::loadTextures("cefetinhoSpawn",7,"Textures/characters/cefetinho/spawn/")).size()!=7)out=false;
    
        //graduacao
          //atack
            //melee
              //air
                if((GL::loadTextures("graduacaoAtkMeleeAirDown",1,"Textures/characters/graduacao/atack/melee/air/down/")).size()!=1)out=false;
                if((GL::loadTextures("graduacaoAtkMeleeAirFront",1,"Textures/characters/graduacao/atack/melee/air/front/")).size()!=1)out=false;
                if((GL::loadTextures("graduacaoAtkMeleeAirUp",1,"Textures/characters/graduacao/atack/melee/air/up/")).size()!=1)out=false;
              //ground
                if((GL::loadTextures("graduacaoAtkMeleeGroundFront",1,"Textures/characters/graduacao/atack/melee/ground/front/")).size()!=1)out=false;
                if((GL::loadTextures("graduacaoAtkMeleeGroundUp",1,"Textures/characters/graduacao/atack/melee/ground/up/")).size()!=1)out=false;
            //ranged
              //air
                if((GL::loadTextures("graduacaoAtkRangedAirDown",1,"Textures/characters/graduacao/atack/ranged/air/down/")).size()!=1)out=false;
                if((GL::loadTextures("graduacaoAtkRangedAirFront",1,"Textures/characters/graduacao/atack/ranged/air/front/")).size()!=1)out=false;
                if((GL::loadTextures("graduacaoAtkRangedAirUp",1,"Textures/characters/graduacao/atack/ranged/air/up/")).size()!=1)out=false;
              //ground
                if((GL::loadTextures("posgradAtkRangedGroundFront",4,"Textures/characters/graduacao/atack/ranged/ground/front/")).size()!=4)out=false;
                if((GL::loadTextures("posgradAtkRangedGroundUp",1,"Textures/characters/graduacao/atack/ranged/ground/up/")).size()!=1)out=false;
          //damage
            if((GL::loadTextures("graduacaoDmgAir",1,"Textures/characters/graduacao/damage/air/")).size()!=1)out=false;
            if((GL::loadTextures("graduacaoDmgGround",1,"Textures/characters/graduacao/damage/ground/")).size()!=1)out=false;
          if((GL::loadTextures("graduacaoDie",4,"Textures/characters/graduacao/die/")).size()!=4)out=false;
          if((GL::loadTextures("graduacaoIdle",5,"Textures/characters/graduacao/idle/")).size()!=5)out=false;
          if((GL::loadTextures("graduacaoJump",1,"Textures/characters/graduacao/jump/")).size()!=1)out=false;
          //move
            if((GL::loadTextures("graduacaoMoveRun",4,"Textures/characters/graduacao/move/run/")).size()!=4)out=false;
            if((GL::loadTextures("graduacaoMoveWalk",4,"Textures/characters/graduacao/move/walk/")).size()!=4)out=false;
          if((GL::loadTextures("graduacaoSpawn",7,"Textures/characters/graduacao/spawn/")).size()!=7)out=false;
    
        //posgrad
        //atack
          //melee
            //air
              if((GL::loadTextures("posgradAtkMeleeAirDown",1,"Textures/characters/posgrad/atack/melee/air/down/")).size()!=1)out=false;
              if((GL::loadTextures("posgradAtkMeleeAirFront",1,"Textures/characters/posgrad/atack/melee/air/front/")).size()!=1)out=false;
              if((GL::loadTextures("posgradAtkMeleeAirUp",1,"Textures/characters/posgrad/atack/melee/air/up/")).size()!=1)out=false;
            //ground
              if((GL::loadTextures("posgradAtkMeleeGroundFront",1,"Textures/characters/posgrad/atack/melee/ground/front/")).size()!=1)out=false;
              if((GL::loadTextures("posgradAtkMeleeGroundUp",1,"Textures/characters/posgrad/atack/melee/ground/up/")).size()!=1)out=false;
          //ranged
            //air
              if((GL::loadTextures("posgradAtkRangedAirDown",1,"Textures/characters/posgrad/atack/ranged/air/down/")).size()!=1)out=false;
              if((GL::loadTextures("posgradAtkRangedAirFront",1,"Textures/characters/posgrad/atack/ranged/air/front/")).size()!=1)out=false;
              if((GL::loadTextures("posgradAtkRangedAirUp",1,"Textures/characters/posgrad/atack/ranged/air/up/")).size()!=1)out=false;
            //ground
              if((GL::loadTextures("posgradAtkRangedGroundFront",4,"Textures/characters/posgrad/atack/ranged/ground/front/")).size()!=4)out=false;
              if((GL::loadTextures("posgradAtkRangedGroundUp",1,"Textures/characters/posgrad/atack/ranged/ground/up/")).size()!=1)out=false;
        //damage
          if((GL::loadTextures("posgradDmgAir",1,"Textures/characters/posgrad/damage/air/")).size()!=1)out=false;
          if((GL::loadTextures("posgradDmgGround",1,"Textures/characters/posgrad/damage/ground/")).size()!=1)out=false;
        if((GL::loadTextures("posgradDie",4,"Textures/characters/posgrad/die/")).size()!=4)out=false;
        if((GL::loadTextures("posgradIdle",5,"Textures/characters/posgrad/idle/")).size()!=5)out=false;
        if((GL::loadTextures("posgradJump",1,"Textures/characters/posgrad/jump/")).size()!=1)out=false;
        //move
          if((GL::loadTextures("posgradMoveRun",4,"Textures/characters/posgrad/move/run/")).size()!=4)out=false;
          if((GL::loadTextures("posgradMoveWalk",4,"Textures/characters/posgrad/move/walk/")).size()!=4)out=false;
        if((GL::loadTextures("posgradSpawn",7,"Textures/characters/posgrad/spawn/")).size()!=7)out=false;
    return out;
}
bool loadStrings(){
    bool out=true;
    //TODO:carregar strings de idiomas. retorna falso se nao carregou algum arquivo
    if(languageIdx==0){
        //define pt-br
    }
    return out;
}

bool loadMaps(int nOfMaps){
    bool out=true;
    for(int i=0;i<nOfMaps;i++)
        if(!loadMap(Util::getDinamicPath("Maps/map",i,".map")))
            out=false;
    if(!loadMap("Maps/user.map"))
            out=false;
    return out;
}

bool loadMap(char* path){
    nTMap tmp;
    ifstream mapFILE(Util::newPath(path));
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
                    if(tmp2[i]!='\'')
                        tmp5+=tmp2[i];
                    else{
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
        if(strT!="Maps/user.map")
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
bool saveMap(char* path,int idx){
    nTMap save;
    if(idx>=maps.size()&&!(idx<0)){
        cout<<"Erro no index "<<idx<<"do map"<<endl;
        return false;
    }else if(idx>=0){
        save=maps[idx];
    }else{
        save=usrMap;
    }
    ofstream mapFILE(Util::newPath(path));
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
bool loadSounds(){
    bool out=true;
    if(!al->loadSound("Sounds/hey.wav","heyListen",2,false)) out=false;
    if(!al->loadSound("Sounds/Windows Critical Stop.wav","blockShoot",2,false)) out=false;
    if(!al->loadSound("Sounds/agua.wav","agua",1,false)) out=false;
    if(!al->loadSound("Sounds/atkSpec.wav","SpecialAtk",1,false)) out=false;
    if(!al->loadSound("Sounds/blocoQuebrando.wav","BlockBreak",1,false)) out=false;
    if(!al->loadSound("Sounds/chestOpen.wav","chestOpen",1,false)) out=false;
    if(!al->loadSound("Sounds/damageEnemy.wav","enemyDamage",1,false)) out=false;
    if(!al->loadSound("Sounds/damageEnemyArcoIris.wav","enemyDamageSpec",1,false)) out=false;
    if(!al->loadSound("Sounds/damagePlayer.wav","damagePlayer",1,false)) out=false;
    if(!al->loadSound("Sounds/die.wav","diePlayer",1,false)) out=false;
    if(!al->loadSound("Sounds/ice.wav","ice",1,false)) out=false;
    if(!al->loadSound("Sounds/intercampi.wav","intercampi",1,false)) out=false;
    if(!al->loadSound("Sounds/invencibleSong.wav","cafeSong",2,true)) out=false;
    if(!al->loadSound("Sounds/loseGame.wav","loseSong",1,true)) out=false;
    if(!al->loadSound("Sounds/makingTest.wav","makeTest",0.5,true)) out=false;
    if(!al->loadSound("Sounds/menuSong.wav","menuSong",1,true)) out=false;
    if(!al->loadSound("Sounds/mola.wav","mola",1,false)) out=false;
    if(!al->loadSound("Sounds/mouseClick.wav","mouse",0.7,false)) out=false;
    if(!al->loadSound("Sounds/powerUpSound.wav","powerUp",1,false)) out=false;
    if(!al->loadSound("Sounds/sinuca.wav","sinuca",1.3,true)) out=false;
    if(!al->loadSound("Sounds/stage0-SONG.wav","stage0",0.6,true)) out=false;
    if(!al->loadSound("Sounds/stage1-SONG.wav","stage1",0.6,true)) out=false;
    if(!al->loadSound("Sounds/stage2-SONG.wav","stage2",0.6,true)) out=false;
    if(!al->loadSound("Sounds/sword0.wav","sword0",1,false)) out=false;
    if(!al->loadSound("Sounds/sword1.wav","sword1",1,false)) out=false;
    if(!al->loadSound("Sounds/sword2.wav","sword2",1,false)) out=false;
    if(!al->loadSound("Sounds/sword3.wav","sword3",1,false)) out=false;
    if(!al->loadSound("Sounds/sword4.wav","sword4",1,false)) out=false;
    if(!al->loadSound("Sounds/sword5.wav","sword5",1,false)) out=false;
    if(!al->loadSound("Sounds/teletransporte.wav","TP",1,false)) out=false;
    if(!al->loadSound("Sounds/userSong.wav","userSong",0.6,true)) out=false;
    if(!al->loadSound("Sounds/vidroQuebrando.wav","glassBreak",1,false)) out=false;
    if(!al->loadSound("Sounds/GG.wav","GG",1,false)) out=false;

    return out;
}
bool loadSettings(){
    FILE *fp=fopen("savedGame.bin","rb");
  if(fp==NULL){
      cout<<"Arquivo savedGame.bin inexistente, criando novo arquivo"<<endl;
      saveSettings();
      return false;
  }
  SETTINGS tmp;
  fread(&tmp,sizeof(SETTINGS),1,fp);
  AL::setMusicState(tmp.music);
  AL::setSoundState(tmp.sound);
  Player::lives=tmp.lives;
  Player::checkpoint=tmp.checkpoint;
  Player::stage=tmp.currentStage;
  Player::globalCoeficiente=tmp.CR;
  Player::defaultLife=tmp.sword+1;
  fclose(fp);
  return true;
}
bool saveSettings(){
   FILE *fp=fopen("savedGame.bin","wb");
    if(fp==NULL){
      cout<<"Erro ao salvar arquivo savedGame.bin"<<endl;
      return false;
    }
    SETTINGS tmp;
        tmp.sound=AL::getSoundState();
        tmp.music=AL::getMusicState();
        tmp.lives=Player::lives;
        tmp.checkpoint=Player::checkpoint;
        tmp.currentStage=Player::stage;  
        tmp.CR=Player::globalCoeficiente;
        tmp.sword=Player::defaultLife-1;
    fwrite(&tmp,sizeof(SETTINGS),1,fp);
    fclose(fp);
    return true;
}

void setSprites(){
    vector<string> tmp;
    vector<int> tmp2;
    //cefetinho
    tmp.push_back("cefetinhoIdle");
    tmp.push_back("cefetinhoMoveWalk");
    tmp.push_back("cefetinhoMoveRun");
    tmp.push_back("cefetinhoJump");
        tmp.push_back("");
        tmp.push_back("");
    tmp.push_back("cefetinhoDmgGround");
    tmp.push_back("cefetinhoDie");
    tmp.push_back("cefetinhoSpawn");  
    tmp.push_back("cefetinhoAtkMeleeAirDown");
    tmp.push_back("cefetinhoAtkMeleeAirFront");
    tmp.push_back("cefetinhoAtkMeleeAirUp");
    tmp.push_back("cefetinhoAtkMeleeGroundFront");
    tmp.push_back("cefetinhoAtkMeleeGroundUp");
    tmp.push_back("cefetinhoAtkRangedAirDown");
    tmp.push_back("cefetinhoAtkRangedAirFront");
    tmp.push_back("cefetinhoAtkRangedAirUp");
    tmp.push_back("cefetinhoAtkRangedGroundFront");
    tmp.push_back("cefetinhoAtkRangedGroundUp");
    
    tmp2.push_back(5);
    tmp2.push_back(4);
    tmp2.push_back(4);
    tmp2.push_back(1);
        tmp2.push_back(1);
        tmp2.push_back(1);
    tmp2.push_back(1);
    tmp2.push_back(4);
    tmp2.push_back(7);
    tmp2.push_back(1);
    tmp2.push_back(1);
    tmp2.push_back(1);
    tmp2.push_back(1);
    tmp2.push_back(1);
    tmp2.push_back(1);
    tmp2.push_back(1);
    tmp2.push_back(1);
    tmp2.push_back(1);
    tmp2.push_back(1);
      
    playerAnim.push_back(tmp);
    playerAnimSize.push_back(tmp2);
    tmp.clear();
    
    //grad
    tmp.push_back("graduacaoIdle");
    tmp.push_back("graduacaoMoveWalk");
    tmp.push_back("graduacaoMoveRun");
    tmp.push_back("graduacaoJump");
        tmp.push_back("");
        tmp.push_back("");
    tmp.push_back("graduacaoDmgGround");
    tmp.push_back("graduacaoDie");
    tmp.push_back("graduacaoSpawn");  
    tmp.push_back("graduacaoAtkMeleeAirDown");
    tmp.push_back("graduacaoAtkMeleeAirFront");
    tmp.push_back("graduacaoAtkMeleeAirUp");
    tmp.push_back("graduacaoAtkMeleeGroundFront");
    tmp.push_back("graduacaoAtkMeleeGroundUp");
    tmp.push_back("graduacaoAtkRangedAirDown");
    tmp.push_back("graduacaoAtkRangedAirFront");
    tmp.push_back("graduacaoAtkRangedAirUp");
    tmp.push_back("graduacaoAtkRangedGroundFront");
    tmp.push_back("graduacaoAtkRangedGroundUp");
      
    playerAnim.push_back(tmp);
    playerAnimSize.push_back(tmp2);
    tmp.clear();
    
    //posgrad
    tmp.push_back("posgradIdle");
    tmp.push_back("posgradMoveWalk");
    tmp.push_back("posgradMoveRun");
    tmp.push_back("posgradJump");
        tmp.push_back("");
        tmp.push_back("");
    tmp.push_back("posgradDmgGround");
    tmp.push_back("posgradDie");
    tmp.push_back("posgradSpawn");  
    tmp.push_back("posgradAtkMeleeAirDown");
    tmp.push_back("posgradAtkMeleeAirFront");
    tmp.push_back("posgradAtkMeleeAirUp");
    tmp.push_back("posgradAtkMeleeGroundFront");
    tmp.push_back("posgradAtkMeleeGroundUp");
    tmp.push_back("posgradAtkRangedAirDown");
    tmp.push_back("posgradAtkRangedAirFront");
    tmp.push_back("posgradAtkRangedAirUp");
    tmp.push_back("posgradAtkRangedGroundFront");
    tmp.push_back("posgradAtkRangedGroundUp");
      
    playerAnim.push_back(tmp);
    playerAnimSize.push_back(tmp2);
    tmp.clear();
    tmp2.clear();
    
    //enemy
    tmp.push_back("paperWalk");
    tmp.push_back("paperWalk");
    tmp.push_back("paperWalk");
    tmp.push_back("paperWalk");
        tmp.push_back("");
        tmp.push_back("");
    tmp.push_back("paperWalk");
    tmp.push_back("paperWalk");
    
    tmp2.push_back(1);
    tmp2.push_back(2);
    tmp2.push_back(2);
    tmp2.push_back(1);
        tmp2.push_back(1);
        tmp2.push_back(1);
    tmp2.push_back(1);
    tmp2.push_back(1);
      
    enemyAnim.push_back(tmp);
    enemyAnimSize.push_back(tmp2);
    tmp.clear();
    tmp2.clear();
    
    //enemy 2
    tmp.push_back("bookIdle");
    tmp.push_back("bookWalk");
    tmp.push_back("bookWalk");
    tmp.push_back("bookIdle");
        tmp.push_back("");
        tmp.push_back("");
    tmp.push_back("bookIdle");
    tmp.push_back("bookIdle");
    
    tmp2.push_back(1);
    tmp2.push_back(5);
    tmp2.push_back(5);
    tmp2.push_back(1);
        tmp2.push_back(1);
        tmp2.push_back(1);
    tmp2.push_back(1);
    tmp2.push_back(1);
      
    enemyAnim.push_back(tmp);
    enemyAnimSize.push_back(tmp2);
    tmp.clear();
    tmp2.clear();
    
    //enemy 3
    tmp.push_back("calcIdle");
    tmp.push_back("calcWalk");
    tmp.push_back("calcWalk");
    tmp.push_back("calcIdle");
        tmp.push_back("");
        tmp.push_back("");
    tmp.push_back("calcIdle");
    tmp.push_back("calcIdle");
    
    tmp2.push_back(1);
    tmp2.push_back(4);
    tmp2.push_back(4);
    tmp2.push_back(1);
        tmp2.push_back(1);
        tmp2.push_back(1);
    tmp2.push_back(1);
    tmp2.push_back(1);
      
    enemyAnim.push_back(tmp);
    enemyAnimSize.push_back(tmp2);
    tmp.clear();
    tmp2.clear();
}