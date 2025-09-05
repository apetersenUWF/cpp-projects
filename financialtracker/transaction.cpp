#include "transaction.hpp"
#include <sstream>
#include <string>
#include <iostream>
#include <iomanip>
using namespace std;
    Transaction::Transaction() : year(2000), month(1), day(1), amount(0.0), name("") {}
    Transaction::Transaction(int year, int month, int day, double amount, const std::string &name) :   year(year), month(month), day(day), amount(amount), name(name) {}
    /*Transaction::Transaction(const std::string &text) { //basic csv format
        //Assuming the format is "year-month-day,name,amount"
        stringstream ss(text);
        string yearStr, monthStr, dayStr, amountStr, nameStr;
        getline(ss, yearStr, '-');
        getline(ss, monthStr, '-');
        getline(ss, dayStr, ',');
        getline(ss, nameStr, ',');
        getline(ss, amountStr);
        year = stoi(yearStr);
        month = stoi(monthStr);
        day = stoi(dayStr);
        amount = stod(amountStr);
        name = nameStr;
    }*/
        Transaction::Transaction(const std::string &text) { //Parsing USAA transaction format
        stringstream ss(text);
        string yearStr, monthStr, dayStr, amountStr, nameStr, ignore;
        getline(ss, yearStr, '-');
        getline(ss, monthStr, '-');
        getline(ss, dayStr, ',');
        getline(ss, nameStr, ',');
        getline(ss, ignore, ',');
        getline(ss, ignore, ',');
        getline(ss, amountStr, ',');
        year = stoi(yearStr);
        month = stoi(monthStr);
        day = stoi(dayStr);
        amount = stod(amountStr);
        name = nameStr;
    }

    int Transaction::getYear() const {return year;}
    int Transaction::getMonth() const {return month;}
    int Transaction::getDay() const {return day;}
    double Transaction::getAmount() const {return amount;}
    std::string Transaction::getName() const {return name;}

    void Transaction::setYear(int year) {
        this->year = year;
    }
    void Transaction::setMonth(int month) {
        this->month = month;
    }
    void Transaction::setDay(int day) {
        this->day = day;
    }
    void Transaction::setAmount(double amount) {
        this->amount = amount;
    }
    void Transaction::setName(const std::string &name) {
        this->name = name;
    }

    std::string Transaction::info() const {
        stringstream ss;
        ss << setw(4) << setfill('0') << year <<"-" << setw(2) <<  month << "-" << setw(2) << day << setfill(' ') << ", " << name << ": $ " << fixed << setprecision(2) << amount;
        return ss.str();
    }
    bool Transaction::isValid() const {
        if ( year < 0 || month < 1 || month > 12 || day < 1 || day > 31 || name.empty()) {
            return false;
        }
        else {
            return true;
        }
    }