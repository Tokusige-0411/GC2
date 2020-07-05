#pragma once
#include <memory>
#include <vector>
#include "Field.h"
#include "Vector2.h"

#define lpSceneMng SceneMng::GetInstance()

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
	int _frame;
};

