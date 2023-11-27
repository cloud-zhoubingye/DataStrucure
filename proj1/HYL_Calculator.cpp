//��������һ��ͬѧ��HYL���Ĵ��룬�����⡢���ܶ������ƣ�tql

#include<bits/stdc++.h>
using namespace std;
template<class T>class LinkStack{//��ջ (˫������ʵ�֣� 
public:	
	struct Node{T _data;Node* next;Node* pre;};
	Node* head;//ͷָ�� 
	Node* tear;//βָ�� 
	int size;//ջԪ�ظ��� 
	LinkStack(){//���� 
		head=new Node;
		head->next=NULL;
		head->pre=NULL;
		tear=head;
		size=0;
	}
	void insert(T x){//���� 
		size++;
		tear->next=new Node;
		tear->next->pre=tear;
		tear=tear->next;
		tear->_data=x;
		tear->next=NULL;
	}
	T top(){return tear->_data;}//
	void pop(){//
		if(size<=0) return; //��Ԫ�ز����� 
		tear=tear->pre;
		delete tear->next;
		tear->next=NULL; 
		size--;
	}
	bool empty(){return size==0;}

};
class Expression{//���ʽ 
public:
	bool ER=0;//���ʽ�Ϸ���� 
	struct Node{char op;double x;};//opΪ0ʱ��ʾ���֣����߱�ʾ������
	LinkStack<Node> exp; 
	bool IsOp(char ch){//�ж��Ƿ��ǲ����� 
		if(ch=='+'||ch=='-'||ch=='*'||ch=='/'||ch=='!'||ch=='^') return true;
		return false;
	}
	bool IsBa(char ch){if(ch=='('||ch==')')return true;return false;} //�ж��Ƿ�������
	bool Isnum(char ch){//�ж��Ƿ�������
		if(('0'<=ch&&ch<='9')||ch=='.') return true;
		else return false; 
	}
	void check(){//�����ʽ�Ƿ�Ϸ� 
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
	Expression(char* s){//�����ַ���������ʽ�� 
		int i=0;
		while(s[i]!=0)
		{
			if(s[i]==' '){i++;continue;}//���Կո� 
			else if(IsOp(s[i])||IsBa(s[i]))//������ 
			{
				if((s[i]=='+'||s[i]=='-')&&(exp.empty()||exp.tear->_data.op=='(')) exp.insert({0,0.0});//+-��ǰΪ����Ӹ��������� 0
				exp.insert({s[i],0.0});
				if(s[i]=='!') exp.insert({0,0.0});//�׳˺���Ӹ��������� 0
				i++;continue;
			}
			else if(Isnum(s[i]))// �ַ�ת��double��С�� 
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
	void Run(const Expression& e){          //ִ�б��ʽ
		if(e.ER){printf("������ʽ���Ϸ���\n");return;} 
		LinkStack<Expression::Node>::Node* p=e.exp.head->next;
		while(p!=NULL)
		{
			if(p->_data.op==0) {//������ֱ����ջ
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
	LinkStack<double> OPND;       //������ջ 
	LinkStack<char> OPTR;         //������ջ          
	int isp(char op){//ջ��������
		if(op=='+'||op=='-') return 6;
		else if(op=='*'||op=='/') return 4;
		else if(op=='^') return 2;
		else if(op=='!') return 0;
		else if(op=='(') return 10;
	}                       
	int icp(char op){//ջ��������
		if(op=='+'||op=='-') return 7;
		else if(op=='*'||op=='/') return 5;
		else if(op=='^') return 3;
		else if(op=='!') return 1;
		else if(op=='(') return -1;
	}            
	double DoOperator(double x, char op, double y){//�γ�����ָ���������
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
 
