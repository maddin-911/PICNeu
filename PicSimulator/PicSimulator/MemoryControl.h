#pragma once

#include <mutex>

#define PROG_MEM_SIZE 1000
#define MEM_SIZE 128
#define STACK_SIZE 8
#define EEPROM_SIZE 64

class MemoryControl
{
public:
	MemoryControl();
	~MemoryControl();
	void setMem(unsigned char address, unsigned char value);
	void setMem(unsigned char address, unsigned char value, unsigned char bank);
	unsigned char getMem(unsigned char address);
	unsigned char getMem(unsigned char address, unsigned char bank);

	void setProg_mem(unsigned short address, unsigned short value);
	unsigned short getProg_mem(unsigned short address);

	void setEeprom(unsigned char address, unsigned char value);
	unsigned char getEeprom(unsigned char address);

	void setPLC(unsigned short plc);
	void setPLCunsafe(unsigned short plc);
	unsigned short getPLC();
	unsigned short getPLCunsafe();

	void setW(unsigned char w);
	unsigned char getW();

	void setStatus(int flag, int value);
	void setStatusUnsafe(int flag, int value);
	bool getStatus(int flag);

	void setOption(int flag, int value);
	bool getOption(int flag);
	bool getOptionUnsafe(int flag);

	void setIntcon(int flag, int value);
	bool getIntcon(int flag);

	void setLastA(unsigned char value);
	void setLastB(unsigned char value);

	bool getLastB(unsigned char n);
	bool getLastA(unsigned char n);

	void stackPush(unsigned short plc);
	unsigned short stackPop();
	unsigned short getStack(unsigned short address);

	void resetMemory(bool resetProgMem);

	void setTris(unsigned char n, bool b, bool value);
	bool getTris(unsigned char n, bool b);

	void setPort(unsigned char n, bool b, bool value);
	bool getPort(unsigned char n, bool b);

	void setSleep(bool value);
	bool getSleep(); //tät mir auch mal nich schlecht

	void setWatchdog(bool value);
	bool getWatchdog();

	void setWatchdogTimer();
	unsigned short getWatchdogTimer();

	unsigned short getNibble(unsigned short value);

	unsigned short getPrescalerRate();

	unsigned short getTimerPrescaler();
	void setTimerPrescaler(unsigned short value);

	unsigned short getEEcon2();
	void setEEcon1(int flag, int value);
	bool getEEcon1(int flag);
	int getEepromState();
	void setEepromState(int value);

	static const int FLAG_CARRY = 0;
	static const int FLAG_DIGIT = 1;
	static const int FLAG_ZERO = 2;
	static const int FLAG_POWER = 3;
	static const int FLAG_TIME = 4;
	static const int FLAG_BANK = 5;

protected:
private:
	std::mutex mutex;

	unsigned char mem[MEM_SIZE];
	unsigned char w;
	unsigned char OPTION_REG;
	unsigned char PORTA;
	unsigned char PORTB;
	unsigned char TRISA;
	unsigned char TRISB;
	unsigned char EECON1;
	unsigned char EECON2;
	unsigned short prog_mem[PROG_MEM_SIZE];
	unsigned short stack[STACK_SIZE];
	unsigned char eeprom[EEPROM_SIZE];
	unsigned char LASTA; //Variable, die den letzten Status von PORTA speichert, um Flankenwechsel zu erkennen
	unsigned char LASTB;
	bool SLEEP;

	int EepromState;

	unsigned short TimerPrescaler;

	bool WatchdogActive;
	long WatchdogTimer;
};

