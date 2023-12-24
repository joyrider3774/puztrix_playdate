#include "cworldparts.h"
#include "pd_helperfuncs.h"
#include "cbox.h"


CWorldParts::CWorldParts()
{
    int Level,x,y;
    for (Level=1;Level<MaxLevels;Level++)
    {
        LevelPackData[((Level-1)*((NrOfCols*NrOfRows)+1))]=0;
        for (y=0;y<NrOfRows;y++)
            for (x=0;x<NrOfCols;x++)
            {
                LevelPackData[((Level-1)*((NrOfCols*NrOfRows)+1))+(y*NrOfCols)+x+1] = 0;
            }
    }
	ItemCount = 0;
    LevelCount = 0;
    LevelNumber = 0;
    LevelMoves = 1;
	DisableSorting = false;
	TilesImage = NULL;
}

int CWorldParts::GetLevel()
{
    return LevelNumber;
}

bool CWorldParts::CanMoveToRight()
{
    int Teller;
    for(Teller=0;Teller<ItemCount;Teller++)
        if(Items[Teller]->GetType() != 5)
        if(Items[Teller]->CanMoveTo(Items[Teller]->GetPlayFieldX()+1,Items[Teller]->GetPlayFieldY()))
            return true;

    return false;
}

bool CWorldParts::CanMoveToUp()
{
    int Teller;
    for(Teller=0;Teller<ItemCount;Teller++)
        if(Items[Teller]->CanMoveTo(Items[Teller]->GetPlayFieldX(),Items[Teller]->GetPlayFieldY()-1))
            return true;
    return false;
}

bool CWorldParts::CanMoveToDown()
{
    int Teller;
    for(Teller=0;Teller<ItemCount;Teller++)
        if(Items[Teller]->CanMoveTo(Items[Teller]->GetPlayFieldX(),Items[Teller]->GetPlayFieldY()+1))
            return true;
    return false;
}

bool CWorldParts::CanMoveToLeft()
{
    int Teller;
    for(Teller=0;Teller<ItemCount;Teller++)
        if(Items[Teller]->CanMoveTo(Items[Teller]->GetPlayFieldX()-1,Items[Teller]->GetPlayFieldY()))
            return true;
    return false;
}

void CWorldParts::SetLevel(int aLevelNumber)
{
    if ((aLevelNumber > 0) && (aLevelNumber <= LevelCount))
    {
        LevelNumber = aLevelNumber;
        LoadLevel();
    }
    else
    {
        RemoveAll();
        LevelNumber = 0;
        LevelMoves = 0;
    }
}

bool CWorldParts::IsMoving()
{
    int Teller;
    for (Teller=0;Teller<ItemCount;Teller++)
        if(Items[Teller]->IsMoving)
            return true;
    return false;
}

void CWorldParts::IncLevel()
{
    if(LevelNumber < LevelCount)
    {
        LevelNumber++;
        LoadLevel();
    }
}

void CWorldParts::DecLevel()
{
    if(LevelNumber > 1)
    {
        LevelNumber--;
        LoadLevel();
    }
}

bool CWorldParts::LoadLevelPack(char* LevelPackName,char* Startpath)
{
    SDFile *fp;
    long FileSize;
    char *FileName;

    LevelCount = 0;
    pd->file->mkdir("levels");
	pd->system->formatString(&FileName,"levels/%s", LevelPackName);
    fp = pd->file->open(FileName, (FileOptions)(kFileRead|kFileReadData));
    pd->system->realloc(FileName, 0);
	if(fp)
    {
        pd->file->seek(fp, 0, SEEK_END);
        FileSize = pd->file->tell(fp);
        pd->file->seek(fp, 0, SEEK_SET);

        if(FileSize <= ((NrOfRows*NrOfCols)+1)*MaxLevels)
        {
            pd->file->read(fp, LevelPackData, FileSize);
            pd->file->close(fp);
            LevelCount = FileSize / ((NrOfRows*NrOfCols)+1);
        }
        else
        {
            pd->file->close(fp);
            return false;
        }
    }
    else
        return false;

    return true;

}

void CWorldParts::AssignImage(LCDBitmap *Image)
{
    int Teller;
    TilesImage = Image;
    for (Teller=0;Teller<ItemCount;Teller++)
        Items[Teller]->AssignImage(Image);
}

