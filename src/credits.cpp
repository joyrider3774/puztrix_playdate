#include <SDL.h>
#include <SDL_framerate.h>
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
        if(GlobalSoundEnabled)
		{
            if (! Mix_PlayingMusic())
            {
                SelectedMusic =	rand()%(MusicCount);
                Mix_PlayMusic(Music[SelectedMusic],0);
                SetVolume(Volume);
            }
		}
	    SDL_Event Event;
        while (SDL_PollEvent(&Event))
        {

            if (Event.type == SDL_QUIT)
                GameState=GSQuit;
            
            if ((Event.type == SDL_KEYDOWN))
            {
                switch(Event.key.keysym.sym)
                {
                    case SDLK_PLUS:
                        IncVolume();
                        break;

                    case SDLK_MINUS:
                        DecVolume();
                        break;

                    case SDLK_l:
                        NextSkin();
                        WorldParts->AssignImage(IMGBlocks);
                        break;

                    default:
                        PreviousGameState = GameState;
                        GameState = GSTitleScreenInit;
                        break;

                }
            }

        }
		char Text[512];
        SDL_BlitSurface(IMGTitleScreen,NULL,SDLScreen,NULL);
        sprintf(Text,"Puztrix was created by\nWillems Davy, Willems Soft 2009\n\nPuztrix is a remake of gravnic,\nwhich is part of puznic, a\ngame for the nintendo console.\n\nMusic was created by\nradiance of iris - 1996\nTaken from the modarchive\n\nSounds created using sfxr");
        WriteText(SDLScreen,font,Text,strlen(Text),115,60,-1,TextColor);
    }
}
