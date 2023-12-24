#include <pd_api.h>
#include "pd_helperfuncs.h"
#include "cselector.h"
#include "main.h"


// constructor, parameters define the starting position
CSelector::CSelector(const int PlayFieldXin,const int PlayFieldYin, const int OffsetXin, const int OffsetYin)
{
	IMGSelect = NULL;
	CurrentPoint.X = PlayFieldXin; // set current position
	CurrentPoint.Y = PlayFieldYin;
	Offset.X = OffsetXin;
	Offset.Y = OffsetYin;
	Visible = true;
	DrawCount = 0;
}

// Will set the position only if it lays within the board boundary
void CSelector::Hide()
{
	Visible = false;
}

void CSelector::Show()
{
	Visible = true;
}

void CSelector::SetPosition(const int PlayFieldXin,const int PlayFieldYin)
{
	// check if the new position is inside the board, if so set the new position as the currentposition
	if ((PlayFieldYin>=0) && (PlayFieldYin < NrOfRows) && (PlayFieldXin >= 0) && (PlayFieldXin < NrOfCols))
	{
		CurrentPoint.X = PlayFieldXin;
		CurrentPoint.Y = PlayFieldYin;
	}
}

// return the current position
SPoint CSelector::GetPosition()
{
	return CurrentPoint;
}

// Draw the blue box on the current position, with the offsets in mind
void CSelector::Draw(LCDBitmap *Surface)
{
	if (Visible && IMGSelect)
	{
		DrawCount++;
		if(DrawCount > 10)
			DrawCount = 0;
		pd->graphics->pushContext(Surface);
		if(DrawCount > 5)
			pd->graphics->fillRect(Offset.X + CurrentPoint.X * (TileWidth), Offset.Y + CurrentPoint.Y * (TileHeight), TileWidth, TileHeight, kColorBlack);
		else
			pd->graphics->drawBitmap(IMGSelect, Offset.X + CurrentPoint.X * (TileWidth), Offset.Y + CurrentPoint.Y * (TileHeight), kBitmapUnflipped);
		pd->graphics->popContext();
	}
}

LCDBitmap* CSelector::GetBitmap()
{
	if(!IMGSelect)
		IMGSelect = pd->graphics->newBitmap(TileWidth, TileHeight, kColorClear);
	return IMGSelect;
}

CSelector::~CSelector()
{
	pd->graphics->freeBitmap(IMGSelect);
}