void CWorldParts::RemoveAll()
{
	int Teller;
	for (Teller=0;Teller<ItemCount;Teller++)
	{
		delete Items[Teller];
		Items[Teller] = NULL;
	}
	ItemCount=0;
}
void CWorldParts::Remove(int PlayFieldXin,int PlayFieldYin)
{
	int Teller1,Teller2;
	for (Teller1=0;Teller1<ItemCount;Teller1++)
	{
		if ((Items[Teller1]->GetPlayFieldX() == PlayFieldXin) && (Items[Teller1]->GetPlayFieldY() == PlayFieldYin))
		{
			delete Items[Teller1];
			for (Teller2=Teller1;Teller2<ItemCount-1;Teller2++)
				Items[Teller2] = Items[Teller2+1];
			ItemCount--;
			Teller1--;
		}
	}
}

void CWorldParts::Remove(int PlayFieldXin,int PlayFieldYin,int Type)
{
	int Teller1,Teller2;
	for (Teller1=0;Teller1<ItemCount;Teller1++)
	{
		if ((Items[Teller1]->GetPlayFieldX() == PlayFieldXin) && (Items[Teller1]->GetPlayFieldY() == PlayFieldYin) && (Items[Teller1]->GetType() == Type))
		{
			delete Items[Teller1];
			for (Teller2=Teller1;Teller2<ItemCount-1;Teller2++)
				Items[Teller2] = Items[Teller2+1];
			ItemCount--;
			Teller1--;
		}
	}
}


void CWorldParts::Add(CWorldPart *WorldPart)
{
	if( ItemCount < NrOfRows*NrOfCols )
	{
		WorldPart->ParentList = this;
		Items[ItemCount] = WorldPart;
		ItemCount++;
		//SortY();
	}
}


void CWorldParts::Sort()
{
	int Teller1,Teller2,Index;
	CWorldPart *Part;
	if (!DisableSorting)
	{
		for (Teller1 = 1; Teller1 <ItemCount;Teller1++)
		{
			Index = Items[Teller1]->GetZ();
			Part = Items[Teller1];
			Teller2 = Teller1;
			while ((Teller2 > 0) && (Items[Teller2-1]->GetZ() > Index))
			{
				Items[Teller2] = Items[Teller2 - 1];
				Teller2--;
			}
			Items[Teller2] = Part;
		}
	}

}

void CWorldParts::SortY()
{
	int Teller1,Teller2,Index;
	CWorldPart *Part;
	if (!DisableSorting)
	{
		for (Teller1 = 1; Teller1 <ItemCount;Teller1++)
		{
			Index = Items[Teller1]->GetPlayFieldY();
			Part = Items[Teller1];
			Teller2 = Teller1;
			while ((Teller2 > 0) && (Items[Teller2-1]->GetPlayFieldY() < Index))
			{
				Items[Teller2] = Items[Teller2 - 1];
				Teller2--;
			}
			Items[Teller2] = Part;
		}
	}

}

void CWorldParts::NewLevelPack()
{
    RemoveAll();
    int Level,x,y;
    for (Level=1;Level<MaxLevels;Level++)
    {
        LevelPackData[((Level-1)*((NrOfCols*NrOfRows)+1))]=0;
        for (y=0;y<NrOfRows;y++)
            for (x=0;x<NrOfCols;x++)
            {
                LevelPackData[((Level-1)*((NrOfCols*NrOfRows)+1))+(y*NrOfCols)+x+1] = 0;
            }
    }
    LevelCount = 0;
    LevelNumber = 0;
    LevelMoves = 0;
}

void CWorldParts::SaveLevelPack(char* LevelPackName,char* Startpath)
{
    SDFile *fp;
    pd->file->mkdir("levels");
	char *FileName;
    pd->system->formatString(&FileName,"levels/%s", LevelPackName);
    fp = pd->file->open(FileName, (FileOptions)(kFileWrite));
    if(fp)
    {
        pd->file->write(fp, LevelPackData,LevelCount*((NrOfRows*NrOfCols)+1));
#ifndef _WINDLL
        pd->file->flush(fp);
#endif
        pd->file->close(fp);
    }
}

void CWorldParts::AddLevel()
{
    int x,y;
    if (LevelCount + 1 < MaxLevels)
    {
        LevelCount = LevelCount+1;
        LevelPackData[((LevelCount-1)*((NrOfCols*NrOfRows)+1))]=1;
        for (y=0;y<NrOfRows;y++)
            for (x=0;x<NrOfCols;x++)
            {
                LevelPackData[((LevelCount-1)*((NrOfCols*NrOfRows)+1))+(y*NrOfCols)+x+1] = 0;
            }
        SetLevel(LevelCount);
    }
}

