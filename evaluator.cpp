#include <cctype>
#include <cmath>
#include <stdexcept>
#include <unordered_map>
#include "evaluator.h"
using namespace std;

// setting up order presedence based on table given in assignment (also pemdas)
unordered_map<string, int> Evaluator::precedence = {
    {"!", 8}, {"++", 8}, {"--", 8}, {"-", 8},  {"^", 7},  {"*", 6},
    {"/", 6}, {"%", 6},  {"+", 5},  {"-", 5},  {">", 4},  {">=", 4},
    {"<", 4}, {"<=", 4}, {"==", 3}, {"!=", 3}, {"&&", 2}, {"||", 1}};

// Described in .h, returns T/F
bool Evaluator::isUnary(const string &op) {
  return op == "!" || op == "++" || op == "--" || op == "-";
}
// Described in .h, returns T/F
bool Evaluator::isBinary(const string &op) {
  return precedence.count(op) && !isUnary(op);
}

// Applies the --, negative - ! and ++
int Evaluator::applyUnary(const string &op, int operand) {
  if (op == "!")
    return !operand;
  if (op == "++")
    return operand + 1;
  if (op == "--")
    return operand - 1;
  if (op == "-")
    return -operand;
  throw invalid_argument("Unknown unary operator: " + op);
}

// Applies basic math
int Evaluator::applyBinary(const string &op, int left, int right) {
  if (op == "+")
    return left + right;
  if (op == "-")
    return left - right;
  if (op == "*")
    return left * right;
  if (op == "/") {
    if (right == 0)
      // checking for div by 0
      throw invalid_argument("Division by zero");
    return left / right;
  }
  if (op == "%") {
    if (right == 0)
      // check for div by zero
      throw invalid_argument("Division by zero");
    return left % right;
  }
  if (op == "^")
    return pow(left, right);
  if (op == ">")
    return left > right;
  if (op == ">=")
    return left >= right;
  if (op == "<")
    return left < right;
  if (op == "<=")
    return left <= right;
  if (op == "==")
    return left == right;
  if (op == "!=")
    return left != right;
  if (op == "&&")
    return left && right;
  if (op == "||")
    return left || right;
  throw invalid_argument("Unknown binary operator: " + op);
}

void Evaluator::processOperator(stack<int> &operands,
                                stack<string> &operators) {
  string op = operators.top();
  operators.pop();

  if (isUnary(op)) {
    if (operands.empty())
      throw invalid_argument("Missing operand for unary operator");
    int operand = operands.top();
    operands.pop();
    operands.push(applyUnary(op, operand));
  } else if (isBinary(op)) {
    if (operands.size() < 2)
      throw invalid_argument("Missing operands for binary operator");
    int right = operands.top();
    operands.pop();
    int left = operands.top();
    operands.pop();
    operands.push(applyBinary(op, left, right));
  } else {
    throw invalid_argument("Unknown operator: " + op);
  }
}

void Evaluator::handleOperator(stack<int> &operands, stack<string> &operators,
                               const string &op) {
  while (!operators.empty() && precedence[operators.top()] >= precedence[op]) {
    processOperator(operands, operators);
  }
  operators.push(op);
}
// Main function here, processes all math
int Evaluator::eval(const string &expression) {
  stack<int> operands;
  stack<string> operators;
  string token;

  for (size_t i = 0; i < expression.length(); ++i) {
    char c = expression[i];

    if (isspace(c))
      continue;

    if (isdigit(c)) {
      int num = 0;
      while (i < expression.length() && isdigit(expression[i])) {
        num = num * 10 + (expression[i] - '0');
        ++i;
      }
      --i;
      operands.push(num);
    } else if (c == '(') {
      operators.push("(");
    } else if (c == ')') {
      while (!operators.empty() && operators.top() != "(") {
        processOperator(operands, operators);
      }
      if (operators.empty())
        throw invalid_argument("Mismatched parentheses");
      operators.pop();
    } else {
      string op(1, c);

      // handles + -
      if (c == '+' || c == '-') {
        if (i + 1 < expression.length() && expression[i + 1] == c) {
          op += c;
          ++i;
        }

        else if (i == 0 || expression[i - 1] == '(' ||
                 isUnary(string(1, expression[i - 1]))) {
          op = (c == '+') ? "+" : "-";
        }
      }

      // checks ><
      if (i + 1 < expression.length() &&
          (expression[i + 1] == '=' || expression[i + 1] == '>' ||
           expression[i + 1] == '<')) {
        op += expression[i + 1];
        ++i;
      }

      // checks * or |
      if (c == '&' || c == '|') {
        if (i + 1 < expression.length() && expression[i + 1] == c) {
          op += c;
          ++i;
        } else {
          throw invalid_argument("Invalid operator: " + string(1, c));
        }
      }
      // Majority error handling
      if (isUnary(op) && (operands.empty() || expression[i - 1] == '(')) {
        operators.push(op);
      } else if (isBinary(op)) {
        handleOperator(operands, operators, op);
      } else {
        throw invalid_argument("Invalid operator: " + op);
      }
    }
  }

  while (!operators.empty()) {
    processOperator(operands, operators);
  }

  if (operands.size() != 1)
    throw invalid_argument("Invalid expression");
  return operands.top();
}