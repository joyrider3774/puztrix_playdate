#include <pd_api.h>
#include <cstring>
#include "main.h"
#include "levelpackdone.h"
#include "gamefuncs.h"

void LevelpackDoneInit()
{
	WasNewHighScore = AddHighScore(Score, WorldParts->GetLevel());
}

void LevelpackDone()
{
    if (GameState == GSLevelpackDoneInit)
	{
		LevelpackDoneInit();
		GameState = GSLevelpackDone;
	}
   
    if (GameState == GSLevelpackDone)
    {
	    pd->graphics->drawBitmap(IMGLevelpackDone, 0, 0, kBitmapUnflipped);
        char *Text;
		pd->system->formatString(&Text,"Score: %d\nLevel Number: %d\nMoves Left: %d\nRetry: %d\n",Score,WorldParts->GetLevel(),MaxMoves,Retries);
        drawTextColor(true, NULL, font, Text, strlen(Text), kASCIIEncoding, 21, 50, kColorBlack, false);
		pd->system->realloc(Text, 0);
        DrawArrows(NULL);

		if (Input->KeyboardPushed[SDLK_a] || Input->KeyboardPushed[SDLK_b] || Input->KeyboardPushed[SDLK_UP] || 
			Input->KeyboardPushed[SDLK_LEFT] || Input->KeyboardPushed[SDLK_DOWN] || Input->KeyboardPushed[SDLK_RIGHT])
		{
			CAudio_PlaySound(Sounds[SND_Select],0);
        	PreviousGameState = GameState;
            if(WasNewHighScore)
				GameState=GSHighScoreSubmitInit;
			else
				GameState=GSTitleScreenInit;
        }
    }
}
