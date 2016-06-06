#include "stdafx.h"
#include "MemoryControl.h"
#include "math.h"


MemoryControl::MemoryControl()
{
	resetMemory(true);
}


MemoryControl::~MemoryControl()
{
}

void MemoryControl::setMem(unsigned char address, unsigned char value) {
	std::unique_lock<std::mutex> lock(mutex);

	if(address == 0)
		address = mem[4];

	if (mem[3] & 32) //Bank 1
	{
		switch (address)
		{
		case 1:
			OPTION_REG = value;
			///setMem(address, value, 1);
			break;
		case 5:
			TRISA = value;
			break;
		case 6:
			TRISB = value;
			break;
		case 8:
			EECON1 = value;
			break;
		case 9:
			EECON2 = value;
			break;
		default:
			if (address < sizeof(mem))
				MemoryControl::mem[address] = value;
			break;
		}
	}
	else
	{
		if (address < sizeof(mem))
			MemoryControl::mem[address] = value;
	}
}

void MemoryControl::setMem(unsigned char address, unsigned char value, unsigned char bank) {
	std::unique_lock<std::mutex> lock(mutex);

	if(address == 0)
		address = mem[4];
	if(bank==1) //Bank 1
	{
		switch(address)
		{
		case 1:
			OPTION_REG = value;
			break;
		case 5:
			TRISA = value;
			break;
		case 6:
			TRISB = value;
			break;
		case 8:
			EECON1 = value;
			break;
		case 9:
			EECON2 = value;
			break;
		default:
			if(address < sizeof(mem))
				MemoryControl::mem[address] = value;
			break;
		}
	}
	else
	{
		if(address < sizeof(mem))
			MemoryControl::mem[address] = value;
	}
}

unsigned char MemoryControl::getMem(unsigned char address) {
	std::unique_lock<std::mutex> lock(mutex);

	if(address == 0)
		address = mem[4];
	if(mem[3] & 32) //Bank 1
	{
		switch(address)
		{
		case 1:
			return OPTION_REG;
		case 5:
			return TRISA;
		case 6:
			return TRISB;
		case 8:
			return EECON1;
		case 9:
			return EECON2;
		default:
			return MemoryControl::mem[address];
		}
	}
	else
	{
		return MemoryControl::mem[address];
	}
}

unsigned char MemoryControl::getMem(unsigned char address, unsigned char bank) {
	std::unique_lock<std::mutex> lock(mutex);
	
	if(bank == 1) //Bank 1
	{
		switch(address)
		{
		case 1:
			return OPTION_REG;
		case 5:
			return TRISA;
		case 6:
			return TRISB;
		case 8:
			return EECON1;
		case 9:
			return EECON2;
		default:
			return MemoryControl::mem[address];
		}
	}
	else
	{
		return MemoryControl::mem[address];
	}
}

void MemoryControl::setProg_mem(unsigned short address, unsigned short value) {
	std::unique_lock<std::mutex> lock(mutex);

	if (address < sizeof(prog_mem))
		MemoryControl::prog_mem[address] = value & 0x3FFF; //Mask to 14Bit
}

unsigned short MemoryControl::getProg_mem(unsigned short address) {
	std::unique_lock<std::mutex> lock(mutex);

	return MemoryControl::prog_mem[address];
}

void MemoryControl::setEeprom(unsigned char address, unsigned char value) {
	std::unique_lock<std::mutex> lock(mutex);

	if (address < sizeof(eeprom))
		MemoryControl::eeprom[address] = value;
}

unsigned char MemoryControl::getEeprom(unsigned char address) {
	std::unique_lock<std::mutex> lock(mutex);

	return MemoryControl::eeprom[address];
}

void MemoryControl::setPLC(unsigned short plc)
{
	std::unique_lock<std::mutex> lock(mutex);

	setPLCunsafe(plc);
}

