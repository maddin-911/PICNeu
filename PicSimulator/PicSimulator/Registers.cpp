#include "stdafx.h"
#include "Registers.h"


Registers::Registers()
{
}


Registers::~Registers()
{
}

unsigned char Registers::getW()
{
	return this->w;
}

void Registers::setW(unsigned char w)
{
	this->w = w;
}

void Registers::addW(unsigned char v)
{
	this->w += v;
}
