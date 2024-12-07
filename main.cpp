//Name: Nicolas Aponte
//Project: Project 1A. 
//Solo project
#include <iostream>
#include "evaluator.h"

using namespace std;

int main() {
    Evaluator eval;
    //try catch for error handling
    try { //test examples
        cout << eval.eval("-4+5") << "\n";     //1        
        cout << eval.eval("6+2^2*3") << "\n";   //18        
        cout << eval.eval("1==2") << "\n";      //0 (since false)         
        cout << eval.eval("1+6 > 2") << "\n";          // 1 since true
        cout << eval.eval("(4>=4) && 0") << "\n";      //0 since false
        cout << eval.eval("(3+2)*3") << "\n";          // 15
        cout << eval.eval("50/0") << "\n";     // Error (div by zero)
    } catch (const invalid_argument &e) {
      //stops program as soon as error is caught. 
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}
