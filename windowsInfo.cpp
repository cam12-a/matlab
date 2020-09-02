// coursaVlad.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include <windows.h>
#include <direct.h>
#include <stdio.h>
#include<locale.h>
#include<string>
#include<string.h>
#include<tchar.h>
#include<lmcons.h>
#include<winioctl.h>
#include <strsafe.h>
#include<intrin.h >
#define DIV 1000000000
#define WIDTH 7
#define STORAGE_TEMPERATURE_VALUE_NOT_REPORTED           0x8000
#pragma warning(disable : 4996)
#pragma comment(lib, "user32.lib")
#define BUFSIZE 256

typedef void (WINAPI *PGNSI)(LPSYSTEM_INFO);
typedef BOOL(WINAPI *PGPI)(DWORD, DWORD, DWORD, DWORD, PDWORD);

namespace
{

	struct cpuid_regs
	{
		DWORD   Eax;
		DWORD   Ebx;
		DWORD   Ecx;
		DWORD   Edx;
	};

	std::string SplitIntoChars(DWORD Value)
	{
		std::string Str;
		char const * pCursor = (char const *)&Value;
		for (int i = 0; i < sizeof(Value); ++i) {
			Str += pCursor[i];
		}
		return Str;
	}

	std::string GetCpuVendorSubstring(DWORD Eax)
	{
		cpuid_regs Regs;
		__cpuid((int *)&Regs, Eax);
		std::string Str;
		Str += SplitIntoChars(Regs.Eax);
		Str += SplitIntoChars(Regs.Ebx);
		Str += SplitIntoChars(Regs.Ecx);
		Str += SplitIntoChars(Regs.Edx);
		return Str;
	}

	std::string GetCpuVendorString()
	{
		std::string VendorString;
		cpuid_regs Regs;
		__cpuid((int *)&Regs, 0x80000000);
		if (Regs.Eax >= 0x80000004)
		{
			VendorString =
				GetCpuVendorSubstring(0x80000002) +
				GetCpuVendorSubstring(0x80000003) +
				GetCpuVendorSubstring(0x80000004)
				;
		}
		return VendorString;
	}

} // namespace



int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int fShow)
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(pCmdLine);
	UNREFERENCED_PARAMETER(fShow);

	MessageBoxA(
		NULL,
		GetCpuVendorString().c_str(),
		"INFO - CPUID Vendor String",
		MB_ICONINFORMATION
	);
	return EXIT_SUCCESS;
}
/*
	Description: this function return a list of disk in
	you'r cumputer
	return value none;
*/
void PrintNameOfDisk();
/*
	Description: возвращает число-битовую маску в которой
храняться все доступные диски
	return int
*/
int NumberOfDiskHex();
/*
	Description: This function will return the disk's name
	return : array of char
*/
char * ReturnNameOfDisk();
/*
	Description: This function return the number of disks in
	you'r PC
	return int;
*/
int NumberOfDisk();
/*
	Description: this function will detect the drive's type
	return array of LPCWSTR;
*/
void DriverTypePC();
/*
	Description: This function will return the informations about
	disk
	return char;
*/
/*
	Description: this function print the volume of disk, the file system and the volume name
	return bool;
*/
bool ReturnInformationOfDisk(int &);
/*
	Description: this function print the name of allowed disk;
	return array of char
*/
char* PrintDriverTypePC(int &i);
/*
	Description: this function convert an array char to LPWSTR
	return lpwstr string
*/
LPWSTR ConvertArrayCharToPLCWSTR(wchar_t *wtext, char *driver);
/*
	Description: this function print the information about free space on the disk
	return bool
*/
bool ReturnDiskInfo();
/*
	Description: this funtion return a status of memory
	return  void
*/
void GlobalStatusMemory();
/*
	Description: this funvtion return the current computer name
	return bool;
*/
void ComputerName();
/*
	Description: this function return the system's information
	return none;
*/
void SysInfo();
/*
	Description: this function will return the temperature of CPU
	return int
*/
int GetTempe();
/*
	Description: this function return the bios's version
	return none;
*/
void GetBiosVersion();
/*
	Description: This function will return the information of OC
	return none
*/
BOOL OCInfo(LPTSTR pszOS);


