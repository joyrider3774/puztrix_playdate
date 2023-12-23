#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <dirent.h>
#include <sys/stat.h>
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
	FILE *Fp;
	Fp = fopen(FileName,"rb");
	if (Fp)
	{
		fclose(Fp);
		return true;
	}
	else
		return false;
}


void WriteText(SDL_Surface* Surface,TTF_Font* FontIn,char* Tekst,int NrOfChars,int X,int Y,int YSpacing,SDL_Color ColorIn)
{
	char List[100][255];
	int Lines,Teller,Chars;
	SDL_Rect DstRect;
	SDL_Surface* TmpSurface1;
	Lines = 0;
	Chars = 0;
	for (Teller=0;Teller<NrOfChars;Teller++)
	{
		if(Lines > 100)
			break;
		if((Tekst[Teller] == '\n') || (Chars==255))
		{
			List[Lines][Chars]='\0';
			Lines++;
			Chars = 0;
		}
		else
		{
		 	List[Lines][Chars]=Tekst[Teller];
		 	Chars++;
		}
	}
	List[Lines][Chars] = '\0';
	for (Teller=0;Teller <= Lines;Teller++)
	{
		if(strlen(List[Teller]) > 0)
		{
			TmpSurface1 = TTF_RenderText_Blended(FontIn,List[Teller],ColorIn);
			DstRect.x = X;
			DstRect.y = Y + (Teller) * TTF_FontLineSkip(FontIn) + (Teller*YSpacing);
			DstRect.w = TmpSurface1->w;
			DstRect.h = TmpSurface1->h;
			SDL_BlitSurface(TmpSurface1,NULL,Surface,&DstRect);
			SDL_FreeSurface(TmpSurface1);
		}
	}
}

