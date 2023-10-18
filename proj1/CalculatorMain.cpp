//
// Created by Zhou on 09/10/2023.
//
/**
 * @author
 * Zhou BingYe \n
 * 2022141460032
 */
#include "iostream"
#include "sstream"
#include "map"
#include "vector"
#include "stack"
#include "cmath"
using namespace std;

/**
 * @var READ_EOF as a sign of end of input stream \n
 * @var priorityTableIsp as table of priority-of-inStackOperators \n
 * @var priorityTableIcp as table of priority-of-outStackOperators \n
 * @var EOF_FLAG means '=' \n
 * @var LESS_THAN/MORE_THAN/EQUAL_TO is state flags \n
 */
map<char,int>priorityTableIsp={{'=',1},{'(',2},{'*',6},{'/',6},
                                    {'%',6},{'+',4},{'-',4},{')',7}};
map<char,int>priorityTableIcp={{'=',1},{'(',7},{'*',5},{'/',5},
                                    {'%',5},{'+',3},{'-',3},{')',2}};
constexpr double READ_EOF=-9999;
constexpr int LESS_THAN=1;
constexpr int MORE_THAN=2;
constexpr int EQUAL_TO=3;

/**
 * @class
 * A error message of illegal input to be threw 
 */
class illegalInput{
    string message;
public:
    explicit illegalInput(const string &message) : message(message) {}
    string errorMessage(){return message;}
    static void checkError(string s){
        if (s=="q")
            exit(0);
        if (s[s.size()-1]!='=')
            throw illegalInput{"Input Error! Last element should be '='!"};
        for (int i = 0; i < s.size()-1; ++i) {
            if (s[i]==s[i+1]&& !isdigit(s[i]))
                throw illegalInput{"Input Error! "};
        }
    }
};

/**
 * @class
 * A error message of calculate error to be throw
 */
class CalError{
    string message;
public:
    explicit CalError(const string &message) : message(message) {}
    string errorMessage(){return message;}
};

/**
 * @class
 * includes two read fct
 */
class Read{
public:
    /**
     * @def
     * static double getNum( stringstream& ss)
     * @param
     * ss
     * @return
     * a double number read from stream
     * @throw
     *  illegalInput
     */
    static double getNum( stringstream& ss){
        double number=0;
        ss>>number;
        if (ss.eof())
            return READ_EOF;
        if (ss.bad()||ss.fail())
            throw illegalInput{"Input Error!\nExpected Input : Double Number !\n"};
        return number;
    }
    /**
     * @def
     *  static char getChar(stringstream& ss)
     *  @param ss
     *  @return
     *  a char value read from stream
     *  @throw
     *  illegalInput
     */
    static char getChar(stringstream& ss){
        char ch=0;
        ss >> ch;
        if (ss.eof())
            return READ_EOF;
        if (ss.bad()||ss.fail())
            throw illegalInput{"Input Error!\nExpected Input : A Char !\n"};
        return ch;
    }
};
 
 /**
  * @class
  * do calculation
  * includes two fct
  */
