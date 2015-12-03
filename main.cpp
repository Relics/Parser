#include <iostream>
#include <string.h>
#include <string>

using namespace std;
#define MAXNUMBER 100		//定义允许输入的最大字符

//定义LR分析的几个动作
typedef enum Action
{
	error,shift,reduce,accept,gotoState
}Action;

//定义终结符和非终结符
typedef enum Variable
{
	id,num,l,r,add,sub,mul,divi,acc,E,T,F,Ep
}Variable;

//定义LR表项内容
typedef struct LRTableItem
{
	Action action;  //动作
	int state;		//状态或表达式号
}LRTableItem;

//定义表达式形式
typedef struct Expression
{
	int front;	//表达式前项
	string rear;	//表达式后项
	int length;		//表达式后项的长度
}Expression;

//定义符号表表项内容
typedef struct Character{
	string charString;		//输入符号
	int type;				//符号类型
}Character;

Expression grammar[10];			//文法
LRTableItem LRTable[17][25]={};	//LR分析表
string inputString;				//输入字符串
Character inputArray[MAXNUMBER];//符号表
int stack[MAXNUMBER]={0};		//栈
int stackCount=0;				//栈顶位置
int ip=0;						//当前符号表指针位置
int inputCount=0;				//记录符号表元素个数

//初始化文法
void init_grammar(){
	grammar[0].front=Ep;grammar[0].rear="E";grammar[0].length=1;
	grammar[1].front=E;grammar[1].rear="E+F";grammar[1].length=3;
	grammar[2].front=E;grammar[2].rear="E-F";grammar[2].length=3;
	grammar[3].front=E;grammar[3].rear="T";grammar[3].length=1;
	grammar[4].front=T;grammar[4].rear="T*F";grammar[4].length=3;
	grammar[5].front=T;grammar[5].rear="T/F";grammar[5].length=3;
	grammar[6].front=T;grammar[6].rear="F";grammar[6].length=1;
	grammar[7].front=F;grammar[7].rear="id";grammar[7].length=1;
	grammar[8].front=F;grammar[8].rear="(E)";grammar[8].length=3;
	grammar[9].front=F;grammar[9].rear="number";grammar[9].length=1;
}

