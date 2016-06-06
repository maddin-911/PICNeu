#pragma once
class Registers
{
public:
	Registers();
	~Registers();
	void setW(unsigned char w);
	void addW(unsigned char v);
	unsigned char getW();
private:
	unsigned char w;
};

