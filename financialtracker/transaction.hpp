#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP
#include <string>
class Transaction {
    int year, month, day;
    double amount;
    std::string name;
public:
    Transaction();
    Transaction(int year, int month, int day, double amount, const std::string &name);
    Transaction(const std::string &text);
    std::string transformString(std::string text) const;

    int getYear() const;
    int getMonth() const;
    int getDay() const;
    double getAmount() const;
    std::string getName() const;

    void setYear(int year);
    void setMonth(int month);
    void setDay(int day);
    void setAmount(double amount);
    void setName(const std::string &name);

    std::string info() const;
    bool isValid() const;
};


#endif