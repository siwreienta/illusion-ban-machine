#include "optimization.h"

#include <iostream>
#include <stack>
#include <string>

using namespace std;

int a, b, cur_num = 0, n;
char cur_op, x;
stack <int> numbers;
stack <int> operators;

void do_ops(){
    a = numbers.top();
    numbers.pop();
    b = numbers.top();
    numbers.pop();
    cur_op = operators.top();
    operators.pop();
    if (cur_op == '/'){
        numbers.push(b / a);
    }
    else if (cur_op == '*'){
        numbers.push(a * b);
    }
    else if (cur_op == '+'){
        numbers.push(a + b);
    }
    else{
        numbers.push(b - a);
    }
}

int main(){
    char ci[1000001];
    string s;
    readWord(ci);
    s = ci;
    n = s.size();
    for (int i = 0; i < n; i++){
        x = s[i];
        if ('0' <= x && '9' >= x){
            cur_num *= 10;
            cur_num += (int)(x - '0');
            if (i == n - 1 || !(s[i + 1] >= '0' && s[i + 1] <= '9')){
                numbers.push(cur_num);
                cur_num = 0;
            }
            continue;
        }
        if (x == '('){
            operators.push(x);
        }
        else if (x == '/' || x == '*'){
            while (operators.size() > 0 && (operators.top() == '*' || operators.top() == '/')){
                do_ops();
            }
            operators.push(x);
        }
        else {
            while (operators.size() > 0 && operators.top() != '('){
                do_ops();
            }
            if (x != ')'){
                operators.push(x);
            }
            else{
                operators.pop();
                while (operators.size() > 0 && operators.top() != '('){
                    do_ops();
                }
            }
        }
    }
    while (operators.size() > 0){
        do_ops();
    }
    writeInt(numbers.top());    
    writeChar('\n');
    return 0;
}