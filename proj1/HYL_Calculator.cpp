//这是另外一个同学（HYL）的代码，错误监测、功能都很完善，tql

#include<bits/stdc++.h>
using namespace std;
template<class T>class LinkStack{//链栈 (双向链表实现） 
public:	
	struct Node{T _data;Node* next;Node* pre;};
	Node* head;//头指针 
	Node* tear;//尾指针 
	int size;//栈元素个数 
	LinkStack(){//构造 
		head=new Node;
		head->next=NULL;
		head->pre=NULL;
		tear=head;
		size=0;
	}
	void insert(T x){//插入 
		size++;
		tear->next=new Node;
		tear->next->pre=tear;
		tear=tear->next;
		tear->_data=x;
		tear->next=NULL;
	}
	T top(){return tear->_data;}//
	void pop(){//
		if(size<=0) return; //无元素不弹出 
		tear=tear->pre;
		delete tear->next;
		tear->next=NULL; 
		size--;
	}
	bool empty(){return size==0;}

};
class Expression{//表达式 
public:
	bool ER=0;//表达式合法标记 
	struct Node{char op;double x;};//op为0时表示数字，否者表示操作符
	LinkStack<Node> exp; 
	bool IsOp(char ch){//判断是否是操作符 
		if(ch=='+'||ch=='-'||ch=='*'||ch=='/'||ch=='!'||ch=='^') return true;
		return false;
	}
	bool IsBa(char ch){if(ch=='('||ch==')')return true;return false;} //判断是否是括号
	bool Isnum(char ch){//判断是否是数字
		if(('0'<=ch&&ch<='9')||ch=='.') return true;
		else return false; 
	}
	void check(){//检查表达式是否合法 
		LinkStack<Node>::Node* p=exp.head->next;int num=0,num2=0;
		while(p!=NULL) 
		{
			if(IsOp(p->_data.op)) num2--;
			if(p->_data.op==0) num2++;
			if(p->_data.op=='(') num++;
			if(p->_data.op==')') num--;
			if(p->next!=NULL)
			{
				if(p->_data.op) if(IsOp(p->_data.op)&&IsOp(p->next->_data.op)) ER=1;
				if(p->_data.op==0&&p->next->_data.op==0) ER=1;
			}
			if(num<0) ER=1;
			p=p->next;
		}
		if(num!=0||num2!=1) ER=1;
	} 
	Expression(char* s){//输入字符串构造表达式类 
		int i=0;
		while(s[i]!=0)
		{
			if(s[i]==' '){i++;continue;}//忽略空格 
			else if(IsOp(s[i])||IsBa(s[i]))//操作符 
			{
				if((s[i]=='+'||s[i]=='-')&&(exp.empty()||exp.tear->_data.op=='(')) exp.insert({0,0.0});//+-号前为（添加辅助操作数 0
				exp.insert({s[i],0.0});
				if(s[i]=='!') exp.insert({0,0.0});//阶乘后添加辅助操作数 0
				i++;continue;
			}
			else if(Isnum(s[i]))// 字符转化double型小数 
			{
				double res=0;
				int vis1=-1e9;
				while(Isnum(s[i])){	
					if(s[i]!='.') res=res*10+s[i]-'0',vis1++;
					else vis1=0;
					i++;
				}
				while(--vis1>=0) res=res/10;
				exp.insert({0,res}); 
			}
			else ER=1,i++;
		}
		check();
	}
};
class Calculator{
public:
	void Run(const Expression& e){          //执行表达式
		if(e.ER){printf("输入表达式不合法！\n");return;} 
		LinkStack<Expression::Node>::Node* p=e.exp.head->next;
		while(p!=NULL)
		{
			if(p->_data.op==0) {//操作数直接入栈
				OPND.insert(p->_data.x);
				p=p->next;
				continue;
			}
			if(p->_data.op==')'){
				while(OPTR.top()!='(') Step_Cla();
				OPTR.pop();p=p->next;
				continue;
			} 
			while(!OPTR.empty()&&isp(OPTR.top())<icp(p->_data.op)) Step_Cla();
			OPTR.insert(p->_data.op);
			p=p->next;
		}
		while(!OPTR.empty()) Step_Cla();
		printf("%g\n",OPND.head->next->_data);
	}                
private:
	LinkStack<double> OPND;       //操作数栈 
	LinkStack<char> OPTR;         //操作符栈          
	int isp(char op){//栈内优先数
		if(op=='+'||op=='-') return 6;
		else if(op=='*'||op=='/') return 4;
		else if(op=='^') return 2;
		else if(op=='!') return 0;
		else if(op=='(') return 10;
	}                       
	int icp(char op){//栈外优先数
		if(op=='+'||op=='-') return 7;
		else if(op=='*'||op=='/') return 5;
		else if(op=='^') return 3;
		else if(op=='!') return 1;
		else if(op=='(') return -1;
	}            
	double DoOperator(double x, char op, double y){//形成运算指令，进行运算
		if(op=='+') return x+y;
		else if(op=='-') return x-y;
		else if(op=='*') return x*y;
		else if(op=='/') return x/y;
		else if(op=='^') return pow(x,y);
		else if(op=='!') return tgamma(x+1); 
	}
	void Step_Cla(){
		double y=OPND.top();OPND.pop();
		double x=OPND.top();OPND.pop();
		char op=OPTR.top();OPTR.pop();
		OPND.insert(DoOperator(x,op,y));
	}
};
int main()
{
	char s[1005];
	while(cin.getline(s,1000))
	{
		//cout<<s<<endl;
		Expression e(s);
		Calculator Cla;
		Cla.Run(e);
	}
	return 0;
}
 
