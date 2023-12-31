#ifndef SDL2API
#include <pdcpp/pdnewlib.h>
#endif
#include <pd_api.h>
#include "pd_helperfuncs.h"
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
#include "highscore.h"
#include "leveleditor.h"
#include "cinput.h"
#include "options.h"
#include "submitscore.h"

using namespace std;
LCDBitmap *IMGBackgroundLevelEditor=NULL,*IMGBackground=NULL,*IMGBlocks=NULL,*IMGFloor = NULL,*IMGLevelDone=NULL, *IMGArrows1=NULL, *IMGArrows2=NULL,*IMGGameOver=NULL,*IMGLevelpackDone=NULL,*IMGTitleScreen=NULL,*IMGGrid=NULL;
LCDFont* font,*BigFont,*BigFont2;
//SDL_Color TextColor;
int ScoreStatus = 0, RetryScore=0, ClearScore=0, MoveScore=0, TotalScore=0;
int SelectedBlock=0;
bool KeyPressed = false;
Movements Movement = MNone;
CWorldParts* WorldParts;
GameStates GameState = GSTitleScreenInit;
GameStates PreviousGameState = GSTitleScreenInit;
int MaxMoves=0,Retries=5,InstalledLevelPacksCount=0,SelectedLevelPack=0,InstalledSkinsCount=0,SelectedSkin=-1,Grid=1;
int Volume = 128;
char InstalledLevelPacks[MaxLevelPacks][FILENAME_MAX];
int Music;
int Sounds[NrOfSounds];
char InstalledSkins[MaxSkins][FILENAME_MAX];
char SkinName[FILENAME_MAX];
unsigned int Frames=0,FrameTime=0;
float CurrentMs = 0.0f;
CInput *Input;
unsigned int Score=0;
HighScore HighScores[MaxHighScores];
bool WasNewHighScore = false;

int titleSelection = 0;
int editorSelectionX = 0, editorSelectionY = 0;
bool editing = false;
int optionSelection = 0;

int mainLoop(void *ud)
{
	Input->Update();
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
		case GSHighScoresInit:
		case GSHighScores:
			ShowHighScores();
			break;
		case GSOptionsInit:
		case GSOptions:
			Options();
			break;
		case GSHighScoreSubmitInit:
		case GSHighScoreSubmit:
			SubmitScore();
			break;
		default :
			break;
	}
	if (showFPS)
	{
		pd->system->drawFPS(0,0);
		Frames++;
		if (pd->system->getCurrentTimeMilliseconds() - FrameTime >= 1000)
		{
			CurrentMs = (float)(1000.0f / Frames);
			Frames = 0;
			FrameTime += 1000;
		}
		char* TmpText;
		pd->system->formatString(&TmpText, "FPS: %.0f\n", 1000.0f / CurrentMs);
		pd->system->realloc(TmpText, 0);

	}

	return 1;
}

static void resetGlobals()
{
	IMGBackgroundLevelEditor = NULL;
	IMGBackground = NULL;
	IMGBlocks = NULL;
	IMGFloor = NULL;
	IMGLevelDone = NULL;
	IMGArrows1 = NULL;
	IMGArrows2 = NULL;
	IMGGameOver = NULL;
	IMGLevelpackDone = NULL;
	IMGTitleScreen = NULL;
	IMGGrid = NULL;
	font = NULL;
	BigFont = NULL;
	BigFont2 = NULL;
	ScoreStatus = 0;
	RetryScore=0;
	ClearScore=0;
	MoveScore=0;
	TotalScore=0;
	SelectedBlock=0;
	KeyPressed = false;
	Movement = MNone;
	WorldParts = NULL;
	GameState = GSTitleScreenInit;
	PreviousGameState = GSTitleScreenInit;
	MaxMoves=0;
	Retries=5;
	InstalledLevelPacksCount=0;
	SelectedLevelPack=0;
	InstalledSkinsCount=0;
	SelectedSkin=-1;
	Grid=1;
	Volume = 128;
	Music = -1;
	Frames=0;
	FrameTime=0;
	CurrentMs = 0.0f;
	Input = NULL;
	Score=0;
	WasNewHighScore = false;
	titleSelection = 0;
	editorSelectionX = 0;
	editorSelectionY = 0;
	editing = false;
	optionSelection = 0;
}

static void setupGame()
{
	resetGlobals();
	Input = new CInput(pd, 10);
	CAudio_Init(false, 1.0f);
	font = loadFontAtPath("data/font");
	BigFont = loadFontAtPath("data/bigfont");
	BigFont2 = loadFontAtPath("data/bigfont2");
    SearchForLevelPacks();
    LoadSounds();
    LoadGraphics();
	LoadSettings();
	WorldParts = new CWorldParts();
    WorldParts->AssignImage(IMGBlocks);
    Retries = 5;
    Score = 0;
	CAudio_PlayMusic(Music, -1);
}

static void TerminateGame()
{
    SaveSettings();
    UnLoadGraphics();
    clearDrawtextBitmapCache();
	UnloadSounds();
    UnloadMusic();
	WorldParts->RemoveAll();
	delete WorldParts;
	delete Input;
}


#ifdef __cplusplus
#ifndef SDL2API
extern "C" {
#endif
#endif

#ifdef _WINDLL
__declspec(dllexport)
#endif

int eventHandler(PlaydateAPI* playdate, PDSystemEvent event, uint32_t arg)
{
	if ( event == kEventInit )
	{
		#ifndef SDL2API
		eventHandler_pdnewlib(pd, event, arg);
		#endif
		setPDPtr(playdate);
		playdate->display->setRefreshRate(FRAMERATE);
		playdate->system->setUpdateCallback(mainLoop, NULL);
		setupGame();
	}

	if (event == kEventTerminate)
	{
		TerminateGame();
	}
	return 0;
}

#ifdef __cplusplus
#ifndef SDL2API
}
#endif
#endif