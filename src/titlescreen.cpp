#include <pd_api.h>
#include "main.h"
#include "titlescreen.h"
#include "gamefuncs.h"
#include "pd_helperfuncs.h"

int titleSelection = 0;

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
		if(Input->KeyboardPushed[SDLK_UP])
		{
			titleSelection--;
			if (titleSelection < 0)
				titleSelection = 0;
		}

		if(Input->KeyboardPushed[SDLK_DOWN])
		{
			titleSelection++;
			if (titleSelection > 3)
				titleSelection = 3;
		}

		if((titleSelection == 2) && Input->KeyboardPushed[SDLK_LEFT])
		{
			SelectedLevelPack--;
			if (SelectedLevelPack < 0)
				SelectedLevelPack = InstalledLevelPacksCount-1;
		}

		if((titleSelection == 2) && Input->KeyboardPushed[SDLK_RIGHT])
		{
			SelectedLevelPack++;
			if (SelectedLevelPack == InstalledLevelPacksCount)
				SelectedLevelPack = 0;
		}

		if(Input->KeyboardPushed[SDLK_a])
		{
			CAudio_PlaySound( Sounds[SND_Select], 0);
			switch(titleSelection)
			{
				case 0:
					WorldParts->LoadLevelPack(InstalledLevelPacks[SelectedLevelPack]);
					WorldParts->SetLevel(1);
					Retries = 5;
					Score = 0;
					PreviousGameState = GameState;
					GameState=GSGameInit;
					break;
				case 1:
					WorldParts->LoadLevelPack(InstalledLevelPacks[SelectedLevelPack]);
					WorldParts->SetLevel(1);
					Retries = 5;
					Score = 0;
					PreviousGameState = GameState;
					GameState=GSLevelEditorInit;
					break;
				case 2:
					SelectedLevelPack++;
					if (SelectedLevelPack == InstalledLevelPacksCount)
						SelectedLevelPack = 0;
					break;
				case 3:
					PreviousGameState = GameState;
					GameState = GSCreditsInit;
					break;
				default:
					break;
			}
			
        }

        pd->graphics->drawBitmap(IMGTitleScreen, 0, 0, kBitmapUnflipped);
		
		const int LeftX = 113;
		LCDColor color;
		
		if(titleSelection == 0)
			color = kColorBlack;
		else
			color = (LCDColor) kColorGrey;
		drawTextColor(true, NULL, BigFont2, "Play", 4, kASCIIEncoding, LeftX, 65, color, false);

		if(titleSelection == 1)
			color = kColorBlack;
		else
			color = (LCDColor) kColorGrey;
		drawTextColor(true, NULL, BigFont2, "Level Editor", 12, kASCIIEncoding, LeftX, 95, color, false);

		if(titleSelection == 2)
			color = kColorBlack;
		else
			color = (LCDColor) kColorGrey;
		char *Text;
		size_t len = strlen(InstalledLevelPacks[SelectedLevelPack]) -4 ;
		char *Tmp = (char *)pd->system->realloc(NULL, (len +1) * sizeof(char));
		memset(Tmp, 0, (len+1)*sizeof(char));
		strncpy(Tmp, InstalledLevelPacks[SelectedLevelPack], len);
		pd->system->formatString(&Text, "<%s>", Tmp);
		drawTextColor(true, NULL, BigFont2, Text, strlen(Text), kASCIIEncoding, LeftX, 125, color, false);
		pd->system->realloc(Text, 0);
		pd->system->realloc(Tmp, 0);

		if(titleSelection == 3)
			color = kColorBlack;
		else
			color = (LCDColor) kColorGrey;
		drawTextColor(true, NULL, BigFont2, "Credits", 7 , kASCIIEncoding, LeftX, 155, color, false);
    }
}
