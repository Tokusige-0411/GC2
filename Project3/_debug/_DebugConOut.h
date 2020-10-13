#pragma once
#ifdef _DEBUG
#include <windows.h>
#include <memory>
#include <assert.h>
// ﾃﾞﾊﾞｯｸﾞﾒｯｾｰｼﾞ用定義

#define TRACE(fmt, ...) printf(fmt, __VA_ARGS__);

#define AST(){\
	CHAR ast_mes[256];\
	wsprintf(ast_mes,"%s %d行目\0",__FILE__,__LINE__);\
	MessageBox(0,ast_mes,"ｱｻｰﾄ表示", MB_OK);\
	}

class _DebugConOut
{
public:
	static _DebugConOut& GetInstance()
	{
		return *sInstance;
	}

private:
	struct _DebugConOutDeleter
	{
		void operator()(_DebugConOut * _debugConOut) const
		{
			delete _debugConOut;
		}
	};

	_DebugConOut();
	~_DebugConOut();

	FILE* _debugFp = nullptr;
	static std::unique_ptr<_DebugConOut, _DebugConOutDeleter> sInstance;
};

#else	//_DEBUG
#define TRACE(fmt, ...)
#endif	//_DEBUG