#include <pd_api.h>
#include <cstring>
#include  "caudio.h"
#include "main.h"
#include "gamefuncs.h"

void StageClearInit()
{
    CAudio_PlaySound(Sounds[SND_Win],0);
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
	bool playBackSound = true;
    if (GameState == GSStageClear)
    {
        pd->graphics->drawBitmap(IMGLevelDone, 0, 0, kBitmapUnflipped);
    	char *Text;
		pd->system->formatString(&Text,"Score: %d\nLevel Number: %d\nMoves Left: %d\nRetry: %d\n",Score,WorldParts->GetLevel(),MaxMoves,Retries);
        drawTextColor(true, NULL, font, Text, strlen(Text), kASCIIEncoding, 21, 50, kColorBlack, false);
		pd->system->realloc(Text, 0);
        pd->system->formatString(&Text,"Clear Bonus: %dx1000 = %4d\nRetry Bonus: %dx1000 = %d\nMoves Bonus:%dx500  = %d\n",(WorldParts->GetLevel() / 10) + 1,ClearScore,Retries,RetryScore,MaxMoves,MoveScore);
        drawTextColor(true, NULL, font, Text, strlen(Text), kASCIIEncoding, 182, 65, kColorBlack, false);
		pd->system->realloc(Text, 0);
        pd->system->formatString(&Text,"Total Bonus: %d\n",TotalScore);
        drawTextColor(true, NULL, BigFont, Text, strlen(Text), kASCIIEncoding, 182, 140, kColorBlack, false);
		pd->system->realloc(Text, 0);
        pd->system->formatString(&Text,"Score: %d\n",Score);
        drawTextColor(true, NULL, BigFont, Text, strlen(Text), kASCIIEncoding, 182, 170, kColorBlack, false);
		pd->system->realloc(Text, 0);
        DrawArrows(NULL);
		
		if(Input->KeyboardPushed[SDLK_b])
		{
			PreviousGameState = GameState;
			GameState = GSTitleScreenInit;
			playBackSound = false;
		}

		if (Input->KeyboardPushed[SDLK_a] || Input->KeyboardPushed[SDLK_UP] || 
			Input->KeyboardPushed[SDLK_LEFT] || Input->KeyboardPushed[SDLK_DOWN] || Input->KeyboardPushed[SDLK_RIGHT])
		{
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
        }
        if(ScoreStatus == 1)
        {
            if(TotalScore > 0)
            {
                Score+=100;
                TotalScore -=100;
                if(TotalScore % 300 == 0)
                    CAudio_PlaySound(Sounds[SND_Score],0);
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
	if(playBackSound && (GameState != GSStageClear) && (GameState != GSStageClearInit))
    	CAudio_PlaySound(Sounds[SND_WinExit],0);
}
