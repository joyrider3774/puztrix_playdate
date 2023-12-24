#include <pd_api.h>
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

        //Moet in deze volgorde !!!!

        //1) als we niet moven en er zijn geen deathblocks blijf moven in de richting die we gingen
        if(!WorldParts->IsMoving())
        {
            for (int X=0;X<NrOfCols;X++)
                for (int Y=0;Y<NrOfRows;Y++)
                    if(EraseBlocks(WorldParts,X,Y) > 1)
                        if(GlobalSoundEnabled)
                            CAudio_PlaySound(Sounds[SND_Destroy],0);

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
		if(Input->KeyboardPushed[SDLK_b])
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
				GameState = GSTitleScreenInit;
			}
		}

		if(Input->KeyboardPushed[SDLK_a])
		{
			if((Movement != MNone))
			{
				if(GlobalSoundEnabled)
					CAudio_PlaySound(Sounds[SND_NoMoves],0);
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
		}

		if(!KeyPressed && Input->KeyboardPushed[SDLK_LEFT])
		{
			if(MaxMoves > 0)
				if(!DeathBlocks(WorldParts))
					if(!WorldParts->IsMoving())
						if(WorldParts->CanMoveToLeft())
						{
							WorldParts->MoveLeft();
							Movement = MLeft;
							MaxMoves--;
							KeyPressed = true;
							CAudio_PlaySound(Sounds[SND_DirectionMove],0);
						}
		}

		if(!KeyPressed && Input->KeyboardPushed[SDLK_RIGHT])
		{
			if(MaxMoves > 0)
				if(!DeathBlocks(WorldParts))
					if(!WorldParts->IsMoving())
						if(WorldParts->CanMoveToRight())
						{
							WorldParts->MoveRight();
							Movement = MRight;
							MaxMoves--;
							KeyPressed = true;
							CAudio_PlaySound(Sounds[SND_DirectionMove],0);
						}
		}

        if(!KeyPressed && Input->KeyboardPushed[SDLK_UP])
		{
            if(MaxMoves > 0)
                if(!DeathBlocks(WorldParts))
                    if(!WorldParts->IsMoving())
                        if(WorldParts->CanMoveToUp())
                        {
                            WorldParts->MoveUp();
                            Movement = MUp;
                            MaxMoves--;
                            KeyPressed = true;
                            CAudio_PlaySound(Sounds[SND_DirectionMove],0);
                        }
		}

		if(!KeyPressed && Input->KeyboardPushed[SDLK_DOWN])
		{
            if(MaxMoves > 0)
                if(!DeathBlocks(WorldParts))
                    if(!WorldParts->IsMoving())
                        if(WorldParts->CanMoveToDown())
                        {
                            WorldParts->MoveDown();
                            Movement = MDown;
                            MaxMoves--;
                            KeyPressed = true;
                            CAudio_PlaySound(Sounds[SND_DirectionMove],0);
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
                CAudio_PlaySound(Sounds[SND_NoMoves],0);
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
                CAudio_PlaySound(Sounds[SND_NoMoves],0);
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


        pd->graphics->drawBitmap(IMGBackground, 0, 0, kBitmapUnflipped);
        char *Text;
		pd->system->formatString(&Text,"Score: %d\nLevel Number: %d\nMoves Left: %d\nRetry: %d\n",Score,WorldParts->GetLevel(),MaxMoves,Retries);
        drawTextColor(false, NULL, font, Text, strlen(Text), kASCIIEncoding, 21, 50, kColorBlack, false);
        pd->system->realloc(Text, 0);
		DrawFloor(WorldParts, NULL);
        WorldParts->Draw(NULL); //bij draw event van blocks wordt de death en remove flag gezet
        DrawArrows(NULL);
        RemoveBlocks(WorldParts); // Dus wis deze blokken als er zouden zijn
    }
}
