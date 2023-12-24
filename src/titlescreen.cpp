#include <pd_api.h>
#include "main.h"
#include "titlescreen.h"
#include "gamefuncs.h"
#include "pd_helperfuncs.h"

void TitleScreenInit()
{
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
		if(Input->KeyboardPushed[SDLK_a])
		{
			CAudio_PlaySound( Sounds[SND_Select], 0);
			WorldParts->LoadLevelPack(InstalledLevelPacks[SelectedLevelPack]);
			WorldParts->SetLevel(1);
			Retries = 5;
			Score = 0;
			PreviousGameState = GameState;
			GameState=GSGameInit;
			
        }

        pd->graphics->drawBitmap(IMGTitleScreen, 0, 0, kBitmapUnflipped);
        char *Text;
		pd->system->formatString(&Text,"Play\nLevel Editor\n<%s>\nCredits",InstalledLevelPacks[SelectedLevelPack]);
        drawTextColor(true, NULL, BigFont2, Text, strlen(Text), kASCIIEncoding, 120, 65, kColorBlack, false);        
		pd->system->realloc(Text, 0);
    }
}