int main(int argc, char **argv)
{
	setlocale(LC_ALL, "Russian");
	TCHAR szOS[BUFSIZE];
	//PrintNameOfDisk();
	//PrintDriverType();
	std::cout << "список дисков "; std::cout << std::endl;
	DriverTypePC();
	std::cout<<std::endl;
	std::cout << "Иформация о диске "; std::cout << std::endl;
	//ReturnInformationOfDisk();
	ReturnDiskInfo();
	std::cout << "Состояние памяти "; std::cout << std::endl;
	GlobalStatusMemory();
	std::cout << std::endl;
	ComputerName();
	std::cout << "Информация о операционной системе "; std::cout << std::endl;
	if (OCInfo(szOS)) _tprintf(TEXT("\n%s\n"), szOS);
	std::cout << std::endl;
	std::cout << "Иформация о процессоре "; std::cout << std::endl;
	SysInfo();
	std::cout << std::endl;
	std::wcout << GetCpuVendorString().c_str();
	std::cout << std::endl;
	//std::cout << GetTempe();
	//GetBiosVersion();
	delete ReturnNameOfDisk();
	//delete DriverTypePC();
	return 0;
}



void PrintNameOfDisk()
{
	if (NumberOfDisk() > 0)
	{
		for (int i = 0; i < NumberOfDisk(); i++)
		{
			std::cout << *(ReturnNameOfDisk() + i) << std::endl;
		}
		delete ReturnNameOfDisk();
	}
}



int NumberOfDiskHex()
{
	DWORD uDriveMask = GetLogicalDrives();

	return uDriveMask;
}



int NumberOfDisk()
{
	int n;
	int i1 = 0;
	for (int i = 0; i < NumberOfDiskHex(); i++)
	{
		n = ((NumberOfDiskHex() >> i) & 0x00000001);
		if (n == 1)
		{
			i1++;
		}
	}
	return i1;
}



char * ReturnNameOfDisk()
{
	int n;
	char dd[4];
	int const ND = NumberOfDisk();
	char *ListDisk;
	ListDisk = new char[ND];
	int i1 = 0;
	for (int i = 0; i < NumberOfDiskHex(); i++)
	{
		n = ((NumberOfDiskHex() >> i) & 0x00000001);
		if (n == 1)
		{
			dd[0] = char(65 + i); dd[1] = ':'; dd[2] = '\\'; dd[3] = 0;
			ListDisk[i1] = char(65 + i);
			i1++;
		}
	}
	return ListDisk;

}