//初始化LR分析表
void init_lrtable(){
	LRTable[0][E].action=gotoState; LRTable[0][E].state=1;
	LRTable[0][T].action=gotoState; LRTable[0][T].state=2;
	LRTable[0][F].action=gotoState; LRTable[0][F].state=3;
	LRTable[0][id].action=shift; LRTable[0][id].state=4;
	LRTable[0][num].action=shift; LRTable[0][num].state=5;
	LRTable[0][l].action=shift; LRTable[0][l].state=6;

	LRTable[1][add].action=shift; LRTable[1][add].state=9;
	LRTable[1][sub].action=shift; LRTable[1][sub].state=10;
	LRTable[1][acc].action=accept; LRTable[1][l].state=0;

	LRTable[2][mul].action=shift; LRTable[2][mul].state=11;
	LRTable[2][divi].action=shift; LRTable[2][divi].state=12;
	LRTable[2][add].action=reduce; LRTable[2][add].state=3;
	LRTable[2][sub].action=reduce; LRTable[2][sub].state=3;
	LRTable[2][r].action=reduce; LRTable[2][r].state=3;
	LRTable[2][acc].action=reduce; LRTable[2][acc].state=3;


	LRTable[3][mul].action=reduce; LRTable[3][mul].state=6;
	LRTable[3][divi].action=reduce; LRTable[3][divi].state=6;
	LRTable[3][add].action=reduce; LRTable[3][add].state=6;
	LRTable[3][sub].action=reduce; LRTable[3][sub].state=6;
	LRTable[3][r].action=reduce; LRTable[3][r].state=6;
	LRTable[3][acc].action=reduce; LRTable[3][acc].state=6;

	LRTable[4][mul].action=reduce; LRTable[4][mul].state=7;
	LRTable[4][divi].action=reduce; LRTable[4][divi].state=7;
	LRTable[4][add].action=reduce; LRTable[4][add].state=7;
	LRTable[4][sub].action=reduce; LRTable[4][sub].state=7;
	LRTable[4][r].action=reduce; LRTable[4][r].state=7;
	LRTable[4][acc].action=reduce; LRTable[4][acc].state=7;

	LRTable[5][mul].action=reduce; LRTable[5][mul].state=9;
	LRTable[5][divi].action=reduce; LRTable[5][divi].state=9;
	LRTable[5][add].action=reduce; LRTable[5][add].state=9;
	LRTable[5][sub].action=reduce; LRTable[5][sub].state=9;
	LRTable[5][r].action=reduce; LRTable[5][r].state=9;
	LRTable[5][acc].action=reduce; LRTable[5][acc].state=9;

	LRTable[6][E].action=gotoState; LRTable[6][E].state=7;
	LRTable[6][T].action=gotoState; LRTable[6][T].state=2;
	LRTable[6][F].action=gotoState; LRTable[6][F].state=3;
	LRTable[6][id].action=shift; LRTable[6][id].state=4;
	LRTable[6][num].action=shift; LRTable[6][num].state=5;
	LRTable[6][l].action=shift; LRTable[6][l].state=6;

	LRTable[7][add].action=shift; LRTable[7][add].state=9;
	LRTable[7][sub].action=shift; LRTable[7][sub].state=10;
	LRTable[7][r].action=shift; LRTable[7][r].state=8;

	LRTable[8][mul].action=reduce; LRTable[8][mul].state=8;
	LRTable[8][divi].action=reduce; LRTable[8][divi].state=8;
	LRTable[8][add].action=reduce; LRTable[8][add].state=8;
	LRTable[8][sub].action=reduce; LRTable[8][sub].state=8;
	LRTable[8][r].action=reduce; LRTable[8][r].state=8;
	LRTable[8][acc].action=reduce; LRTable[8][acc].state=8;

	LRTable[9][T].action=gotoState; LRTable[9][T].state=13;
	LRTable[9][F].action=gotoState; LRTable[9][F].state=3;
	LRTable[9][id].action=shift; LRTable[9][id].state=4;
	LRTable[9][num].action=shift; LRTable[9][num].state=5;
	LRTable[9][l].action=shift; LRTable[9][l].state=6;

	LRTable[10][T].action=gotoState; LRTable[10][T].state=14;
	LRTable[10][F].action=gotoState; LRTable[10][F].state=3;
	LRTable[10][id].action=shift; LRTable[10][id].state=4;
	LRTable[10][num].action=shift; LRTable[10][num].state=5;
	LRTable[10][l].action=shift; LRTable[10][l].state=6;

	LRTable[11][F].action=gotoState; LRTable[11][F].state=15;
	LRTable[11][id].action=shift; LRTable[11][id].state=4;
	LRTable[11][num].action=shift; LRTable[11][num].state=5;
	LRTable[11][l].action=shift; LRTable[11][l].state=6;

	LRTable[12][F].action=gotoState; LRTable[12][F].state=16;
	LRTable[12][id].action=shift; LRTable[12][id].state=4;
	LRTable[12][num].action=shift; LRTable[12][num].state=5;
	LRTable[12][l].action=shift; LRTable[12][l].state=6;

	LRTable[13][mul].action=shift; LRTable[13][mul].state=11;
	LRTable[13][divi].action=shift; LRTable[13][divi].state=12;
	LRTable[13][add].action=reduce; LRTable[13][add].state=1;
	LRTable[13][sub].action=reduce; LRTable[13][sub].state=1;
	LRTable[13][r].action=reduce; LRTable[13][r].state=1;
	LRTable[13][acc].action=reduce; LRTable[13][acc].state=1;

	LRTable[14][mul].action=shift; LRTable[14][mul].state=11;
	LRTable[14][divi].action=shift; LRTable[14][divi].state=12;
	LRTable[14][add].action=reduce; LRTable[14][add].state=2;
	LRTable[14][sub].action=reduce; LRTable[14][sub].state=2;
	LRTable[14][r].action=reduce; LRTable[14][r].state=2;
	LRTable[14][acc].action=reduce; LRTable[14][acc].state=2;

	LRTable[15][mul].action=reduce; LRTable[15][mul].state=4;
	LRTable[15][divi].action=reduce; LRTable[15][divi].state=4;
	LRTable[15][add].action=reduce; LRTable[15][add].state=4;
	LRTable[15][sub].action=reduce; LRTable[15][sub].state=4;
	LRTable[15][r].action=reduce; LRTable[15][r].state=4;
	LRTable[15][acc].action=reduce; LRTable[15][acc].state=4;

	LRTable[16][mul].action=reduce; LRTable[16][mul].state=5;
	LRTable[16][divi].action=reduce; LRTable[16][divi].state=5;
	LRTable[16][add].action=reduce; LRTable[16][add].state=5;
	LRTable[16][sub].action=reduce; LRTable[16][sub].state=5;
	LRTable[16][r].action=reduce; LRTable[16][r].state=5;
	LRTable[16][acc].action=reduce; LRTable[16][acc].state=5;
}

//查找终结符合非终结符
string findVar(int var){
	switch (var){
	case id:return "id";
	case num:return "number";
	case l:return "(";
	case r:return ")";
	case add:return "+";
	case sub:return "-";
	case mul:return "*";
	case divi:return "/";
	case acc: return "$";
	case E:return "E";
	case F:return "F";
	case T:return "T";
	case Ep:return "E'";
	default:
		break;
	}
}

//汇报错误函数
void error_print(){
	cout<<"\n--------------------------------------------------------"<<endl;
	cout<<"error"<<endl;
	system("pause");
	exit(0);
};

//查LR分析表
LRTableItem search(int state,int c){
	return LRTable[state][c];
}

//判断是否为字母
bool isLetter(char C){
	if((C>='a'&&C<='z')||(C>='A'&&C<='Z'))
		return true;
	else return false;
}

