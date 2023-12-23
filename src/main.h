#ifndef MAIN_H
#define MAIN_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_framerate.h>
#include <SDL_ttf.h>
#include "cworldparts.h"

const int MaxLevelPacks=200,
		  MaxMusicFiles=26,
		  NrOfSounds=10,
		  SND_Destroy = 0,
		  SND_NoMoves = 1,
		  SND_Win = 2,
		  SND_WinExit = 3,
		  SND_DirectionMove = 4,
		  SND_GameOver = 5,
		  SND_Score = 6,
		  SND_LevelEditorSelect = 7,
		  SND_Select = 8,
		  SND_Error = 9,
		  MaxSkins=20,
		  FixedFPS=30;

enum GameStates {
	GSNone,GSGameOver,GSTitleScreen,GSLevelpackDone,GSCredits,GSIntro,GSQuit,GSGame,GSStageClear,GSStageSelect,GSLevelEditor,GSLevelEditorMenu,
	GSGameOverInit,GSTitleScreenInit,GSLevelpackDoneInit,GSCreditsInit,GSIntroInit,GSGameInit,GSStageClearInit,GSStageSelectInit,GSLevelEditorInit,GSLevelEditorMenuInit
};

enum Movements {MNone=4,MDown=2,MUp=0,MLeft=3,MRight=1};

typedef struct SPoint SPoint;
struct SPoint{
    int X;
    int Y;
};

extern SDL_Surface *SDLScreen, *IMGBackgroundLevelEditor,*IMGBackground,*IMGBlocks,*IMGFloor,*IMGLevelDone, *IMGArrows1, *IMGArrows2,*IMGGameOver,*IMGLevelpackDone,*IMGTitleScreen,*IMGGrid;
extern TTF_Font* font,*BigFont;

extern SDL_Color TextColor;

extern int ScoreStatus, RetryScore, ClearScore, MoveScore, TotalScore, SelectedBlock;
extern bool KeyPressed, GlobalSoundEnabled;
extern Movements Movement;
extern CWorldParts* WorldParts;
extern GameStates GameState, PreviousGameState;
extern int MaxMoves,Score,Retries,MusicCount,SelectedMusic,InstalledLevelPacksCount,SelectedLevelPack,InstalledSkinsCount,SelectedSkin,Grid;
extern int Volume;
extern char InstalledLevelPacks[MaxLevelPacks][FILENAME_MAX];
extern Mix_Music *Music[MaxMusicFiles];
extern Mix_Chunk *Sounds[NrOfSounds];
extern char StartPath[PATH_MAX];
extern char InstalledSkins[MaxSkins][FILENAME_MAX];
extern char SkinName[FILENAME_MAX];
extern FPSmanager Fpsman;

#endif
