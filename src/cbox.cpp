#include "cbox.h"

CBox::CBox(const int PlayFieldXin,const int PlayFieldYin,SDL_Surface *IMG,int ID) : CWorldPart(PlayFieldXin,PlayFieldYin)
{
    Image = IMG;
    Type = ID;
    MoveSpeedY = 4;
    MoveSpeedX = 4;
    MoveDelay = 0;
    AnimDelay = 0;
    AnimPhase = 0;
    loops = 0;
    Death = false;
    Remove = false;
}


bool CBox::CanMoveTo(const int PlayFieldXin,const int PlayFieldYin)
{
    CWorldPart *Box;
    int DiffX,DiffY;



    if ((PlayFieldXin < 0) || (PlayFieldXin >= NrOfCols) || (PlayFieldYin < 0) || (PlayFieldYin >= NrOfRows))
        return false;

    if (Type == 5)
        return false;

    DiffX = PlayFieldX - PlayFieldXin;
    DiffY = PlayFieldY - PlayFieldYin;



    Box = ParentList->GetItemFromPosPlayField(PlayFieldXin,PlayFieldYin);
    if(Box == NULL)
        return true;
    else
        return Box->CanMoveTo(Box->GetPlayFieldX()-DiffX,Box->GetPlayFieldY()-DiffY);
}


void CBox::Draw(SDL_Surface* Surface)
{

    if(Death)
    {
        AnimDelay++;
        if(AnimDelay == 4)
        {
            AnimDelay = 0;
            AnimPhase++;
            if (AnimPhase == 2)
            {
                AnimPhase =0;
                loops++;
                if(loops == 5)
                {
                    AnimPhase = 1;
                    Remove = true;
                }
            }
        }
    }
    if (Image )
    {
        SDL_Rect SrcRect,DstRect;
        SrcRect.x = AnimPhase * TileWidth;
        SrcRect.y = (Type-1) * TileHeight;
        SrcRect.w = TileWidth;
        SrcRect.h = TileHeight;
        {
            DstRect.x = X;
            DstRect.y = Y;
        }
        DstRect.w = TileWidth;
        DstRect.h = TileHeight;
        SDL_BlitSurface(Image,&SrcRect,Surface,&DstRect);
    }
}


void CBox::Event_Moving(int ScreenPosX,int ScreenPosY,int ScreenXi, int ScreenYi)
{

}

void CBox::Event_BeforeDraw()
{

}

void  CBox::Event_ArrivedOnNewSpot()
{
    this->MoveTo(PlayFieldX +(PlayFieldX-PreviousPlayFieldX),PlayFieldY+ (PlayFieldY-PreviousPlayFieldY),false);
}