//判断是否为数字
bool isDigit(char C){
	if((C>='0'&&C<='9'))
		return true;
	else return false;
}

//字符分析，并填入字符表
void analysis_char(){
	string charstring=inputArray[inputCount].charString;
	if(charstring=="(")inputArray[inputCount].type=l;
	else if (charstring==")")inputArray[inputCount].type=r;
	else if (charstring=="+")inputArray[inputCount].type=add;
	else if (charstring=="-")inputArray[inputCount].type=sub;
	else if (charstring=="*")inputArray[inputCount].type=mul;
	else if (charstring=="/")inputArray[inputCount].type=divi;
	else if (charstring=="$")inputArray[inputCount].type=acc;
	else if(isdigit(charstring[0])){
		//分析number
		for(int i=1;charstring[i]!='\0';i++){
			if(!isdigit(charstring[i])) error_print();
		}
		inputArray[inputCount].type=num;
	}
	else if(isLetter(charstring[0])||charstring[0]=='_'){
		//分析id
		for(int i=1;charstring[i]!='\0';i++){
			if(!((isLetter(charstring[i])||isdigit(charstring[i])
				||charstring[i]=='_')))
				error_print();
		}
		inputArray[inputCount].type=id;
	}
	else error_print();
}

//按照空格将字符串进行分割
void split_string(){
	//空格的ascii码
	char pattern=32;
	int size=inputString.size();
	for(int i=0; i<size; i++)
	{
		//寻找空格，返回位置
		int pos=inputString.find(pattern,i);
		if(pos<size)
		{
			//剪切字符串放入到字符表中
			string s=inputString.substr(i,pos-i);
			inputArray[inputCount].charString=s;
			//分析符号类型，并填入符号表
			analysis_char();
			inputCount++;
			//从pos位置继续找
			i=pos;	
		}
	}
}

//输出栈
void outputStack(){
	cout<<"0";
	for(int i=1;i<stackCount;i+=2){
		cout<<findVar(stack[i])<<stack[i+1];
	}
	cout<<"\t\t";
}

//输出输入
void outputInput(){
	for(int i=ip;i<inputCount;i++){
		cout<<inputArray[i].charString;
	}
	cout<<"\t\t";
}

//LR分析程序
void analysis(){
	cout<<"--------------------------------------------------------"<<endl;
	cout<<"栈\t\t输入\t\t分析动作"<<endl;
	cout<<"--------------------------------------------------------"<<endl;
	while (true)
	{
		outputStack();
		outputInput();
		//当前状态为栈顶元素
		int state=stack[stackCount];
		//输入字符（终结符）
		int c=inputArray[ip].type;
		//查LR表得到动作
		LRTableItem item=search(state,c);
		//动作匹配
		switch (item.action)
		{
		case shift:{
			//action为移进
			stack[++stackCount]=c;
			stack[++stackCount]=item.state;
			ip++;
			//在屏幕上输出分析动作
			cout<<"shift "<<item.state<<endl;
			break;
				   }
		case reduce:{
			//action为reduce
			//确定规约所用的表达式
			Expression exp=grammar[item.state];
			//从栈中移除2*exp.length个元素
			for(int i=0;i<2*exp.length;i++){
				stack[stackCount]=0;
				stackCount--;
			}
			//栈顶元素为当前状态
			state=stack[stackCount];
			//非终结符
			int c=exp.front;
			//非终结符进栈
			stack[++stackCount]=c;
			//查goto表后转移状态进栈
			stack[++stackCount]=search(state,c).state;
			//在屏幕上输出分析动作
			cout<<"reduce by "<<findVar(exp.front)<<"->"<<exp.rear<<endl;
			break;
					}
		case accept:{
			//action为accept,分析出口
			cout<<"accept"<<endl;
			return;
					}
		default:{
			//action为其他
			error_print();
				}
		}
	}
	//错误状态
	error_print();
}

//输入字符串
void input(){
	cout<<"--------------------------------------------------------"<<endl;
	cout<<"编译原理实验二：LR语法分析器\n";
	cout<<"--------------------------------------------------------"<<endl;
	cout<<"请输入字符串(最多输入"<<MAXNUMBER<<"个字符,输入$表示结束)：\n";
	cout<<"要求输入的每个变量或符号之间用空格隔开。"<<endl;
	cout<<"--------------------------------------------------------"<<endl;
	//输入表达式，输入$符号表示输入结束
	char hello[MAXNUMBER];
	scanf("%[^$]",hello);
	inputString=hello;
	inputString=inputString+"$ ";
	//分离在inputArray数组中
	split_string();
}

//主程序入口
int main(){
	//输入字符串
	input();
	//对文法和LR分析表进行初始化
	init_grammar();
	init_lrtable();
	//进行LR分析
	analysis();
	//打印结果
	cout<<"--------------------------------------------------------"<<endl;
	cout<<"success"<<endl;
	cout<<"--------------------------------------------------------"<<endl;
	system("pause");
	return 0;
}