void MemoryControl::setPLCunsafe(unsigned short plc) //OHNE MUTEX, AUFRUF NUR THREADSAFE
{
	if(plc >= 1000)
		plc = 0;

	unsigned char plcPartA = plc & 255;
	unsigned char plcPartB = (plc & 7936) >> 8; //mask to PCLATH
	mem[2] = plcPartA; 
	mem[10] = plcPartB; //obere 5 bit werden in PCLATH geschrieben
}

unsigned short MemoryControl::getPLC()
{
	std::unique_lock<std::mutex> lock(mutex);
	
	return getPLCunsafe();
}

unsigned short MemoryControl::getPLCunsafe()
{
	unsigned short plc = mem[2];
	plc += (mem[10] << 8); //PCLATH wird dazu"addiert" (höchstwertige 5 bit)

	return plc;
}

void MemoryControl::setW(unsigned char w)
{
	std::unique_lock<std::mutex> lock(mutex);

	this-> w = w;
}

unsigned char MemoryControl::getW()
{
	std::unique_lock<std::mutex> lock(mutex);

	return w;
}

void MemoryControl::setStatus(int flag, int value)
{
	std::unique_lock<std::mutex> lock(mutex);

	setStatusUnsafe(flag, value);
}

void MemoryControl::setStatusUnsafe(int flag, int value) //OHNE MUTEX, AUFRUF NUR THREADSAFE
{
	unsigned char statusreg = mem[3];
	if(value == 1) statusreg |= 1 << flag;
	if(value == 0) statusreg &= ~(1 << flag);
	mem[3] = statusreg;
}

bool MemoryControl::getStatus(int flag)
{
	std::unique_lock<std::mutex> lock(mutex);

	unsigned char statusreg = mem[3];
	unsigned short mask = pow(2.0, flag);

	return (statusreg&mask) >= 1;
}

void MemoryControl::setOption(int flag, int value)
{
	std::unique_lock<std::mutex> lock(mutex);

	unsigned char optionreg = OPTION_REG;
	if(value == 1) optionreg |= 1 << flag;
	if(value == 0) optionreg &= ~(1 << flag);
	OPTION_REG = optionreg;
}

bool MemoryControl::getOption(int flag)
{
	std::unique_lock<std::mutex> lock(mutex);

	return getOptionUnsafe(flag);
}

bool MemoryControl::getOptionUnsafe(int flag) //OHNE MUTEX, AUFRUF NUR THREADSAFE
{
	unsigned short mask = pow(2.0, flag);

	return (OPTION_REG&mask) >= 1;
}

void MemoryControl::setIntcon(int flag, int value)
{
	std::unique_lock<std::mutex> lock(mutex);

	unsigned char intconreg = mem[11];
	if(value == 1) intconreg |= 1 << flag;
	if(value == 0) intconreg &= ~(1 << flag);
	mem[11] = intconreg;
}

bool MemoryControl::getIntcon(int flag)
{
	std::unique_lock<std::mutex> lock(mutex);

	unsigned char intconreg = mem[11];
	unsigned short mask = pow(2.0, flag);

	return (intconreg&mask) >= 1;
}

void MemoryControl::stackPush(unsigned short plc) //"schiebt" einen Wert ins Stack-Register
{
	std::unique_lock<std::mutex> lock(mutex);

	for (int i = 0; i < STACK_SIZE; i++)
	{
		if (stack[i] == 0)
		{
			stack[i] = plc;
			return;
		}
	}
}

unsigned short MemoryControl::stackPop() //gibt letzten EIntrag des Stack-Registers wieder
{
	std::unique_lock<std::mutex> lock(mutex);

	for (int i = STACK_SIZE - 1; i >= 0; i--)
	{
		if (stack[i] != 0)
		{
			unsigned short plc = stack[i];
			stack[i] = 0;
			return plc;
		}
	}
}

unsigned short MemoryControl::getStack(unsigned short address) {
	std::unique_lock<std::mutex> lock(mutex);

	return stack[address];
}

