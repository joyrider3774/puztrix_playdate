#ifndef CWORLDPARTS_H
#define CWORLDPARTS_H

#include <pd_api.h>
#include "cworldpart.h"

const int NrOfRows = 12,
		  NrOfCols = 10,
		  IDBox1 = 1,
		  IDBox2 = 2,
		  IDBox3 = 3,
          IDBox4 = 4,
          TileWidth = 20,
          TileHeight = 20,
          MaxLevels = 100,
		  Xoffset = 175,
          Yoffset = 0;

class CWorldPart;

class CWorldParts
{
 private:
 	void Sort();
 	bool DisableSorting;
 	LCDBitmap *TilesImage;
 	int LevelNumber;
 	char LevelPackData[((NrOfRows*NrOfCols)+1)*MaxLevels];
 public:
    void AssignImage(LCDBitmap *Image);
    LCDBitmap* GetAssignedImage();
 	CWorldPart *Items[NrOfRows*NrOfCols];
 	int ItemCount;
 	int LevelCount;
 	int LevelMoves;
 	CWorldParts();
 	int GetLevel();
 	void AddLevel();
 	void DelLevel();
 	void SetLevel(int aLevelNumber);
 	void IncLevel();
 	void DecLevel();
 	void NewLevelPack();
 	void Add(CWorldPart *WorldPart);
 	void Move();
 	void HistoryAdd();
 	void HistoryGoBack();
 	void Draw(LCDBitmap *Surface);
 	void Remove(int PlayFieldXin,int PlayFieldYin);
 	void Remove(int PlayFieldXin,int PlayFieldYin,int Type);
 	void RemoveAll();
	void SaveLevel();
	void SaveLevelPack(char* LevelPackName);
	bool LoadLevel();
	bool LoadLevelPack(char* LevelPackName);
	bool IsMoving();
	void MoveDown();
	void MoveUp();
	void MoveLeft();
	void MoveRight();
	void SortY();
	bool CanMoveToLeft();
	bool CanMoveToRight();
	bool CanMoveToUp();
	bool CanMoveToDown();
	CWorldPart* GetItemFromPosPlayField(int PlayFieldXin,int PlayFieldYin);
 	~CWorldParts();
};

#endif
