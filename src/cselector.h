#ifndef CSELECTOR_H
#define CSELECTOR_H

#include <pd_api.h>
#include "main.h"

class CSelector
{
    private:
        SPoint CurrentPoint; //variables to hold the current position and the selected position
		SPoint Offset;
		LCDBitmap *IMGSelect;
		int DrawCount;
    public:
        bool Visible;
        CSelector(const int PlayFieldXin,const int PlayFieldYin, const int OffsetXin, const int OffsetYin);
        void Hide();
        void Show();
        void SetPosition(const int PlayFieldXin,const int PlayFieldYin);
        SPoint GetPosition();
		LCDBitmap* GetBitmap();
        void Draw(LCDBitmap *Surface);
        ~CSelector();
};
#endif