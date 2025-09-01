#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

// Function to return precedence of operators
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// Function to apply an operator to two operands
int applyOp(int a, int b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return b != 0 ? a / b : 0;
    }
    return 0;
}

// Main evaluation function
int evaluate(char* expression) {
    int values[MAX], valTop = -1;
    char ops[MAX], opTop = -1;
    int i = 0;

    while (i < strlen(expression)) {
        if (isspace(expression[i])) {
            i++;
            continue;
        }

        // If current token is a number
        if (isdigit(expression[i])) {
            int val = 0;
            while (i < strlen(expression) && isdigit(expression[i])) {
                val = val * 10 + (expression[i] - '0');
                i++;
            }
            values[++valTop] = val;
        }
        // If current token is '('
        else if (expression[i] == '(') {
            ops[++opTop] = expression[i];
            i++;
        }
        // If current token is ')'
        else if (expression[i] == ')') {
            while (opTop >= 0 && ops[opTop] != '(') {
                int b = values[valTop--];
                int a = values[valTop--];
                char op = ops[opTop--];
                values[++valTop] = applyOp(a, b, op);
            }
            if (opTop >= 0) opTop--; // Pop '('
            i++;
        }
        // If current token is an operator
        else if (strchr("+-*/", expression[i])) {
            while (opTop >= 0 && precedence(ops[opTop]) >= precedence(expression[i])) {
                int b = values[valTop--];
                int a = values[valTop--];
                char op = ops[opTop--];
                values[++valTop] = applyOp(a, b, op);
            }
            ops[++opTop] = expression[i];
            i++;
        }
    }

    // Apply remaining operators
    while (opTop >= 0) {
        int b = values[valTop--];
        int a = values[valTop--];
        char op = ops[opTop--];
        values[++valTop] = applyOp(a, b, op);
    }

    return values[valTop];
}

int main() {
    char expression[MAX];
    printf("Enter an arithmetic expression: ");
    fgets(expression, sizeof(expression), stdin);

    // Remove newline character if present
    size_t len = strlen(expression);
    if (len > 0 && expression[len - 1] == '\n') {
        expression[len - 1] = '\0';
    }

    int result = evaluate(expression);
    printf("Result: %d\n", result);
    return 0;
}
