#include "FileMgr.h"

FileMgr::FileMgr()
{
	Info info;
	info.token="";
	info.space=0;
	block.push(info);	
}

FileMgr::~FileMgr(void)
{
	if(readfile.is_open())
		readfile.close();
	if(writefile.is_open())
		writefile.close();
}

bool FileMgr::openFile(string& inpath,string& outpath)
{
	readfile.open(inpath.c_str(),ios::in);
	writefile.open(outpath.c_str(),ios::out);
	return readfile.is_open() && writefile.is_open();
}

bool FileMgr::readline(string& line)
{
	getline(readfile,line);
	return readfile.eof();
}

int FileMgr::getTopBlockSpace()
{
	int ret = 0;
	if(!block.empty())
	{
		ret = block.top().space;		
	}
	return ret;
}

void FileMgr::addSpaceLine(string& line,int space)
{
	size_t pos = line.find_first_not_of(" \t");
	if(pos != string::npos)
	line.erase(0,pos);
	line.insert(0,space,' ');
}

void FileMgr::pushInfoToBlock(string& s,int space)
{
	Info info;
	info.token = s;
	info.space = space;
	block.push(info);
}

void FileMgr::checkAndAddToken(const string& token,int& ind,
							   const string& line,vector<string>& vectoken,
							   int& vecsize)
{
	if(ind>0 && (line[ind-1] != ' ' && line[ind-1] != '\t' 
		&& line[ind-1] != '{' && line[ind-1] != '}' && line[ind-1] != ';'))
		return;

	int i=0,j=0;
	int token_len = token.length();
	for(i=ind;line[i] != '\t' && line[i]!=' ' && line[i]!='(';i++)
		if(line[i] != token[j++])
			break;
	
	if(i-ind == token_len)
	{
		vectoken.push_back(token);
		ind=ind+token.length()-1;
		vecsize++;
	}
}

bool FileMgr::IsBeginBlock(const string& token)
{
	if(!token.compare("for") 
		|| !token.compare("while") 
		|| !token.compare("if") 
		|| !token.compare("else")
		|| !token.compare("switch")		
		)
		return true;
	else
		return false;
}

void FileMgr::parseLine(string& line,vector<string>& vectoken)
{
	int len = line.length();
	int vecsize = 0;
	for(int i=0;i < len;i++)
	{		
		if(line[i] == '/' )
		{
			if(i+1 < len)
			{
				if(line[i+1] == '/')
				{					
					vectoken.push_back("//");					
					vecsize++;
					i=i+2;
				}
				else if(line[i+1] == '*')
				{					
					vectoken.push_back("/*");					
					vecsize++;
					i=i+2;
				}
			}
		}
		else if(line[i] == '*' && i+1 < len && line[i+1] == '/')
		{			
			vectoken.erase(vectoken.begin(),vectoken.end());			
			vectoken.push_back("*/");
			vecsize = 1;			
			i=i+2;
		}
		else if(vecsize == 0 || vectoken.back().compare("/*"))
		{
			if(line[i] == '{')
			{
				vectoken.push_back("{");				
				vecsize++;
			}
			else if(line[i] == '}')
			{
				vectoken.push_back("}");				
				vecsize++;
			}
			else if(line[i] == ';')
			{				
				if(vecsize>0 && IsBeginBlock(vectoken.back()))
				{
					vectoken.erase(vectoken.end()-1);
					vecsize--;
				}				
			}
			else if(line[i] == '(')
			{
				vectoken.push_back("(");
				vecsize++;
			}
			else if(line[i] == ')')
			{
				if(vecsize>0 && !vectoken.back().compare("("))
				{
					vectoken.erase(vectoken.end()-1);
					vecsize--;
				}
				else
				{
					vectoken.push_back(")");
					vecsize++;
				}
			}
			else if(i==0 || line[i-1] == ' ' || line[i-1] == '\t' || 
				line[i-1] == '{' || line[i-1] == '}' || line[i-1] == ';')
			{
				if(line[i] == 'f')
				{
					checkAndAddToken("for",i,line,vectoken,vecsize);			
				}
				else if(line[i] == 'w')
				{
					checkAndAddToken("while",i,line,vectoken,vecsize);			
				}
				else if(line[i] == 'i')
				{
					checkAndAddToken("if",i,line,vectoken,vecsize);			
				}
				else if(line[i] == 'e')
				{
					checkAndAddToken("else",i,line,vectoken,vecsize);			
				}
				else if(line[i] == 'c')
				{
					checkAndAddToken("case",i,line,vectoken,vecsize);			
				}
				else if(line[i] == 'b')
				{
					checkAndAddToken("break",i,line,vectoken,vecsize);			
				}
				else if(line[i] == 's')
				{
					checkAndAddToken("switch",i,line,vectoken,vecsize);			
				}
			}
		}		
	}
	
}

void FileMgr::getSpace(string& line,int& space)
{
	string token;	

	vector<string> vectoken;

	int i=0,size=0;		

	parseLine(line,vectoken);

	size = vectoken.size();

	space = getTopBlockSpace();
	
	if(true == IsBeginBlock(block.top().token))
	{		
		block.pop();
	}	

	if(size > 0)
	{		
		while(i < size)
		{
			if(!block.top().token.compare("/*") && !vectoken[i].compare("*/"))
			{
				block.pop();
			}
			else if(!block.top().token.compare("/*") || !vectoken[i].compare("/*"))
			{
				if(!vectoken[i].compare("/*"))
					pushInfoToBlock(vectoken[i],space);
			}			
			else if(!vectoken[i].compare("("))
			{
				size_t pos1 = line.rfind("(");
				size_t pos2 = line.find_first_not_of(" \t");
				pushInfoToBlock(vectoken[i],pos1-pos2+space);
			}
			else if(!vectoken[i].compare(")") || (!vectoken[i].compare("break")&& !block.top().token.compare("case")))
			{
				block.pop();					
			}
			else if(!vectoken[i].compare("case"))
			{
				if(!block.top().token.compare("case"))
				{
					block.pop();
					space = max(0,space-OFFSET);
				}
				pushInfoToBlock(vectoken[i],space+OFFSET);
			}			
			else if(i == 0)
			{
				if(true == IsBeginBlock(vectoken[i]) || !vectoken[i].compare("{"))
				{
					if(!vectoken[i].compare("{"))
					{					
						space = max(space - OFFSET,0);
					}

					pushInfoToBlock(vectoken[i],space+OFFSET);
				}						
				else if(!vectoken[i].compare("}") && (!block.top().token.compare("{") || !block.top().token.compare("case")))
				{
					space = max(0,getTopBlockSpace()-OFFSET);
					block.pop();
				}
			}
			else
			{
				if(true == IsBeginBlock(vectoken[i-1]))
				{
					int _topspace = getTopBlockSpace();
					block.pop();
					if(!vectoken[i].compare("{"))
						pushInfoToBlock(vectoken[i],_topspace);
					else
						pushInfoToBlock(vectoken[i],_topspace+OFFSET);
				}
				else if(!vectoken[i].compare("}"))
				{
					block.pop();
				}
			}
				
			i++;
		}
	}	
}

void FileMgr::indentline(string& line)
{	
	int space;

	getSpace(line,space);		
		
	addSpaceLine(line,space);	

	writefile << line << endl;	
}