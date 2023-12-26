#include <pd_api.h>
#include "highscore.h"
#include "gamefuncs.h"
#include "main.h"

void ShowHighScoresInit()
{

}

void ShowHighScores()
{
	if (GameState == GSHighScoresInit)
	{
		ShowHighScoresInit();
		GameState = GSHighScores;
	}
    if (GameState == GSHighScores)
    {
	    if (Input->KeyboardPushed[SDLK_a] || Input->KeyboardPushed[SDLK_b] || Input->KeyboardPushed[SDLK_UP] || 
			Input->KeyboardPushed[SDLK_LEFT] || Input->KeyboardPushed[SDLK_DOWN] || Input->KeyboardPushed[SDLK_RIGHT])
		{
			PreviousGameState = GameState;
            GameState = GSTitleScreenInit;
        }
		pd->graphics->drawBitmap(IMGTitleScreen, 0, 0, kBitmapUnflipped);
        
		char *Text;

		pd->system->formatString(&Text,"#  score");
        drawTextColor(true, NULL, BigFont, Text, strlen(Text), kASCIIEncoding, 115, 87 + (-1* 20), kColorBlack, false);
		pd->system->realloc(Text, 0);

		pd->system->formatString(&Text,"lvl");
        drawTextColor(true, NULL, BigFont, Text, strlen(Text), kASCIIEncoding, 260, 87 + (-1* 20), kColorBlack, false);
		pd->system->realloc(Text, 0);	
		
		pd->graphics->drawLine(110,87, 287, 87,1, kColorBlack);
		
		for (int i = 0; i < MaxHighScores; i++)
		{
			pd->system->formatString(&Text,"%d. %09u", i+1, HighScores[i].Score);
        	drawTextColor(true, NULL, BigFont, Text, strlen(Text), kASCIIEncoding, 115, 87 + (i* 20), kColorBlack, false);
			pd->system->realloc(Text, 0);

			pd->system->formatString(&Text,"%02d", HighScores[i].Level);
        	drawTextColor(true, NULL, BigFont, Text, strlen(Text), kASCIIEncoding, 260, 87 + (i* 20), kColorBlack, false);
			pd->system->realloc(Text, 0);			
		}
    }
}
