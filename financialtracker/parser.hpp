#ifndef PARSER_HPP
#define PARSER_HPP
#include <vector>
#include "transaction.hpp"
    struct ModeInfo {
        std::string name;
        int count = 0;
    };
class Parser {
    std::vector<Transaction> transactions;
public:
    Parser();
    bool load(const std::string &filename);

    std::vector<Transaction> getTransactions() const;
    std::vector<int> getAllIndices() const; //returns all indices of transactions
    std::string getTransactionName(const int index) const; 
    std::string getTransactionDate(const int index) const; //returns date in YYYY-MM-DD format
    int getTransactionDay(const int index) const;
    int getTransactionMonth(const int index) const;
    int getTransactionYear(const int index) const; 
    int getTransactionCount() const;
    std::string getTransactionInfo(const int index) const;
    double getTransactionAmount(const int index) const;
    std::string transformString(std::string text) const;

    int findMaxTransaction(const std::vector<int>& indices) const;
    int findMaxPayday(const std::vector<int>& indices) const;
    double findGrossTotal(const std::vector<int>& indices) const;
    double findIncome(const std::vector<int>& indices) const;
    double findExpenses(const std::vector<int>& indices) const;
    ModeInfo findMode(const std::vector<int>& indices) const;
    
    void printAllTransactions() const;
    void printTransactions(const std::vector<int>& indices) const;
    void printTransaction(const int index) const;

};


#endif