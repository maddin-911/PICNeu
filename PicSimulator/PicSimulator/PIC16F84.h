#include "MemoryControl.h"
#include "Registers.h"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <thread>

#pragma once
class PIC16F84
{
private:
	std::thread picThread;
	int cycleCount = 0;
public:
	MemoryControl memoryControl;
	PIC16F84();
	~PIC16F84();
	bool checkInterrupt();
	int execInstruction();
	void WDT();
	void execCycle();
	void start();
	void parseLST(CString path);
	void reset(bool resetProgMem);

	static const int EEDATA = 8;
	static const int EEADDR = 9;
};