void LoadGraphics()
{
	char FileName[PATH_MAX+FILENAME_MAX];
    SDL_Surface *Tmp;
    FILE *ColorFile;
    int R,G,B;


	if(IMGArrows1)
		SDL_FreeSurface(IMGArrows1);
	if(IMGArrows2)
		SDL_FreeSurface(IMGArrows2);
	if(IMGBackground)
		SDL_FreeSurface(IMGBackground);
    if(IMGLevelDone)
		SDL_FreeSurface(IMGLevelDone);
	if(IMGBlocks)
		SDL_FreeSurface(IMGBlocks);
    if(IMGFloor)
        SDL_FreeSurface(IMGFloor);
    if(IMGGameOver)
        SDL_FreeSurface(IMGGameOver);
    if(IMGLevelpackDone)
		SDL_FreeSurface(IMGLevelpackDone);
    if(IMGTitleScreen)
		SDL_FreeSurface(IMGTitleScreen);
    if(IMGGrid)
        SDL_FreeSurface(IMGGrid);
    if(IMGBackgroundLevelEditor)
		SDL_FreeSurface(IMGBackgroundLevelEditor);


    sprintf(FileName,"%sskins/%s/grid.png",StartPath,SkinName);
    if (FileExists(FileName))
        Tmp = IMG_Load(FileName);
    else
    {
        sprintf(FileName,"%sgraphics/grid.png",StartPath);
        Tmp = IMG_Load(FileName);
    }
    IMGGrid= SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);


    sprintf(FileName,"%sskins/%s/gameover.png",StartPath,SkinName);
    if (FileExists(FileName))
        Tmp = IMG_Load(FileName);
    else
    {
        sprintf(FileName,"%sgraphics/gameover.png",StartPath);
        Tmp = IMG_Load(FileName);
    }
    IMGGameOver= SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

    sprintf(FileName,"%sskins/%s/titlescreen.png",StartPath,SkinName);
    if (FileExists(FileName))
        Tmp = IMG_Load(FileName);
    else
    {
        sprintf(FileName,"%sgraphics/titlescreen.png",StartPath);
        Tmp = IMG_Load(FileName);
    }
    IMGTitleScreen= SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);


    sprintf(FileName,"%sskins/%s/arrows1.png",StartPath,SkinName);
    if (FileExists(FileName))
        Tmp = IMG_Load(FileName);
    else
    {
        sprintf(FileName,"%sgraphics/arrows1.png",StartPath);
        Tmp = IMG_Load(FileName);
    }
    IMGArrows1= SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

    sprintf(FileName,"%sskins/%s/arrows2.png",StartPath,SkinName);
    if (FileExists(FileName))
        Tmp = IMG_Load(FileName);
    else
    {
        sprintf(FileName,"%sgraphics/arrows2.png",StartPath);
        Tmp = IMG_Load(FileName);
    }
    IMGArrows2= SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

    sprintf(FileName,"%sskins/%s/leveldone.png",StartPath,SkinName);
    if (FileExists(FileName))
        Tmp = IMG_Load(FileName);
    else
    {
        sprintf(FileName,"%sgraphics/leveldone.png",StartPath);
        Tmp = IMG_Load(FileName);
    }
    IMGLevelDone= SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

    sprintf(FileName,"%sskins/%s/levelpackdone.png",StartPath,SkinName);
    if (FileExists(FileName))
        Tmp = IMG_Load(FileName);
    else
    {
        sprintf(FileName,"%sgraphics/levelpackdone.png",StartPath);
        Tmp = IMG_Load(FileName);
    }
    IMGLevelpackDone= SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);


	sprintf(FileName,"%sskins/%s/background.png",StartPath,SkinName);
    if (FileExists(FileName))
        Tmp = IMG_Load(FileName);
    else
    {
        sprintf(FileName,"%sgraphics/background.png",StartPath);
        Tmp = IMG_Load(FileName);
    }
    IMGBackground= SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

    sprintf(FileName,"%sskins/%s/backgroundleveleditor.png",StartPath,SkinName);
    if (FileExists(FileName))
        Tmp = IMG_Load(FileName);
    else
    {
        sprintf(FileName,"%sgraphics/backgroundleveleditor.png",StartPath);
        Tmp = IMG_Load(FileName);
    }
    IMGBackgroundLevelEditor= SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);


    sprintf(FileName,"%sskins/%s/floor.png",StartPath,SkinName);
    if (FileExists(FileName))
        Tmp = IMG_Load(FileName);
    else
    {
        sprintf(FileName,"%sgraphics/floor.png",StartPath);
        Tmp = IMG_Load(FileName);
    }
    IMGFloor= SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

    sprintf(FileName,"%sskins/%s/blocks.png",StartPath,SkinName);
    if (FileExists(FileName))
        Tmp = IMG_Load(FileName);
    else
    {
        sprintf(FileName,"%sgraphics/blocks.png",StartPath);
        Tmp = IMG_Load(FileName);
    }
    IMGBlocks= SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);


    sprintf(FileName,"%sskins/%s/color.txt",StartPath,SkinName);
    if (FileExists(FileName))
    {
        ColorFile = fopen(FileName,"r");
        fscanf(ColorFile,"TextColor_R=%d\nTextColor_G=%d\nTextColor_B=%d\n",&R,&G,&B);
        fclose(ColorFile);
        TextColor.r = R;
        TextColor.g = G;
        TextColor.b = B;
    }
    else
    {
        sprintf(FileName,"%sgraphics/color.txt",StartPath);
        ColorFile = fopen(FileName,"r");
        fscanf(ColorFile,"TextColor_R=%d\nTextColor_G=%d\nTextColor_B=%d\n",&R,&G,&B);
        fclose(ColorFile);
        TextColor.r = R;
        TextColor.g = G;
        TextColor.b = B;
    }
}


void UnLoadGraphics()
{
    if(IMGArrows1)
		SDL_FreeSurface(IMGArrows1);
	if(IMGArrows2)
		SDL_FreeSurface(IMGArrows2);
	if(IMGBackground)
		SDL_FreeSurface(IMGBackground);
    if(IMGLevelDone)
		SDL_FreeSurface(IMGLevelDone);
	if(IMGBlocks)
		SDL_FreeSurface(IMGBlocks);
    if(IMGFloor)
        SDL_FreeSurface(IMGFloor);
    if(IMGGameOver)
        SDL_FreeSurface(IMGGameOver);
    if(IMGLevelpackDone)
		SDL_FreeSurface(IMGLevelpackDone);
    if(IMGTitleScreen)
		SDL_FreeSurface(IMGTitleScreen);
    if(IMGGrid)
        SDL_FreeSurface(IMGGrid);
    if(IMGBackgroundLevelEditor)
		SDL_FreeSurface(IMGBackgroundLevelEditor);
}

