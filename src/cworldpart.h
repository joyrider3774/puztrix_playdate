#ifndef CWORLDPART_H
#define CWORLDPART_H

#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>

#include "cworldparts.h"

class CWorldParts;

class CWorldPart
{
 protected:
    int MoveDelayCounter;
    bool FirstArriveEventFired;
 	int Type,MoveSpeedX,MoveSpeedY,MoveDelay,Xi,Yi,X,Y,PreviousPlayFieldX,PreviousPlayFieldY,AnimPhase,PlayFieldX,PlayFieldY,Z,Group;
 	bool BHistory;
 	SDL_Surface *Image;

 public:
    CWorldParts *ParentList;
 	bool IsMoving;
 	bool Selected;

 	CWorldPart(const int PlayFieldXin,const int PlayFieldYin);
 	void SetMovesSpeed(int aMoveSpeedX,int aMoveSpeedY);
 	int GetGroup();
 	int GetType();
 	int GetX();
 	int GetY();
 	int GetMoveSpeedX();
 	int GetMoveSpeedY();
 	int GetPlayFieldX();
 	int GetPlayFieldY();
 	int GetZ();
 	int GetAnimPhase();
 	void MoveDown();
 	void MoveUp();
 	void MoveLeft();
 	void MoveRight();
 	void AssignImage(SDL_Surface *Surface);
 	void SetAnimPhase(int AnimPhaseIn);
 	virtual void Event_ArrivedOnNewSpot();
 	virtual void Event_BeforeDraw();
 	virtual void Event_LeaveCurrentSpot();
 	virtual void Event_Moving(int ScreenPosX,int ScreenPosY,int ScreenXi, int ScreenYi);
 	void SetPosition(const int PlayFieldXin,const int PlayFieldYin);
 	virtual void MoveTo(const int PlayFieldXin,const int PlayFieldYin,bool BackWards);
 	virtual bool CanMoveTo(const int PlayFieldXin,const int PlayFieldYin);
 	virtual void Move();
 	virtual void Draw(SDL_Surface* Surface);
 	virtual ~CWorldPart();
};

#endif
