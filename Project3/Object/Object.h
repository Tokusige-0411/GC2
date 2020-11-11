#pragma once
#include <Vector2.h>

class Object
{
public:
	Object();
	virtual ~Object();
	virtual void Update(void);
	virtual void Draw(void);

private:
	virtual void Init(void);

protected:
	Vector2 pos_;
	float dir_;
};

