#ifndef GAMEFUNCS_H
#define GAMEFUNCS_H

#include <pd_api.h>
#include  "pd_helperfuncs.h"
#include "cworldparts.h"

void GetFilePath(char *InputFile,char *result);
bool CanPlay(CWorldParts *aWorldParts);
char chr(int ascii);
bool FileExists(char * FileName);
void DrawArrows(LCDBitmap *Surface);
bool NoMovesLeft(CWorldParts *aWorldParts);
bool LevelWon(CWorldParts *aWorldParts);
int EraseBlocks(CWorldParts *aWorldParts,int X,int Y);
void DrawFloor(CWorldParts *aWorldParts,LCDBitmap* Surface);
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