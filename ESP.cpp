#include <Windows.h>
#include <iostream>
#include "MemMan.h"

MemMan MemClass;

struct offset
{
	DWORD m_iTeamNum = 0xF4;
	DWORD m_iGlowIndex = 0xA40C;
	DWORD dwEntityList = 0x4D04A94;
	DWORD dwLocalPlayer = 0xCF2A3C;
	DWORD dwGlowObjectManager = 0x5244FD0;
}offset;

struct variables
{
	DWORD localPlayer;
	DWORD gameModule;
}val;

int main()
{
	int ProcessID = MemClass.getProcess(L"csgo.exe");
	val.gameModule = MemClass.getModule(ProcessID,L"client_panorama.dll");
	val.localPlayer = MemClass.readMem<DWORD>(val.gameModule + offset.dwLocalPlayer);

	if (val.localPlayer == NULL)
		while (val.localPlayer == NULL)

			val.localPlayer = MemClass.readMem<DWORD>(val.gameModule + offset.dwLocalPlayer);
	while (true)
	{
		DWORD glowObject = MemClass.readMem<DWORD>(val.gameModule + offset.dwGlowObjectManager);
		int myTeam = MemClass.readMem<int>(val.localPlayer + offset.m_iTeamNum);
		for (short int i = 0; i < 60; i++)
		{
			DWORD entity = MemClass.readMem<DWORD>(val.gameModule + offset.dwEntityList + i * 0x10);
			if (entity != NULL)
			{
				int glowIndx = MemClass.readMem<int>(entity + offset.m_iGlowIndex);
				int entityTeam = MemClass.readMem<int>(entity + offset.m_iTeamNum);

				if (myTeam == entityTeam)
				{
					MemClass.writeMem<float>(glowObject + ((glowIndx * 0x38) + 0x4),  0);
					MemClass.writeMem<float>(glowObject + ((glowIndx * 0x38) + 0x8),  0);
					MemClass.writeMem<float>(glowObject + ((glowIndx * 0x38) + 0xC),  2);
					MemClass.writeMem<float>(glowObject + ((glowIndx * 0x38) + 0x10), 1);
				}
				else
				{
					MemClass.writeMem<float>(glowObject + ((glowIndx * 0x38) + 0x4), 2);
					MemClass.writeMem<float>(glowObject + ((glowIndx * 0x38) + 0x8), 0);
					MemClass.writeMem<float>(glowObject + ((glowIndx * 0x38) + 0xC), 0);
					MemClass.writeMem<float>(glowObject + ((glowIndx * 0x38) + 0x10),1);
				}
				MemClass.writeMem<bool>(glowObject + ((glowIndx * 0x38) + 0x24), true);
				MemClass.writeMem<bool>(glowObject + ((glowIndx * 0x38) + 0x25), false);
			}
		}
		Sleep(1);
	}
	return 0;
}