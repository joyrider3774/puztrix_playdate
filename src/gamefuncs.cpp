#include <pd_api.h>
#include <sys/stat.h>
#include <limits.h>
#include "caudio.h"
#include "pd_helperfuncs.h"
#include "sdl_helpertypes.h"
#include "cworldparts.h"
#include "cbox.h"
#include "gamefuncs.h"
#include "main.h"

void GetFilePath(char *InputFile,char *result)
{
    int Teller,Pos=0;
    for(Teller=strlen(InputFile)-1;Teller>=0;Teller--)
        if(InputFile[Teller] == '/' || InputFile[Teller] == '\\')
        {
            Pos = Teller+1;
            break;
        }
    memcpy(result,InputFile,Pos);
}

bool CanPlay(CWorldParts *aWorldParts)
{
    int Teller,NumBlock1=0,NumBlock2=0,NumBlock3=0;
    for(Teller=0;Teller<aWorldParts->ItemCount;Teller++)
    {
        if(aWorldParts->Items[Teller]->GetType() == 1)
            NumBlock1++;
        if(aWorldParts->Items[Teller]->GetType() == 2)
            NumBlock2++;
        if(aWorldParts->Items[Teller]->GetType() == 3)
            NumBlock3++;

    }
    return ((NumBlock1 >= 2) || (NumBlock2 >= 2) || (NumBlock3 >=2));

}

char chr(int ascii)
{
	return((char)ascii);
}

int ord(char chr)
{
	return((int)chr);
}


bool FileExists(char * FileName)
{
	SDFile *Fp;
	Fp = pd->file->open(FileName, (FileOptions)(kFileReadData | kFileRead));
	if (Fp)
	{
		pd->file->close(Fp);
		return true;
	}
	else
		return false;
}

void LoadGraphics()
{
    IMGGrid = loadImageAtPath("graphics/grid");
    IMGGameOver = loadImageAtPath("graphics/gameover");
    IMGTitleScreen = loadImageAtPath("graphics/titlescreen");
    IMGArrows1 = loadImageAtPath("graphics/arrows1");
    IMGArrows2 = loadImageAtPath("graphics/arrows2");
    IMGLevelDone = loadImageAtPath("graphics/leveldone");
    IMGLevelpackDone = loadImageAtPath("graphics/levelpackdone");
    IMGBackground = loadImageAtPath("graphics/background");
    IMGBackgroundLevelEditor = loadImageAtPath("graphics/backgroundleveleditor");
    IMGFloor = loadImageAtPath("graphics/floor");
    IMGBlocks = loadImageAtPath("graphics/blocks");
}


void UnLoadGraphics()
{
    if(IMGArrows1)
		pd->graphics->freeBitmap(IMGArrows1);
	if(IMGArrows2)
		pd->graphics->freeBitmap(IMGArrows2);
	if(IMGBackground)
		pd->graphics->freeBitmap(IMGBackground);
    if(IMGLevelDone)
		pd->graphics->freeBitmap(IMGLevelDone);
	if(IMGBlocks)
		pd->graphics->freeBitmap(IMGBlocks);
    if(IMGFloor)
        pd->graphics->freeBitmap(IMGFloor);
    if(IMGGameOver)
        pd->graphics->freeBitmap(IMGGameOver);
    if(IMGLevelpackDone)
		pd->graphics->freeBitmap(IMGLevelpackDone);
    if(IMGTitleScreen)
		pd->graphics->freeBitmap(IMGTitleScreen);
    if(IMGGrid)
        pd->graphics->freeBitmap(IMGGrid);
    if(IMGBackgroundLevelEditor)
		pd->graphics->freeBitmap(IMGBackgroundLevelEditor);
}

// void LoadSettings()
// {
// 	FILE *Fp;
// 	char FileName[PATH_MAX+FILENAME_MAX];
// 	sprintf(FileName,"%s/.puztrixsettings",getenv("HOME"));
// 	Fp = fopen(FileName,"rt");
// 	if (Fp)
// 	{
// 		fscanf(Fp,"Volume=%d\n",&Volume);
// 		fscanf(Fp,"Skin=%s\n",&SkinName);
// 		fclose(Fp);
// 	}
// 	else
// 	{
// 		Volume = 128;
// 	}
// 	SetVolume(Volume);
// }

// void SaveSettings()
// {
// 	FILE *Fp;
// 	char FileName[PATH_MAX+FILENAME_MAX];
// 	sprintf(FileName,"%s/.puztrixsettings",getenv("HOME"));
// 	Fp = fopen(FileName,"wt");
// 	if (Fp)
// 	{
// 		fprintf(Fp,"Volume=%d\n",Volume);
// 		fprintf(Fp,"Skin=%s\n",SkinName);
// 		fclose(Fp);
// #ifdef N810
// 		sync();
// #endif
// 	}
// }

