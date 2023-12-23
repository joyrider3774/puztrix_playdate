#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <limits.h>
#include "main.h"
#include "cworldparts.h"
#include "cbox.h"
#include "gamefuncs.h"
#include "stageclear.h"
#include "levelpackdone.h"
#include "credits.h"
#include "titlescreen.h"
#include "gameover.h"
#include "game.h"
#include "leveleditor.h"

using namespace std;
SDL_Surface *IMGBackgroundLevelEditor=NULL,*IMGBackground=NULL,*IMGBlocks=NULL,*IMGFloor = NULL,*IMGLevelDone=NULL, *IMGArrows1=NULL, *IMGArrows2=NULL,*IMGGameOver=NULL,*IMGLevelpackDone=NULL,*IMGTitleScreen=NULL,*IMGGrid=NULL;
SDL_Surface* SDLScreen;
TTF_Font* font,*BigFont;
SDL_Color TextColor;
int ScoreStatus = 0, RetryScore=0, ClearScore=0, MoveScore=0, TotalScore=0;
int SelectedBlock=0;
bool KeyPressed = false;
Movements Movement = MNone;
CWorldParts* WorldParts;
GameStates GameState = GSTitleScreenInit;
GameStates PreviousGameState = GSTitleScreenInit;
int MaxMoves=0,Score=0,Retries=5,MusicCount=0,SelectedMusic=0,InstalledLevelPacksCount=0,SelectedLevelPack=0,InstalledSkinsCount=0,SelectedSkin=-1,Grid=1;
bool GlobalSoundEnabled = true;
int Volume = 128;
char InstalledLevelPacks[MaxLevelPacks][FILENAME_MAX];
Mix_Music *Music[MaxMusicFiles];
Mix_Chunk *Sounds[NrOfSounds];
char StartPath[PATH_MAX];
char InstalledSkins[MaxSkins][FILENAME_MAX];
char SkinName[FILENAME_MAX];
FPSmanager Fpsman;

void mainLoop()
{
	switch(GameState)
	{
		case GSTitleScreenInit:
		case GSTitleScreen:
			TitleScreen();
			break;
		case GSLevelpackDoneInit:
		case GSLevelpackDone:
			LevelpackDone();
			break;
		case GSGameInit:
		case GSGame:
			Game();
			break;
		case GSStageClearInit:
		case GSStageClear:
			StageClear();
			break;
		case GSGameOverInit:
		case GSGameOver:
			GameOver();
			break;
		case GSLevelEditorInit:
		case GSLevelEditor:
			LevelEditor();
			break;
		case GSCreditsInit:
		case GSCredits:
			Credits();
			break;
		default :
			break;
	}
	SDL_Flip(SDLScreen);
    SDL_framerateDelay(&Fpsman);
}

int main(int argc, char **argv)
{
	char Filename[PATH_MAX+FILENAME_MAX];
	GetFilePath(argv[0],StartPath);
	WorldParts = new CWorldParts();
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO ) == 0)
	{
		printf("SDL Succesfully initialized\n");
        SDLScreen = SDL_SetVideoMode( 400 ,240,16, SDL_HWSURFACE );
		if(SDLScreen)
		{
		    SDL_WM_SetCaption("Puztrix","Puztrix");
		    SDL_FillRect(SDLScreen,NULL,SDL_MapRGB(SDLScreen->format,0,0,0));
		    printf("Succesfully Set %dx%dx16\n",800,480);
			SDL_ShowCursor(SDL_ENABLE);
            if (Mix_OpenAudio(22050,AUDIO_S16,MIX_DEFAULT_CHANNELS,512) < 0)
            {
                GlobalSoundEnabled = false;
                printf("Failed to initialise sound!\n");
                printf("%s\n",Mix_GetError());
            }
            else
            {
                printf("Audio Succesfully initialised!\n");
            }
            if (TTF_Init() == 0)
            {
                printf("Succesfully initialized TTF\n");
				sprintf(Filename,"%sdata/font.ttf",StartPath);
				font = TTF_OpenFont(Filename,10);				
				BigFont = TTF_OpenFont(Filename,16);
                if (font&&BigFont)
                {
                    printf("Succesfully Loaded fonts\n");
                    TTF_SetFontStyle(font,TTF_STYLE_NORMAL);
                    SDL_initFramerate(&Fpsman);
                    SDL_setFramerate(&Fpsman,FixedFPS);
                    SearchForLevelPacks();
                    SearchForSkins();
                    SearchForMusic();
                    LoadSounds();
                    LoadGraphics();
                    LoadSettings();
                    WorldParts->AssignImage(IMGBlocks);
                    Retries = 5;
                    Score = 0;
                    while (GameState != GSQuit)
                    {
                        mainLoop();
                    }
                    SaveSettings();
                    UnLoadGraphics();
                    UnloadSounds();
                    UnloadMusic();
                    TTF_CloseFont(font);
                    font=NULL;
                }
                else
                {
                    printf("Failed to Load fonts\n");
                }
                TTF_Quit();
            }
            else
            {
                printf("Failed to initialize TTF\n");
            }
            SDL_FreeSurface(SDLScreen);
            SDLScreen=NULL;
            SDLScreen=NULL;
            Mix_CloseAudio();
		}
		else
		{
			printf("Failed to Set Videomode %dx%dx16\n",800, 480);
		}
		SDL_Quit();
	}
	else
	{
		printf("Couldn't initialise SDL!\n");
	}
	WorldParts->RemoveAll();
	return 0;

}
