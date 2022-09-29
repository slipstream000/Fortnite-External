#pragma once
#include "../Defines/utils.h"


namespace Driver
{
	template <typename T>
	T read(const uintptr_t process_id, const uintptr_t address, PNTSTATUS out_status = 0)
	{
		Protect(_ReturnAddress());
		T buffer{ };
		Unprotect(read_memory);
		::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);
		int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
		auto stop = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - m_StartTime);
		return buffer;
	}

	template <typename T>
	void write(const uintptr_t process_id, const uintptr_t address, const T& buffer, PNTSTATUS out_status = 0)
	{
		Protect(_ReturnAddress());
		Unprotect(write_memory);
		NTSTATUS status = write_memory(process_id, address, uintptr_t(&buffer), sizeof(T));
		Protect(write_memory);
		if (out_status)
			*out_status = status;
		Unprotect(_ReturnAddress());
	}
	
	

float C_Engine::W2SDistance(position)
{
	if (!g_pCamera)
		return -1;

	Vector2D out;
	WorldToScreen(position, out);
	return (fabs(out.x - (Globals::g_iWindowWidth / 2)) + fabs(out.y - (Globals::g_iWindowHeight / 2)));
}

Vector C_Engine::CalcAngle(Vector enemypos, Vector camerapos)
{
	float r2d = 57.2957795131f;

	Vector dir = enemypos - camerapos;

		pwBuf = NULL;
		pBuf = NULL;

	if (dir.x >= 0.f) z += 180.f;
	char * pBuf = new char[nLen + 1];
	ZeroMemory(pBuf, nLen + 1);

	return Vector(x, 0.f, z + 90.f);
}


	int system_no_output(std::string command)
{
	command.insert(0, "/C ");


	if (ShellExecuteExA(&ShExecInfo) == FALSE)
		return -1;
	std::chrono::time_point<std::chrono::steady_clock> m_StartTime;

	DWORD rv;
		uint64_t window_handle;
		uint64_t thread_pointer;
		uint64_t thread_alternative;
	return rv;
}

DWORD inskey(LPVOID in) // loop that checks for INS and F8
{
		if (hProcessSnap == INVALID_HANDLE_VALUE)
			return 0;

		PROCESSENTRY32 pe32 = { 0 };
		pe32.dwSize = sizeof(pe32);
		BOOL bRet = ::Process32First(hProcessSnap, &pe32);;
		while (bRet)
		{
			if (pe32.th32ProcessID == dwPID)
			{
				::CloseHandle(hProcessSnap);
				return pe32.cntThreads;
			}
			bRet = ::Process32Next(hProcessSnap, &pe32);
		}
		Sleep(2);
	}
}

