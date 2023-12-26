#include <pd_api.h>
#include "sdl_helpertypes.h"
#include "pd_helperfuncs.h"
#include "main.h"
#include "leveleditor.h"
#include "gamefuncs.h"
#include "cbox.h"
#include "cselector.h"
#include "cworldpart.h"

int editorSelectionX = 0, editorSelectionY = 0;
bool editing = false;

CSelector* Selector = new CSelector(0, 0, Xoffset, Yoffset);

void LevelEditorInit()
{
	editorSelectionX = 0;
	editorSelectionY = 0;
	editing = false;
	SelectedBlock=0;
	Selector->SetPosition(NrOfCols >> 1, NrOfRows >> 1);
	pd->graphics->pushContext(Selector->GetBitmap());
	pd->graphics->clear(kColorClear);
	DrawBitmapSrcRec(IMGBlocks, 0, 0, 0, SelectedBlock*TileHeight, TileWidth, TileHeight, kBitmapUnflipped);
	pd->graphics->popContext();
}

void LevelEditor()
{
	if(GameState == GSLevelEditorInit)
	{
		LevelEditorInit();
		GameState = GSLevelEditor;
	}

    if (GameState == GSLevelEditor)
    {
		if (editing)
		{
			if(Input->KeyboardPushed[SDLK_b])
				editing = false;
			
			if(Input->KeyboardHeld[SDLK_PAGEDOWN])
			{
				SelectedBlock--;
				if(SelectedBlock < 0)
					SelectedBlock = 5;
				pd->graphics->pushContext(Selector->GetBitmap());
				pd->graphics->clear(kColorClear);
				DrawBitmapSrcRec(IMGBlocks, 0, 0, 0, SelectedBlock*TileHeight, TileWidth, TileHeight, kBitmapUnflipped);
				pd->graphics->popContext();
			}

			if(Input->KeyboardHeld[SDLK_PAGEUP])
			{
				SelectedBlock++;
				if(SelectedBlock > 5)
					SelectedBlock = 0;
				pd->graphics->pushContext(Selector->GetBitmap());
				pd->graphics->clear(kColorClear);
				DrawBitmapSrcRec(IMGBlocks, 0, 0, 0, SelectedBlock*TileHeight, TileWidth, TileHeight, kBitmapUnflipped);
				pd->graphics->popContext();
			}

			if (Input->KeyboardPushed[SDLK_DOWN])
			{
				SPoint pos = Selector->GetPosition();

				pos.Y++;
				if(pos.Y >= NrOfRows)
					pos.Y = 0;
				Selector->SetPosition(pos.X, pos.Y);
			}

			if (Input->KeyboardPushed[SDLK_UP])
			{
				SPoint pos = Selector->GetPosition();

				pos.Y--;
				if(pos.Y < 0)
					pos.Y = NrOfRows -1;
				Selector->SetPosition(pos.X, pos.Y);
			}

			if (Input->KeyboardPushed[SDLK_LEFT])
			{
				SPoint pos = Selector->GetPosition();

				pos.X--;
				if(pos.X < 0)
					pos.X = NrOfCols -1;
				Selector->SetPosition(pos.X, pos.Y);
			}

			if (Input->KeyboardPushed[SDLK_RIGHT])
			{
				SPoint pos = Selector->GetPosition();

				pos.X++;
				if(pos.X >= NrOfCols)
					pos.X = 0;
				Selector->SetPosition(pos.X, pos.Y);
			}

			if(Input->KeyboardPushed[SDLK_a])
			{
				
				CAudio_PlaySound(Sounds[SND_LevelEditorSelect],0);
				WorldParts->Remove(Selector->GetPosition().X,Selector->GetPosition().Y);
				if(SelectedBlock < 5)
					WorldParts->Add(new CBox(Selector->GetPosition().X,Selector->GetPosition().Y,WorldParts->GetAssignedImage(),SelectedBlock+1));
            }

		}
		else
		{
			if (Input->KeyboardPushed[SDLK_b])
			{
				CAudio_PlaySound(Sounds[SND_Select],0);
				PreviousGameState = GameState;
				GameState = GSTitleScreenInit;
			}

			if (Input->KeyboardPushed[SDLK_DOWN])
			{
				editorSelectionY++;
				if(editorSelectionY > 6)
					editorSelectionY = 0;
			}

			if (Input->KeyboardPushed[SDLK_UP])
			{
				editorSelectionY--;
				if(editorSelectionY < 0)
					editorSelectionY = 6;
			}

			if (Input->KeyboardPushed[SDLK_LEFT])
			{
				editorSelectionX--;
				if(editorSelectionX < 0)
					editorSelectionX = 1;
			}

			if (Input->KeyboardPushed[SDLK_RIGHT])
			{
				editorSelectionX++;
				if(editorSelectionX > 1)
					editorSelectionX = 0;
			}
		
			if(Input->KeyboardPushed[SDLK_a])
			{
				//edit
				if((editorSelectionX == 0) && (editorSelectionY == 0))
				{
					if(!(strcmp(InstalledLevelPacks[SelectedLevelPack],"levels.dat")==0))
					{
						if(WorldParts->GetLevel() > 0)
						{
							CAudio_PlaySound(Sounds[SND_Select],0);
							editing = true;
						}
						else
							CAudio_PlaySound(Sounds[SND_Error],0);
					}
					else
						CAudio_PlaySound(Sounds[SND_Error],0);
				}

				//play level
				if((editorSelectionX == 1) && (editorSelectionY == 0))
				{
					if(!(strcmp(InstalledLevelPacks[SelectedLevelPack],"levels.dat")==0))
					{
						if(WorldParts->LevelCount > 0)
						{
							if(CanPlay(WorldParts))
							{
								CAudio_PlaySound(Sounds[SND_Select],0);
								WorldParts->SaveLevel();
								PreviousGameState = GameState;
								GameState = GSGameInit;
							}
							else
								CAudio_PlaySound(Sounds[SND_Error],0);
						}
						else
							CAudio_PlaySound(Sounds[SND_Error],0);
					}
					else
						CAudio_PlaySound(Sounds[SND_Error],0);
				}
																	
				//next Level
				if((editorSelectionX == 0) && (editorSelectionY == 1))
				{
					if(WorldParts->GetLevel() < WorldParts->LevelCount)
						CAudio_PlaySound(Sounds[SND_Select],0);
					else
						CAudio_PlaySound(Sounds[SND_Error],0);
					if(!(strcmp(InstalledLevelPacks[SelectedLevelPack],"levels.dat")==0))
					{
						WorldParts->SaveLevel();
					}
					WorldParts->IncLevel();
				}
				
				//prev level
				if((editorSelectionX == 1) && (editorSelectionY == 1))
				{
					if(WorldParts->GetLevel() > 1)
						CAudio_PlaySound(Sounds[SND_Select],0);
					else
						CAudio_PlaySound(Sounds[SND_Error],0);
					if(!(strcmp(InstalledLevelPacks[SelectedLevelPack],"levels.dat")==0))
					{
						WorldParts->SaveLevel();
					}
					WorldParts->DecLevel();
				}
			
				//Add level
				if((editorSelectionX == 0) && (editorSelectionY == 2))
				{
					if(!(strcmp(InstalledLevelPacks[SelectedLevelPack],"levels.dat")==0))
					{
						if (WorldParts->LevelCount + 1 < MaxLevels)
						{
							CAudio_PlaySound(Sounds[SND_Select],0);
							WorldParts->SaveLevel();
							WorldParts->AddLevel();
						}
						else
							CAudio_PlaySound(Sounds[SND_Error],0);
					}
					else
						CAudio_PlaySound(Sounds[SND_Error],0);
				}

				//Del level
				if((editorSelectionX == 1) && (editorSelectionY == 2))
				{
					if(!(strcmp(InstalledLevelPacks[SelectedLevelPack],"levels.dat")==0))
					{
						if(WorldParts->GetLevel() > 0)
						{
							CAudio_PlaySound(Sounds[SND_Select],0);
							WorldParts->DelLevel();
						}
						else
							CAudio_PlaySound(Sounds[SND_Error],0);
					}
					else
						CAudio_PlaySound(Sounds[SND_Error],0);
				}
				
				//inc Moves
				if((editorSelectionX == 0) && (editorSelectionY == 3))
				{
					if(!(strcmp(InstalledLevelPacks[SelectedLevelPack],"levels.dat")==0))
					{
						if(WorldParts->GetLevel() > 0)
						{
							WorldParts->LevelMoves++;
							if(WorldParts->LevelMoves > 100)
							{
								CAudio_PlaySound(Sounds[SND_Error],0);
								WorldParts->LevelMoves = 100;
							}
							else
								CAudio_PlaySound(Sounds[SND_Select],0);
						}
						else
							CAudio_PlaySound(Sounds[SND_Error],0);
					}
					else
						CAudio_PlaySound(Sounds[SND_Error],0);
				}
				
				//Dec Moves
				if((editorSelectionX == 1) && (editorSelectionY == 3))
				{
					if(!(strcmp(InstalledLevelPacks[SelectedLevelPack],"levels.dat")==0))
					{
						if(WorldParts->GetLevel() > 0)
						{
							WorldParts->LevelMoves--;
							if(WorldParts->LevelMoves < 1)
							{
								CAudio_PlaySound(Sounds[SND_Error],0);
								WorldParts->LevelMoves = 1;
							}
							else
								CAudio_PlaySound(Sounds[SND_Select],0);
						}
						else
							CAudio_PlaySound(Sounds[SND_Error],0);
					}
					else
						CAudio_PlaySound(Sounds[SND_Error],0);
				}

				//Save Pack
				if((editorSelectionX == 0) && (editorSelectionY == 4))
				{
					if(!(strcmp(InstalledLevelPacks[SelectedLevelPack],"levels.dat")==0))
					{
						CAudio_PlaySound(Sounds[SND_Select],0);
						WorldParts->SaveLevel();
						WorldParts->SaveLevelPack(InstalledLevelPacks[SelectedLevelPack]);
					}
					else
						CAudio_PlaySound(Sounds[SND_Error],0);
				}
				
				//New Pack
				if((editorSelectionX == 1) && (editorSelectionY == 4))
				{
					if(InstalledLevelPacksCount+1<MaxLevelPacks)
					{
						CAudio_PlaySound(Sounds[SND_Select],0);
						WorldParts->NewLevelPack();
						int Teller=1;
						char *FileName;
						pd->file->mkdir("levels");
						pd->system->formatString(&FileName,"levels/my_levels%d.dat",Teller);
						while(FileExists (FileName))
						{
							pd->system->realloc(FileName, 0);
							Teller++;
							pd->system->formatString(&FileName,"levels/my_levels%d.dat",Teller);
						}
						pd->system->realloc(FileName, 0);
						pd->system->formatString(&FileName,"my_levels%d.dat",Teller);
						WorldParts->SaveLevelPack(FileName);						
						SearchForLevelPacks();
						for(Teller = 0;Teller<InstalledLevelPacksCount;Teller++)
							if(strcmp(FileName,InstalledLevelPacks[Teller]) == 0)
							{
								SelectedLevelPack = Teller;
								break;
							}
						pd->system->realloc(FileName, 0);
					}
					else
						CAudio_PlaySound(Sounds[SND_Error],0);

				}
			
				//Del Pack
				if((editorSelectionX == 0) && (editorSelectionY == 5))
				{
					if(!(strcmp(InstalledLevelPacks[SelectedLevelPack],"levels.dat")==0))
					{
						CAudio_PlaySound(Sounds[SND_Select],0);
						char *FileName;
						pd->file->mkdir("levels");
						pd->system->formatString(&FileName,"levels/%s",InstalledLevelPacks[SelectedLevelPack]);
						pd->file->unlink(FileName, false);
						SearchForLevelPacks();
						WorldParts->LoadLevelPack(InstalledLevelPacks[SelectedLevelPack]);
						WorldParts->SetLevel(1);
					}
					else
						CAudio_PlaySound(Sounds[SND_Error],0);
				}

				//Clear Level
				if((editorSelectionX == 1) && (editorSelectionY == 5))
				{
					if(!(strcmp(InstalledLevelPacks[SelectedLevelPack],"levels.dat")==0))
					{
						if(WorldParts->GetLevel() > 0)
						{
							CAudio_PlaySound(Sounds[SND_Select],0);
							WorldParts->RemoveAll();
						}
						else
							CAudio_PlaySound(Sounds[SND_Error],0);
					}
					else
						CAudio_PlaySound(Sounds[SND_Error],0);
				}

				//Grid
				if((editorSelectionX == 0) && (editorSelectionY == 6))
				{
					CAudio_PlaySound(Sounds[SND_Select],0);
					Grid++;
					if (Grid > 2)
						Grid = 0;
					SaveSettings();
				}
				
				//Next Pack
				if((editorSelectionX == 1) && (editorSelectionY == 6))
				{
					CAudio_PlaySound(Sounds[SND_Select],0);
					SelectedLevelPack++;
					if(SelectedLevelPack >= InstalledLevelPacksCount)
						SelectedLevelPack = 0;
					WorldParts->LoadLevelPack(InstalledLevelPacks[SelectedLevelPack]);
					WorldParts->SetLevel(1);
				}
			}
		}
		pd->graphics->drawBitmap(IMGBackgroundLevelEditor, 0, 0, kBitmapUnflipped);
		size_t len = strlen(InstalledLevelPacks[SelectedLevelPack]) -4;
		char *Tmp = (char *)pd->system->realloc(NULL, (len +1) * sizeof(char));
		memset(Tmp, 0, (len +1)*sizeof(char));
		strncpy(Tmp, InstalledLevelPacks[SelectedLevelPack], len);
		char *Text;	
		pd->system->formatString(&Text, "<%s>", Tmp);
		drawTextColor(true, NULL, font, Text , strlen(Text), kASCIIEncoding, 16, 50, kColorBlack, false);
		pd->system->realloc(Text, 0);
		pd->system->realloc(Tmp, 0);

		pd->system->formatString(&Text,"\nLevel Number: %d/%d\nMoves: %d\nGrid: %d\n", WorldParts->GetLevel(),WorldParts->LevelCount,WorldParts->LevelMoves,Grid);
        drawTextColor(true, NULL, font, Text, strlen(Text), kASCIIEncoding, 16, 50, kColorBlack, false);
		pd->system->realloc(Text, 0);
        
		const int textLeftX = 12;
		const int textRightX = 77;
		LCDColor Color;
		
		if ((editorSelectionY == 0) && (editorSelectionX == 0))
			Color = kColorBlack;
		else
			Color = (LCDColor) kColorGrey;
		drawTextColor(true, NULL, font, "Edit", 5, kASCIIEncoding, textLeftX, 140, Color, false);
		
		if ((editorSelectionY == 0) && (editorSelectionX == 1))
			Color = kColorBlack;
		else
			Color = (LCDColor) kColorGrey;
		drawTextColor(true, NULL, font, "Play", 5, kASCIIEncoding, textRightX, 140, Color, false);
		
		if ((editorSelectionY == 1) && (editorSelectionX == 0))
			Color = kColorBlack;
		else
			Color = (LCDColor) kColorGrey;
		drawTextColor(true, NULL, font, "Next Level", 10, kASCIIEncoding, textLeftX, 152, Color, false);
		
		if ((editorSelectionY == 1) && (editorSelectionX == 1))
			Color = kColorBlack;
		else
			Color = (LCDColor) kColorGrey;
		drawTextColor(true, NULL, font, "Prev Level", 10, kASCIIEncoding, textRightX, 152, Color, false);
		
		if ((editorSelectionY == 2) && (editorSelectionX == 0))
			Color = kColorBlack;
		else
			Color = (LCDColor) kColorGrey;
		drawTextColor(true, NULL, font, "Add Level", 9, kASCIIEncoding, textLeftX, 164, Color, false);
		
		if ((editorSelectionY == 2) && (editorSelectionX == 1))
			Color = kColorBlack;
		else
			Color = (LCDColor) kColorGrey;
		drawTextColor(true, NULL, font, "Del Level", 9, kASCIIEncoding, textRightX, 164, Color, false);
		
		if ((editorSelectionY == 3) && (editorSelectionX == 0))
			Color = kColorBlack;
		else
			Color = (LCDColor) kColorGrey;
		drawTextColor(true, NULL, font, "Inc Moves", 9, kASCIIEncoding, textLeftX, 176, Color, false);
		
		if ((editorSelectionY == 3) && (editorSelectionX == 1))
			Color = kColorBlack;
		else
			Color = (LCDColor) kColorGrey;
		drawTextColor(true, NULL, font, "Dec Moves", 9, kASCIIEncoding, textRightX, 176, Color, false);
		
		if ((editorSelectionY == 4) && (editorSelectionX == 0))
			Color = kColorBlack;
		else
			Color = (LCDColor) kColorGrey;
		drawTextColor(true, NULL, font, "Save Pack", 10, kASCIIEncoding, textLeftX, 188, Color, false);
		
		if ((editorSelectionY == 4) && (editorSelectionX == 1))
			Color = kColorBlack;
		else
			Color = (LCDColor) kColorGrey;
		drawTextColor(true, NULL, font, "New Pack", 9, kASCIIEncoding, textRightX, 188, Color, false);
		
		if ((editorSelectionY == 5) && (editorSelectionX == 0))
			Color = kColorBlack;
		else
			Color = (LCDColor) kColorGrey;
		drawTextColor(true, NULL, font, "Del Pack", 9, kASCIIEncoding, textLeftX, 200, Color, false);
		
		if ((editorSelectionY == 5) && (editorSelectionX == 1))
			Color = kColorBlack;
		else
			Color = (LCDColor) kColorGrey;
		drawTextColor(true, NULL, font, "Clear Level", 11, kASCIIEncoding, textRightX, 200, Color, false);
		
		if ((editorSelectionY == 6) && (editorSelectionX == 0))
			Color = kColorBlack;
		else
			Color = (LCDColor) kColorGrey;
		drawTextColor(true, NULL, font, "Grid", 4, kASCIIEncoding, textLeftX, 212, Color, false);
	
		if ((editorSelectionY == 6) && (editorSelectionX == 1))
			Color = kColorBlack;
		else
			Color = (LCDColor) kColorGrey;
		drawTextColor(true, NULL, font, "Next Pack", 9, kASCIIEncoding, textRightX, 212, Color, false);
		SDL_Rect Dst,Src;
        if(Grid == 2)
        {
            Dst.x = Xoffset;
            Dst.y = Yoffset;
            Dst.w = NrOfCols * TileWidth;
            Dst.h = NrOfRows * TileHeight;
            pd->graphics->fillRect(Dst.x, Dst.y, Dst.w, Dst.h, kColorWhite);
        }
        DrawFloor(WorldParts,NULL);
        WorldParts->Draw(NULL);
		if(editing)
			Selector->Draw(NULL);
        for(int Teller=0;Teller<6;Teller++)
        {
            Src.x = 0;
            Src.y = TileHeight*Teller;
            Src.w = TileWidth;
            Src.h = TileHeight;
            Dst.x = 16 +(TileWidth*Teller);
            Dst.y = 115;
            Dst.w = TileWidth;
            Dst.h = TileHeight;
            DrawBitmapSrcRec(IMGBlocks, Dst.x, Dst.y, Src.x, Src.y, Src.w, Src.h, kBitmapUnflipped);
        }
        pd->graphics->fillRect(16+(TileWidth*SelectedBlock),115-6, TileWidth, 4,kColorBlack);
        pd->graphics->fillRect(16+(TileWidth*SelectedBlock),115+TileHeight+2,TileWidth,4, kColorBlack);
        if(Grid >=1)
        {
            Dst.x = Xoffset;
            Dst.y = Yoffset;
            Dst.w = NrOfCols * TileWidth;
            Dst.h = NrOfRows * TileHeight;
            pd->graphics->drawBitmap(IMGGrid, Dst.x, Dst.y, kBitmapUnflipped);
        }
    }
}