class Calculation{
private:
    /**
 * @def
 * static bool isValidOp(char ch)
 * @param ch
 * @return
 *  true if ch is valid operator
 *  false if others
 */
    static bool isValidOp(char ch){
        if (priorityTableIsp[ch]==0)
            return false;
        return true;
    }
    /**
     * @def
     * static int compOp(char ch1,char ch2)
     * @param ch1
     * @param ch2
     * @return
     * LESS_THAN(1), MORE_THAN(2), EQUAL_TO(3)
     */
    static int compOp(char ch1,char ch2){
        if (priorityTableIsp[ch1] < priorityTableIcp[ch2])
            return LESS_THAN;
        else if (priorityTableIsp[ch1] > priorityTableIcp[ch2])
            return MORE_THAN;
        else if (priorityTableIsp[ch1] == priorityTableIcp[ch2]&&ch2=='=')
            return EQUAL_TO;
        else
            return LESS_THAN;   //keep and wait until '=' occurs
    }
    /**
     * @def
     * static string pullExpression(stack<char>&opStack, stack<double>&numStack)
     * @param opStack
     * @param numStack
     * @return
     * expression string between '(' and ')'
     */
    static string pullExpression(stack<char>&opStack, stack<double>&numStack){
        string expression;
        while (true){
            double num_expression=numStack.top();
            numStack.pop();
            expression=to_string(num_expression)+expression;
            char ch_expression=opStack.top();
            opStack.pop();
            if (ch_expression!='('){
                expression=ch_expression+ expression;
            } else{
                break;
            }
        }
        return expression;
    }
    /**
  * @def
  * static double basicCal(double num1,double num2,char op)
  * 【calculate basic expression like 5+3 or 6/2】
  * @param num1
  * @param num2
  * @param op
  * @return
  * a double value: num1 op num2
  * @throw
  * CalError
 */
    static double basicCal(double num1,double num2,char op){
        switch (op) {
            case '*':
                return num1*num2;
            case '/':{
                if (num2==0)
                    throw CalError{"Calculate Error! Divide by zero!"};
                return num1/num2;
            }
            case '%':{
                if (fabs(int(num1)-num1)>=0.001||fabs(int(num2)-num2)>=0.001)
                    return fmod(num1,num2);
                if (int(num2)==0)
                    throw CalError{"Calculate Error! Mod by zero!"};
                return int(num1)%int(num2);
            }
            case '+':
                return num1+num2;
            case '-':
                return num1-num2;
        }
    }
    /**
     * @def
     * static void doBasicCal(stack<char>&opStack, stack<double>&numStack)
     * 【for the first two elements in numStack and first element in opStack, compute (num1 op num2)】
     * @param opStack
     * @param numStack
     */
    static void doBasicCal(stack<char>&opStack, stack<double>&numStack){
        double num2=numStack.top();
        numStack.pop();
        char  ch=opStack.top();
        opStack.pop();
        double num1=numStack.top();
        numStack.pop();
        double result= basicCal(num1,num2,ch);
        numStack.push(result);
    }
public:
    /**
      * @def
      *  static double Cal(string s) \n
      * @param s
      *  @return
      *  calculate double result
      *  @throw
      *  illegalInput
     */
    static double Cal(string s){
        //define var
        stringstream ss{s};
        char ch=Read::getChar(ss);
        double num=0;
        stack<char>opStack;
        stack<double>numStack;
        //get char or num and calculate
        while (true){
            if (isdigit(ch)){
                //if is a digit, put it into stack
                ss.putback(ch);
                num=Read::getNum(ss);
                numStack.push(num);
                ch=Read::getChar(ss);
                continue;
            }
            if (!isValidOp(ch))
                throw illegalInput{"Input Error!\nExpected Input : + - * / % !\nOperator offered is INVALID !\n"};
            /**
             * up to now, we certainly have a valid ch
             */
            //if is the first time
            if (opStack.empty()){
                if (ch!='='){
                    opStack.push(ch);
                    ch=Read::getChar(ss);
                    continue;
                } else {
                    return numStack.top();
                }
            }
            //compare if Isp(opStackTop) >、<、== Icp(ch)
            switch (compOp(opStack.top(),ch)) {
                case LESS_THAN:{
                    opStack.push(ch);
                    break;
                }
                case MORE_THAN:{
                    doBasicCal(opStack,numStack);
                    if (ch==')'){
                        if (opStack.top()=='('){
                            opStack.pop();
                        }
                    }
                    else if (ch=='=')
                        ss.putback(ch);
                    else
                        opStack.push(ch);
                    if (opStack.size()==0)
                        return numStack.top();
                    break;
                }
                case EQUAL_TO:{
                    opStack.pop();
                    break;
                }
            }
            //read next char
            ch=Read::getChar(ss);
        }
    }
    /**
     * @def
     *  static void dealFactorial(string& s)
     * @param
     * s
     * @return
     * void
     */
    static void dealFactorial(string& s){
        stringstream ss{s};
        stringstream ss1{""};
        char ch=0;
        double num=0;
        ss>>ch;
        while (ss.good()){
            if (isdigit(ch)){
                ss.putback(ch);
                ss>>num;
                ss>>ch;
                if (ch=='!'){
                    if (fabs(num- int(num))>=0.01)
                        ss1<<tgamma(num+1);
                    else
                        ss1<<fact(int(num));
                } else{
                    ss1<<num;
                    ss1<<ch;
                }
            } else{
                ss1<<ch;
            }
            ss>>ch;
        }
        s="";
        ss1>>s;
    }
    /**
     * @def
     *  static int fact(int num)
     * @param num
     * @return
     * factorial result
     */
    static int fact(int num){
        if (num==1)
            return num;
        else
            return num* fact(num-1);
    }
};

void calculate(){
    while (true){
        try{
            cout<<"\nEnter expression: \n > ";
            string s;
            cin>>s;
            Calculation::dealFactorial(s);
            illegalInput::checkError(s);
            cout<<Calculation::Cal(s)<<endl;
        } catch (exception&e){
            cout<<"Input Error !  "<<e.what()<<endl;
        } catch(CalError&e){
            cout<<e.errorMessage()<<endl;
        } catch (illegalInput&e) {
            cout<<e.errorMessage()<<endl;
        }
    }
}

int main(){
    calculate();
}