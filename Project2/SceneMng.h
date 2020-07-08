#pragma once
#include <memory>
#include <vector>
#include <map>
#include "Field.h"
#include "Vector2.h"
#include "INPUT_ID.h"

#define lpSceneMng SceneMng::GetInstance()

enum class PLAYER_NUM
{
	ONE,
	TWO,
	MAX,
};

class SceneMng
{
public:
	static SceneMng& GetInstance()
	{
		static SceneMng s_Instance;
		return s_Instance;
	}

	void Run();
	void Draw();

	int GetFrameCount();

private:
	bool SysInit();
	SceneMng();
	~SceneMng();

	const Vector2 _screenSize;

	std::vector<std::shared_ptr<Field>> _playerField;

	std::map<PLAYER_NUM, std::map<INPUT_ID, std::pair<int, int>>> _keyData;
	std::map<PLAYER_NUM, std::map<INPUT_ID, int>> _keyList;

	int _frame;
};