void DriverTypePC()
{
	int d;
	char l[3] = ":\\";
	char driver[4] = "";
	LPCWSTR *a;
	a = new LPCWSTR[NumberOfDisk()];
	for (int i = 0; i < NumberOfDisk(); i++)
	{
		//std::cout<< PrintDriverTypePC(i);
		//driver[] = PrintDriverTypePC(i);
		//ptr = ConvertArrayCharToPLCWSTR(wtext, driver);
		driver[0] = *(ReturnNameOfDisk() + i);
		for (int j = 1; j < 4; j++)
		{
			driver[j] = l[j - 1];
		}
		std::cout << driver;
		wchar_t wtext[4];
		//mbstowcs(wtext, driver, strlen(driver) + 1);//Plus null
		LPWSTR ptr = wtext;
		ptr = ConvertArrayCharToPLCWSTR(wtext, driver);
		//a[i] = ptr;
		d = GetDriveType(ptr);
		if (d == DRIVE_UNKNOWN) std::cout << " Неизвестно" << std::endl;
		if (d == DRIVE_NO_ROOT_DIR) std::cout << " DRIVE NO ROOT DIR" << std::endl;
		if (d == DRIVE_REMOVABLE) std::cout << "USB" << std::endl;
		if (d == DRIVE_FIXED) std::cout << " Диск" << std::endl;
		if (d == DRIVE_REMOTE) std::cout << " REMOTE" << std::endl;
		if (d == DRIVE_CDROM) std::cout << " CDROM" << std::endl;
		if (d == DRIVE_RAMDISK) std::cout << " RAMDISK" << std::endl;

	}
	//return a;

}
char* PrintDriverTypePC(int &i)
{

	char l[3] = ":\\";
	char driver[4] = "";
	char *b;
	b = new char[NumberOfDisk()];
	driver[0] = *(ReturnNameOfDisk() + i);
	b[0] = driver[0];
	for (int j = 1; j < 4; j++)
	{
		b[j] = driver[j] = l[j - 1];
	}
	return b;
}
LPWSTR ConvertArrayCharToPLCWSTR(wchar_t *wtext, char *driver)
{
	mbstowcs(wtext, driver, strlen(driver) + 1);//Plus null
	LPWSTR ptr = wtext;
	return ptr;
}
bool ReturnInformationOfDisk(int &i)
{
	wchar_t wtext[4];
	LPWSTR ptr;
	char *driver;
	bool b = true;
	TCHAR VolumeNameBuffer[100];
	TCHAR FileSystemNameBuffer[100];
	unsigned long VolumeSerialNumber;
	LPDWORD lpMaximumComponentLength = {};
	DWORD  nVolumeNameSize = {};
	LPDWORD lpFileSystemFlags = {};
	
		driver = PrintDriverTypePC(i);
		ptr = ConvertArrayCharToPLCWSTR(wtext, driver);

		if (GetVolumeInformation(ptr, (TCHAR*)VolumeNameBuffer, 100, &VolumeSerialNumber, NULL, NULL, FileSystemNameBuffer, 100))
		{
			/*std::cout <<"Disk "<< PrintDriverTypePC(i);
			std::wcout << " Volume Name is " << VolumeNameBuffer;
			std::wcout << " " << VolumeSerialNumber;
			std::wcout << " " << lpMaximumComponentLength ;
			std::wcout << " " << lpFileSystemFlags;
			//printf("VolumeSerialNumber: 0x%X\n", VolumeSerialNumber);*/
			_tprintf(TEXT(" Файловая система  "));std::wcout<< FileSystemNameBuffer << std::endl;
			//std::wcout << " " << "Файловая система " << FileSystemNameBuffer << std::endl;

		}
		else {
			std::wcout << PrintDriverTypePC(i) << " Невозможно определить объем " << std::endl;
		}
	return true;
}
bool ReturnDiskInfo()
{
	DWORDLONG FreeBytesAvailable;
	DWORDLONG TotalNumberOfBytes;
	DWORDLONG TotalNumberOfFreeBytes;
	wchar_t wtext[4];
	LPWSTR ptr;
	char *driver;
	bool b = true;
	int d;
	for (int i = 0; i < NumberOfDisk(); i++)
	{
		driver = PrintDriverTypePC(i);
		ptr = ConvertArrayCharToPLCWSTR(wtext, driver);
		d = GetDriveType(ptr);
		if (d == DRIVE_FIXED)
		{
			ReturnInformationOfDisk(i);
			if (GetDiskFreeSpaceEx(ptr, (PULARGE_INTEGER)&FreeBytesAvailable, (PULARGE_INTEGER)&TotalNumberOfBytes, (PULARGE_INTEGER)&TotalNumberOfFreeBytes))
			{
				std::cout << PrintDriverTypePC(i) <<std::endl;
				printf("Доступно  =  %I64u ГБ\t ", FreeBytesAvailable /(1024 * 1024 * 1024));
				//std::cout << PrintDriverTypePC(i) << " Total Number Of Free Bytes = " << (unsigned long)TotalNumberOfFreeBytes<< std::endl;
				printf("Всего = %I64u ГБ\t", TotalNumberOfBytes / (1024 * 1024 * 1024));
				printf("Свободно = %I64u ГБ\t", TotalNumberOfFreeBytes / (1024 * 1024 * 1024));


				//std::cout << PrintDriverTypePC(i) << " Total Number Of Bytes = " << (unsigned long)TotalNumberOfBytes / (1024 * 1024*1024)<< std::endl;
			}
			else {
				std::wcout << PrintDriverTypePC(i) << " Невозможно определить объем " << std::endl;

			}
		}

	}

	return true;
}

