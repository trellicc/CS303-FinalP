#include <stack>
#include <string>
#include <unordered_map>
using namespace std;
class Evaluator {
private:
  // order precedence map
  static unordered_map<string, int> precedence;

  bool isUnary(const string &op);
  // PRE: Checks if operator is unary.
  // POST: Returns true if operator is unary, false otherwise.
  bool isBinary(const string &op);
  // PRE: Checks if operator is binary.
  // POST: Returns true if operator is binary, false otherwise.
  int applyUnary(const string &op, int operand);
  // PRE: Takes operator and operand, applying the operator to the operand.
  // POST: returns the result of applying the operator to the operand.
  int applyBinary(const string &op, int left, int right);
  // PRE: Takes operator and operand, left and right numbers
  // POST: returns the result of applying the operator to the operand.
  void processOperator(stack<int> &operands, stack<string> &operators);
  // PRE: Takes stack of operands and stack of operators
  // POST: Applies operators to operands
  void handleOperator(stack<int> &operands, stack<string> &operators,
                      const string &op);
  // PRE: Takes stack of operands and stack of operators, and operator
  // POST: Applies operator to operands
public:
  // Main func
  int eval(const string &expression);
};
