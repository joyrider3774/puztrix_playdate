#ifndef GAMEFUNCS_H
#define GAMEFUNCS_H

#include <SDL.h>
#include <SDL_ttf.h>
#include "cworldparts.h"

void GetFilePath(char *InputFile,char *result);
bool CanPlay(CWorldParts *aWorldParts);
char chr(int ascii);
bool FileExists(char * FileName);
void WriteText(SDL_Surface* Surface,TTF_Font* FontIn,char* Tekst,int NrOfChars,int X,int Y,int YSpacing,SDL_Color ColorIn);
void DrawArrows(SDL_Surface *Surface);
bool NoMovesLeft(CWorldParts *aWorldParts);
bool LevelWon(CWorldParts *aWorldParts);
int EraseBlocks(CWorldParts *aWorldParts,int X,int Y);
void DrawFloor(CWorldParts *aWorldParts,SDL_Surface* Surface);
bool DeathBlocks(CWorldParts *aWorldParts);
void RemoveBlocks(CWorldParts *aWorldParts);
void UnloadSounds();
void LoadSounds();
void UnloadMusic();
void SearchForLevelPacks();
void SearchForSkins();
void SearchForMusic();
void SaveSettings();
void LoadSettings();
void DecVolume();
void IncVolume();
void SetVolume(const int VolumeIn);
void NextSkin();
void UnLoadGraphics();
void LoadGraphics();

#endif