void SearchForLevelPacksListFiles(const char* path, void* userdata)
{
	pd->system->logToConsole(path);
	const char *ext = NULL;
	size_t len = strlen(path);
	
	if(len > 4)
	{
		ext = path + strlen(path) - 4;
	}

	if (ext != NULL)
		if ((strcmp(ext, ".dat") == 0) && (strcmp(path, "levels.dat") !=0))
		{
			strcpy(InstalledLevelPacks[InstalledLevelPacksCount], path);
			InstalledLevelPacksCount++;
		}
}

void SearchForLevelPacks()
{
	InstalledLevelPacksCount = 0;
	SelectedLevelPack = 0;
	strcpy(InstalledLevelPacks[InstalledLevelPacksCount], "levels.dat");
	InstalledLevelPacksCount++;
	pd->file->listfiles("levels", &SearchForLevelPacksListFiles, NULL, 0);
	pd->system->logToConsole("bleh");
}


void UnloadMusic()
{
	CAudio_UnLoadMusic(Music);
}

void LoadSounds()
{
    Sounds[SND_Destroy] = CAudio_LoadSound("destroy");
	Sounds[SND_NoMoves] = CAudio_LoadSound("nomoves");
	Sounds[SND_Win] = CAudio_LoadSound("win");
	Sounds[SND_WinExit] = CAudio_LoadSound("winexit");
	Sounds[SND_DirectionMove] = CAudio_LoadSound("directionmove");
	Sounds[SND_GameOver] = CAudio_LoadSound("gameover");
	Sounds[SND_Score] = CAudio_LoadSound("score");
	Sounds[SND_LevelEditorSelect] = CAudio_LoadSound("leveleditorselect");
	Sounds[SND_Select] = CAudio_LoadSound("select");
	Sounds[SND_Error] = CAudio_LoadSound("error");
	Music = CAudio_LoadMusic("music");
}


void UnloadSounds()
{
	int Teller;
	for (Teller=0;Teller<NrOfSounds;Teller++)
		if(Sounds[Teller])
			CAudio_UnLoadSound(Sounds[Teller]);
}


void RemoveBlocks(CWorldParts *aWorldParts)
{
    CBox* Box;
    int Teller;
    for (Teller=0;Teller<aWorldParts->ItemCount;Teller++)
    {
        Box = (CBox*) aWorldParts->Items[Teller];
        if(Box->Remove)
        {
            aWorldParts->Remove(Box->GetPlayFieldX(), Box->GetPlayFieldY());
            Teller--;
        }
    }
}

bool DeathBlocks(CWorldParts *aWorldParts)
{
    CBox* Box;
    int Teller;
    for (Teller=0;Teller<aWorldParts->ItemCount;Teller++)
    {
        Box = (CBox*) aWorldParts->Items[Teller];
        if(Box->Death || Box->Remove)
            return true;
    }
    return false;
}

void DrawFloor(CWorldParts *aWorldParts,LCDBitmap* Surface)
{
    int X,Y,Z;
    CWorldPart *Part;
    bool BlockLeft,BlockUp,BlockRight,BlockDown;
    pd->graphics->pushContext(Surface);
	for(Y=0;Y<NrOfRows;Y++)
    {
        for (X=0;X<NrOfCols;X++)
        {

            BlockDown = false;
            BlockUp = false;
            BlockLeft = false;
            BlockRight = false;
            for(Z=X;Z>=0;Z--)
            {
                Part = aWorldParts->GetItemFromPosPlayField(Z,Y);
                if(Part != NULL)
                {
                    if(Part->GetType() == 5)
                    {
                        BlockLeft = true;
                        break;
                    }
                }
            }

            for(Z=X;Z<NrOfCols;Z++)
            {
                Part = aWorldParts->GetItemFromPosPlayField(Z,Y);
                if(Part != NULL)
                {
                    if(Part->GetType() == 5)
                    {
                        BlockRight = true;
                        break;
                    }
                }
            }

            for(Z=Y;Z>=0;Z--)
            {
                Part = aWorldParts->GetItemFromPosPlayField(X,Z);
                if(Part != NULL)
                {
                    if(Part->GetType() == 5)
                    {
                        BlockUp = true;
                        break;
                    }
                }
            }

            for(Z=Y;Z<NrOfRows;Z++)
            {
                Part = aWorldParts->GetItemFromPosPlayField(X,Z);
                if(Part != NULL)
                {
                    if(Part->GetType() == 5)
                    {
                        BlockDown = true;
                        break;
                    }
                }
            }
			
            if(BlockDown && BlockLeft && BlockRight && BlockUp)
            {
                DrawBitmapSrcRec(IMGFloor,(X*TileWidth) + Xoffset, (Y*TileWidth) + Yoffset, 0, 0, TileWidth, TileHeight, kBitmapUnflipped);
            }
        }
    }
	pd->graphics->popContext();
}

