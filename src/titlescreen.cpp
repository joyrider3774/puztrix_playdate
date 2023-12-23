#include <SDL_framerate.h>
#include <SDL.h>
#include <time.h>
#include "main.h"
#include "titlescreen.h"
#include "gamefuncs.h"

void TitleScreenInit()
{
	srand((int) time(NULL));
    Score=0;
}

void TitleScreen()
{
	if(GameState == GSTitleScreenInit)
	{
		TitleScreenInit();
		GameState = GSTitleScreen;
	}
    
    if (GameState == GSTitleScreen)
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
						Mix_PlayChannel(-1, Sounds[SND_Select], 0);
                        WorldParts->LoadLevelPack(InstalledLevelPacks[SelectedLevelPack],StartPath);
                        WorldParts->SetLevel(1);
                        Retries = 5;
                        Score = 0;
                        PreviousGameState = GameState;
                        GameState=GSGameInit;
                        break;

                }
            }

        }

        SDL_BlitSurface(IMGTitleScreen,NULL,SDLScreen,NULL);
        char Text[512];
		sprintf(Text,"Play\nLevel Editor\n<%s>\nCredits\nQuit",InstalledLevelPacks[SelectedLevelPack]);
        WriteText(SDLScreen,BigFont,Text,strlen(Text),120,65,7,TextColor);        
    }
}
