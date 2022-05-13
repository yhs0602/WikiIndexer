#include<iostream>
#include<fstream>
#include<string>
#include <windows.h>

#define ASSERT(a)\
	do\
	{if(!(a)){\
			MessageBox(0,"Assertion failed","assert",0);\
		}\
	}while(0);

using namespace std;

enum Phase
{
    PH_INIT,
    PH_SUBJECT,
    PH_SPACE,
    PH_ARTICLE
};

HRESULT WikiIndex(ofstream &file,int numFile);
Phase GetPhase(string &buf,Phase prev);
const char* PhaseToStr(Phase p);
int main()
{
	ofstream out("out.txt");
	//ostream out;
	//out.rdbuf(cout.rdbuf());
	int index=1;
	for(; index<97; ++index)
	{
		if(WikiIndex(out,index)<0)
			break;
		printf("%d\n",index);
	}
	system("pause");
	return 0;
}
HRESULT WikiIndex(ofstream &file,int numFile)
{
	char infilename[MAX_PATH];
	snprintf(infilename,MAX_PATH,"wiki.html_%03d.txt",numFile);
	//puts(infilename);
	ifstream in(infilename);
	if(!in.is_open())
	{
		MessageBox(0,"File not Exist","Error",0);
		return -1;
	}
	string buf;
	string articleName;
	int numArticles=0;
	Phase phase=PH_INIT,prev=PH_INIT;
	while(::getline(in,buf)>0)
	{
		phase=GetPhase(buf,prev);
		//cout<<" prev "<<PhaseToStr(prev)<<" phase "<<PhaseToStr(phase)<<endl;
		if(prev==PH_INIT)
		{
			prev=phase;
		}
		switch(phase)
		{
			case PH_SPACE:
			case PH_ARTICLE:
				break;
			case PH_SUBJECT:
				file<<articleName<<" : "<<numFile<<"째 파일의 "<<numArticles<<"째 문서"<<endl;
				++numArticles;
				articleName=buf;
				break;
			default:
				ASSERT(false);
		}
		prev=phase;
	}
	file<<articleName<<" : "<<numFile<<"째 파일의 "<<numArticles<<"째 문서"<<endl;
}
Phase GetPhase(string &buf,Phase prev)
{
	if(buf[0]=='\n'||buf[0]=='\r'||buf[0]=='\0')
		return PH_SPACE;
	switch(prev)
	{
		case PH_SPACE:
			return PH_SUBJECT;
			break;
		case PH_SUBJECT:
		case PH_ARTICLE:
			return PH_ARTICLE;
		case PH_INIT:
		{
			if(buf.length()<32)return PH_SUBJECT;
			if(buf.find("분류:")||buf.find("thumb"))
			{
				return PH_ARTICLE;
			}
			string part=buf.substr(0,20);
			if(part.find(" 넘겨주기 "))
			{
				return PH_SUBJECT;//PH_ARTICLE_NUMGYOJUGI;
			}
			return PH_SUBJECT;
			break;

		}
		default:
			ASSERT(false);
	}
	ASSERT(false);
	return PH_INIT;
}

const char* PhaseToStr(Phase p)
{
	switch(p)
	{
		case PH_ARTICLE:
			return "ARTICLE";
			break;
		case PH_INIT:
			return "INIT";
			break;
		case PH_SUBJECT:
			return "SUBJECT";
			break;
		case PH_SPACE:
			return "SPACE";
			break;
	}
	return NULL;
}