void GlobalStatusMemory()
{
	MEMORYSTATUSEX statex;

	statex.dwLength = sizeof(statex);

	GlobalMemoryStatusEx(&statex);
	_tprintf(TEXT("Процент используемый память  %*ld .\n"),
		WIDTH, statex.dwMemoryLoad);
	_tprintf(TEXT("Объем физической памяти %*I64d ГБ.\t"),
		WIDTH, statex.ullTotalPhys/DIV);
	_tprintf(TEXT("Объем  физической свободной памяти %*I64d  МБ .\n"),
		WIDTH, statex.ullAvailPhys/(1000*1000));
	_tprintf(TEXT("Объем виртуальной памяти  %*I64d ГБ.\t"),
		WIDTH, statex.ullTotalVirtual/ DIV);
	_tprintf(TEXT("Объем виртуальной свободной памяти  %*I64d МБ.\t"),
		WIDTH, statex.ullAvailVirtual / (1000 * 1000));

}
void ComputerName()
{
	WCHAR ComputerName[MAX_COMPUTERNAME_LENGTH + 1];
	unsigned long len_ComputerName = MAX_COMPUTERNAME_LENGTH + 1;
	char UserName[UNLEN + 1];
	unsigned long len_UserName = UNLEN + 1;
	std::cout << std::endl;
	if (GetComputerName((WCHAR*)ComputerName, &len_ComputerName))
		_tprintf(TEXT("Имя компьютера %s \n"), ComputerName);
		//std::wcout << " computer Name " << ComputerName << std::endl;
	else
		std::cout << "Не удалось найти имя компьютера !!! " << std::endl;
	if (GetUserNameA(UserName, &len_UserName))
		std::cout << "Имя пользователя " << UserName << std::endl;
	else
		std::cout << "Не удалось найти имя пользователя !!! " << std::endl;
}
void SysInfo()
{
	SYSTEM_INFO siSysInfo;

	// Copy the hardware information to the SYSTEM_INFO structure. 

	GetSystemInfo(&siSysInfo);

	// Display the contents of the SYSTEM_INFO structure. 

	//printf("  OEM ID: %u\n", siSysInfo.dwOemId);
	printf("  Количество ядра: %u\n",
		siSysInfo.dwNumberOfProcessors);
	//printf("  Page size: %u\n", siSysInfo.dwPageSize);
	//printf("  Тип процесора: %u\n", siSysInfo.dwProcessorType);
	printf("  Минимальный адрес : %lx\n",
		siSysInfo.lpMinimumApplicationAddress);
	printf("  Максимальный адрес : %lx\n",
		siSysInfo.lpMaximumApplicationAddress);
}

