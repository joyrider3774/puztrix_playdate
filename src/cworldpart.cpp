#include "cworldpart.h"

void CWorldPart::MoveDown()
{
    if(CanMoveTo(PlayFieldX,PlayFieldY+1))
        MoveTo(PlayFieldX,PlayFieldY+1,false);
}

void CWorldPart::MoveUp()
{
    if(CanMoveTo(PlayFieldX,PlayFieldY-1))
        MoveTo(PlayFieldX,PlayFieldY-1,false);
}


void CWorldPart::MoveLeft()
{
    if(CanMoveTo(PlayFieldX-1,PlayFieldY))
        MoveTo(PlayFieldX-1,PlayFieldY,false);
}


void CWorldPart::MoveRight()
{
    if(CanMoveTo(PlayFieldX+1,PlayFieldY))
        MoveTo(PlayFieldX+1,PlayFieldY,false);
}

void CWorldPart::AssignImage(SDL_Surface *Surface)
{
    Image = Surface;
}

int CWorldPart::GetMoveSpeedX()
{
    return MoveSpeedX;
}

int CWorldPart::GetMoveSpeedY()
{
    return MoveSpeedY;
}

int CWorldPart::GetGroup()
{
    return Group;
}

int CWorldPart::GetType()
{
    return Type;
}

int CWorldPart::GetX()
{
    return X;
}

int CWorldPart::GetY()
{
    return Y;
}

int CWorldPart::GetPlayFieldX()
{
    return PlayFieldX;
}

int CWorldPart::GetPlayFieldY()
{
    return PlayFieldY;
}

int CWorldPart::GetZ()
{
    return Z;
}

int CWorldPart::GetAnimPhase()
{
    return AnimPhase;
}

void CWorldPart::SetAnimPhase(int AnimPhaseIn)
{
    AnimPhase = AnimPhaseIn;
}

void CWorldPart::Event_ArrivedOnNewSpot()
{
}

void CWorldPart::Event_BeforeDraw()
{
}

void CWorldPart::Event_LeaveCurrentSpot()
{
}

void CWorldPart::Event_Moving(int ScreenPosX,int ScreenPosY,int ScreenXi, int ScreenYi)
{
}

void CWorldPart::SetPosition(const int PlayFieldXin,const int PlayFieldYin)
{
    if ((PlayFieldXin >= 0) && (PlayFieldXin < NrOfCols) && (PlayFieldYin >= 0) && (PlayFieldYin < NrOfRows))
    {
        PlayFieldX=PlayFieldXin;
        PlayFieldY=PlayFieldYin;
        PreviousPlayFieldX = PlayFieldXin;
        PreviousPlayFieldY = PlayFieldYin;
        X=Xoffset+(PlayFieldXin*TileWidth);
        Y=Yoffset+(PlayFieldYin*TileHeight);
        //Event_ArrivedOnNewSpot();
    }
}

void CWorldPart::MoveTo(const int PlayFieldXin,const int PlayFieldYin,bool BackWards)
{

        if((PlayFieldXin != PlayFieldX) || (PlayFieldYin != PlayFieldY))
            if(this->CanMoveTo(PlayFieldXin,PlayFieldYin) || BackWards)
            {
                PreviousPlayFieldX = PlayFieldX;
                PreviousPlayFieldY = PlayFieldY;
                PlayFieldX = PlayFieldXin;
                PlayFieldY = PlayFieldYin;
                if(X < Xoffset+(PlayFieldX*TileWidth))
                    Xi = MoveSpeedX;
                if(X > Xoffset+(PlayFieldX*TileWidth))
                    Xi = -MoveSpeedX;
                if(Y > Yoffset+(PlayFieldY*TileHeight))
                    Yi = -MoveSpeedY;
                if(Y < Yoffset+(PlayFieldY*TileHeight))
                    Yi = MoveSpeedY;
                IsMoving = true;
                Event_LeaveCurrentSpot();
            }

}

bool CWorldPart::CanMoveTo(const int PlayFieldXin,const int PlayFieldYin)
{
    return false;
}

void CWorldPart::Move()
{
    if (IsMoving)
    {
        if (MoveDelayCounter == MoveDelay)
        {
            X += Xi;
            Y += Yi;
            Event_Moving(X,Y,Xi,Yi);
            if ((X == Xoffset+(PlayFieldX * TileWidth)) && (Y == Yoffset+(PlayFieldY * TileHeight)))
            {
                IsMoving = false;
                Xi = 0;
                Yi = 0;
                Event_ArrivedOnNewSpot();
            }
            MoveDelayCounter = -1;
        }
        MoveDelayCounter++;
    }
}

void CWorldPart::Draw(SDL_Surface* Surface)
{
    //printf("Start draw type:%d\n",Type);
    if((Type != 0))
    if (Image )
    {
        Event_BeforeDraw();
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
    if (Selected)
    {
        {
            boxRGBA(Surface,X,Y,X+TileWidth-1,Y+TileHeight-1,0,0,200,15);
            rectangleRGBA(Surface,X,Y,X+TileWidth-1,Y+TileHeight-1,0,0,255,50);
        }
    }
    //printf("End draw type:%d\n",Type);
}

CWorldPart::~CWorldPart()
{

}

void CWorldPart::SetMovesSpeed(int aMoveSpeedX ,int aMoveSpeedY)
{
    MoveSpeedX = aMoveSpeedX;
    MoveSpeedY = aMoveSpeedY;
}

CWorldPart::CWorldPart(const int PlayFieldXin,const int PlayFieldYin)
{
    PlayFieldX=PlayFieldXin;
    PlayFieldY=PlayFieldYin;
    Xi=0;
    Yi=0;
    X=Xoffset+(PlayFieldXin*TileWidth);
    Y=Yoffset+(PlayFieldYin*TileHeight);
    Type=0;
    MoveDelay=0;
    MoveDelayCounter=0;
    IsMoving = false;
    MoveSpeedX=0;
    MoveSpeedY=0;
    Image = NULL;
    ParentList = 0;
    AnimPhase=0;
    Selected = false;
    FirstArriveEventFired = false;
    Z=0;
    Group=0;
}
