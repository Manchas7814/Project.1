#include "binary_tree.h"

int main() {
    // Open the file with the arithmetic expressions
    ifstream infile("expressions.txt");

    if (!infile) {
        cerr << "Error opening file!" << endl;
        return 1;
    }

    string expr;
    ExpressionTree tree;

    // Process each line from the file
    while (getline(infile, expr)) {
        cout << "Processing expression: " << expr << endl;

        // First, check if the expression is valid
        if (!tree.isValidExpression(expr)) {
            cout << "Skipping invalid expression..." << endl;
            continue;  // Skip this expression and move to the next one
        }

        // Build the expression tree
        tree.infixToPostfix(expr);

        // Print the expression tree in In-Order (infix)
        cout << "Infix (In-order) Expression: ";
        tree.printInOrder();

        // Print the expression tree in Pre-Order (prefix)
        cout << "Prefix (Pre-order) Expression: ";
        tree.printPreOrder();

        // Print the expression tree in Post-Order (postfix)
        cout << "Postfix (Post-order) Expression: ";
        tree.printPostOrder();

        // Evaluate the expression and print the result
        cout << "Result of the expression: " << tree.evaluate() << endl;
        cout << "-----------------------------" << endl;
    }
    infile.close();
    return 0;
}