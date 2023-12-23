#include <SDL_framerate.h>
#include <SDL.h>
#include "main.h"
#include "leveleditor.h"
#include "gamefuncs.h"
#include "cbox.h"

void LevelEditorInit()
{
	SelectedBlock=0;
}

void LevelEditor()
{
	if(GameState == GSLevelEditorInit)
	{
		LevelEditorInit();
		GameState = GSLevelEditor;
	}

    if (GameState == GSLevelEditor)
    {

        if(GlobalSoundEnabled)
            if (! Mix_PlayingMusic())
            {
                SelectedMusic =	rand()%(MusicCount);
                Mix_PlayMusic(Music[SelectedMusic],0);
                SetVolume(Volume);
            }

    	SDL_Event Event;
        while (SDL_PollEvent(&Event))
        {

            if (Event.type == SDL_QUIT)
                GameState=GSQuit;

            if (Event.type == SDL_MOUSEBUTTONUP)
            {
                if(true)
                {
                    if((Event.button.x > 9) && (Event.button.x < 9+136) && (Event.button.y > 3) && (Event.button.y < 3 + 36))
                    {
                        Mix_PlayChannel(-1,Sounds[SND_Select],0);
                        PreviousGameState = GameState;
                        GameState = GSTitleScreenInit;
                    }
                    else
                    if((Event.button.x >=Xoffset) && (Event.button.x < Xoffset+(NrOfCols*TileWidth) ) && (WorldParts->LevelCount > 0))
                    {
                        if(!(strcmp(InstalledLevelPacks[SelectedLevelPack],"levels.dat")==0))
                        {
                            if(WorldParts->GetLevel() > 0)
                            {
                                Mix_PlayChannel(-1,Sounds[SND_LevelEditorSelect],0);
                                WorldParts->Remove((Event.button.x - Xoffset) / TileWidth,Event.button.y/TileHeight);
                                if(SelectedBlock < 5)
                                    WorldParts->Add(new CBox((Event.button.x - Xoffset)/ TileWidth,Event.button.y/TileHeight,WorldParts->GetAssignedImage(),SelectedBlock+1));
                            }
                            else
                                Mix_PlayChannel(-1,Sounds[SND_Error],0);

                        }
                        else
                            Mix_PlayChannel(-1,Sounds[SND_Error],0);
                    }
                    else
                    {
                        if((Event.button.x >= 16) && (Event.button.x <16+(6*TileWidth)) && (Event.button.y >=115) && (Event.button.y < 115+TileHeight))
                        {
                            if(!(strcmp(InstalledLevelPacks[SelectedLevelPack],"levels.dat")==0))
                            {
                                if(WorldParts->GetLevel() > 0)
                                {
                                    Mix_PlayChannel(-1,Sounds[SND_Select],0);
                                    SelectedBlock = (Event.button.x - 33) / TileWidth;
                                }
                                else
                                    Mix_PlayChannel(-1,Sounds[SND_Error],0);

                            }
                            else
                                Mix_PlayChannel(-1,Sounds[SND_Error],0);
                        }
                        else
                            if((Event.button.x >= 15) && (Event.button.x <15+58) && (Event.button.y >=140) && (Event.button.y < 140+12))
                            {

                                if(WorldParts->GetLevel() < WorldParts->LevelCount)
                                    Mix_PlayChannel(-1,Sounds[SND_Select],0);
                                else
                                    Mix_PlayChannel(-1,Sounds[SND_Error],0);
                                if(!(strcmp(InstalledLevelPacks[SelectedLevelPack],"levels.dat")==0))
                                {
                                    WorldParts->SaveLevel();
                                }
                                WorldParts->IncLevel();
                            }
                            else
                                if((Event.button.x >= 15+60) && (Event.button.x <15+58+60) && (Event.button.y >=140) && (Event.button.y < 140+12))
                                {
                                    if(WorldParts->GetLevel() > 1)
                                        Mix_PlayChannel(-1,Sounds[SND_Select],0);
                                    else
                                        Mix_PlayChannel(-1,Sounds[SND_Error],0);
                                    if(!(strcmp(InstalledLevelPacks[SelectedLevelPack],"levels.dat")==0))
                                    {
                                        WorldParts->SaveLevel();
                                    }
                                    WorldParts->DecLevel();
                                }
                                else
                                    if((Event.button.x >= 15) && (Event.button.x <15+58) && (Event.button.y >=155) && (Event.button.y < 155+12))
                                    {
                                        if(!(strcmp(InstalledLevelPacks[SelectedLevelPack],"levels.dat")==0))
                                        {
                                            Mix_PlayChannel(-1,Sounds[SND_Select],0);
                                            WorldParts->SaveLevel();
                                            WorldParts->AddLevel();
                                        }
                                        else
                                            Mix_PlayChannel(-1,Sounds[SND_Error],0);
                                    }
                                    else
                                        if((Event.button.x >= 15+60) && (Event.button.x <15+58+60) && (Event.button.y >=155) && (Event.button.y < 155+12))
                                        {
                                            if(!(strcmp(InstalledLevelPacks[SelectedLevelPack],"levels.dat")==0))
                                            {
                                                if(WorldParts->GetLevel() > 0)
                                                {
                                                    Mix_PlayChannel(-1,Sounds[SND_Select],0);
                                                    WorldParts->DelLevel();
                                                }
                                                else
                                                    Mix_PlayChannel(-1,Sounds[SND_Error],0);
                                            }
                                            else
                                                Mix_PlayChannel(-1,Sounds[SND_Error],0);
                                        }
                                        else
                                            if((Event.button.x >= 15) && (Event.button.x <15+58) && (Event.button.y >=170) && (Event.button.y < 170+12))
                                            {
                                                if(!(strcmp(InstalledLevelPacks[SelectedLevelPack],"levels.dat")==0))
                                                {
                                                    if(WorldParts->GetLevel() > 0)
                                                    {
                                                        WorldParts->LevelMoves++;
                                                        if(WorldParts->LevelMoves > 100)
                                                        {
                                                            Mix_PlayChannel(-1,Sounds[SND_Error],0);
                                                            WorldParts->LevelMoves = 100;
                                                        }
                                                        else
                                                            Mix_PlayChannel(-1,Sounds[SND_Select],0);
                                                    }
                                                    else
                                                        Mix_PlayChannel(-1,Sounds[SND_Error],0);
                                                }
                                                else
                                                    Mix_PlayChannel(-1,Sounds[SND_Error],0);
                                            }
                                            else
                                                if((Event.button.x >= 15+60) && (Event.button.x <15+58+60) && (Event.button.y >=170) && (Event.button.y < 170+12))
                                                {
                                                    if(!(strcmp(InstalledLevelPacks[SelectedLevelPack],"levels.dat")==0))
                                                    {
                                                        if(WorldParts->GetLevel() > 0)
                                                        {
                                                            WorldParts->LevelMoves--;
                                                            if(WorldParts->LevelMoves < 1)
                                                            {
                                                                Mix_PlayChannel(-1,Sounds[SND_Error],0);
                                                                WorldParts->LevelMoves = 1;
                                                            }
                                                            else
                                                                Mix_PlayChannel(-1,Sounds[SND_Select],0);
                                                        }
                                                        else
                                                            Mix_PlayChannel(-1,Sounds[SND_Error],0);
                                                    }
                                                    else
                                                        Mix_PlayChannel(-1,Sounds[SND_Error],0);
                                                }
                                                else
                                                    if((Event.button.x >= 15) && (Event.button.x <15+58) && (Event.button.y >=185) && (Event.button.y < 185+12))
                                                    {
                                                        if(!(strcmp(InstalledLevelPacks[SelectedLevelPack],"levels.dat")==0))
                                                        {
                                                            Mix_PlayChannel(-1,Sounds[SND_Select],0);
                                                            WorldParts->SaveLevel();
                                                            WorldParts->SaveLevelPack(InstalledLevelPacks[SelectedLevelPack],StartPath);
                                                        }
                                                        else
                                                            Mix_PlayChannel(-1,Sounds[SND_Error],0);
                                                    }
                                                    else
                                                        if((Event.button.x >= 15+60) && (Event.button.x <15+58+60) && (Event.button.y >=185) && (Event.button.y < 185+12))
                                                        {
                                                            if(InstalledLevelPacksCount+1<MaxLevelPacks)
                                                            {
                                                                Mix_PlayChannel(-1,Sounds[SND_Select],0);
                                                                WorldParts->NewLevelPack();
                                                                int Teller=1;
                                                                char FileName[FILENAME_MAX+PATH_MAX];
    															sprintf(FileName,"%slevels/my_levels%d.dat",StartPath,Teller);
                                                                while(FileExists(FileName))
                                                                {
                                                                    Teller++;
                                                                    sprintf(FileName,"%slevels/my_levels%d.dat",StartPath,Teller);
                                                                }
                                                                sprintf(FileName,"my_levels%d.dat",Teller);
                                                                WorldParts->SaveLevelPack(FileName,StartPath);
                                                                SearchForLevelPacks();
                                                                for(Teller = 0;Teller<InstalledLevelPacksCount;Teller++)
                                                                    if(strcmp(FileName,InstalledLevelPacks[Teller]) == 0)
                                                                    {
                                                                        SelectedLevelPack = Teller;
                                                                        break;
                                                                    }
                                                            }
                                                            else
                                                                Mix_PlayChannel(-1,Sounds[SND_Error],0);

                                                        }
                                                        else
                                                            if((Event.button.x >= 15) && (Event.button.x <15+58) && (Event.button.y >=200) && (Event.button.y < 200+12))
                                                            {
                                                                if(!(strcmp(InstalledLevelPacks[SelectedLevelPack],"levels.dat")==0))
                                                                {
                                                                    if(WorldParts->GetLevel() > 0)
                                                                    {
                                                                        Mix_PlayChannel(-1,Sounds[SND_Select],0);
                                                                        WorldParts->RemoveAll();
                                                                    }
                                                                    else
                                                                        Mix_PlayChannel(-1,Sounds[SND_Error],0);
                                                                }
                                                                else
                                                                    Mix_PlayChannel(-1,Sounds[SND_Error],0);
                                                            }
                                                            else
                                                                if((Event.button.x >= 15+60) && (Event.button.x <15+58+60) && (Event.button.y >=200) && (Event.button.y < 200+12))
                                                                {
                                                                    if(!(strcmp(InstalledLevelPacks[SelectedLevelPack],"levels.dat")==0))
                                                                    {
                                                                        Mix_PlayChannel(-1,Sounds[SND_Select],0);
        				                                                char FileName[FILENAME_MAX+PATH_MAX];
																		sprintf(FileName,"%slevels/%s",StartPath,InstalledLevelPacks[SelectedLevelPack]);
                                                                        remove(FileName);
#ifdef N810
                                                                        sync();
#endif
                                                                        SearchForLevelPacks();
                                                                        WorldParts->LoadLevelPack(InstalledLevelPacks[SelectedLevelPack],StartPath);
                                                                        WorldParts->SetLevel(1);
                                                                    }
                                                                    else
                                                                        Mix_PlayChannel(-1,Sounds[SND_Error],0);
                                                                }
                                                                else
                                                                    if((Event.button.x >= 30) && (Event.button.x <15+58) && (Event.button.y >=215) && (Event.button.y < 215+12))
                                                                    {
                                                                        Mix_PlayChannel(-1,Sounds[SND_Select],0);
                                                                        Grid++;
                                                                        if (Grid > 2)
                                                                            Grid = 0;
                                                                    }
                                                                    else
                                                                        if((Event.button.x >= 15+60) && (Event.button.x <15+58+60) && (Event.button.y >=215) && (Event.button.y < 215+12))
                                                                        {
                                                                            if(!(strcmp(InstalledLevelPacks[SelectedLevelPack],"levels.dat")==0))
                                                                            {
                                                                                if(WorldParts->LevelCount > 0)
                                                                                {
                                                                                    if(CanPlay(WorldParts))
                                                                                    {
                                                                                        Mix_PlayChannel(-1,Sounds[SND_Select],0);
                                                                                        WorldParts->SaveLevel();
                                                                                        PreviousGameState = GameState;
                                                                                        GameState = GSGameInit;
                                                                                    }
                                                                                    else
                                                                                        Mix_PlayChannel(-1,Sounds[SND_Error],0);
                                                                                }
                                                                                else
                                                                                    Mix_PlayChannel(-1,Sounds[SND_Error],0);
                                                                            }
                                                                            else
                                                                                Mix_PlayChannel(-1,Sounds[SND_Error],0);
                                                                        }
                                                                        else
                                                                            if((Event.button.x >= 20) && (Event.button.x <135) && (Event.button.y >=50) && (Event.button.y < 62))
                                                                            {
                                                                                Mix_PlayChannel(-1,Sounds[SND_Select],0);
                                                                                SelectedLevelPack++;
                                                                                if(SelectedLevelPack >= InstalledLevelPacksCount)
                                                                                    SelectedLevelPack = 0;
                                                                                WorldParts->LoadLevelPack(InstalledLevelPacks[SelectedLevelPack],StartPath);
                                                                                WorldParts->SetLevel(1);
                                                                            }

                    }
                }
            }

            if ((Event.type == SDL_KEYDOWN))
            {
                switch(Event.key.keysym.sym)
                {
                    case SDLK_PLUS:
                        IncVolume();
                        break;

                    case SDLK_MINUS:
                        DecVolume();
                        break;

                    case SDLK_l:
                        NextSkin();
                        WorldParts->AssignImage(IMGBlocks);
                        break;

                    default:
                        break;

                }
            }

        }

        SDL_BlitSurface(IMGBackgroundLevelEditor,NULL,SDLScreen,NULL);
        char Text[512];	
		sprintf(Text,"%s\nLevel Number: %d/%d\nMoves: %d\nGrid: %d\n",InstalledLevelPacks[SelectedLevelPack], WorldParts->GetLevel(),WorldParts->LevelCount,WorldParts->LevelMoves,Grid);
        WriteText(SDLScreen,font,Text,strlen(Text),21,50,2,TextColor);
        sprintf(Text,"Next Level  Prev Level\nAdd Level   Del Level\nInc Moves   Dec Moves\nSave Pack   New Pack\nClear Level  Del Pack\nGrid            Play");
        WriteText(SDLScreen,font,Text,strlen(Text),16,140,2,TextColor);

		SDL_Rect Dst,Src;
        if(Grid == 2)
        {
            Dst.x = Xoffset;
            Dst.y = Yoffset;
            Dst.w = NrOfCols * TileWidth;
            Dst.h = NrOfRows * TileHeight;
            SDL_FillRect(SDLScreen,&Dst,SDL_MapRGB(SDLScreen->format,255,255,255));
        }
        DrawFloor(WorldParts,SDLScreen);
        WorldParts->Draw(SDLScreen);
        for(int Teller=0;Teller<6;Teller++)
        {
            Src.x = 0;
            Src.y = TileHeight*Teller;
            Src.w = TileWidth;
            Src.h = TileHeight;
            Dst.x = 16 +(TileWidth*Teller);
            Dst.y = 115;
            Dst.w = TileWidth;
            Dst.h = TileHeight;
            SDL_BlitSurface(IMGBlocks,&Src,SDLScreen,&Dst);
        }
        boxRGBA(SDLScreen,33+(TileWidth*SelectedBlock),115-2,16+(TileWidth*(SelectedBlock+1))-1,115-4,255,255,255,255);
        boxRGBA(SDLScreen,33+(TileWidth*SelectedBlock),115+TileHeight+2,16+(TileWidth*(SelectedBlock+1))-1,115+TileHeight+4,255,255,255,255);
        if(Grid >=1)
        {
            Dst.x = Xoffset;
            Dst.y = Yoffset;
            Dst.w = NrOfCols * TileWidth;
            Dst.h = NrOfRows * TileHeight;
            SDL_BlitSurface(IMGGrid,NULL,SDLScreen,&Dst);
        }
    }
}
