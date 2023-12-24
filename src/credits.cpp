#include <pd_api.h>
#include "credits.h"
#include "gamefuncs.h"
#include "main.h"

void CreditsInit()
{

}

void Credits()
{
	if (GameState == GSCreditsInit)
	{
		CreditsInit();
		GameState = GSCredits;
	}
    if (GameState == GSCredits)
    {
	    if(Input->KeyboardPushed[SDLK_a])
		{
			PreviousGameState = GameState;
            GameState = GSTitleScreenInit;
        }
		pd->graphics->drawBitmap(IMGTitleScreen, 0, 0, kBitmapUnflipped);
        char *Text;
		pd->system->formatString(&Text,"      Puztrix was created by\n Willems Davy - joyrider3774\n\n      Puztrix is a remake of\n      gravnic, part of puznic\n    (Nintendo NES console)\n\n      Music was created by\n     radiance of iris - 1996\n Taken from the modarchive\n\nSounds created using sfxr");
        drawTextColor(true, NULL, font, Text, strlen(Text), kASCIIEncoding, 108, 60, kColorBlack, false);
		pd->system->realloc(Text, 0);
    }
}
