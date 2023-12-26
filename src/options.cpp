#include <pd_api.h>
#include <cstring>
#include "credits.h"
#include "gamefuncs.h"
#include "main.h"

int optionSelection = 0;

void OptionsInit()
{
	optionSelection = 0;
}

void Options()
{
	if (GameState == GSOptionsInit)
	{
		OptionsInit();
		GameState = GSOptions;
	}
    if (GameState == GSOptions)
    {
	    if (Input->KeyboardPushed[SDLK_b])
		{
			PreviousGameState = GameState;
            GameState = GSTitleScreenInit;
        }

		if(Input->KeyboardPushed[SDLK_UP])
		{
			optionSelection--;
			if (optionSelection < 0)
				optionSelection = 0;
		}

		if(Input->KeyboardPushed[SDLK_DOWN])
		{
			optionSelection++;
			if (optionSelection > 2)
				optionSelection = 2;
		}

		if((optionSelection == 0) && (Input->KeyboardPushed[SDLK_LEFT] || Input->KeyboardPushed[SDLK_RIGHT]))
		{
			CAudio_PlaySound(Sounds[SND_Select], 0);
			bool tmp = CAudio_GetMusicEnabled();
			tmp = !tmp;
			CAudio_SetMusicEnabled(tmp);
			CAudio_PlayMusic(Music, -1);
			SaveSettings();
		}

		if((optionSelection == 1) && (Input->KeyboardPushed[SDLK_LEFT] || Input->KeyboardPushed[SDLK_RIGHT]))
		{
			CAudio_PlaySound(Sounds[SND_Select], 0);
			bool tmp = CAudio_GetSoundEnabled();
			tmp = !tmp;
			CAudio_SetSoundEnabled(tmp);
			SaveSettings();
		}

		if(Input->KeyboardPushed[SDLK_a])
		{
			CAudio_PlaySound( Sounds[SND_Select], 0);
			switch(optionSelection)
			{
				case 0:
				{
					CAudio_PlaySound(Sounds[SND_Select], 0);
					bool tmp = CAudio_GetMusicEnabled();
					tmp = !tmp;
					CAudio_SetMusicEnabled(tmp);
					CAudio_PlayMusic(Music, -1);
					SaveSettings();
					break;
				}
				case 1:
				{
					CAudio_PlaySound(Sounds[SND_Select], 0);
					bool tmp = CAudio_GetSoundEnabled();
					tmp = !tmp;
					CAudio_SetSoundEnabled(tmp);
					SaveSettings();
					break;
				}
				case 2:
				{
					CAudio_PlaySound(Sounds[SND_Select], 0);
					for (int i = 0; i < MaxHighScores; i++)
					{
						HighScores[i].Score = 0;
						HighScores[i].Level = 0;
					}
					SaveSettings();
					break;
				}
				default:
					break;
			}
			
        }

		pd->graphics->drawBitmap(IMGTitleScreen, 0, 0, kBitmapUnflipped);
		const int LeftX = 113;
		LCDColor color;
		
		if(optionSelection == 0)
			color = kColorBlack;
		else
			color = (LCDColor) kColorGrey;
		char *Text;
		pd->system->formatString(&Text, "Music: %s", CAudio_GetMusicEnabled() ? "On": "Off");
		drawTextColor(true, NULL, BigFont2, Text, strlen(Text), kASCIIEncoding, LeftX, 62, color, false);
		pd->system->realloc(Text, 0);

		if(optionSelection == 1)
			color = kColorBlack;
		else
			color = (LCDColor) kColorGrey;

		pd->system->formatString(&Text, "Sound: %s", CAudio_GetSoundEnabled() ? "On": "Off");
		drawTextColor(true, NULL, BigFont2, Text, strlen(Text), kASCIIEncoding, LeftX, 87, color, false);
		pd->system->realloc(Text, 0);

		if(optionSelection == 2)
			color = kColorBlack;
		else
			color = (LCDColor) kColorGrey;

		pd->system->formatString(&Text, "Reset High Score");
		drawTextColor(true, NULL, BigFont2, Text, strlen(Text), kASCIIEncoding, LeftX, 112, color, false);
		pd->system->realloc(Text, 0);
    }
}
