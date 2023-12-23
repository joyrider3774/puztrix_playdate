#include <SDL_framerate.h>
#include <SDL.h>
#include "main.h"
#include "game.h"
#include "gamefuncs.h"


void GameInit()
{
	Movement = MNone;
    MaxMoves = WorldParts->LevelMoves;
}

void Game()
{
	if (GameState == GSGameInit)
	{
		GameInit();
		GameState = GSGame;
	}

    if (GameState == GSGame)
    {

        if(GlobalSoundEnabled)
            if (! Mix_PlayingMusic())
            {
                SelectedMusic =	rand()%(MusicCount);
                Mix_PlayMusic(Music[SelectedMusic],0);
                SetVolume(Volume);
            }
        //Moet in deze volgorde !!!!

        //1) als we niet moven en er zijn geen deathblocks blijf moven in de richting die we gingen
        if(!WorldParts->IsMoving())
        {


            for (int X=0;X<NrOfCols;X++)
                for (int Y=0;Y<NrOfRows;Y++)
                    if(EraseBlocks(WorldParts,X,Y) > 1)
                        if(GlobalSoundEnabled)
                            Mix_PlayChannel(-1,Sounds[SND_Destroy],0);



            if(!DeathBlocks(WorldParts))
            switch (Movement)
            {
                case MLeft:
                    WorldParts->MoveLeft();
                    break;
                case MRight:
                    WorldParts->MoveRight();
                    break;
                case MUp:
                    WorldParts->MoveUp();
                    break;
                case MDown:
                    WorldParts->MoveDown();
                    break;
                default:
                    break;
            }
        }

        //2) Zet de richting die we willen moven en start moving zo dat ismoving true teruggeeft
        KeyPressed = false;
        SDL_Event Event;
    	while (SDL_PollEvent(&Event))
        {

            if (Event.type == SDL_QUIT)
                GameState=GSQuit;

            if ((Event.type == SDL_KEYDOWN) && !KeyPressed)
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

                    case SDLK_RETURN:
                        if((Movement != MNone))
                        {
                            if(GlobalSoundEnabled)
                                Mix_PlayChannel(-1,Sounds[SND_NoMoves],0);
                            Movement = MNone;
                            Retries--;
                            if(Retries == -1)
                            {
                                if(PreviousGameState == GSLevelEditor)
                                {
                                    PreviousGameState = GameState;
                                    GameState = GSLevelEditorInit;
                                    WorldParts->LoadLevel();
                                    Retries = 5;
                                }
                                else
                                {
                                    GameState = GSGameOverInit;
                                    Retries = 0;
                                }
                            }
                            else
                            {
                                WorldParts->SetLevel(WorldParts->GetLevel());
                                MaxMoves = WorldParts->LevelMoves;
                            }



                        }
                        break;

                    case SDLK_LEFT:
                        if(MaxMoves > 0)
                        if(!DeathBlocks(WorldParts))
                        if(!WorldParts->IsMoving())
                        if(WorldParts->CanMoveToLeft())
                        {
                            WorldParts->MoveLeft();
                            Movement = MLeft;
                            MaxMoves--;
                            KeyPressed = true;
                            Mix_PlayChannel(-1,Sounds[SND_DirectionMove],0);
                        }
                        break;

                    case SDLK_RIGHT:
                        if(MaxMoves > 0)
                        if(!DeathBlocks(WorldParts))
                        if(!WorldParts->IsMoving())
                        if(WorldParts->CanMoveToRight())
                        {
                            WorldParts->MoveRight();
                            Movement = MRight;
                            MaxMoves--;
                            KeyPressed = true;
                            Mix_PlayChannel(-1,Sounds[SND_DirectionMove],0);
                        }
                        break;

                    case SDLK_UP:
                        if(MaxMoves > 0)
                        if(!DeathBlocks(WorldParts))
                        if(!WorldParts->IsMoving())
                        if(WorldParts->CanMoveToUp())
                        {
                            WorldParts->MoveUp();
                            Movement = MUp;
                            MaxMoves--;
                            KeyPressed = true;
                            Mix_PlayChannel(-1,Sounds[SND_DirectionMove],0);
                        }
                        break;

                    case SDLK_DOWN:
                        if(MaxMoves > 0)
                        if(!DeathBlocks(WorldParts))
                        if(!WorldParts->IsMoving())
                        if(WorldParts->CanMoveToDown())
                        {
                            WorldParts->MoveDown();
                            Movement = MDown;
                            MaxMoves--;
                            KeyPressed = true;
                            Mix_PlayChannel(-1,Sounds[SND_DirectionMove],0);
                        }
                        break;

                    default:
                        break;

                }
            }

        }

        if(LevelWon(WorldParts))
        {
            if(PreviousGameState == GSLevelEditor)
            {
                PreviousGameState = GameState;
                GameState = GSLevelEditorInit;
                WorldParts->LoadLevel();
                Retries = 5;
            }
            else
            {
                PreviousGameState = GameState;
                GameState = GSStageClearInit;
            }

        }

        //3) anders wordt dit true als we bij 2 ni starten met moven en stopt het direct wanneer er geen moves meer over zijn
        if((MaxMoves == 0) && !WorldParts->IsMoving() && !DeathBlocks(WorldParts) && (GameState != GSStageClearInit) && (GameState != GSLevelEditorInit))
        {
            if(GlobalSoundEnabled)
                Mix_PlayChannel(-1,Sounds[SND_NoMoves],0);
            Movement = MNone;
            Retries--;
            if(Retries == -1)
            {
                if(PreviousGameState == GSLevelEditor)
                {
                    PreviousGameState = GameState;
                    GameState = GSLevelEditorInit;
                    WorldParts->LoadLevel();
                    Retries = 5;
                }
                else
                {
                    GameState = GSGameOverInit;
                    Retries = 0;
                }
            }
            else
            {
                WorldParts->SetLevel(WorldParts->GetLevel());
                MaxMoves = WorldParts->LevelMoves;
            }
        }

        if(NoMovesLeft(WorldParts))
        {
            if(GlobalSoundEnabled)
                Mix_PlayChannel(-1,Sounds[SND_NoMoves],0);
            Movement = MNone;
            Retries--;
            if(Retries == -1)
            {
                if(PreviousGameState == GSLevelEditor)
                {
                    PreviousGameState = GameState;
                    GameState = GSLevelEditorInit;
                    WorldParts->LoadLevel();
                    Retries = 5;
                }
                else
                {
                    GameState = GSGameOverInit;
                    Retries = 0;
                }
            }
            else
            {
                WorldParts->SetLevel(WorldParts->GetLevel());
                MaxMoves = WorldParts->LevelMoves;
            }
        }

        WorldParts->Move();


        SDL_BlitSurface(IMGBackground,NULL,SDLScreen,NULL);
        char Text[512];
		sprintf(Text,"Score: %d\nLevel Number: %d\nMoves Left: %d\nRetry: %d\n",Score,WorldParts->GetLevel(),MaxMoves,Retries);
        WriteText(SDLScreen,font,Text,strlen(Text),21,50,0,TextColor);
        DrawFloor(WorldParts,SDLScreen);
        WorldParts->Draw(SDLScreen); //bij draw event van blocks wordt de death en remove flag gezet
        DrawArrows(SDLScreen);
        RemoveBlocks(WorldParts); // Dus wis deze blokken als er zouden zijn
    }
}
