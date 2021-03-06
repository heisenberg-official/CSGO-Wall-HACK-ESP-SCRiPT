#include <vector>
#include <iomanip>
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

class BlueMethMem
{
public:
	 BlueMethMem();
	~BlueMethMem();

	template <class val>
	val readMem(uintptr_t addr)
	{
		val x;
		ReadProcessMemory(handle, (LPBYTE*)addr, &x, sizeof(x), NULL);
		return x;
	}

	template <class val>
	val WMemory(uintptr_t addr, val x)
	{
		WriteProcessMemory(handle, (LPBYTE*)addr, &x, sizeof(x), NULL);
		return x;
	}

	uintptr_t getProcess(const wchar_t*);
	uintptr_t getModule(uintptr_t, const wchar_t*);
	uintptr_t getAddress(uintptr_t, std::vector<uintptr_t>);

private:
	HANDLE handle;
};

BlueMethMem::BlueMethMem() { handle = NULL; }

BlueMethMem::~BlueMethMem()
{
	CloseHandle(handle);
}

uintptr_t BlueMethMem::getProcess(const wchar_t* proc)
{
	HANDLE hProcessId = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	uintptr_t process;
	PROCESSENTRY32 pEntry;
	pEntry.dwSize = sizeof(pEntry);
	do
	{
		if (!_wcsicmp(pEntry.szExeFile, proc))
		{
			process = pEntry.th32ProcessID;
			CloseHandle(hProcessId);
			handle = OpenProcess(PROCESS_ALL_ACCESS, false, process);
		}
	} while (Process32Next(hProcessId, &pEntry));
	return process;
}

uintptr_t BlueMethMem::getModule(uintptr_t procId, const wchar_t* modName)
{
	HANDLE hModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
	MODULEENTRY32 mEntry;
	mEntry.dwSize = sizeof(mEntry);
	do
	{
		if (!_wcsicmp(mEntry.szModule, modName))
		{
			CloseHandle(hModule);
			return (uintptr_t)mEntry.hModule;
		}
	} while (Module32Next(hModule, &mEntry));
	return 0;
}

uintptr_t BlueMethMem::getAddress(uintptr_t addr, std::vector<uintptr_t> vect)
{
	for (int i = 0; i < vect.size(); i++)
	{
		ReadProcessMemory(handle, (BYTE*)addr, &addr, sizeof(addr), 0);
		addr += vect[i];
	}
	return addr;
}

BlueMethMem MethClass;

struct offset
{
	DWORD m_iTeamNum = 0xF4;
	DWORD m_iGlowIndex = 0xA40C;
	DWORD dwEntityList = 0x4D07DD4;
	DWORD dwLocalPlayer = 0xCF5A4C;
	DWORD dwGlowOBJManager = 0x5248228;
}offset;

struct variables
{
	DWORD localPlayer;
	DWORD gameModule;
}val;

int main()
{
	std::string UserMain = "HEiSENBERG", Passwd = "spacex", USER, PASS;int LL = 0;
	std::cout << "[-------------------   [HEiSENBERG.IO]   ------------------]\n";
	std::cout << "[------ ENTER YOUR LOGiN CREDENTiALS [HEiSENBERG.ME] ------]\n";
	std::cout << "ENTER YOUR USERNAME: >>> "; std::cin >> USER;
	std::cout << "ENTER YOUR PASSWORD: >>> "; std::cin >> PASS;

	if (UserMain != USER || Passwd != PASS)
	{
		std::cout << "\nBAD-AUTH >>> [IP-WiLL-BE-REPORTED-TO-ViSWANATHAN]\n";
		for (int i = 0; i < 5; i++)
		{
			Beep(2250, 500);
		}
		Sleep(2000);
		exit(0);
	}
	else
	{
		system("cls"); std::cout << "\nAUTH-SUCCESS >>> [DOWNLOADING-BLUE-METH-FROM-SJT]\n"; Beep(250, 1000);
		std::cout << "WAiT >>> ";
		while (LL < 10)
		{
			std::cout << "[<=>]";
			Sleep(500);
			LL++;
		}
	}

	MessageBoxA(0, "HEiSENBERG WiLL NOW INJECT BLUE METH IN YOUR RAM", "HEiSENBERG.ME", 0);
	MessageBoxA(0, "[SUCCESS] - FEEL THE GAMESENSE", "HEiSENBERG.ME", 0);
	MessageBoxA(0, "[1] DiSCORD - HEiSENBERG#0487\n[2] STEAM - https://steamcommunity.com/id/-heisen-official/", "CONTACT", 0);
	printf("\n\n[USAGE]\n[1]-ViSHNU-THiRD-EYE-ESP  [INSERT-KEY]\n[2]-EXiT-UNHOOK [END-KEY]\n");

	int ProcessID = MethClass.getProcess(L"csgo.exe");
	val.gameModule = MethClass.getModule(ProcessID, L"client_panorama.dll");
	val.localPlayer = MethClass.readMem<DWORD>(val.gameModule + offset.dwLocalPlayer);

	if (val.localPlayer == NULL)
		while (val.localPlayer == NULL)
			val.localPlayer = MethClass.readMem<DWORD>(val.gameModule + offset.dwLocalPlayer);

	bool KEY_TOG = false;

	while (true)
	{
		DWORD GlowOBJ = MethClass.readMem<DWORD>(val.gameModule + offset.dwGlowOBJManager);
		int myTeam = MethClass.readMem<int>(val.localPlayer + offset.m_iTeamNum);
		for (short int i = 0; i < 60; i++)
		{

			DWORD entity = MethClass.readMem<DWORD>(val.gameModule + offset.dwEntityList + i * 0x10);

			if (KEY_TOG && entity != NULL)
			{
				int GlowINDX = MethClass.readMem<int>(entity + offset.m_iGlowIndex);
				int entityTeam = MethClass.readMem<int>(entity + offset.m_iTeamNum);

				if (myTeam == entityTeam)
				{
					MethClass.WMemory<float>(GlowOBJ + ((GlowINDX * 0x38) + 0x4), 0);
					MethClass.WMemory<float>(GlowOBJ + ((GlowINDX * 0x38) + 0x8), 0);
					MethClass.WMemory<float>(GlowOBJ + ((GlowINDX * 0x38) + 0xC), 2);
					MethClass.WMemory<float>(GlowOBJ + ((GlowINDX * 0x38) + 0x10),1);
				}
				else
				{
					MethClass.WMemory<float>(GlowOBJ + ((GlowINDX * 0x38) + 0x4), 2);
					MethClass.WMemory<float>(GlowOBJ + ((GlowINDX * 0x38) + 0x8), 0);
					MethClass.WMemory<float>(GlowOBJ + ((GlowINDX * 0x38) + 0xC), 0);
					MethClass.WMemory<float>(GlowOBJ + ((GlowINDX * 0x38) + 0x10),1);
				}

				MethClass.WMemory<bool>(GlowOBJ + ((GlowINDX * 0x38) + 0x24), true);
				MethClass.WMemory<bool>(GlowOBJ + ((GlowINDX * 0x38) + 0x25), false);
			}

			if (GetAsyncKeyState(VK_INSERT))
			{
				if (KEY_TOG == false)
				{
					Sleep(250);
					KEY_TOG = true;
				}
				else if (KEY_TOG == true)
				{
					Sleep(250);
					KEY_TOG = false;
				}
			}

			if (GetAsyncKeyState(VK_END))
			{
				exit(0);
			}
		}
		
		Sleep(1);
	}
	return 0;
}