void NextSkin()
{
    char Text[500];
    SDL_Color Color = {0,0,0,255};
    if(InstalledSkinsCount > 0)
    {
        SelectedSkin++;
        if (SelectedSkin >=InstalledSkinsCount)
            SelectedSkin = -1;
        if(SelectedSkin == -1)
        {
            sprintf(SkinName,"%s","");
            sprintf(Text,"Please Wait Loading Skin:\nDefault Skin");
        }
        else
        {
            sprintf(SkinName,"%s",InstalledSkins[SelectedSkin]);
            sprintf(Text,"Please Wait Loading Skin:\n%s",SkinName);
        }
        boxRGBA(SDLScreen,0,0,399,50,255,255,255,200);
        boxRGBA(SDLScreen,2,2,398,49,0,0,0,200);
        WriteText(SDLScreen,BigFont,Text,strlen(Text),10,0,0,Color);
        SDL_Flip(SDLScreen);
        LoadGraphics();
    }
}


void SetVolume(const int VolumeIn)
{
	if (GlobalSoundEnabled)
	{
		Mix_Volume(0,VolumeIn);
		Mix_Volume(1,VolumeIn);
		Mix_VolumeMusic(2*(VolumeIn /3));
	}
}

void IncVolume()
{
	if (Volume < 128)
	{
		Volume += 8;
		SetVolume(Volume);
	}
}

void DecVolume()
{
	if (Volume > 0)
	{
		Volume-= 8;
		SetVolume(Volume);
	}
}

void LoadSettings()
{
	FILE *Fp;
	char FileName[PATH_MAX+FILENAME_MAX];
	sprintf(FileName,"%s/.puztrixsettings",getenv("HOME"));
	Fp = fopen(FileName,"rt");
	if (Fp)
	{
		fscanf(Fp,"Volume=%d\n",&Volume);
		fscanf(Fp,"Skin=%s\n",&SkinName);
		fclose(Fp);
	}
	else
	{
		Volume = 128;
	}
	SetVolume(Volume);
}

void SaveSettings()
{
	FILE *Fp;
	char FileName[PATH_MAX+FILENAME_MAX];
	sprintf(FileName,"%s/.puztrixsettings",getenv("HOME"));
	Fp = fopen(FileName,"wt");
	if (Fp)
	{
		fprintf(Fp,"Volume=%d\n",Volume);
		fprintf(Fp,"Skin=%s\n",SkinName);
		fclose(Fp);
#ifdef N810
		sync();
#endif
	}
}



void SearchForMusic()
{
	struct dirent *Entry;
	DIR *Directory;
	struct stat Stats;
	int Teller;
	char FileName[PATH_MAX+FILENAME_MAX];
	char Path[PATH_MAX];

    sprintf(FileName,"%smusic/title.mod",StartPath);
    Music[0] = Mix_LoadMUS(FileName);
	Teller=1;

	sprintf(Path,"%smusic",StartPath);
	Directory = opendir(Path);
	if (Directory)
	{
		Entry=readdir(Directory);
		while(Entry)
		{
			sprintf(FileName,"%smusic/%s",StartPath,Entry->d_name);
			stat(FileName,&Stats);
			if(!S_ISDIR(Stats.st_mode))
			{
				if(strncmp(".", Entry->d_name, 1) && (strcmp("title.mod",Entry->d_name) != 0) && (Teller< MaxMusicFiles))
				{
				//	printf("%s\n",Entry->d_name);
					if (GlobalSoundEnabled)
					{
						Music[Teller] = Mix_LoadMUS(FileName);
						Teller++;
					}
				}
			}
			Entry=readdir(Directory);
		}
		closedir(Directory);
	}
	MusicCount = Teller;
}

void SearchForSkins()
{
	struct dirent *Entry;
	DIR *Directory;
	struct stat Stats;
	int Teller=0;
	char FileName[PATH_MAX+FILENAME_MAX];
	char Path[PATH_MAX];
	sprintf(Path,"%sskins",StartPath);
	Directory = opendir(Path);
	if (Directory)
	{
		Entry=readdir(Directory);
		while(Entry)
		{
			sprintf(FileName,"%sskins/%s",StartPath,Entry->d_name);
			stat(FileName,&Stats);
			if(S_ISDIR(Stats.st_mode))
			{
				//printf("%s\n",Entry->d_name);
				if(strncmp(".", Entry->d_name, 1)  && (Teller< MaxSkins) && (strlen(Entry->d_name) < FILENAME_MAX-1))
				{
					sprintf(InstalledSkins[Teller],"%s",Entry->d_name);
					Teller++;
				}
			}
			Entry=readdir(Directory);
		}
		closedir(Directory);
	}
	InstalledSkinsCount = Teller;
	SelectedSkin=-1;
}