int GetTempe()
{
	STORAGE_TEMPERATURE_INFO temp = {};
	return temp.Temperature;
}
void GetBiosVersion()
{
	HKEY rKey;
	TCHAR Reget[256];
	DWORD RegetPath = sizeof(Reget);
	RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"HARDWARE\\DESCRIPTION\\System\\BIOS", NULL, KEY_QUERY_VALUE, &rKey);
	RegQueryValueEx(rKey, L"BIOSVersion", NULL, NULL, (LPBYTE)&Reget, &RegetPath);
	std::wcout << Reget;
	//printf("%ls", Reget);
}
BOOL OCInfo(LPTSTR pszOS)
{
	OSVERSIONINFOEX osvi;
	SYSTEM_INFO si;
	PGNSI pGNSI;
	PGPI pGPI;
	BOOL bOsVersionInfoEx;
	DWORD dwType;

	ZeroMemory(&si, sizeof(SYSTEM_INFO));
	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));

	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO*)&osvi);

	if (!bOsVersionInfoEx) return 1;

	pGNSI = (PGNSI)GetProcAddress(
		GetModuleHandle(TEXT("kernel32.dll")),
		"GetNativeSystemInfo");
	if (NULL != pGNSI)
		pGNSI(&si);
	else GetSystemInfo(&si);
	
	if (VER_PLATFORM_WIN32_NT == osvi.dwPlatformId &&
		osvi.dwMajorVersion > 4)
	{
		StringCchCopy(pszOS, BUFSIZE, TEXT("Microsoft "));

		// Test for the specific product.

		if (osvi.dwMajorVersion == 6)
		{
			if (osvi.dwMinorVersion == 0)
			{
				if (osvi.wProductType == VER_NT_WORKSTATION)
					StringCchCat(pszOS, BUFSIZE, TEXT("Windows Vista "));
				else StringCchCat(pszOS, BUFSIZE, TEXT("Windows Server 2008 "));
			}

			if (osvi.dwMinorVersion == 1 || osvi.dwMinorVersion == 2)
			{
				if (osvi.wProductType == VER_NT_WORKSTATION && osvi.dwMinorVersion == 1)
					StringCchCat(pszOS, BUFSIZE, TEXT("Windows 7 "));
				else
					if (osvi.wProductType == VER_NT_WORKSTATION && osvi.dwMinorVersion == 2)
						StringCchCat(pszOS, BUFSIZE, TEXT("Windows 8 "));
					else
						StringCchCat(pszOS, BUFSIZE, TEXT("Windows Server 2008 R2 "));
			}

			pGPI = (PGPI)GetProcAddress(
				GetModuleHandle(TEXT("kernel32.dll")),
				"GetProductInfo");

			pGPI(osvi.dwMajorVersion, osvi.dwMinorVersion, 0, 0, &dwType);

			switch (dwType)
			{
			case PRODUCT_ULTIMATE:
				StringCchCat(pszOS, BUFSIZE, TEXT("Ultimate Edition"));
				break;
			case PRODUCT_PROFESSIONAL:
				StringCchCat(pszOS, BUFSIZE, TEXT("Professional"));
				break;
			case PRODUCT_HOME_PREMIUM:
				StringCchCat(pszOS, BUFSIZE, TEXT("Home Premium Edition"));
				break;
			case PRODUCT_HOME_BASIC:
				StringCchCat(pszOS, BUFSIZE, TEXT("Home Basic Edition"));
				break;
			case PRODUCT_ENTERPRISE:
				StringCchCat(pszOS, BUFSIZE, TEXT("Enterprise Edition"));
				break;
			case PRODUCT_BUSINESS:
				StringCchCat(pszOS, BUFSIZE, TEXT("Business Edition"));
				break;
			case PRODUCT_STARTER:
				StringCchCat(pszOS, BUFSIZE, TEXT("Starter Edition"));
				break;
			case PRODUCT_CLUSTER_SERVER:
				StringCchCat(pszOS, BUFSIZE, TEXT("Cluster Server Edition"));
				break;
			case PRODUCT_DATACENTER_SERVER:
				StringCchCat(pszOS, BUFSIZE, TEXT("Datacenter Edition"));
				break;
			case PRODUCT_DATACENTER_SERVER_CORE:
				StringCchCat(pszOS, BUFSIZE, TEXT("Datacenter Edition (core installation)"));
				break;
			case PRODUCT_ENTERPRISE_SERVER:
				StringCchCat(pszOS, BUFSIZE, TEXT("Enterprise Edition"));
				break;
			case PRODUCT_ENTERPRISE_SERVER_CORE:
				StringCchCat(pszOS, BUFSIZE, TEXT("Enterprise Edition (core installation)"));
				break;
			case PRODUCT_ENTERPRISE_SERVER_IA64:
				StringCchCat(pszOS, BUFSIZE, TEXT("Enterprise Edition for Itanium-based Systems"));
				break;
			case PRODUCT_SMALLBUSINESS_SERVER:
				StringCchCat(pszOS, BUFSIZE, TEXT("Small Business Server"));
				break;
			case PRODUCT_SMALLBUSINESS_SERVER_PREMIUM:
				StringCchCat(pszOS, BUFSIZE, TEXT("Small Business Server Premium Edition"));
				break;
			case PRODUCT_STANDARD_SERVER:
				StringCchCat(pszOS, BUFSIZE, TEXT("Standard Edition"));
				break;
			case PRODUCT_STANDARD_SERVER_CORE:
				StringCchCat(pszOS, BUFSIZE, TEXT("Standard Edition (core installation)"));
				break;
			case PRODUCT_WEB_SERVER:
				StringCchCat(pszOS, BUFSIZE, TEXT("Web Server Edition"));
				break;
			}
		}

		if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2)
		{
			if (GetSystemMetrics(SM_SERVERR2))
				StringCchCat(pszOS, BUFSIZE, TEXT("Windows Server 2003 R2, "));
			else if (osvi.wSuiteMask & VER_SUITE_STORAGE_SERVER)
				StringCchCat(pszOS, BUFSIZE, TEXT("Windows Storage Server 2003"));
			else if (osvi.wSuiteMask & VER_SUITE_WH_SERVER)
				StringCchCat(pszOS, BUFSIZE, TEXT("Windows Home Server"));
			else if (osvi.wProductType == VER_NT_WORKSTATION &&
				si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
			{
				StringCchCat(pszOS, BUFSIZE, TEXT("Windows XP Professional x64 Edition"));
			}
			else StringCchCat(pszOS, BUFSIZE, TEXT("Windows Server 2003, "));

			// Test for the server type.
			if (osvi.wProductType != VER_NT_WORKSTATION)
			{
				if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)
				{
					if (osvi.wSuiteMask & VER_SUITE_DATACENTER)
						StringCchCat(pszOS, BUFSIZE, TEXT("Datacenter Edition for Itanium-based Systems"));
					else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
						StringCchCat(pszOS, BUFSIZE, TEXT("Enterprise Edition for Itanium-based Systems"));
				}

				else if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
				{
					if (osvi.wSuiteMask & VER_SUITE_DATACENTER)
						StringCchCat(pszOS, BUFSIZE, TEXT("Datacenter x64 Edition"));
					else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
						StringCchCat(pszOS, BUFSIZE, TEXT("Enterprise x64 Edition"));
					else StringCchCat(pszOS, BUFSIZE, TEXT("Standard x64 Edition"));
				}

				else
				{
					if (osvi.wSuiteMask & VER_SUITE_COMPUTE_SERVER)
						StringCchCat(pszOS, BUFSIZE, TEXT("Compute Cluster Edition"));
					else if (osvi.wSuiteMask & VER_SUITE_DATACENTER)
						StringCchCat(pszOS, BUFSIZE, TEXT("Datacenter Edition"));
					else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
						StringCchCat(pszOS, BUFSIZE, TEXT("Enterprise Edition"));
					else if (osvi.wSuiteMask & VER_SUITE_BLADE)
						StringCchCat(pszOS, BUFSIZE, TEXT("Web Edition"));
					else StringCchCat(pszOS, BUFSIZE, TEXT("Standard Edition"));
				}
			}
		}

		if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1)
		{
			StringCchCat(pszOS, BUFSIZE, TEXT("Windows XP "));
			if (osvi.wSuiteMask & VER_SUITE_PERSONAL)
				StringCchCat(pszOS, BUFSIZE, TEXT("Home Edition"));
			else StringCchCat(pszOS, BUFSIZE, TEXT("Professional"));
		}

		if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0)
		{
			StringCchCat(pszOS, BUFSIZE, TEXT("Windows 2000 "));

			if (osvi.wProductType == VER_NT_WORKSTATION)
			{
				StringCchCat(pszOS, BUFSIZE, TEXT("Professional"));
			}
			else
			{
				if (osvi.wSuiteMask & VER_SUITE_DATACENTER)
					StringCchCat(pszOS, BUFSIZE, TEXT("Datacenter Server"));
				else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
					StringCchCat(pszOS, BUFSIZE, TEXT("Advanced Server"));
				else StringCchCat(pszOS, BUFSIZE, TEXT("Server"));
			}
		}

		// Include service pack (if any) and build number.

		if (_tcslen(osvi.szCSDVersion) > 0)
		{
			StringCchCat(pszOS, BUFSIZE, TEXT(" "));
			StringCchCat(pszOS, BUFSIZE, osvi.szCSDVersion);
		}

		TCHAR buf[80];

		StringCchPrintf(buf, 80, TEXT(" (build %d)"), osvi.dwBuildNumber);
		StringCchCat(pszOS, BUFSIZE, buf);

		if (osvi.dwMajorVersion >= 6)
		{
			if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
				StringCchCat(pszOS, BUFSIZE, TEXT(", 64-bit"));
			else if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL)
				StringCchCat(pszOS, BUFSIZE, TEXT(", 32-bit"));
		}
		//std::cout << si.dwProcessorType ;
		return TRUE;
	}

	else
	{
		printf("Ошибка .\n");
		return FALSE;
	}
	
}

