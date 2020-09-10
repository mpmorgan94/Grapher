#ifndef CALCULATE_H
#define CALCULATE_H

#include <string>
#include <stack>
#include <map>
#include <vector>
#include <fstream>

class Calculate
{

public:

    Calculate(){}
    Calculate(std::string equation, double x);

    //Reads Equation String
    void readEquation(std::string equation, double x);

    //Writes results to private answer data value
    void returnResults(std::string answer);

    //Where calculations are done
    void popAndProcess();

    //Load Maps
    void loadPriorityTableMap();

    //Checks if token is an operator
    bool isOperator(std::string tokenToCheck);

    //Computes the new value and return that value
    double findNewValue(double a, double b, char op);

    //Resets stacks
    void resetStacks();

    //getter for answer
    double getAnswer() {return answer;}


private:

    double answer = 0.0;

    std::stack<char> op;
    std::stack<int> priority;
    std::stack<std::string> value;

    std::map<std::string, int> inputPriorityTable;
    std::map<std::string, int> stackPriorityTable;

    std::vector<double> answers;
    std::vector <std::string> tokens;

    std::ifstream inFileStream;
    std::ofstream outFileStream;

};


#endif