void CWorldParts::DelLevel()
{
    int x,y,Level;

    if (LevelCount > 0)
    {
        for(Level = LevelNumber;Level < LevelCount;Level++)
        {
            LevelPackData[((Level-1)*((NrOfCols*NrOfRows)+1))]=LevelPackData[((Level)*((NrOfCols*NrOfRows)+1))];
            for (y=0;y<NrOfRows;y++)
                for (x=0;x<NrOfCols;x++)
                {
                    LevelPackData[((Level-1)*((NrOfCols*NrOfRows)+1))+(y*NrOfCols)+x+1] = LevelPackData[((Level)*((NrOfCols*NrOfRows)+1))+(y*NrOfCols)+x+1];
                }

        }

        LevelPackData[((LevelCount-1)*((NrOfCols*NrOfRows)+1))]=0;
            for (y=0;y<NrOfRows;y++)
                for (x=0;x<NrOfCols;x++)
                {
                    LevelPackData[((LevelCount-1)*((NrOfCols*NrOfRows)+1))+(y*NrOfCols)+x+1] = 0;
                }


        LevelCount--;
        if (LevelCount == 0)
        {
            RemoveAll();
            LevelNumber = 0;
            LevelMoves = 0;
        }
        else
        {
            if(LevelNumber > LevelCount)
                LevelNumber = LevelCount;
            LoadLevel();
        }
    }

}

void CWorldParts::SaveLevel()
{
    int x,y;
    CWorldPart *Part;
    if(LevelNumber > 0)
    {
        LevelPackData[((LevelNumber-1)*((NrOfCols*NrOfRows)+1))]=LevelMoves;
        for (y=0;y<NrOfRows;y++)
            for (x=0;x<NrOfCols;x++)
            {
                Part= GetItemFromPosPlayField(x,y);
                if(Part==NULL)
                    LevelPackData[((LevelNumber-1)*((NrOfCols*NrOfRows)+1))+(y*NrOfCols)+x+1] = 0;
                else
                    LevelPackData[((LevelNumber-1)*((NrOfCols*NrOfRows)+1))+(y*NrOfCols)+x+1] = Part->GetType();
            }
    }
}

LCDBitmap*  CWorldParts::GetAssignedImage()
{
    return TilesImage;
}

bool CWorldParts::LoadLevel()
{
    int x,y,type;
    if ((LevelNumber <= LevelCount) && (LevelNumber > 0))
    {
        RemoveAll();
        DisableSorting = true;
        LevelMoves = LevelPackData[((LevelNumber-1)*((NrOfCols*NrOfRows)+1))];
        for(y=0;y<NrOfRows;y++)
            for(x=0;x<NrOfCols;x++)
            {
                type = LevelPackData[((LevelNumber-1)*((NrOfCols*NrOfRows)+1))+(y*NrOfCols)+x+1];
                if(type !=0)
                    Add( new CBox(x,y,TilesImage,type));
            }
        DisableSorting = false;
        return true;
    }
    return false;
}

void CWorldParts::Move()
{
	int Teller;
	for (Teller=0;Teller<ItemCount;Teller++)
		Items[Teller]->Move();
}

void CWorldParts::MoveDown()
{
	int Teller;
	for (Teller=0;Teller<ItemCount;Teller++)
		Items[Teller]->MoveDown();
}

void CWorldParts::MoveLeft()
{
	int Teller;
	for (Teller=0;Teller<ItemCount;Teller++)
		Items[Teller]->MoveLeft();
}

void CWorldParts::MoveRight()
{
	int Teller;
	for (Teller=0;Teller<ItemCount;Teller++)
		Items[Teller]->MoveRight();
}

void CWorldParts::MoveUp()
{
	int Teller;
	for (Teller=0;Teller<ItemCount;Teller++)
		Items[Teller]->MoveUp();
}

void CWorldParts::Draw(LCDBitmap *Surface)
{
	int Teller;
	for (Teller=0;Teller<ItemCount;Teller++)
	{
	    if((Items[Teller]->GetPlayFieldX() >= 0) && (Items[Teller]->GetPlayFieldX() <= NrOfCols) &&
           (Items[Teller]->GetPlayFieldY() >= 0) && (Items[Teller]->GetPlayFieldY() <= NrOfRows))
           {
                Items[Teller]->Draw(Surface);
           }
	}
}

CWorldPart* CWorldParts::GetItemFromPosPlayField(int PlayFieldXin,int PlayFieldYin)
{
    int Teller;
	for (Teller=0;Teller<ItemCount;Teller++)
	{
	    if((Items[Teller]->GetPlayFieldX() == PlayFieldXin) && (Items[Teller]->GetPlayFieldY() == PlayFieldYin) )
            return Items[Teller];
	}
	return NULL;
}


CWorldParts::~CWorldParts()
{
	int Teller;
	for (Teller=0;Teller<ItemCount;Teller++)
	{
		delete Items[Teller];
		Items[Teller] = NULL;
	}
}


