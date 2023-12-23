#include <SDL_framerate.h>
#include <SDL.h>
#include "main.h"
#include "gameover.h"
#include "gamefuncs.h"

void GameOverInit()
{
	Mix_PlayChannel(-1,Sounds[SND_GameOver],0);
    
}

void GameOver()
{
    if(GameState == GSGameOverInit)
	{
		GameOverInit();
		GameState = GSGameOver;
	}
	
	if (GameState == GSGameOver )
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
                        Mix_PlayChannel(-1,Sounds[SND_Select],0);
                        PreviousGameState = GameState;
                        GameState=GSTitleScreenInit;
                        break;

                }
            }

        }
        SDL_BlitSurface(IMGGameOver,NULL,SDLScreen,NULL);
        char Text[512];
		sprintf(Text,"Score: %d\nLevel Number: %d\nMoves Left: %d\nRetry: %d\n",Score,WorldParts->GetLevel(),MaxMoves,Retries);
        WriteText(SDLScreen,font,Text,strlen(Text),21,50,0,TextColor);
        DrawArrows(SDLScreen);
    }
}