int EraseBlocks(CWorldParts *aWorldParts,int X,int Y)
{
    CBox *Start,*Down,*Up,*Left,*Right;
    int Result = 0;

    Start = (CBox*) aWorldParts->GetItemFromPosPlayField(X,Y) ;
    if(Start != NULL)
    {
        Result +=1;
        if((Start->GetType() !=5) && (Start->GetType() !=4) && !Start->Death)
        {
            Left = (CBox*)aWorldParts->GetItemFromPosPlayField(X-1,Y);
            if((Left != NULL))
            if((Left->GetType() == Start->GetType()))
            {
                Result +=1;
                Left->Death = true;
                Start->Death = true;
                Result += EraseBlocks(WorldParts,X-2,Y);
            }

            Down = (CBox*)aWorldParts->GetItemFromPosPlayField(X,Y+1);
            if((Down != NULL))
            if(Down->GetType() == Start->GetType())
            {
                Result +=1;
                Down->Death = true;
                Start->Death = true;
                Result +=  EraseBlocks(WorldParts,X,Y+2);
            }

            Up = (CBox*)aWorldParts->GetItemFromPosPlayField(X,Y-1);
            if((Up != NULL))
            if(Up->GetType() == Start->GetType())
            {

                Result +=1;
                Up->Death = true;
                Start->Death = true;
                Result +=  EraseBlocks(WorldParts,X,Y-2);
            }


            Right =(CBox*) aWorldParts->GetItemFromPosPlayField(X+1,Y);
            if((Right != NULL))
            if(Right->GetType() == Start->GetType())
            {
                Result +=1;
                Right->Death = true;
                Start->Death = true;
                Result +=  EraseBlocks(WorldParts,X+2,Y);
            }

        }
    }
    return Result;
}

bool LevelWon(CWorldParts *aWorldParts)
{
    int Teller;
    for (Teller=0;Teller<WorldParts->ItemCount;Teller++)
    {
        if((aWorldParts->Items[Teller]->GetType() > 0) && (aWorldParts->Items[Teller]->GetType() < 4))
            return false;
    }
    return true;
}

bool NoMovesLeft(CWorldParts *aWorldParts)
{
    int Teller,NumBlock1=0,NumBlock2=0,NumBlock3=0;
    for(Teller=0;Teller<aWorldParts->ItemCount;Teller++)
    {
        if(aWorldParts->Items[Teller]->GetType() == 1)
            NumBlock1++;
        if(aWorldParts->Items[Teller]->GetType() == 2)
            NumBlock2++;
        if(aWorldParts->Items[Teller]->GetType() == 3)
            NumBlock3++;

    }
    return ((NumBlock1 == 1) || (NumBlock2 == 1) || (NumBlock3 ==1));

}

void DrawArrows(LCDBitmap *Surface)
{
	const SDL_Rect DestRectsArrows[4] = {{58,110,44,44}, {98,145,44,44},{58,179,44,44},{16,145,44,44}};
    SDL_Rect SrcRect,DstRect;
    int Teller;
	pd->graphics->pushContext(Surface);
    for(Teller=0;Teller<4;Teller++)
    {
        SrcRect.x = 0;
        SrcRect.w = 44;
        SrcRect.h = 44;
        DstRect.w = 44;
        DstRect.h = 44;
        if(Teller != Movement)
        {
            SrcRect.y = Teller * 44;
            DstRect.x = DestRectsArrows[Teller].x;
            DstRect.y = DestRectsArrows[Teller].y;
            DrawBitmapSrcRec(IMGArrows1, DstRect.x, DstRect.y, SrcRect.x, SrcRect.y, SrcRect.w, SrcRect.h, kBitmapUnflipped);
        }
        else
            if(Movement != MNone)
            {
                SrcRect.y = Movement * 44;
                DstRect.x = DestRectsArrows[Movement].x;
                DstRect.y = DestRectsArrows[Movement].y;
                DrawBitmapSrcRec(IMGArrows2, DstRect.x, DstRect.y, SrcRect.x, SrcRect.y, SrcRect.w, SrcRect.h, kBitmapUnflipped);
            }
    }
	pd->graphics->popContext();
}