void MemoryControl::resetMemory(bool resetProgMem) { //setzt Standard-Values, bei Wunsch auch im Programmspeicher
	std::unique_lock<std::mutex> lock(mutex);

	for (int i = 0; i < MEM_SIZE; i++)
		mem[i] = 0;

	mem[3] = 24;
	OPTION_REG = 255;
	TRISA = 255;
	TRISB = 255;
	EECON1 = 0;
	EECON2 = 0;

	w = 0;
	setPLCunsafe(0);

	if (resetProgMem)
		for (int i = 0; i < PROG_MEM_SIZE; i++)
			prog_mem[i] = 0;

	for (int i = 0; i < EEPROM_SIZE; i++)
		eeprom[i] = 0;

	for (int i = 0; i < STACK_SIZE; i++)
		stack[i] = 0;
}

void MemoryControl::setPort(unsigned char n, bool b, bool value) //if b==true : Ports von PORTB werden gesetzt
{
	std::unique_lock<std::mutex> lock(mutex);

	if (!b) 
	{
		if(value)
		{
			PORTA |= 1 << n;
		}			
		else
		{
			PORTA &= ~(1 << n);
		}
		mem[5] = PORTA;
	}
	else 
	{
		if(value)
		{
			PORTB |= 1 << n;
		}
		else
		{
			PORTB &= ~(1 << n);
		}
		mem[6] = PORTB;
	}
}

bool MemoryControl::getPort(unsigned char n, bool b) //if b == true, Ausgänge von PORTB werden zurückgegeben
{
	std::unique_lock<std::mutex> lock(mutex);

	if (!b) {
		return PORTA & (int)pow(2.0, n);
	}
	return PORTB & (int)pow(2.0, n);
}

void MemoryControl::setTris(unsigned char n, bool b, bool value) //bool b false: PORT A, true: PORT B
{
	std::unique_lock<std::mutex> lock(mutex);

	if (!b) {
		if (value)
			TRISA |= 1 << n;
		else
			TRISA &= ~(1 << n);
	}
	else {
		if (value)
			TRISB |= 1 << n;
		else
			TRISB &= ~(1 << n);
	}
}

bool MemoryControl::getTris(unsigned char n, bool b) //bool b false: PORT A, true: PORT B
{
	std::unique_lock<std::mutex> lock(mutex);

	if (!b) {
		return TRISA & (int)pow(2.0, n);
	}
	return TRISB & (int)pow(2.0, n);
}

void MemoryControl::setLastA(unsigned char value) //speichert aktuelle A-Konfig
{
	std::unique_lock<std::mutex> lock(mutex);

	LASTA = value;
}

void MemoryControl::setLastB(unsigned char value) //speichert aktuelle B-Konfig
{
	std::unique_lock<std::mutex> lock(mutex);

	LASTB = value;
}

bool MemoryControl::getLastA(unsigned char n)//gibt vorhergehende A-Konfig aus (Flankenerkennung)
{
	std::unique_lock<std::mutex> lock(mutex);

	return LASTA & (int)pow(2.0, n);
}

bool MemoryControl::getLastB(unsigned char n) //gibt vorhergehende B-Konfig aus (Flankenerkennung)
{
	std::unique_lock<std::mutex> lock(mutex);
	return LASTB & (int)pow(2.0, n);
}

bool MemoryControl::getSleep(){ std::unique_lock<std::mutex> lock(mutex); return SLEEP; }
void MemoryControl::setSleep(bool value)
{
	std::unique_lock<std::mutex> lock(mutex);

	SLEEP = value; //setzt sleep mit Default-Sleep-Mem-Values
	if(value)
	{
		//Default Values for Sleep Mode
		setPLCunsafe(0);

		setStatusUnsafe(7, 0); //STATUS
		setStatusUnsafe(6, 0);
		setStatusUnsafe(5, 0);

		mem[10] = 0; //PCLATH

		mem[11] = 0; //INTCON

		OPTION_REG = 255; //OPTION
		
		TRISA = 31;
		TRISB = 255;

		EECON1 = 0;
	}
	if(!value)
	{
		setPLCunsafe(getPLCunsafe() + 1);
	}
}

