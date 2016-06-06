#include "stdafx.h"
#include "PIC16F84.h"
#include <Windows.h>

PIC16F84::PIC16F84()
{
}


PIC16F84::~PIC16F84()
{
}

void PIC16F84::WDT()
{
	SYSTEMTIME time;
	GetSystemTime(&time);
	LONG time_ms = (time.wSecond * 1000) + time.wMilliseconds;
	if(memoryControl.getWatchdog()) //wenn WDT aktiv
	{
		if(time_ms > memoryControl.getWatchdogTimer())
		{
			unsigned short plc = memoryControl.getPLC();

			memoryControl.resetMemory(false);
			memoryControl.setSleep(false);
			memoryControl.setStatus(4, 0);

			memoryControl.setPLC(plc);
		}
	}
	memoryControl.setWatchdogTimer();
}

void PIC16F84::execCycle()
{
	WDT();
	//EEPROM Write
	//EEPROM State Machine
	if(memoryControl.getEEcon2() == 0x55) memoryControl.setEepromState(1); //State-Machine auf State 1 setzen

	if(memoryControl.getEepromState() == 1 && (memoryControl.getEEcon2() == 0xAA)) memoryControl.setEepromState(2); //State-Machine auf State 2 setzen

	if((memoryControl.getEepromState() == 2) && memoryControl.getEEcon1(1) && memoryControl.getEEcon1(2)) //Wenn State-Machine write erlaubt, WR und WREnable auf 1 gesetzt sind, Schreibvorgang starten
	{
		memoryControl.setEeprom(memoryControl.getMem(EEADDR,0), memoryControl.getMem(EEDATA,0)); 
		memoryControl.setEEcon1(4, 1);
	}

	//EEPROM Read
	if(!memoryControl.getEEcon1(7) && memoryControl.getEEcon1(0)) //zum lesen muss bit7 gelöscht und bit0 gesetzt sein
	{
		memoryControl.setMem(EEDATA, memoryControl.getEeprom(memoryControl.getMem(EEADDR)));
	}

	//Interrupt-Flags prüfen und ggf setzen
	if(memoryControl.getPort(0, true) != memoryControl.getLastB(0)) //RB0 Interrupt
	{
		if(memoryControl.getOption(6)) //Option<6> = INTEDG, bei 1 wird Interrupt bei steigender Flanke ausgelöst
		{
			if(memoryControl.getPort(0, true)) //Wenn Wert jetzt positiv, Flankenwechsel also auch positiv
			{
				unsigned short intcon = memoryControl.getMem(11);
				intcon |= 1 << 1;
				memoryControl.setMem(11, intcon);
			}
		}
		if(!memoryControl.getOption(6))
		{
			if(!memoryControl.getPort(0, true))
			{
				unsigned short intcon = memoryControl.getMem(11);
				intcon |= 1 << 1;
				memoryControl.setMem(11, intcon);
			}
		}
	}
	
	for(int i = 4; i < 8; i++) //Ports RB4 ... RB7 fungieren bei Flankenwechsel als Interruptquelle
	{
		if(memoryControl.getPort(i, true) != memoryControl.getLastB(i)) //momentane Beschaltung wird mit voriger verglichen
		{
			if(memoryControl.getTris(i, true)) //Interrupt wird nur ausgelöst, wenn Port als Eingang konfiguriert ist
			{
				unsigned short intcon = memoryControl.getMem(11);
				intcon |= 1 << 0;
				memoryControl.setMem(11, intcon);
			}
		}
	}
	//Timer0
	if(!memoryControl.getOption(3)) //checken, ob Prescaler zu Timer0 gehört
	{
		bool TimerUp = false;
		if(!memoryControl.getOption(5)) TimerUp = true; //Wenn interner Takt verwendet wird, wird hochgezählt
		else //RA ist Taktquelle
		{
			if(memoryControl.getOption(4))//TOSE 1 = High/Low-Flanke wird gezählt
			{
				if(!memoryControl.getPort(0, false) && memoryControl.getLastA(0))
				{
					TimerUp = true;
				}
			}
			else //TOSE 0 = Low/High-Flanke wird gezählt
			{
				if(memoryControl.getPort(0, false) && !memoryControl.getLastA(0))
				{
					TimerUp = true;
				}
			}
		}
		if(TimerUp)
		{
			if(memoryControl.getTimerPrescaler() < memoryControl.getPrescalerRate()) // "Zwischenzähler" erhöhen, bis Prescaler Erhöhung von Tmr0 zulässt
			{
				memoryControl.setTimerPrescaler(memoryControl.getTimerPrescaler() + 1);
			}
			if(memoryControl.getTimerPrescaler() == memoryControl.getPrescalerRate())
			{
				memoryControl.setTimerPrescaler(0);
				if(memoryControl.getMem(1) == 255) //Interrupt-Fall
				{
					unsigned short intcon = memoryControl.getMem(11);
					intcon |= 1 << 2;
					memoryControl.setMem(11, intcon);
				}
				memoryControl.setMem(1, memoryControl.getMem(1) + 1);
			}
		}
	}
	else
	{
		bool TimerUp = false;
		if(!memoryControl.getOption(5)) TimerUp = true; //Wenn interner Takt verwendet wird, wird hochgezählt
		else //RA ist Taktquelle
		{
			if(memoryControl.getOption(4))//TOSE 1 = High/Low-Flanke wird gezählt
			{
				if(!memoryControl.getPort(0, false) && memoryControl.getLastA(0))
				{
					TimerUp = true;
				}
			}
			else //TOSE 0 = Low/High-Flanke wird gezählt
			{
				if(memoryControl.getPort(0, false) && !memoryControl.getLastA(0))
				{
					TimerUp = true;
				}
			}
		}
		if(TimerUp)
		{
			if(memoryControl.getMem(1) == 255) //Interrupt-Fall
			{
				unsigned short intcon = memoryControl.getMem(11);
				intcon |= 1 << 2;
				memoryControl.setMem(11, intcon);
			}
			memoryControl.setMem(1, memoryControl.getMem(1) + 1); //ansonsten timer ohne Prescaler erhöhen
		}
	}

	//2-Cycle-Instructions
	if(cycleCount <= 0)
	{
		if(!checkInterrupt()) cycleCount = execInstruction(); //eigentliche Instruktion wird ausgeführt, Rückgabewert (für 2-Cycle-Instr) zwischengespeichert
	}

	cycleCount--;

	//momentane Port-Konfig speichern für Erkennung von Flankenwechsel
	memoryControl.setLastA(memoryControl.getMem(5));
	memoryControl.setLastB(memoryControl.getMem(6));
}
bool PIC16F84::checkInterrupt()
{
	//4 interrupt sources: T0, INT0, RB0, EE
	unsigned short intcon = memoryControl.getMem(11);
	bool GlobalInterrupt = intcon & 128;
	bool EepromInterrupt = intcon & 64;
	bool Timer0Enable = intcon & 32;
	bool RBExtEnable = intcon & 16;
	bool RBChangeEnable = intcon & 8;
	bool Timer0Flag = intcon & 4;
	bool RBExtFlag = intcon & 2;
	bool RBChangeFlag = intcon & 1;
	if ((Timer0Enable&&Timer0Flag) || (RBExtEnable&&RBExtFlag) || (RBChangeEnable&&RBChangeFlag)) ///EEPROM Interrupt Flag
	{
		memoryControl.setSleep(false);
		if (GlobalInterrupt) //Interrupt-Routinen aktiv
		{
			execInstruction();
			memoryControl.stackPush(memoryControl.getPLC()); //momentanen Programmzähler auf den Stack pushen
			memoryControl.setPLC(4); //Programmzähler auf Interrupt-Routine setzen
			unsigned short intnew = memoryControl.getMem(11);
			intnew &= ~(1 << 7); //GIE-Bit auf 0 setzen um weitere Interrupts zu verhindern
			memoryControl.setMem(11, intnew);
			return 1;
		}
	}
	return memoryControl.getSleep();
}

