#include <pd_api.h>
#include "main.h"
#include "gameover.h"
#include "gamefuncs.h"

void GameOverInit()
{
	CAudio_PlaySound(Sounds[SND_GameOver],0);
    
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
		if (Input->KeyboardPushed[SDLK_a])
		{
			CAudio_PlaySound(Sounds[SND_Select],0);
			PreviousGameState = GameState;
			GameState=GSTitleScreenInit;
        }
        pd->graphics->drawBitmap(IMGGameOver, 0, 0, kBitmapUnflipped);
        char *Text;
		pd->system->formatString(&Text,"Score: %d\nLevel Number: %d\nMoves Left: %d\nRetry: %d\n",Score,WorldParts->GetLevel(),MaxMoves,Retries);
        drawTextColor(false, NULL, font, Text, strlen(Text), kASCIIEncoding, 21, 50, kColorBlack, false);
        pd->system->realloc(Text, 0);
		DrawArrows(NULL);
    }
}


