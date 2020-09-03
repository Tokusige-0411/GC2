#pragma once
#include <vector>
#include "BaseScene.h"
#include "../Vector2.h"
#include"../input/Controller.h"
class ResultScene :
	public BaseScene
{
public:
	ResultScene(unique_Base scene, bool draw, bool update);
	~ResultScene();
	unique_Base Update(unique_Base own) override;
	void Draw(void) override;

private:
	bool Init(void);								// ‰Šú‰»

	unique_Base childScene_;						// ”wŒi
	bool draw_;										// childScene‚ğ•`‰æ‚·‚é‚©
	bool update_;									// childScene‚ğXV‚·‚é‚©
	int black_;										// ”wŒi
	int resultMenu_;								// ÒÆ­°‰æ–Ê
	int menuCursor_;								// ÒÆ­°¶°¿ÙÊİÄŞÙ
	int menuCount_;									// ‘I‘ğˆÊ’u
	std::vector<Vector2> cursorPos_;				// ÒÆ­°¶°¿Ù‚ÌˆÚ“®’n“_
	std::unique_ptr<Controller> contoroller_;		// InputŠÖŒW;
	std::vector<std::unique_ptr<Controller>> contVec_;
};
