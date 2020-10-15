#pragma once
#include<array>
#include<map>
#include"INPUT_ID.h"

enum class ContType
{
	KeyBoard,
	Mouse,
	Pad,
	Max
};

static ContType begin(ContType) { return ContType::KeyBoard; }
static ContType end(ContType) { return ContType::Max; }
static ContType operator*(ContType id) { return id; }
static ContType operator++(ContType& id) { return id = static_cast<ContType>(std::underlying_type<ContType>::type(id) + 1); }

enum class Trg
{
	Now,
	Old,
	Max
};

using TrgBool = std::array<bool, static_cast<size_t>(Trg::Max)>;
using ContData = std::map<INPUT_ID, TrgBool>;

struct Controller
{
	void operator()()
	{
		Update();
	}

	const ContData& GetContData(){ return _data;}
	virtual ContType GetType() = 0;
	virtual bool SetUp(int no) = 0;

private:
	virtual void Update(void) = 0;

protected:
	ContData _data;
};