int PIC16F84::execInstruction()
{
	unsigned short plc = memoryControl.getPLC();
	unsigned short instruction = memoryControl.getProg_mem(plc);
	unsigned short opcode = instruction;

	//14-Bit-Opcodes
	opcode = instruction;
	switch (opcode)
	{
		case 8: //RETURN
		{
			memoryControl.setPLC(memoryControl.stackPop());
			return 2;
		}
		case 100: //CLRWDT
		{
			memoryControl.setWatchdogTimer();
			memoryControl.setStatus(4, 1);
			memoryControl.setStatus(3, 1);
			memoryControl.setPLC(plc + 1); 
			return 1;
		}
		case 9: //RETFIE
		{
			unsigned short intnew = memoryControl.getMem(11);
			intnew |= 1 << 7; //GIE-Bit auf 1 setzen um weitere Interrupts zu ermöglichen
			memoryControl.setMem(11, intnew);
			memoryControl.setPLC(memoryControl.stackPop());
			return 2;
		}
		case 99: //SLEEP
		{
			memoryControl.setSleep(true);
			memoryControl.setStatus(4, 1);
			memoryControl.setStatus(3, 0);
			memoryControl.setWatchdogTimer();
			if(memoryControl.getOption(3)) //checken, ob Prescaler zu WDT gehört
			{
				memoryControl.setTimerPrescaler(0);
			}
			memoryControl.setPLC(plc + 1);
			return 1;
		}
	}

	//7-Bit-Opcodes
	opcode = instruction >> 7;
	switch(opcode)
	{
		case 2: //CLRW
		{
			memoryControl.setW(0);
			memoryControl.setStatus(MemoryControl::FLAG_ZERO, 1);
			memoryControl.setPLC(plc + 1);
			return 1;
		}
		case 0: //NOP
		{
			memoryControl.setPLC(plc + 1);
			return 1;
		}
	}

	//6-Bit-Opcodes
	opcode = instruction >> 8;
	switch(opcode)
	{
		case 7: //ADDWF
		{	
			unsigned char addr = instruction & 127; //get last 7 bits from instruction
			unsigned char w_reg = memoryControl.getW();
			unsigned char f_reg = memoryControl.getMem(addr);
			unsigned char value = w_reg + f_reg;
			if(instruction & 128) //if "d" is set
			{
				memoryControl.setMem(addr, value);
			}
			else //if "d" isn't set, result is stored in W reg
			{
				memoryControl.setW(value);
			}
			if(value == 0) memoryControl.setStatus(MemoryControl::FLAG_ZERO, 1);
			else memoryControl.setStatus(MemoryControl::FLAG_ZERO, 0);
				
			if(w_reg + f_reg > 255) memoryControl.setStatus(MemoryControl::FLAG_CARRY, 1);
			else memoryControl.setStatus(MemoryControl::FLAG_CARRY, 0);
					
			if((w_reg + f_reg > 15)&&(w_reg<15)&&(f_reg<15)) memoryControl.setStatus(MemoryControl::FLAG_DIGIT, 1);
			else memoryControl.setStatus(MemoryControl::FLAG_DIGIT, 0);

			memoryControl.setPLC(plc + 1);
			return 1;
		}
		case 5: //ANDWF
		{
			unsigned char addr = instruction & 127; //get last 7 bits from instruction
			unsigned char w_reg = memoryControl.getW();
			unsigned char f_reg = memoryControl.getMem(addr);
			unsigned char value = w_reg & f_reg;
			if(instruction & 128) //if "d" is set
			{
				memoryControl.setMem(addr, value);
			}
			else //if "d" isn't set, result is stored in W reg
			{
				memoryControl.setW(value);
			}
			if(value == 0) memoryControl.setStatus(MemoryControl::FLAG_ZERO, 1);
			else memoryControl.setStatus(MemoryControl::FLAG_ZERO, 0);

			memoryControl.setPLC(plc + 1);
			return 1;
		}
		case 1: //CLRF
		{
			unsigned char addr = instruction & 127; //get last 7 bits from instruction
			memoryControl.setMem(addr, 0);
			memoryControl.setStatus(MemoryControl::FLAG_ZERO, 1);

			memoryControl.setPLC(plc + 1);
			return 1;
		}
		case 9: //COMF
		{
			unsigned char addr = instruction & 127; //get last 7 bits from instruction
			unsigned char f_reg = memoryControl.getMem(addr);
			unsigned char value = ~ f_reg;
			if(instruction & 128) //if "d" is set
			{
				memoryControl.setMem(addr, value);
			}
			else //if "d" isn't set, result is stored in W reg
			{
				memoryControl.setW(value);
			}
			if(value == 0) memoryControl.setStatus(MemoryControl::FLAG_ZERO, 1);
			else memoryControl.setStatus(MemoryControl::FLAG_ZERO, 0);

			memoryControl.setPLC(plc + 1);
			return 1;
		}
		case 3: //DECF
		{
			unsigned char addr = instruction & 127; //get last 7 bits from instruction
			unsigned char f_reg = memoryControl.getMem(addr);
			unsigned char value = f_reg-1;
			if(instruction & 128) //if "d" is set
			{
				memoryControl.setMem(addr, value);
			}
			else //if "d" isn't set, result is stored in W reg
			{
				memoryControl.setW(value);
			}
			if(value == 0) memoryControl.setStatus(MemoryControl::FLAG_ZERO, 1);
			else memoryControl.setStatus(MemoryControl::FLAG_ZERO, 0);

			memoryControl.setPLC(plc + 1);
			return 1;
		}
		case 11: //DECFSZ
		{
			unsigned char addr = instruction & 127; //get last 7 bits from instruction
			unsigned char f_reg = memoryControl.getMem(addr);
			unsigned char value = f_reg - 1;
			if(instruction & 128) //if "d" is set
			{
				memoryControl.setMem(addr, value);
			}
			else //if "d" isn't set, result is stored in W reg
			{
				memoryControl.setW(value);
			}
			if(value == 0) memoryControl.setStatus(MemoryControl::FLAG_ZERO, 1);
			else memoryControl.setStatus(MemoryControl::FLAG_ZERO, 0);

			if(value == 0)
			{
				memoryControl.setPLC(plc + 2);
				return 2;
			}
			else {
				memoryControl.setPLC(plc + 1);
				return 1;
			}
			return 2;
		}
		case 10: //INCF
		{
			unsigned char addr = instruction & 127; //get last 7 bits from instruction
			unsigned char f_reg = memoryControl.getMem(addr);
			unsigned char value = f_reg + 1;
			if(instruction & 128) //if "d" is set
			{
				memoryControl.setMem(addr, value);
			}
			else //if "d" isn't set, result is stored in W reg
			{
				memoryControl.setW(value);
			}
			if(value == 0) memoryControl.setStatus(MemoryControl::FLAG_ZERO, 1);
			else memoryControl.setStatus(MemoryControl::FLAG_ZERO, 0);

			memoryControl.setPLC(plc + 1);
			return 1;
		}
		case 15: //INCFSZ
		{
			unsigned char addr = instruction & 127; //get last 7 bits from instruction
			unsigned char f_reg = memoryControl.getMem(addr);
			unsigned char value = f_reg + 1;
			if(instruction & 128) //if "d" is set
			{
				memoryControl.setMem(addr, value);
			}
			else //if "d" isn't set, result is stored in W reg
			{
				memoryControl.setW(value);
			}
			if(value == 0) memoryControl.setStatus(MemoryControl::FLAG_ZERO, 1);
			else memoryControl.setStatus(MemoryControl::FLAG_ZERO, 0);

			if (value == 0)
			{
				memoryControl.setPLC(plc + 2);
				return 2;
			}
			else {
				memoryControl.setPLC(plc + 1);
				return 1;
			}

		}
		case 4: //IORWF
		{
			unsigned char addr = instruction & 127; //get last 7 bits from instruction
			unsigned char w_reg = memoryControl.getW();
			unsigned char f_reg = memoryControl.getMem(addr);
			unsigned char value = f_reg | w_reg;
			if(instruction & 128) //if "d" is set
			{
				memoryControl.setMem(addr, value);
			}
			else //if "d" isn't set, result is stored in W reg
			{
				memoryControl.setW(value);
			}
			if(value == 0) memoryControl.setStatus(MemoryControl::FLAG_ZERO, 1);
			else memoryControl.setStatus(MemoryControl::FLAG_ZERO, 0);

			memoryControl.setPLC(plc + 1);
			return 1;
		}
		case 8: //MOVF
		{
			unsigned char addr = instruction & 127; //get last 7 bits from instruction
			unsigned char f_reg = memoryControl.getMem(addr);
			unsigned char value = f_reg;
			if(instruction & 128) //if "d" is set
			{
				memoryControl.setMem(addr, value);
			}
			else //if "d" isn't set, result is stored in W reg
			{
				memoryControl.setW(value);
			}
			if(value == 0) memoryControl.setStatus(MemoryControl::FLAG_ZERO, 1);
			else memoryControl.setStatus(MemoryControl::FLAG_ZERO, 0);

			memoryControl.setPLC(plc + 1);
			return 1;
		}
		case 0: //MOVWF
		{
			unsigned short addr = instruction & 127; //get last 7 bits from instruction
			unsigned short w_reg = memoryControl.getW();
			memoryControl.setMem(addr, w_reg);

			memoryControl.setPLC(memoryControl.getPLC() + 1);
			return 1;
		}
		case 13: //RLF
		{
			unsigned short addr = instruction & 127; //get last 7 bits from instruction
			unsigned short f_reg = memoryControl.getMem(addr);
			unsigned short carry_now = memoryControl.getStatus(MemoryControl::FLAG_CARRY);
			unsigned short carry_new = 0;

			if(f_reg & 128) carry_new = 1;
			unsigned short value = f_reg << 1;
			if(carry_now == 1) value += 1;

			if(instruction & 128) //if "d" is set
			{
				memoryControl.setMem(addr, value);
			}
			else //if "d" isn't set, result is stored in W reg
			{
				memoryControl.setW(value);
			}
			if(carry_new == 1) memoryControl.setStatus(MemoryControl::FLAG_CARRY, 1);
			else memoryControl.setStatus(MemoryControl::FLAG_CARRY, 0);

			memoryControl.setPLC(plc + 1);
			return 1;
		}
		case 12: //RRF
		{
			unsigned short addr = instruction & 127; //get last 7 bits from instruction
			unsigned short f_reg = memoryControl.getMem(addr);
			unsigned short carry_now = memoryControl.getStatus(MemoryControl::FLAG_CARRY);
			unsigned short carry_new = 0;

			if(f_reg & 1) carry_new = 1;
			unsigned short value = f_reg >> 1;
			if(carry_now == 1) value += 128;

			if(instruction & 128) //if "d" is set
			{
				memoryControl.setMem(addr, value);
			}
			else //if "d" isn't set, result is stored in W reg
			{
				memoryControl.setW(value);
			}
			if(carry_new == 1) memoryControl.setStatus(MemoryControl::FLAG_CARRY, 1);
			else memoryControl.setStatus(MemoryControl::FLAG_CARRY, 0);

			memoryControl.setPLC(plc + 1);
			return 1;
		}
		case 2: //SUBWF
		{
			unsigned short addr = instruction & 127; //get last 7 bits from instruction
			unsigned short w_reg = memoryControl.getW();
			unsigned short f_reg = memoryControl.getMem(addr);
			unsigned short value = f_reg - w_reg;
			if(instruction & 128) //if "d" is set
			{
				memoryControl.setMem(addr, value);
			}
			else //if "d" isn't set, result is stored in W reg
			{
				memoryControl.setW(value);
			}
			if(value == 0) memoryControl.setStatus(MemoryControl::FLAG_ZERO, 1);
			else memoryControl.setStatus(MemoryControl::FLAG_ZERO, 0);

			if(w_reg < f_reg) memoryControl.setStatus(MemoryControl::FLAG_CARRY, 1);
			else memoryControl.setStatus(MemoryControl::FLAG_CARRY, 0);

			if(memoryControl.getNibble(f_reg)>=memoryControl.getNibble(w_reg)) memoryControl.setStatus(MemoryControl::FLAG_DIGIT, 1);
			else memoryControl.setStatus(MemoryControl::FLAG_DIGIT, 0);

			memoryControl.setPLC(plc + 1);
			return 1;
		}
		case 14: //SWAPF
		{
			unsigned short addr = instruction & 127; //get last 7 bits from instruction
			unsigned short f_reg = memoryControl.getMem(addr);
			unsigned short value = (f_reg & 0x0F) << 4 | (f_reg & 0xF0) >> 4;
			if(instruction & 128) //if "d" is set
			{
				memoryControl.setMem(addr, value);
			}
			else //if "d" isn't set, result is stored in W reg
			{
				memoryControl.setW(value);
			}

			memoryControl.setPLC(plc + 1);
			return 1;
		}
		case 6: //XORWF
		{
			unsigned short addr = instruction & 127; //get last 7 bits from instruction
			unsigned short w_reg = memoryControl.getW();
			unsigned short f_reg = memoryControl.getMem(addr);
			unsigned short value = f_reg ^ w_reg;
			if(instruction & 128) //if "d" is set
			{
				memoryControl.setMem(addr, value);
			}
			else //if "d" isn't set, result is stored in W reg
			{
				memoryControl.setW(value);
			}
			if(value == 0) memoryControl.setStatus(MemoryControl::FLAG_ZERO, 1);
			else memoryControl.setStatus(MemoryControl::FLAG_ZERO, 0);

			memoryControl.setPLC(plc + 1);
			return 1;
		}
		case 57: //ANDLW
		{
			unsigned short literal = instruction & 255; //get last 8 bits from instruction
			unsigned short w_reg = memoryControl.getW();
			unsigned short value = w_reg & literal;
			memoryControl.setW(value);
			if(value == 0) memoryControl.setStatus(MemoryControl::FLAG_ZERO, 1);
			else memoryControl.setStatus(MemoryControl::FLAG_ZERO, 0);

			memoryControl.setPLC(plc + 1);
			return 1;
		}
		case 56: //IORLW
		{
			unsigned short literal = instruction & 255; //get last 8 bits from instruction
			unsigned short w_reg = memoryControl.getW();
			unsigned short value = w_reg | literal;
			memoryControl.setW(value);
			if(value == 0) memoryControl.setStatus(MemoryControl::FLAG_ZERO, 1);
			else memoryControl.setStatus(MemoryControl::FLAG_ZERO, 0);

			memoryControl.setPLC(plc + 1);
			return 1;
		}
		case 58: //XORLW
		{
			unsigned short literal = instruction & 255; //get last 8 bits from instruction
			unsigned short w_reg = memoryControl.getW();
			unsigned short value = w_reg ^ literal;
			memoryControl.setW(value);
			if(value == 0) memoryControl.setStatus(MemoryControl::FLAG_ZERO, 1);
			else memoryControl.setStatus(MemoryControl::FLAG_ZERO, 0);

			memoryControl.setPLC(plc + 1);
			return 1;
		}

	}
	//5-Bit-Opcodes
	opcode = instruction >> 9;
	switch(opcode)
	{
		case 31: //ADDLW
		{
			unsigned short literal = instruction & 255; //get last 8 bits from instruction
			unsigned short w_reg = memoryControl.getW();
			unsigned short value = w_reg + literal;
			memoryControl.setW(value);
			if(value == 0) memoryControl.setStatus(MemoryControl::FLAG_ZERO, 1);
			else memoryControl.setStatus(MemoryControl::FLAG_ZERO, 0);

			if(value > 128) memoryControl.setStatus(MemoryControl::FLAG_CARRY, 1);
			else memoryControl.setStatus(MemoryControl::FLAG_CARRY, 0);

			if((w_reg <= 15) && (literal <= 15) && (value >= 16)) memoryControl.setStatus(MemoryControl::FLAG_DIGIT, 1);
			else memoryControl.setStatus(MemoryControl::FLAG_DIGIT, 0);

			memoryControl.setPLC(plc + 1);
			return 1;
		}
		case 30: //SUBLW
		{
			unsigned char literal = instruction & 255; //get last 8 bits from instruction
			unsigned char w_reg = memoryControl.getW();
			unsigned char value = literal - w_reg;
			memoryControl.setW(value);
			if(value == 0) memoryControl.setStatus(MemoryControl::FLAG_ZERO, 1);
			else memoryControl.setStatus(MemoryControl::FLAG_ZERO, 0);

			if(w_reg < literal) memoryControl.setStatus(MemoryControl::FLAG_CARRY, 1);
			else memoryControl.setStatus(MemoryControl::FLAG_CARRY, 0);

			if(w_reg < literal) memoryControl.setStatus(MemoryControl::FLAG_DIGIT, 1);
			else memoryControl.setStatus(MemoryControl::FLAG_DIGIT, 0);

			memoryControl.setPLC(plc + 1);
			return 1;
		}
	}

	//4-Bit-Opcodes
	opcode = instruction >> 10;
	switch(opcode)
	{
		case 4: //BCF
		{
			unsigned char bit = (instruction & 896) >> 7;
			unsigned char addr = instruction & 127; //get last 7 bits from 
			unsigned char f_reg = memoryControl.getMem(addr);
			unsigned char value = f_reg &= ~(1 << bit);
			memoryControl.setMem(addr, value);

			memoryControl.setPLC(plc + 1);
			return 1;
		}
		case 5: //BSF
		{ 
			unsigned short bit = (instruction & 896) >> 7;
			unsigned short addr = instruction & 127; //get last 7 bits from 
			unsigned short f_reg = memoryControl.getMem(addr);
			unsigned short value = f_reg |= 1 << bit;
			memoryControl.setMem(addr, value);
			memoryControl.setPLC(plc + 1);
			return 1;
		}
		case 6: //BTFSC  
		{
			unsigned char bit = (instruction & 896) >> 7; 
			unsigned char addr = instruction & 127; //get last 7 bits from 
			unsigned char f_reg = memoryControl.getMem(addr);
			unsigned char mask = pow(2.0, bit);
			if((f_reg&mask) >= 1)
			{
				memoryControl.setPLC(plc + 1);
			}
			else
			{
				memoryControl.setPLC(plc + 2);
			}
			return 2;
		}
		case 7: //BTFSS
		{
			unsigned char bit = (instruction & 896) >> 7; ///shift added
			unsigned char addr = instruction & 127; //get last 7 bits from 
			unsigned char f_reg = memoryControl.getMem(addr);
			unsigned char mask = pow(2.0, bit);
			if((f_reg&mask) >= 1)
			{
				memoryControl.setPLC(plc + 2);
			}
			else
			{
				memoryControl.setPLC(plc + 1);
			}
			return 2;
		}
		case 12: //MOVLW
		{
			unsigned char literal = instruction & 255; //get last 8 bits from instruction
			memoryControl.setW(literal);

			memoryControl.setPLC(plc + 1);
			return 1;
		}
		case 13: //RETLW
		{
			unsigned short literal = instruction & 255; //get last 8 bits from instruction
			memoryControl.setW(literal);
			memoryControl.setPLC(memoryControl.stackPop());
			return 2;			
		}
	}

	//3-Bit-Opcodes
	opcode = instruction >> 11;
	switch (opcode)
	{
		case 4: //CALL
		{
			unsigned short k = instruction & 2047;
			unsigned char PCLATH = memoryControl.getMem(10);

			if (PCLATH & 8 != 0)
				k |= 1 << 12;
			else
				k &= ~(1 << 12);

			if (PCLATH & 16 != 0)
				k |= 1 << 13;
			else
				k &= ~(1 << 13);

			memoryControl.stackPush(memoryControl.getPLC() + 1);

			memoryControl.setPLC(k);
			return 2;
		}
		case 5: //GOTO
		{
			unsigned short k = instruction & 2047;
			unsigned char PCLATH = memoryControl.getMem(10);

			if (PCLATH & 8 == 1)
				k |= 1 << 12;
			else
				k &= ~(1 << 12);

			if (PCLATH & 16 == 1)
				k |= 1 << 13;
			else
				k &= ~(1 << 13);

			memoryControl.setPLC(k);

			return 2;
		}
	}
}

void PIC16F84::start()
{
}

void PIC16F84::parseLST(CString path)
{
	std::ifstream file(path);
	std::string str;
	int progMemI = 0;

	while (std::getline(file, str))
	{
		if (str[0] != ' ')
		{
			int loc_space = -1;
			std::string value = "";

			for (int i = 0; i < sizeof(str); i++)
			{
				if (loc_space >= 0)
					value += str[i];

				if (str[i] == ' ')
				{
					if (loc_space >= 0)
						break;

					loc_space = i;
				}
			}

			unsigned short instruction = std::stoi(value, 0, 16);
			memoryControl.setProg_mem(progMemI++, instruction);
		}
	}
}

void PIC16F84::reset(bool resetProgMem)
{
	memoryControl.resetMemory(resetProgMem);
	memoryControl.setWatchdogTimer();
}