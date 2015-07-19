
#include <iostream>
#include <string>
#include <fstream>
#include <stack>
#include <vector>

using namespace std;

typedef struct info
{
	string token;
	int space;
}Info;

class FileMgr
{
public:
	FileMgr();
	~FileMgr();
	bool openFile(string& inpath,string& outpath);
	bool readline(string& line);	
	void indentline(string& line);		
private:
	int getTopBlockSpace();	
	void addSpaceLine(string& line,int space);
	bool IsBeginBlock(const string& token);
	void getSpace(string& line,int& space);
	void pushInfoToBlock(string& s,int space);
	void checkAndAddToken(const string& token,int& ind,const string& line,vector<string>& vectoken,int& vecsize);
	void parseLine(string& line,vector<string>& vectoken);
private:
	fstream readfile;
	fstream writefile;
	stack<Info> block;	
	static const int OFFSET = 4;
};
