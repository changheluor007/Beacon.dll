#pragma once
#ifndef _PIPE_CMD_H_
#define _PIPE_CMD_H_
#include "stdafx.h"
#include "stdio.h"


// ִ�� cmd ����, ����ȡִ�н������
BOOL PipeCmd(char* pszCmd, char* pszResultBuffer, DWORD dwResultBufferSize)
{
	HANDLE hReadPipe = NULL;
	HANDLE hWritePipe = NULL;
	SECURITY_ATTRIBUTES securityAttributes = { 0 };
	BOOL bRet = FALSE;
	STARTUPINFOA si = { 0 };
	PROCESS_INFORMATION pi = { 0 };

	// �趨�ܵ��İ�ȫ����
	securityAttributes.bInheritHandle = TRUE;
	securityAttributes.nLength = sizeof(securityAttributes);
	securityAttributes.lpSecurityDescriptor = NULL;
	// ���������ܵ�
	bRet = ::CreatePipe(&hReadPipe, &hWritePipe, &securityAttributes, 0);
	if (FALSE == bRet)
	{
		printf("CreatePipe");
		return FALSE;
	}
	// �����½��̲���
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	si.wShowWindow = SW_HIDE;
	si.hStdError = hWritePipe;
	si.hStdOutput = hWritePipe;
	// �����½���ִ������, ��ִ�н��д�������ܵ���
	bRet = ::CreateProcessA(NULL, pszCmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
	if (FALSE == bRet)
	{
		printf("CreateProcess");
	}
	// �ȴ�����ִ�н���
	::WaitForSingleObject(pi.hThread, INFINITE);
	::WaitForSingleObject(pi.hProcess, INFINITE);
	// �������ܵ��ж�ȡ��������������
	::RtlZeroMemory(pszResultBuffer, dwResultBufferSize);
	::ReadFile(hReadPipe, pszResultBuffer, dwResultBufferSize, NULL, NULL);
	// �رվ��, �ͷ��ڴ�
	::CloseHandle(pi.hThread);
	::CloseHandle(pi.hProcess);
	::CloseHandle(hWritePipe);
	::CloseHandle(hReadPipe);

	return TRUE;
}

#endif
