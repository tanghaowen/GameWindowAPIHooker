#include <Windows.h>
#include <stdio.h>

bool InjectDLL( DWORD Pid );


void main()
{
	STARTUPINFO si = {sizeof(si)}; 
	PROCESS_INFORMATION pi;
	bool stats;
	stats = CreateProcess(  TEXT("�����o�l���ץ�󥷥ѥ뤿�������ա�.exe") , NULL , NULL , NULL ,FALSE , CREATE_SUSPENDED , NULL , NULL, &si ,&pi );

	if ( !stats )
	{
		MessageBox(NULL , TEXT("��������ʧ��") ,TEXT("��������ʧ��"), MB_OK );
	}

	if ( InjectDLL( pi.dwProcessId ) )
	{
		MessageBox(NULL , TEXT("HOOK�ɹ�") ,TEXT("HOOK�ɹ�"), MB_OK );

	}


	getchar();
	ResumeThread( pi.hThread );

	getchar();


}

bool InjectDLL( DWORD Pid )
 {
    LPCSTR lpDllName = "API hooker.dll"; 
    DWORD dwSize = strlen(lpDllName)+1;  
    DWORD dwHasWrite;  
	LPVOID lpRemoteBuf;	
	
	HANDLE hProcess = OpenProcess(PROCESS_VM_OPERATION|PROCESS_VM_WRITE,FALSE,Pid); 
 
	if(hProcess == NULL) 
    { 
		MessageBox( NULL , TEXT("get hProcess error!") ,  TEXT("get hProcess error!") ,MB_OK);
            return false; 
    } 

    lpRemoteBuf = VirtualAllocEx(hProcess,NULL,dwSize,MEM_COMMIT,PAGE_READWRITE); 

    if(WriteProcessMemory(hProcess,lpRemoteBuf,lpDllName,dwSize,&dwHasWrite)) 
    { 
        if(dwHasWrite != dwSize) 
        { 
            VirtualFreeEx(hProcess,lpRemoteBuf,dwSize,MEM_COMMIT); 
            CloseHandle(hProcess); 
            return false; 
        }     
    }
	else 
    { 
		MessageBox( NULL , TEXT("д��Զ�̽����ڴ�ռ����!") ,  TEXT("д��Զ�̽����ڴ�ռ����!") ,MB_OK); 
        CloseHandle(hProcess); 
        return false; 
    } 


     
   DWORD dwNewThreadId; 
   LPVOID lpLoadDll = LoadLibraryA; 
   HANDLE hNewRemoteThread = CreateRemoteThread(hProcess,NULL,0,(LPTHREAD_START_ROUTINE)lpLoadDll,lpRemoteBuf,0,&dwNewThreadId); 

   if(hNewRemoteThread == NULL) 
   {
	    MessageBox( NULL , TEXT("����Զ���߳�ʧ��!") ,  TEXT("����Զ���߳�ʧ��!") ,MB_OK); 
        CloseHandle(hProcess); 
        return false; 
    } 


   return true;

}

