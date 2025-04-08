#pragma once
#include <iostream>


using namespace std;

/*
Load Word (lw), loads a 32-bit word from memory into a register
*/

bool LoadWord(int MEM[], int (&REG)[], int destinationReg, int offset, int sourceReg)
{
    offset = offset / 4;
    int EffAdd = offset + REG[sourceReg];
    if (REG[destinationReg] = MEM[EffAdd])
    {
        return true;
    }
    return false;
}

/*
Store Word (SW) stores a 32-bit word from a register into memory
*/

bool StoreWord(int (&MEM)[], int REG[], int destinationReg, int offset, int sourceReg)
{
    offset = offset / 4;
    int EffAdd = offset + REG[sourceReg];
    if ( MEM[EffAdd] = REG[destinationReg] )
    {
        return true;
    }
    return false;
}
