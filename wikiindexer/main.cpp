#include <iostream>
#define _MBCS
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <conio.h>
#include <windows.h>
#include <forward_list>
#include <string>

using namespace std;

forward_list<string> vfiles;
forward_list<string>::iterator iter;
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

void HandleError()
{
	TCHAR errMes[1024];
	TCHAR str[1024];
	int err;
	err=GetLastError();
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,NULL,err,	0,errMes,1024,NULL);
	wsprintf(str,"에러코드=%d, 메시지=%s",err,errMes);
	MessageBox(0,str,"에러",MB_OK);
	GetLastError();
}


TUCHAR* GetLineOfString(_TUCHAR **szBuf);
void MarkIndices(_TUCHAR *data,FILE *outf,_TUCHAR *inf)
{
	_TUCHAR *szBuf=data;
	int index=0;
	for(; *data!=_T('\0'); data=_tcsinc(data))
	{
		_TUCHAR *whereCLCL=_tcsstr(data,_T("\n\n"));
		if(!whereCLCL)
		{
			break;
		}
		whereCLCL=_tcsninc(whereCLCL,2);
		_TUCHAR *tmp=GetLineOfString(&whereCLCL);
		_ftprintf(outf,"%s: ",inf);
		_fputts(tmp,outf);
		free(tmp);
		int read=szBuf-data;
		++index;
		if(read%100000==0)
		{
			_tprintf("bytes read:%d indices processed:%d",read,index);
		}
	}
}
_TUCHAR* GetLineOfString(_TUCHAR **szBuf)
{
	_TUCHAR *whereCL=_tcschr(*szBuf,_T('\n'));
	if(!whereCL)
		return NULL;
	*whereCL=_T('\0');
	_TUCHAR *ret=_tcsdup(*szBuf);
	*whereCL=_T('\n');
	*szBuf=whereCL;
	return ret;
}

void RenameRecursive(TCHAR *path)
{
	HANDLE hSrch;
	WIN32_FIND_DATA wfd;
	TCHAR fname[MAX_PATH];
	BOOL bResult=TRUE;
	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[MAX_PATH];
	TCHAR newpath[MAX_PATH];
	//TCHAR ext[_MAX_EXT];
	//puts(path);
	GetLastError();
	hSrch=FindFirstFile(path,&wfd);
	if (hSrch==INVALID_HANDLE_VALUE)
	{
		puts("error to acquire findfilehandle");
		HandleError();
	 	return;
	}
	GetLastError();
	
		_splitpath(path,drive,dir,NULL,NULL);
		//lstrcat(ext,".txt");
		while (bResult) {
			if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				if (lstrcmp(wfd.cFileName,".") && lstrcmp(wfd.cFileName,"..")) {
					//PathCombine(newpath,path,"\\*.*");
					wsprintf(newpath,"%s%s%s\\*.*",drive,dir,wfd.cFileName);
					RenameRecursive(newpath);
				}
			} else {
				TCHAR buf[MAX_PATH+50];
				wsprintf(buf,"%s%s%s",drive,dir,wfd.cFileName);
				iter = vfiles.insert_after( iter, string(buf) );
			}
			bResult=FindNextFile(hSrch,&wfd);
		}	
	FindClose(hSrch);
}

void Rename()
{
	char fname[MAX_PATH];
	FILE *outf=fopen("index.txt","wt");
    for( iter = vfiles.begin(); iter != vfiles.end(); ++iter )
    {
        char * name= strdup(iter->c_str());
        TCHAR *outname;
        FILE *inf=fopen(name,"rt");
        TCHAR *data=new TCHAR[GetFileSize((HANDLE)inf->_file,NULL)];
        fread(data,GetFileSize((HANDLE)inf->_file,NULL),1,inf);
		MarkIndices(data,outf,name);
		delete [] data;
		fclose(inf);
		free(name);
		
	}
	fclose(outf);
}


int main(int argc, char** argv)
{
	TCHAR dirt[MAX_PATH];
	TCHAR dir[MAX_PATH];
	printf("file renamer\n\n");
    puts("enter folder name");
    gets(dirt);
    puts(dirt);
    memset(dir,0,MAX_PATH);
    int len=lstrlen(dirt);
    int j=0;
    for(int i=0;i<len;++i)
    {
    	if(dirt[i]!='\"')
    	{
    		dir[j]=dirt[i];
    		++j;
		}
	}
    RenameRecursive(dir);
    Rename();
    system("PAUSE");	
	return 0;
}
