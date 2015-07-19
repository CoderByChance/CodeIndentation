// CodeFileMgration.cpp : Defines the entry point for the console application.
//
#include "FileMgr.h"

int _tmain(int argc, _TCHAR* argv[])
{
	FileMgr FileMgr;
	string input_file = "input.txt";
	string output_file = "out.txt";
	if(false == FileMgr.openFile(input_file,output_file))
	{
		cout << "Not able to open file\n";
	}
	else
	{
		bool bEof = false;
		do
		{
			string line;
			bEof = FileMgr.readline(line);
			FileMgr.indentline(line);			
		}while(bEof == false);
	}
	return 0;
}

