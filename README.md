# CodeIndentation

Objective
Create a project to indent a piece of code written in C++.
Input : Text file having a C++ code written.
Output : Text file having proper indentation in a way that code is readable and executable(If it was executable initially).

Analysis
Question : What is an indented code ?
Answer : Indented code conveys properly the code structure to a human .
How ?...by telling this block A is contained within the other block B.
There are different style for code indentation.
But purpose is same that is code readability.
In our solution we will follow one particular style to indent the code keeping in mind following points -
Indentation technique should not increase the size of code enormously.
 It should not break the written code logic even syntax wise.
The indented code should be same when ported to different platforms/IDE.

Question : What will this program do ?
Answer : It will add spaces to indent the code

Sample 
Input :      
#include<iostream>
using namespace std;

    int main(){
    if(1){
        cout << "In If"<<endl;
    } else cout << "In else" << endl;
        
    return 0;
    }

Output :
#include<iostream>
using namespace std;

int main(){
    if(1){
        cout << "In If"<<endl;
    } else cout << "In else" << endl;
         
    return 0;
}    

