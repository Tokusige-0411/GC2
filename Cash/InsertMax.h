#pragma once
#include"TicketMachine.h"

struct InsertMax
{
	bool operator()(PayType& payType, MapInt& payData, int cash)
	{
		// ���ގx�����ɍs�����A�����x�����ɍs����
		// payType�̕ύX�H
		return true;
	}
};