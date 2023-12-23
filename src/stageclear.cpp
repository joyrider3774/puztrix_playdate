#include <SDL.h>
#include <SDL_mixer.h>
#include "main.h"
#include "gamefuncs.h"

void StageClearInit()
{
    Mix_PlayChannel(-1,Sounds[SND_Win],0);
	ScoreStatus = 0;
	RetryScore=Retries*1000;
    ClearScore=((WorldParts->GetLevel() / 10) + 1)*1000;
    MoveScore=MaxMoves*500;
    TotalScore=RetryScore+ClearScore+MoveScore;
	Movement = MNone;
}

void StageClear()
{

	if(GameState == GSStageClearInit)
	{
		StageClearInit();
		GameState = GSStageClear;
	}
    
    if (GameState == GSStageClear)
    {
        if(GlobalSoundEnabled)
            if (! Mix_PlayingMusic())
            {
                SelectedMusic =	rand()%(MusicCount);
                Mix_PlayMusic(Music[SelectedMusic],0);
                SetVolume(Volume);
            }

        SDL_BlitSurface(IMGLevelDone,NULL,SDLScreen,NULL);
    	char Text[512];
		sprintf(Text,"Score: %d\nLevel Number: %d\nMoves Left: %d\nRetry: %d\n",Score,WorldParts->GetLevel(),MaxMoves,Retries);
        WriteText(SDLScreen,font,Text,strlen(Text),21,50,0,TextColor);
        sprintf(Text,"Clear Bonus: %dx1000 = %4d\nRetry Bonus: %dx1000 = %d\nMoves Bonus:%dx500   = %d\n",(WorldParts->GetLevel() / 10) + 1,ClearScore,Retries,RetryScore,MaxMoves,MoveScore);
        WriteText(SDLScreen,font,Text,strlen(Text),182,65,2,TextColor);
        sprintf(Text,"Total Bonus: %d\n",TotalScore);
        WriteText(SDLScreen,BigFont,Text,strlen(Text),182,140,8,TextColor);
        sprintf(Text,"Score: %d\n",Score);
        WriteText(SDLScreen,BigFont,Text,strlen(Text),182,170,8,TextColor);
        DrawArrows(SDLScreen);
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
                        ScoreStatus++;
                        if(ScoreStatus ==3)
                        {
                            if(WorldParts->GetLevel() + 1 <= WorldParts->LevelCount)
                            {
                                WorldParts->IncLevel();
                                PreviousGameState = GameState;
                                GameState=GSGameInit;
                            }
                            else
                            {
                                PreviousGameState = GameState;
                                GameState = GSLevelpackDoneInit;
                            }
                        }


                        break;

                }
            }

        }
        if(ScoreStatus == 1)
        {
            if(TotalScore > 0)
            {
                Score+=100;
                TotalScore -=100;
                if(TotalScore % 300 == 0)
                    Mix_PlayChannel(-1,Sounds[SND_Score],0);
            }
            else
                ScoreStatus++;
        }

        if(ScoreStatus == 2)
        {
             Score+=TotalScore;
             TotalScore = 0;
        }
    }
	if((GameState != GSStageClear) && (GameState != GSStageClearInit))
    	Mix_PlayChannel(-1,Sounds[SND_WinExit],0);
}
