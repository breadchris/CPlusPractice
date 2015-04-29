#include <iostream>
#include <stack>

using namespace std;

enum TokenType { EOL, VALUE, OPAREN, CPAREN, EXP, MULT, DIV, PLUS, MINUS };
// PREC_TABLE matches order of Token enumeration
struct Precedence
{
int inputSymbol;
int topOfStack;
} PREC_TABLE [ ] =
{
{ 0, -1 }, { 0, 0 }, // EOL, VALUE
{ 100, 0 }, { 0, 99 }, // OPAREN, CPAREN
{ 6, 5 }, // EXP
{ 3, 4 }, { 3, 4 }, // MULT, DIV
{ 1, 2 }, { 1, 2 } // PLUS, MINUS
};
int main ( ) {
stack<TokenType> opStack;
opStack.push(EOL); // EOL == end of line
opStack.push(PLUS);
opStack.push(DIV);
opStack.push(EXP);
opStack.push(EXP);
TokenType topOp;
TokenType lastType = MULT;
while( PREC_TABLE[ lastType ].inputSymbol <=
PREC_TABLE[ topOp = opStack.top( ) ].topOfStack )
{
opStack.pop();
cout << topOp << endl;
}
if( lastType != EOL )
opStack.push( lastType );

cout << endl;

while (!opStack.empty()) {
    opStack.pop();
    cout << opStack.top() << endl;
}
return 0;
}
