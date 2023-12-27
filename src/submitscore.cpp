#include <pd_api.h>
#include <cstring>
#include "main.h"
#include "gameover.h"
#include "gamefuncs.h"
#include "codekey.h"
#include "scoresubmit/src/playdate/C_API/scoresubmit/scoresubmit.h"

LCDBitmap *QRCode = NULL;
char* verifyCode = NULL;

void SubmitScoreInit()
{
	if (QRCode)
		pd->graphics->freeBitmap(QRCode);
	if (verifyCode)
		pd->system->realloc(verifyCode, 0);
	volatile char codekey[64] = "SECRET01SECRET01SECRET01SECRET01SECRET01SECRET01SECRET01SECRET0";
#ifdef PUZTRIX_PLAYDATE_CODEKEY
	PUZTRIX_PLAYDATE_CODEKEY
#endif 
	QRCode = CreateQrCodeSubmit(pd, (char*)codekey, 43, Score, 228-60, &verifyCode);
	pd->system->logToConsole("score: %u verifier:%s %s", Score, verifyCode, codekey);
}

void SubmitScore()
{
    if(GameState == GSHighScoreSubmitInit)
	{
		SubmitScoreInit();
		GameState = GSHighScoreSubmit;
	}
	
	if (GameState == GSHighScoreSubmit )
    {
		if (Input->KeyboardPushed[SDLK_a] || Input->KeyboardPushed[SDLK_b] || Input->KeyboardPushed[SDLK_UP] || 
			Input->KeyboardPushed[SDLK_LEFT] || Input->KeyboardPushed[SDLK_DOWN] || Input->KeyboardPushed[SDLK_RIGHT])
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

		if (QRCode)
		{
			pd->graphics->fillRect(165, 50, 395-165, 228-50, kColorWhite);
			pd->graphics->drawRect(165, 50, 395-165, 228-50, kColorBlack);
			pd->graphics->drawRect(166, 51, 395-167, 228-52, kColorBlack);
			
			char* msg;
			pd->system->formatString(&msg, "SUBMIT YOUR HI-SCORE ONLINE");
			drawTextColor(false, NULL, font, msg, strlen(msg), kASCIIEncoding, 180 , 60, kColorBlack, false);
			pd->system->realloc(msg, 0);
			if (QRCode)
			{
				int w, h;
				pd->graphics->getBitmapData(QRCode, &w, &h, NULL, NULL, NULL);
				pd->graphics->drawBitmap(QRCode, 165+ ((395-165 - w) >> 1), 80, kBitmapUnflipped);
			}
		}
    }
}


