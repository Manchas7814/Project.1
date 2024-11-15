#include <iostream>
#include <stack>
#include <string>
#include <fstream>
#include <cctype>

using namespace std;

// Define the structure of a tree node
class TreeNode {
public:
    string value;  // Can be an operator or an operand (number)
    TreeNode* left;
    TreeNode* right;

    TreeNode(string val) : value(val), left(nullptr), right(nullptr) {}
};

// ExpressionTree class to manage tree building and evaluation
class ExpressionTree {
public:
    TreeNode* root;

    ExpressionTree() : root(nullptr) {}

    // Function to validate the expression for syntax errors
    bool isValidExpression(const string& expr) {
        int openParentheses = 0;
        bool lastWasOperator = false;

        for (int i = 0; i < expr.size(); ++i) {
            char ch = expr[i];

            // Ignore spaces
            if (ch == ' ') continue;

            // Check for invalid characters
            if (!isdigit(ch) && !isOperator(ch) && ch != '(' && ch != ')') {
                cout << "Error: Invalid character '" << ch << "' in expression.\n";
                return false;
            }

            // Parentheses balance check
            if (ch == '(') {
                openParentheses++;
                lastWasOperator = false;
            } else if (ch == ')') {
                openParentheses--;
                if (openParentheses < 0) {
                    cout << "Error: Mismatched parentheses.\n";
                    return false;
                }
                lastWasOperator = false;
            }

            // Check for consecutive operators
            if (isOperator(ch)) {
                if (lastWasOperator) {
                    cout << "Error: Consecutive operators or operators at the beginning/end.\n";
                    return false;
                }
                lastWasOperator = true;
            } else {
                lastWasOperator = false;
            }
        }

        // Ensure balanced parentheses and no operator at the end
        if (openParentheses != 0 || lastWasOperator) {
            cout << "Error: Expression ends with an operator or has unbalanced parentheses.\n";
            return false;
        }

        return true;
    }

    // Convert an infix expression to postfix (RPN)
    void infixToPostfix(const string& expr) {
        stack<char> operators;  // Stack to hold operators
        stack<TreeNode*> operands;  // Stack to hold operands

        for (int i = 0; i < expr.size(); ++i) {
            char ch = expr[i];

            if (isspace(ch)) {
                continue;  // Ignore spaces
            }

            if (isdigit(ch)) {
                // Extract the entire number (in case it's more than one digit)
                string num;
                while (i < expr.size() && (isdigit(expr[i]) || expr[i] == '.')) {
                    num += expr[i++];
                }
                i--;  // Decrement i to correct for the last increment
                operands.push(new TreeNode(num));
            }
            else if (ch == '(') {
                operators.push(ch);
            }
            else if (ch == ')') {
                while (!operators.empty() && operators.top() != '(') {
                    processOperator(operators, operands);
                }
                operators.pop();  // Pop '('
            }
            else if (isOperator(ch)) {
                while (!operators.empty() && precedence(operators.top()) <= precedence(ch)) {
                    processOperator(operators, operands);
                }
                operators.push(ch);
            }
        }

        // Pop remaining operators in the stack
        while (!operators.empty()) {
            processOperator(operators, operands);
        }

        // The final expression tree is the last operand in the stack
        root = operands.top();
    }

    // Method to evaluate the expression tree
    double evaluate(TreeNode* node) {
        if (!node) return 0;

        // If it's a leaf (number), return its value
        if (!node->left && !node->right) {
            return stod(node->value);
        }

        // Recursively evaluate left and right children
        double leftValue = evaluate(node->left);
        double rightValue = evaluate(node->right);

        // Perform the operation based on the operator at the current node
        if (node->value == "+") {
            return leftValue + rightValue;
        }
        else if (node->value == "-") {
            return leftValue - rightValue;
        }
        else if (node->value == "*") {
            return leftValue * rightValue;
        }
        else if (node->value == "/") {
            if (rightValue == 0) {
                cout << "Error: Division by zero\n";
                return 0;  // Return 0 or handle division by zero error
            }
            return leftValue / rightValue;
        }
        cout << "Error: Unsupported operator " << node->value << endl;
        return 0;
    }

    // Wrapper method to evaluate the expression tree
    double evaluate() {
        return evaluate(root);
    }

    // Helper method to print the expression tree (inorder traversal)
    void printInOrder(TreeNode* node) {
        if (node == nullptr) return;
        printInOrder(node->left);
        cout << node->value << " ";
        printInOrder(node->right);
    }

    // In-order traversal (for infix expression)
    void printInOrder() {
        printInOrder(root);
        cout << endl;
    }

    // Pre-order traversal (for prefix expression)
    void printPreOrder(TreeNode* node) {
        if (node == nullptr) return;
        cout << node->value << " ";
        printPreOrder(node->left);
        printPreOrder(node->right);
    }

    // Wrapper for pre-order traversal (prefix expression)
    void printPreOrder() {
        printPreOrder(root);
        cout << endl;
    }

    // Post-order traversal (for postfix expression)
    void printPostOrder(TreeNode* node) {
        if (node == nullptr) return;
        printPostOrder(node->left);
        printPostOrder(node->right);
        cout << node->value << " ";
    }

    // Wrapper for post-order traversal (postfix expression)
    void printPostOrder() {
        printPostOrder(root);
        cout << endl;
    }

private:
    // Check if a character is a valid operator
    bool isOperator(char ch) {
        return ch == '+' || ch == '-' || ch == '*' || ch == '/';
    }

    // Determine the precedence of operators
    int precedence(char op) {
        if (op == '(' || op == ')') return 0;  // Parentheses have highest precedence
        if (op == '*' || op == '/') return 1;  // * and / have precedence 1
        if (op == '+' || op == '-') return 2;  // + and - have precedence 2
        return -1;  // For other cases
    }

    // Process the operator: pop two operands and create a node
    void processOperator(stack<char>& operators, stack<TreeNode*>& operands) {
        char op = operators.top();
        operators.pop();

        TreeNode* right = operands.top();
        operands.pop();
        TreeNode* left = operands.top();
        operands.pop();

        // Create a new node for the operator
        TreeNode* opNode = new TreeNode(string(1, op));
        opNode->left = left;
        opNode->right = right;

        // Push the new operator node to the operand stack
        operands.push(opNode);
    }
};