void SearchForLevelPacks()
{
	struct dirent *Entry;
	DIR *Directory;
	struct stat Stats;
	int Teller=0;
	char FileName[PATH_MAX + FILENAME_MAX];
	char Path[PATH_MAX];

    sprintf(InstalledLevelPacks[0] ,"levels.dat");
	Teller=1;
	sprintf(Path,"%slevels",StartPath);
	Directory = opendir(Path);
	if (Directory)
	{
		Entry=readdir(Directory);
		while(Entry)
		{
			sprintf(FileName,"%slevels/%s",StartPath,Entry->d_name);
			stat(FileName,&Stats);
			if(!S_ISDIR(Stats.st_mode))
			{
				//printf("%s\n",Entry->d_name);
				if(strncmp(".", Entry->d_name, 1)  && (Teller< MaxLevelPacks) && (strcmp("levels.dat",Entry->d_name) != 0))
				{
					sprintf(InstalledLevelPacks[Teller],"%s",Entry->d_name);
					Teller++;
				}
			}
			Entry=readdir(Directory);
		}
		closedir(Directory);
	}
	InstalledLevelPacksCount = Teller;
	SelectedLevelPack=0;
}



void UnloadMusic()
{
	int Teller;
	if (GlobalSoundEnabled)
	{
		Mix_HaltMusic();
		for (Teller=0;Teller < MusicCount;Teller++)
			if (Music[Teller])
				Mix_FreeMusic(Music[Teller]);
	}
}

void LoadSounds()
{
    char FileName[PATH_MAX+FILENAME_MAX];

	if (GlobalSoundEnabled)
	{
	    sprintf(FileName,"%ssound/destroy.wav",StartPath);
        Sounds[SND_Destroy] = Mix_LoadWAV(FileName);
        sprintf(FileName,"%ssound/nomoves.wav",StartPath);
        Sounds[SND_NoMoves] = Mix_LoadWAV(FileName);
        sprintf(FileName,"%ssound/win.wav",StartPath);
        Sounds[SND_Win] = Mix_LoadWAV(FileName);
        sprintf(FileName,"%ssound/winexit.wav",StartPath);
        Sounds[SND_WinExit] = Mix_LoadWAV(FileName);
        sprintf(FileName,"%ssound/directionmove.wav",StartPath);
        Sounds[SND_DirectionMove] = Mix_LoadWAV(FileName);
        sprintf(FileName,"%ssound/gameover.wav",StartPath);
        Sounds[SND_GameOver] = Mix_LoadWAV(FileName);
        sprintf(FileName,"%ssound/score.wav",StartPath);
        Sounds[SND_Score] = Mix_LoadWAV(FileName);
        sprintf(FileName,"%ssound/leveleditorselect.wav",StartPath);
        Sounds[SND_LevelEditorSelect] = Mix_LoadWAV(FileName);
        sprintf(FileName,"%ssound/select.wav",StartPath);
        Sounds[SND_Select] = Mix_LoadWAV(FileName);
        sprintf(FileName,"%ssound/error.wav",StartPath);
        Sounds[SND_Error] = Mix_LoadWAV(FileName);


	}
}


void UnloadSounds()
{
	int Teller;
	for (Teller=0;Teller<NrOfSounds;Teller++)
		if(Sounds[Teller])
			Mix_FreeChunk(Sounds[Teller]);
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

void DrawFloor(CWorldParts *aWorldParts,SDL_Surface* Surface)
{
    int X,Y,Z;
    CWorldPart *Part;
    SDL_Rect DstRect;
    bool BlockLeft,BlockUp,BlockRight,BlockDown;
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
                DstRect.w = TileWidth;
                DstRect.h = TileHeight;
                DstRect.x = (X*TileWidth) + Xoffset;
                DstRect.y = (Y*TileWidth) + Yoffset;
                SDL_BlitSurface(IMGFloor,NULL,Surface,&DstRect);
            }



        }


    }
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

void DrawArrows(SDL_Surface *Surface)
{
	const SDL_Rect DestRectsArrows[4] = {{58,110,44,44}, {98,145,44,44},{58,179,44,44},{16,145,44,44}};
    SDL_Rect SrcRect,DstRect;
    int Teller;

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
            SDL_BlitSurface(IMGArrows1,&SrcRect,Surface,&DstRect);
        }
        else
            if(Movement != MNone)
            {
                SrcRect.y = Movement * 44;
                DstRect.x = DestRectsArrows[Movement].x;
                DstRect.y = DestRectsArrows[Movement].y;
                SDL_BlitSurface(IMGArrows2,&SrcRect,Surface,&DstRect);
            }

    }
}