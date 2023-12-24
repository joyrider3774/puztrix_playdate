#ifndef CBOX_H
#define CBOX_H

#include "cworldpart.h"


class CBox : public CWorldPart
{
    private:
        int loops;
        int AnimDelay;
    public:
        bool Death,Remove;
        CBox(const int PlayFieldXin,const int PlayFieldYin,LCDBitmap *IMG,int ID);
        virtual bool CanMoveTo(const int PlayFieldXin,const int PlayFieldYin);
        virtual void Event_Moving(int ScreenPosX,int ScreenPosY,int ScreenXi, int ScreenYi);
        virtual void Event_ArrivedOnNewSpot();
        virtual void Event_BeforeDraw();
        virtual void Draw(LCDBitmap* Surface);
};


#endif
