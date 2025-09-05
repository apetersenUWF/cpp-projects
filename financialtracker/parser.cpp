#include "parser.hpp"
#include <fstream>
#include <iostream>
using namespace std;


Parser::Parser() {}


bool Parser::load(const std::string &filename) {
    ifstream inFS;
    string infoStr, header;
        inFS.open(filename);
        if (!inFS.is_open()) {
            cout << "File failed to open" << endl;
            return false;
        }
    getline(inFS, header); // Read and ignore the header line
    while (getline (inFS, infoStr)) {
        Transaction tempTransaction(infoStr);
        transactions.push_back(tempTransaction);
        if (!tempTransaction.isValid()) {
            cout << "Invalid transaction: " << infoStr << endl;
            return false;
    }
}
    cout << "File successfully loaded!" << endl;
    inFS.close();
    return true;}


void Parser::printAllTransactions() const {
    for (int i = transactions.size()-1; i >=0; i--) {//prints transactions in reverse order
        cout << transactions.at(i).info() << endl;
    }
}
void Parser::printTransactions(const std::vector<int>& indices) const {
    for (int i = indices.size()-1; i >= 0; i--) { //prints transactions in reverse order
        cout << transactions.at(indices.at(i)).info() << endl;
    }
}
void Parser::printTransaction(const int index) const {
    cout << transactions.at(index).info() << endl;
}


std::string Parser::getTransactionName(const int index) const {return transactions.at(index).getName();}
double Parser::getTransactionAmount(const int index) const {return transactions.at(index).getAmount();}
int Parser::getTransactionDay(const int index) const {return transactions.at(index).getDay();}
int Parser::getTransactionMonth(const int index) const {return transactions.at(index).getMonth();}
int Parser::getTransactionYear(const int index) const {return transactions.at(index).getYear();}
std::string Parser::getTransactionInfo(const int index) const {return transactions.at(index).info();}


int Parser::findMaxTransaction(const std::vector<int>& indices) const { //returns the index of the most negative transaction (largest expense)
    if (indices.empty()) return -1; // No transactions available
    int maxTransactionIndex = indices.at(0);
    for (int i = 1; i < indices.size(); i++) {
        if (transactions.at(indices.at(i)).getAmount() < transactions.at(maxTransactionIndex).getAmount()) {
            maxTransactionIndex = indices.at(i);
        }
    }
    return maxTransactionIndex;
}
int Parser::findMaxPayday(const std::vector<int>& indices) const {
    if (indices.empty()) return -1; // No transactions available
    int maxPaydayIndex = indices.at(0);
    for (int i = 1; i < indices.size(); i++) {
        if (transactions.at(indices.at(i)).getAmount() > transactions.at(maxPaydayIndex).getAmount()) {
            maxPaydayIndex = indices.at(i);
        }
    }
    if (transactions.at(maxPaydayIndex).getAmount() <= 0) {
        maxPaydayIndex = -1; // No payday transactions found
    }
    return maxPaydayIndex;
}
double Parser::findIncome(const std::vector<int>& indices) const {
    double total = 0.0;
    for (int i = 0; i < indices.size(); i++) {
        if (transactions.at(indices.at(i)).getAmount() > 0) {
            total += transactions.at(indices.at(i)).getAmount();
        }
    }
    return total;
}
double Parser::findExpenses(const std::vector<int>& indices) const {
    double total = 0.0;
    for (int i = 0; i < indices.size(); i++) {
        if (transactions.at(indices.at(i)).getAmount() < 0) {
            total += transactions.at(indices.at(i)).getAmount();
        }
    }
    return total;
}
// getGrossTotal returns the sum of all transactions (expenses + income)
double Parser::findGrossTotal(const std::vector<int>& indices) const {
    double total = 0.0;
    for (int i = 0; i < indices.size(); i++) {
        total += transactions.at(indices.at(i)).getAmount();
    }
    return total;
}

int Parser::getTransactionCount() const {return transactions.size();}

ModeInfo Parser::findMode(const std::vector<int>& indices) const {
    ModeInfo mode;
    mode.count = 1;
    int tempCount = 0;
    std::vector<std::string> names;
    for (int i = 0; i < indices.size(); i++) {
        if (transactions.at(indices.at(i)).getAmount() < 0) { //only include expenses in mode search
            names.push_back(transactions.at(indices.at(i)).getName());
        }
    }
    for (int i = 0; i < names.size(); i++) {
        for (int j = 0; j < names.size(); j++) {
            if (transformString(names.at(i)) == transformString(names.at(j))) {
                tempCount++;
            }
        }
            if (tempCount > mode.count) {
                mode.count = tempCount;
                mode.name = names.at(i);
            }
        tempCount = 0; // Reset count for the next name    
    }
    if (mode.count <= 1) {
        mode.name = "";
        mode.count = 0;
    }
    return mode;
}

std::string Parser::transformString(std::string text) const {
        //Transform the string to a more basic format,all lowercase, no punctuation or whitespace
        for (int i = 0; i < text.size(); i++) {
            if (isspace(text[i])) {
                text.erase(i, 1); // Remove whitespace
                i--; // Adjust index after removal
            }
        }
        for (int i = 0; i < text.size(); i++) {
            if (ispunct(text[i])) {
                text.erase(i, 1); // Remove punctuation
                i--; // Adjust index after removal
            }
        }
        for (int i = 0; i < text.size(); i++) {
            text[i] = tolower(text[i]); // Convert to lowercase
        }
        return text;
    }
    std::vector<Transaction> Parser::getTransactions() const { return transactions; }

    std::vector<int> Parser::getAllIndices() const {
        std::vector<int> indices;
        for (int i = 0; i < transactions.size(); i++) {
            indices.push_back(i);
        }
        return indices;
    }
    std::string Parser::getTransactionDate(const int index) const {
        return std::to_string(transactions.at(index).getYear()) + "-" +
               std::to_string(transactions.at(index).getMonth()) + "-" +
               std::to_string(transactions.at(index).getDay());
    }