#ifndef REPORTS_HPP
#define REPORTS_HPP
#include "parser.hpp"
#include "transaction.hpp"
#include <vector>

struct Day {
    int year = 0;
    int month = 0;
    int day = 0;
};

class AppMenu {
    Parser parser;
    std::string transformString(std::string text) const;
    bool isValidDate(int year, int month, int day) const; //checks if date is valid
    bool validateMonthInput(int year, int month) const; //validates month input
    int getFilterChoice() const;
    int getCatagoryChoice() const;
    Day getUserDayInput() const; //recieves YYYY-MM-DD input from user, returns year, month and day as integers
    void displayMainMenu() const; //1 print transactions, 2 View monthly spending breakdown, 3 View yearly spending breakdown,
    void displayReport(const std::vector<int>& indices) const;
    void displayCatagoryReport(const std::vector<int>& indices) const;
    void displayDailyTotalReport(const std::vector<int>& indices) const; //displays daily total report
    void displayFilterMenu() const; //1 Filter by name, 2 Filter by date, 3 Filter by amount, 4 Filter by month, 5 Filter by year,6 filter by catagory
    void displayCategoryMenu() const; //1 BILLS & UTIL, 2 FOOD & DRINK, 3 GROCERIES, 4 SHOPPING, 5 AUTO, 6 OTHER
    void printBarGraph(const int month, const int year) const; //prints a bar graph of the transactions in the provided month and year
    void printDoubleBarGraph(const int year) const; //prints a double bar graph of income vs expenses in the provided year
    std::string centerText(const std::string text, int width) const;

    std::vector<int> findHighestDailyTotal(std::vector<int> indices = {}) const; //finds the highest daily total from the transactions in the indices vector
    std::vector<int> findMiscIndices(std::vector<int> indices = {}) const; //finds indices of transactions that are not in the specified categories
    std::vector<int> filterByCategory(const std::vector <std::string>& filterNames, std::vector <int> indices = {}) const; //filters from transaction sub list
    std::vector<int> filterByName(const std::string& name, std::vector<int> indices = {}) const;
    std::vector<int> filterByDay(int day, int month, int year, std::vector<int> indices = {}) const; //filters transactions by day
    std::vector<int> filterByDates(int startYear, int startMonth, int startDay, int endYear, int endMonth, int endDay, std::vector<int> indices = {}) const;
    std::vector<int> filterByAmount(double minAmount, double maxAmount, std::vector<int> indices = {}) const;
    std::vector<int> filterByMonth(int month, int year, std::vector<int> indices = {}) const;
    std::vector<int> filterByYear(int year, std::vector<int> indices = {}) const;
    std::vector<std::string> loadFilterNames(int catagoryChoice) const; //loads filter names from a file, returns vector of strings
public:
    AppMenu();
    void run();

};

#endif