void MemoryControl::setWatchdog(bool value) //(de)aktiviert den WDT
{
	std::unique_lock<std::mutex> lock(mutex);

	WatchdogActive = value;
}

bool MemoryControl::getWatchdog() //gibt Konfig des WDT wieder
{
	std::unique_lock<std::mutex> lock(mutex);

	return WatchdogActive;
}

void MemoryControl::setWatchdogTimer() //setzt den WDT auf aktuellen Timestamp
{
	std::unique_lock<std::mutex> lock(mutex);

	SYSTEMTIME sysTime;
	GetSystemTime(&sysTime);
	LONG time = (sysTime.wSecond * 1000) + sysTime.wMilliseconds + 18; //Timer "läuft ab" in 18ms
	WatchdogTimer = time;
}

unsigned short MemoryControl::getWatchdogTimer() //gibt Timer-Ablaufdatum wieder
{
	std::unique_lock<std::mutex> lock(mutex);

	return WatchdogTimer;
}

unsigned short MemoryControl::getNibble(unsigned short value) //gibt das niederwertigere Nibble eines Byte wieder... für DC-Flag
{
	std::unique_lock<std::mutex> lock(mutex);

	return value & 15;
}

unsigned short MemoryControl::getPrescalerRate()
{
	std::unique_lock<std::mutex> lock(mutex);

	unsigned short prescalerValue = 0;
	if(getOptionUnsafe(0)) prescalerValue += 1;
	if(getOptionUnsafe(1)) prescalerValue += 2;
	if(getOptionUnsafe(2)) prescalerValue += 4;

	bool isWDT = false; //verschiedene PS0-2 Einstellungen haben unterschiedliche Auswirkungen auf WDT bzw T0
	if(getOptionUnsafe(3)) isWDT = true;

	switch(prescalerValue)
	{
		case 0:
		{
			if(isWDT) return 1;
			else return 2;
		}
		case 1:
		{
			if(isWDT) return 2;
			else return 4;
		}
		case 2:
		{
			if(isWDT) return 4;
			else return 8;
		}
		case 3:
		{
			if(isWDT) return 8;
			else return 16;
		}
		case 4:
		{
			if(isWDT) return 16;
			else return 32;
		}
		case 5:
		{
			if(isWDT) return 32;
			else return 64;
		}
		case 6:
		{
			if(isWDT) return 64;
			else return 128;
		}
		case 7:
		{
			if(isWDT) return 128;
			else return 255;
		}
	}
}

unsigned short MemoryControl::getTimerPrescaler() //gibt Prescaler-Zählwert wieder
{
	std::unique_lock<std::mutex> lock(mutex);

	return TimerPrescaler;
}

void MemoryControl::setTimerPrescaler(unsigned short value) //setzt den Prescaler-Zählwert
{
	std::unique_lock<std::mutex> lock(mutex);

	TimerPrescaler = value;
}

unsigned short MemoryControl::getEEcon2()
{
	std::unique_lock<std::mutex> lock(mutex);

	return EECON2;
}

void MemoryControl::setEEcon1(int flag, int value) //setzt bestimmte Bits des EECON1-Reg
{
	std::unique_lock<std::mutex> lock(mutex);

	unsigned char eeconreg = EECON1;
	if(value == 1) eeconreg |= 1 << flag;
	if(value == 0) eeconreg &= ~(1 << flag);
	EECON1 = eeconreg;
}

bool MemoryControl::getEEcon1(int flag)
{
	std::unique_lock<std::mutex> lock(mutex);

	unsigned short mask = pow(2.0, flag);

	return (EECON1&mask) >= 1;
}

int MemoryControl::getEepromState() //für State-Machine bei EEPROM-Schreibzyklus
{
	std::unique_lock<std::mutex> lock(mutex);

	return EepromState;
}

void MemoryControl::setEepromState(int value)
{
	std::unique_lock<std::mutex> lock(mutex);

	EepromState = value;
}