//To build:
// cl main.cpp /EHsc /FeRenameGuid.exe /link ole32.lib

#include <windows.h>
#include <stdio.h>
#include <string>

using namespace std;
void Recurse(string s,HANDLE h,WIN32_FIND_DATA* cd);

int main(int argc, char* argv[])
{
	printf("Processing %s\n",argv[1]);
	WIN32_FIND_DATA fd;
	string s=argv[1];
	
	s+="\\*.*";
	HANDLE h=FindFirstFile(s.c_str(),&fd);
	Recurse(argv[1],h,&fd);
}
void Recurse(string s,HANDLE h,WIN32_FIND_DATA* cd)
{
	while(true)
	{
		if(!(cd->dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))
		{
			char* start=cd->cFileName;
			size_t size=strlen(start);
			char* ptr=start+s.size()-1;
			if(size>4 && !strcmpi(&start[size-4],".jpg"))
			{
		
				GUID guid = {0};
				wchar_t szGuidW[40] = {0};
				char szGuidA[40] = {0};
				CoCreateGuid(&guid);
				StringFromGUID2(guid, szGuidW, 40);
				WideCharToMultiByte(0, 0, szGuidW, -1, szGuidA, 40, NULL, NULL);
				
				string path(s);
				path+="\\";
				string newname=szGuidA;
			
				path+=newname;
				path+=".jpg";
				
				string orig(s);
				orig+="\\";
				orig+=cd->cFileName;
				
				printf("%s - %s\n",orig.c_str(),path.c_str());
				rename(orig.c_str(),path.c_str());
			}
		}
		else if(cd->dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
		{	//handle directory
			if(strcmp(cd->cFileName,".") && strcmp(cd->cFileName,".."))
			{
				string s2=s;
				s2+="\\";
				s2+=cd->cFileName;
				string s3=s2+"\\*.*";
				WIN32_FIND_DATA cd2;
				HANDLE h2=FindFirstFile(s3.c_str(),&cd2);
				if(h2!=INVALID_HANDLE_VALUE)
				{
					Recurse(s2, h2, &cd2);
				}
				else
				{/*
					DWORD err=GetLastError();
					printf("Recursion failed for %s - GetLastError==%d\n",s2.c_str(),err);*/
				}
			}
		}
		if(!FindNextFile(h,cd))
		{
			FindClose(h);
			break;
		}
	}
}
