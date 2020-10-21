#pragma once
#include <memory>
#include <string>
#include <map>

#define lpTileLoader TileLoader::GetInstance()

enum class TileLayer
{
	Bg,
	Item,
	Obj,
	Char,
};

class TileLoader
{
public:
	static TileLoader& GetInstance()
	{
		return *s_Instance;
	}

private:
	struct TileLoderDeleter
	{
		void operator() (TileLoader* tileLoader) const
		{
			delete tileLoader;
		}
	};

	// ϯ�ߊ֘A
	int width_;					// ��Ͻ��
	int height_;				// �cϽ��
	int tileWidth_;				// ��ى���
	int tileHeight_;			// ��ُc��

	std::map<TileLayer, std::vector<int>> mapData_;

	bool Init(void);
	TileLoader();
	~TileLoader();
	static std::unique_ptr<TileLoader, TileLoderDeleter> s_Instance;
};