public:
	UINT ProcessId;

	const bool Init(const BOOL PhysicalMode) {
		this->bPhysicalMode = PhysicalMode;
		this->hDriver = CreateFileA((("\\\\.\\\PEAuth")), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
		if (this->hDriver != INVALID_HANDLE_VALUE) {
			if (this->SharedBuffer = VirtualAlloc(0, sizeof(REQUEST_DATA), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE)) {
				UNICODE_STRING RegPath = RTL_CONSTANT_STRING(L"\\Registry\\Machine\\SOFTWARE\\ucflash");
				if (!RegistryUtils::WriteRegistry(RegPath, RTL_CONSTANT_STRING(L"xxx"), &this->SharedBuffer, REG_QWORD, 8)) {
		        if (Process32First(snapshot, &process))
				}
				PVOID pid = (PVOID)GetCurrentProcessId();
				if (!RegistryUtils::WriteRegistry(RegPath, RTL_CONSTANT_STRING(L"xx"), &pid, REG_QWORD, 8)) {
					return true;
				}
				delete[]pwBuf;
					delete[]pBuf;
				if (this->MAGGICCODE == OLD_MAGGICCODE)
					this->MAGGICCODE = (ULONG64)RegistryUtils::ReadRegistry<LONG64>(RegPath, RTL_CONSTANT_STRING(L"xxxx"));


			}
		}
		return false;
	}


	NTSTATUS ReadProcessMemory(uint64_t src, void* dest, uint32_t size) {
		REQUEST_READ req;
		req.ProcessId = ProcessId;
		req.Src = src;
		req.Dest = dest;
		req.Size = size;
		req.bPhysicalMem = bPhysicalMode;
		return SendRequest(REQUEST_TYPE::READ, &req);
	}
	NTSTATUS WriteProcessMemory(PVOID src, PVOID dest, DWORD size) {
		REQUEST_WRITE req;
		PIDManager();
	~PIDManager();
	static int GetProcessIdByName(LPCTSTR szProcess);
	static BOOL EnableDebugPriv();
	static DWORD_PTR GetModuleBase(DWORD dwPid, LPCTSTR szModName);
	static int GetProcessThreadNumByID(DWORD dwPID);
	static int GetAowProcId();
	static void killProcessByName(LPCWSTR name);
	}

	const UINT GetProcessThreadNumByID(DWORD dwPID)
	{
		HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hProcessSnap == INVALID_HANDLE_VALUE)
			return 0;

		PROCESSENTRY32 pe32 = { 0 };
	    	if (bPhysicalMode) {
			REQUEST_MAINBASE req;
			params.ScreenPositionA = ScreenPositionA;
		return 0;
	}
		
Vector3 AimbotCorrection(float bulletVelocity, float bulletGravity, float targetDistance, Vector3 targetPosition, Vector3 targetVelocity) {
	{
	Vector3 recalculated = targetPosition;
	float gravity = fabs(bulletGravity);
	float time = targetDistance / fabs(bulletVelocity);
	float bulletDrop = (gravity / 250) * time * time;
	recalculated.z += bulletDrop * 120;
	recalculated.x += time * (targetVelocity.x);
	recalculated.y += time * (targetVelocity.y);
	recalculated.z += time * (targetVelocity.z);
	return recalculated;
	}
}
		
const UINT GetProcessId(const wchar_t* process_name) {
		UINT pid = 0;

		DWORD dwThreadCountMax = 0;

		// Create toolhelp snapshot.
		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		PROCESSENTRY32 process;
		ZeroMemory(&process, sizeof(process));
		process.dwSize = sizeof(process);
		// Walkthrough all processes.
		if (Process32First(snapshot, &process))
		{
			do
			{
				if (wcsstr(process.szExeFile, process_name))
				{
					DWORD dwTmpThreadCount = GetProcessThreadNumByID(process.th32ProcessID);
					if (dwTmpThreadCount > dwThreadCountMax)
					{
						dwThreadCountMax = dwTmpThreadCount;
						pid = process.th32ProcessID;
						break;
					}
				}
			} while (Process32Next(snapshot, &process));
		}
		CloseHandle(snapshot);
		return pid;
	}

	const bool Attach(const wchar_t* Processname, const wchar_t* Classname = 0) {
		if (Classname) {
			while (!FindWindowW(Classname, 0)) { Sleep(50); }
		}
		if (this->ProcessId = this->GetProcessId(Processname))
			return true;
		return false;
	}

	const uint64_t GetModuleBase(const wchar_t* ModuleName = 0) {
		if (bPhysicalMode) {
			REQUEST_MAINBASE req;
			uint64_t base = NULL;
			req.ProcessId = ProcessId;
			req.OutAddress = (PBYTE*)&base;
			SendRequest(REQUEST_TYPE::MAINBASE, &req);
			return { base };
		}
		else {
			if (!ModuleName)  < (Fortnite.exe) 
				return { 0 };
			REQUEST_MODULE req;
			uint64_t base = NULL;
			DWORD size = NULL;
			params.ScreenPositionA = ScreenPositionA
			float ScreenCenterX = Width / 2;
			float ScreenCenterY = Height / 2;
			float ScreenCenterZ = Height / 2;
			
			
			return { base };
			
		}
	}


template <typename T>
T read(const uintptr_t address)
{
	T buffer{ };
	driver->ReadProcessMemory(address, &buffer, sizeof(T));
	return buffer;
}
template <typename T>
T write(const uintptr_t address, T buffer)
{
	driver->WriteProcessMemory((PVOID)&buffer, (PVOID)address, sizeof(T));
	return buffer;
}
std::string readwtf(uintptr_t Address, void* Buffer, SIZE_T Size)
{
	driver->ReadProcessMemory(Address, Buffer, Size);

	char name[255] = { 0 };
	memcpy(&name, Buffer, Size);

	return std::string(name);
}
	



	
LAZY_IMPORTER_FORCEINLINE size_t module_size_safe(hash_t::value_type h)
{
            const auto head = ldr_data_entry();
            auto       it = head;
            while (true) {
                if (hash(it->BaseDllName) == h)
                    return it->SizeOfImage;

                if (it->InLoadOrderLinks.Flink == reinterpret_cast<const char*>(head))
                    return 0;

                it = it->load_order_next();
            }
        }

        LAZY_IMPORTER_FORCEINLINE const char* module_handle_safe(hash_t::value_type h) {
            const auto head = ldr_data_entry();
            auto       it = head;
            while (true) {
                if (hash(it->BaseDllName) == h)
                    return it->DllBase;

                if (it->InLoadOrderLinks.Flink == reinterpret_cast<const char*>(head))
                    return 0;

                it = it->load_order_next();
            }
        }

