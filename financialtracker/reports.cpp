#include "reports.hpp"
#include <iostream>
#include <vector>
#include <iomanip>
#include <cctype>
#include <fstream>
using namespace std;

    AppMenu::AppMenu() {}
    bool AppMenu::isValidDate(int year, int month, int day) const {
        bool isValid = true;
        if (year < 1900 || year > 2100 || month < 1 || month > 12 || day < 1 || day > 31) {
            isValid = false;
        }
        return isValid;
    }
    Day AppMenu::getUserDayInput() const {//recieves YYYY-MM-DD input from user, returns year, month and day as integers
        Day input;
        std::string dateInput;
        cout << "Enter date (YYYY-MM-DD): ";
            while(dateInput.empty()) 
            getline(cin, dateInput);
            if(dateInput.length() == 10 || dateInput[4] == '-' || dateInput[7] == '-') {
            input.year = stoi(dateInput.substr(0, 4));
            input.month = stoi(dateInput.substr(5, 2));
            input.day = stoi(dateInput.substr(8, 2));
            }
        while (dateInput.length() != 10 || dateInput[4] != '-' || dateInput[7] != '-' || !isValidDate(input.year, input.month, input.day)) {
            cout << "Invalid date format. Please enter in YYYY-MM-DD format." << endl;
            cout << "Enter date (YYYY-MM-DD): ";
            getline(cin, dateInput);
            input.year = stoi(dateInput.substr(0, 4));
            input.month = stoi(dateInput.substr(5, 2));
            input.day = stoi(dateInput.substr(8, 2));
        }
        return input;
    }
    void AppMenu::displayReport(const std::vector<int>& indices) const {
        if (indices.empty()) {
            cout << "No transactions found for the selected criteria." << endl;
            return;
        }
        cout << "-----Transactions Report-----" << endl;
        parser.printTransactions(indices);
        cout << "Total Transactions: " << indices.size() << endl;
        cout << "Gross Total: $" << parser.findGrossTotal(indices) << endl;
        cout << "Total Income: $" << parser.findIncome(indices) << endl;
        cout << "Total Expenses: $" << parser.findExpenses(indices) << endl;
        cout << "Max Transaction: " << parser.getTransactionInfo(parser.findMaxTransaction(indices)) << endl;
        if (parser.findMaxPayday(indices) == -1) {
            cout << "No payday transactions found." << endl;
        } else {
        cout << "Max Payday: " << parser.getTransactionInfo(parser.findMaxPayday(indices)) << endl;
        }
        ModeInfo mode = parser.findMode(indices);
        if (mode.count > 0) {
            cout << "Most Frequent Transaction: " << mode.name << " (" << mode.count << " times)" << endl;
        } else {
            cout << "No frequent transactions found." << endl;
        }

    }

    void AppMenu::displayDailyTotalReport(const std::vector<int>& indices) const {
            if (indices.empty()) {
            return;
        }
        std::vector <int> highestDailyTotalIndices = findHighestDailyTotal(indices);
            cout << "-----Highest Daily Total----- ";
            cout << "Date: " << parser.getTransactionDate(highestDailyTotalIndices.at(0)) << endl;
            parser.printTransactions(highestDailyTotalIndices);
            cout << "Total Transactions: " << highestDailyTotalIndices.size() << endl;
            cout << "Total Expenses: $" << parser.findExpenses(highestDailyTotalIndices) << endl;
    }
    void AppMenu::displayMainMenu() const {//1 print transactions, 2 View monthly spending breakdown, 3 View yearly spending breakdown,
        cout << "Financial Analyzing Tool Main Menu:" << endl;
        cout << "1. Print All Transactions" << endl;
        cout << "2. View Monthly Spending Breakdown" << endl;
        cout << "3. View Yearly Spending Breakdown" << endl;
        cout << "4. Filter Transactions" << endl;
        cout << "5. Exit" << endl;
    } 
    void AppMenu::displayFilterMenu() const { //1 Filter by name, 2 Filter by date, 3 Filter by amount, 4 Filter by month, 5 Filter by year,6 filter by catagory
        cout << "Choose a filter option to view trnsaction information:" << endl;
        cout << "1. Filter by Name" << endl;
        cout << "2. Filter by Date" << endl;
        cout << "3. Filter by Amount" << endl;
        cout << "4. Filter by Month" << endl;
        cout << "5. Filter by Year" << endl;
        cout << "6. Filter by Category" << endl;
        cout << "7. Return to Main Menu (Resets Filter Choices)" << endl;
    }
    void AppMenu::displayCategoryMenu() const {//1 BILLS & UTIL, 2 FOOD & DRINK, 3 GROCERIES, 4 SHOPPING, 5 AUTO, 6 UNCATAGORIZED
        cout << "Choose a category to filter by:" << endl;
        cout << "1. BILLS & UTILITIES" << endl;
        cout << "2. FOOD & DRINK" << endl;
        cout << "3. GROCERIES" << endl;
        cout << "4. SHOPPING" << endl;
        cout << "5. AUTO" << endl;
        cout << "6. UNCAZTAGORIZED" << endl;
    } 

    std::vector<int> AppMenu::filterByCategory(const std::vector <std::string>& filterNames, std::vector <int> indices) const {
        std::vector<int> filteredResults;
        if (indices.empty()) {
            indices = parser.getAllIndices(); // If no indices provided, use all transactions
        }
        for (int i = 0; i < indices.size(); i++) {
            for (int j = 0; j < filterNames.size(); j++) {
                if ((transformString(parser.getTransactionName(indices.at(i))).find(filterNames.at(j)) != std::string::npos)) { //if match found 
                    filteredResults.push_back(indices.at(i)); //add index to another vector
                    break; // Break to avoid adding the same transaction multiple times
                }
            }
        }
        return filteredResults;
    }
    std::vector<int> AppMenu::filterByName(const std::string& name, std::vector<int> indices) const {
            if (indices.empty()) {
            indices = parser.getAllIndices(); // If no indices provided, use all transactions
        }
        std::vector<int> filteredResults;
        std::string transformedName = transformString(name); // Transform the name to a basic format
        for (int i = 0; i < indices.size(); i++) {
            if (transformString(parser.getTransactionName(indices.at(i))).find(transformedName) != std::string::npos) {//search for the transformed name in the transaction name
                filteredResults.push_back(indices.at(i)); // Add the index to the results if the name matches
            }
        }
        return filteredResults;
    }
    std::vector<int> AppMenu::filterByDates(int startYear, int startMonth, int startDay, int endYear, int endMonth, int endDay, std::vector<int> indices) const {
        std::vector <int> filteredResults;
            if (indices.empty()) {
            indices = parser.getAllIndices(); // If no indices provided, use all transactions
        }
            const long startDate = startYear * 10000 + startMonth * 100 + startDay;
            const long endDate = endYear * 10000 + endMonth * 100 + endDay;
        for (int i = 0; i < indices.size(); i++) {
            int checkYear = parser.getTransactionYear(indices.at(i));
            int checkMonth = parser.getTransactionMonth(indices.at(i));
            int checkDay = parser.getTransactionDay(indices.at(i));
            long checkDate = checkYear * 10000 + checkMonth * 100 + checkDay;
                                                                                        //converts inputs to YYYYMMDD format for easier comparison
            if (checkDate >= startDate && checkDate <= endDate)
                filteredResults.push_back(indices.at(i));  
    }
    return filteredResults;
    }
    std::vector<int> AppMenu::filterByDay(int day, int month, int year, std::vector<int> indices) const {
        std::vector<int> filteredResults;
        if (indices.empty()) {
            indices = parser.getAllIndices(); // If no indices provided, use all transactions
        }
        for (int i = 0; i < indices.size(); i++) {
            if (parser.getTransactionDay(indices.at(i)) == day &&
                parser.getTransactionMonth(indices.at(i)) == month &&
                parser.getTransactionYear(indices.at(i)) == year) {
                filteredResults.push_back(indices.at(i));
            }
        }
        return filteredResults;
    }//here
    std::vector<int> AppMenu::filterByAmount(double minAmount, double maxAmount, std::vector<int> indices) const {
        std::vector<int> filteredResults;
               if (indices.empty()) {
                indices = parser.getAllIndices(); // If no indices provided, use all transactions
        }
        for (int i = 0; i < indices.size(); i++) {
            double amount = parser.getTransactionAmount(indices.at(i));
            if (amount >= minAmount && amount <= maxAmount) {
                filteredResults.push_back(indices.at(i)); // Add the index to the results if the amount is within the range
            }
        }
        return filteredResults;
    }
    std::vector<int> AppMenu::filterByMonth(int month, int year, std::vector<int> indices) const {
        std::vector<int> filteredResults;
                if (indices.empty()) {
                indices = parser.getAllIndices(); // If no indices provided, use all transactions
        }
        for (int i = 0; i < indices.size(); i++) {
            if (parser.getTransactionMonth(indices.at(i)) == month && parser.getTransactionYear(indices.at(i)) == year) {
                filteredResults.push_back(indices.at(i));
            }
        }
        return filteredResults;
    }
    std::vector<int> AppMenu::filterByYear(int year, std::vector<int> indices) const {
        std::vector<int> filteredResults;
                if (indices.empty()) {
                indices = parser.getAllIndices(); // If no indices provided, use all transactions
        }
        for (int i = 0; i < indices.size(); i++) {
            if (parser.getTransactionYear(indices.at(i)) == year) {
                filteredResults.push_back(indices.at(i));
            }
        }
        return filteredResults;
    }
    std::vector<std::string> AppMenu::loadFilterNames(int catagoryChoice) const {
        std::vector<std::string> filterNames;
        std::string filename;
        switch(catagoryChoice) {
            case 1: filename = "bills_utilities.txt"; break;
            case 2: filename = "food_drinks.txt"; break;
            case 3: filename = "groceries.txt"; break;
            case 4: filename = "shopping.txt"; break;
            case 5: filename = "auto.txt"; break;
            case 6: {   std::vector<std::string> tempNames; //recursive
                        for (int i = 1; i < 6; i++) {
                        tempNames = loadFilterNames(i);
                        filterNames.insert(filterNames.end(), tempNames.begin(), tempNames.end());
                        }
                        return filterNames; //returns all categories combined
                break;
            }
            default: cout << "Invalid category choice." << endl;
        }
        ifstream inFS;
        string line;
        inFS.open(filename);
        if (!inFS.is_open()) {
            cout << "Failed to open filter names file: " << filename << endl;
            return filterNames;
        }
        getline(inFS, line); // Read and ignore the header line
        while(getline(inFS, line)) {
            if (!line.empty()) {
                line = transformString(line);
                filterNames.push_back(line);
            }
        }
        inFS.close();
        return filterNames;
    }

    int AppMenu::getCatagoryChoice() const {
        int choice = 0;
        while (choice < 1 || choice > 6) {
        displayCategoryMenu();
        cin >> choice;
        }
        return choice;
    }
    int AppMenu::getFilterChoice() const {
        int choice = 0;
        while (choice < 1 || choice > 7) {
        displayFilterMenu();
        cin >> choice;
        }
        return choice;
    }
    void AppMenu::displayCatagoryReport(const std::vector<int>& indices) const {
        if (indices.empty()) {
            cout << "No transactions found for the selected criteria." << endl;
            return;
        }
        double  bills, food, groceries, shopping, transport, other,
                billspct, foodpct, groceriespct, shoppingpct, transportpct, otherpct;
        bills = parser.findExpenses(filterByCategory(loadFilterNames(1), indices));
        food = parser.findExpenses(filterByCategory(loadFilterNames(2), indices));
        groceries = parser.findExpenses(filterByCategory(loadFilterNames(3), indices));
        shopping = parser.findExpenses(filterByCategory(loadFilterNames(4), indices));
        transport = parser.findExpenses(filterByCategory(loadFilterNames(5), indices));
        other = parser.findExpenses(findMiscIndices(indices));
        billspct = (bills / parser.findExpenses(indices)) * 100;
        foodpct = (food / parser.findExpenses(indices)) * 100;
        groceriespct = (groceries / parser.findExpenses(indices)) * 100;
        shoppingpct = (shopping / parser.findExpenses(indices)) * 100;
        transportpct = (transport / parser.findExpenses(indices)) * 100;
        otherpct = (other / parser.findExpenses(indices)) * 100;
        cout << "BILLS & UTILITIES: $" << bills << " (" << fixed << setprecision(2) << billspct << "%)" << endl;
        cout << "FOOD & DRINK: $" << food << " (" << fixed << setprecision(2) << foodpct << "%)" << endl;
        cout << "GROCERIES: $" << groceries << " (" << fixed << setprecision(2) << groceriespct << "%)" << endl;
        cout << "SHOPPING: $" << shopping << " (" << fixed << setprecision(2) << shoppingpct << "%)" << endl;
        cout << "AUTO: $" << transport << " (" << fixed << setprecision(2) << transportpct << "%)" << endl;
        cout << "OTHER: $" << other << " (" << fixed << setprecision(2) << otherpct << "%)" << endl;

        if (billspct + foodpct + groceriespct + shoppingpct + transportpct + otherpct > 100.1) {
            cout << "Warning: Total percentage exceeds 100% likely due to overlapping catagory filters" << endl;
            cout << "Consider using more specific filters to avoid this." << endl;
            cout << (billspct + foodpct + groceriespct + shoppingpct + transportpct + otherpct) << "% total" << endl;
        }
    }
    void AppMenu::run() {
        parser.load("USAAtransactionreport.csv"); // Load transactions from file
        int choice = 0;
        char continueFiltering;
        while (choice != 5) {
            displayMainMenu();
            cin >> choice;
            switch (choice) {
                case 1: {
                    parser.printAllTransactions();
                    break;
                }
                case 2: {
                    int month, year;
                    cout << "Enter month (1-12):";
                    cin >> month;
                    cout << "Enter year: ";
                    cin >> year;
                    while (month < 1 || month > 12 || year < 1900 || year > 2100) {
                        cout << "Invalid month or year. Please try again." << endl;
                        cout << "Enter month (1-12): ";
                        cin >> month;
                        cout << "Enter year: ";
                        cin >> year;
                    }
                    std::vector<int> selectedIndices = filterByMonth(month, year);
                    displayReport(selectedIndices);
                    displayDailyTotalReport(selectedIndices);
                    cout << "*****Monthly Spending Breakdown for " << month << "/" << year << "*****" << endl;
                    printBarGraph(month, year);
                    displayCatagoryReport(selectedIndices);
                    break;
                }
                case 3: {
                    int year = 0;
                    while (year < 1900 || year > 2100) {
                    cout << "Enter year: ";
                    cin >> year;
                    }
                    std::vector<int> selectedIndices = filterByYear(year);
                    displayReport(selectedIndices);
                    displayDailyTotalReport(selectedIndices);
                    cout << "*****Yearly Spending Breakdown for " << year << "*****" << endl;
                    printDoubleBarGraph(year);
                    displayCatagoryReport(selectedIndices);
                    break;
                }
                case 4:
                    std::vector<int> filteredIndices;
                    do {
                    int filterChoice = getFilterChoice(); // Get filter choice from user
                    switch (filterChoice) {
                        case 1: {
                            string name;
                            while(name.empty()) {
                            cout << "Enter name to filter by: ";
                            cin.ignore();
                            getline(cin, name);
                            }
                            filteredIndices = filterByName(name, filteredIndices);
                            displayReport(filteredIndices);
                            displayDailyTotalReport(filteredIndices);
                            break;
                        }
                        case 2: {
                            cout << "Start Date:" << endl;
                            Day start = getUserDayInput();
                            cout << "End Date:" << endl;
                            Day end = getUserDayInput();
                            filteredIndices = filterByDates(start.year, start.month, start.day, end.year, end.month, end.day, filteredIndices);
                            displayReport(filteredIndices);
                            displayDailyTotalReport(filteredIndices);
                            break;
                        }
                        case 3: {
                            double minAmount = 1.0; // Initialize to a value greater than maxAmount
                            double maxAmount = 0.0;
                            cout << "Use negative values for expenses, positive values for income." << endl;
                            cout << "Example: -100.00 -25.00 finds expenses between -100.00 and -25.00" << endl;
                            while(minAmount > maxAmount) {
                            cout << "Enter minimum amount: ";
                            cin >> minAmount;
                            cout << "Enter maximum amount: ";
                            cin >> maxAmount;
                            }
                            filteredIndices = filterByAmount(minAmount, maxAmount, filteredIndices);
                            displayReport(filteredIndices);
                            displayDailyTotalReport(filteredIndices);
                            break;
                        }
                        case 4: {
                            int month = 0;
                            int year = 0;
                            while (!validateMonthInput(year, month)) {
                            cout << "Enter month (1-12): ";
                            cin >> month;
                            cout << "Enter year: ";
                            cin >> year;
                            }
                            filteredIndices = filterByMonth(month, year, filteredIndices);
                            displayReport(filteredIndices);
                            displayDailyTotalReport(filteredIndices);
                            break;
                        }
                        case 5: {
                            int year = 0;
                            while (year < 1900 || year > 2100) {
                            cout << "Enter year: ";
                            cin >> year;
                            }
                            filteredIndices = filterByYear(year, filteredIndices);
                            displayReport(filteredIndices);
                            displayDailyTotalReport(filteredIndices);
                            break;
                        }
                        case 6: {
                            int catagoryChoice = getCatagoryChoice();
                            if(catagoryChoice == 6)
                                filteredIndices = findMiscIndices(filteredIndices); // Find indices of transactions that are not in the specified categories
                            else 
                                filteredIndices = filterByCategory(loadFilterNames(catagoryChoice), filteredIndices);
                                displayReport(filteredIndices);
                                displayDailyTotalReport(filteredIndices);
                                break;
                        }
                        default: {
                            cout << "Returning to main menu." << endl;
                            break;
                        }
    }
    cout << "Filter further? (Y/N): ";
        cin >> continueFiltering;
}while (toupper(continueFiltering) == 'Y');
}}}

    std::vector<int> AppMenu::findMiscIndices(std::vector<int> indices) const {
        // Finds indices of transactions that are not in the specified categories
        std::vector<int> miscIndices;
        if (indices.empty()) {
            indices = parser.getAllIndices(); // If no indices provided, use all transactions
        }
        std::vector<std::string> filterNames = loadFilterNames(6); // Loads all category names together
        for (int i = 0; i < indices.size(); i++) {
            bool isMisc = true;
            for (int j = 0; j < filterNames.size(); j++) {
                if (transformString(parser.getTransactionName(indices.at(i))).find(filterNames.at(j)) != std::string::npos) {
                    isMisc = false;
                    break;
                }
            }
            if (isMisc) {
                miscIndices.push_back(indices.at(i));
            }
        }
        return miscIndices;
    }

    std::string AppMenu::transformString(std::string text) const {
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

    bool AppMenu::validateMonthInput(int year, int month) const{
        bool isValid = true;
        if (year < 1900 || year > 2100)
            isValid = false;
        if (month < 1 || month > 12)
            isValid = false;
        return isValid;
    }

    std::vector<int> AppMenu::findHighestDailyTotal(std::vector<int> indices) const {
        Day checkDay, maxDay;
        std::vector<int> highestDailyIndices;
        double highestTotal = 0.0;
        double checkTotal;
        if (indices.empty()) {
            indices = parser.getAllIndices(); // If no indices provided, use all transactions
        }   
            if (parser.getTransactionAmount(indices.at(0)) < 0) {
                checkTotal = parser.getTransactionAmount(indices.at(0));
            }
            else {
                checkTotal = 0.0; // Initialize to 0 if the first transaction is not an expense
            }
            checkDay.year = parser.getTransactionYear(indices.at(0));
            checkDay.month = parser.getTransactionMonth(indices.at(0));
            checkDay.day = parser.getTransactionDay(indices.at(0));
        for (int i = 1; i < indices.size(); i++) {
            if(checkDay.year == parser.getTransactionYear(indices.at(i)) &&  //if same day
            checkDay.day == parser.getTransactionDay(indices.at(i))) {
                if(parser.getTransactionAmount(indices.at(i)) < 0) {
                    // If the transaction is an expense, add to the total for that day
                checkTotal += parser.getTransactionAmount(indices.at(i));
            }
            }
            else {
                if(checkTotal < highestTotal) {
                    highestTotal = checkTotal;  //keep track of highest total and day
                    maxDay = checkDay;
                }
                checkDay.year = parser.getTransactionYear(indices.at(i));
                checkDay.month = parser.getTransactionMonth(indices.at(i));
                checkDay.day = parser.getTransactionDay(indices.at(i));
                if (parser.getTransactionAmount(indices.at(i)) < 0) {
                    checkTotal = parser.getTransactionAmount(indices.at(i)); // Reset total for the new day
                } else {
                    checkTotal = 0.0; // Reset to 0 if the transaction is not an expense
                }
            }
        }
        if (checkTotal < highestTotal) {
            highestTotal = checkTotal; // Check the last day's total
            maxDay = checkDay;
        }
        highestDailyIndices = filterByDay(maxDay.day, maxDay.month, maxDay.year, indices);
        return highestDailyIndices; //return all indices of transactions from the highest day
    }
    string AppMenu::centerText(const string text, int width) const{
        int padding = (width - text.length()) / 2;
        if (padding > 0) {
            return string(padding, ' ') + text + string(padding + (width - text.length()) % 2, ' ');
        } else {
            return text; // No padding needed if text is longer than width
        }
    }
    void AppMenu::printBarGraph(const int month, const int year) const { //y axis is amount, x axis is catagory
        std::vector<int> indices = filterByMonth(month, year);
        if (indices.empty()) {
            cout << "No transactions found for " << month << "/" << year << "." << endl;
            return;
        }
        char graphSymbol = '%';
        int catagoryWidth = 13; // Width for category labels (use odd number for allignment)
        double bills = abs(parser.findExpenses(filterByCategory(loadFilterNames(1), indices)));
        double food = abs(parser.findExpenses(filterByCategory(loadFilterNames(2), indices)));
        double groceries = abs(parser.findExpenses(filterByCategory(loadFilterNames(3), indices)));
        double shopping = abs(parser.findExpenses(filterByCategory(loadFilterNames(4), indices)));  
        double transport = abs(parser.findExpenses(filterByCategory(loadFilterNames(5), indices))); //use abs to simplify graph logic
        double other = abs(parser.findExpenses(findMiscIndices(indices)));
        double total = bills + food + groceries + shopping + transport + other;
        int graphTop;
        if (((static_cast<int>(total)) % 100) > 50)
            graphTop = (static_cast<int>(total) / 100 + 1) * 100; // Round up to the next hundred if over 50
        else
            graphTop = (static_cast<int>(total) / 100) * 100; // Round down to the nearest hundred if 50 or below
        int graphStep = graphTop / 20; // 20 steps on the y-axis
       // cout << graphTop << " " << graphStep << " " << total << endl;
        cout << "*****Spending Graph for the month of " << month << "/" << year << "*****" << endl;
        for (int i = graphTop; i >= 0; i -= graphStep) {
            cout << "$" << setw(6) << i << " |"; // Y-axis label
            if (bills >= i) cout << setw(catagoryWidth/2) << " " << graphSymbol << setw(catagoryWidth/2) << " "; else cout << setw(catagoryWidth) << " ";
            if (food >= i) cout << setw(catagoryWidth/2) << " " << graphSymbol << setw(catagoryWidth/2) << " "; else cout << setw(catagoryWidth) << " ";
            if (groceries >= i) cout << setw(catagoryWidth/2) << " " << graphSymbol << setw(catagoryWidth/2) << " "; else cout << setw(catagoryWidth) << " ";
            if (shopping >= i) cout << setw(catagoryWidth/2) << " " << graphSymbol << setw(catagoryWidth/2) << " "; else cout << setw(catagoryWidth) << " ";
            if (transport >= i) cout << setw(catagoryWidth/2) << " " << graphSymbol << setw(catagoryWidth/2) << " "; else cout << setw(catagoryWidth) << " ";
            if (other >= i) cout << setw(catagoryWidth/2) << " " << graphSymbol << setw(catagoryWidth/2) << " "; else cout << setw(catagoryWidth) << " ";
            cout << endl;
        }
        cout << setw(catagoryWidth * 6 + 9) << setfill('-') << "-" << setfill(' ') << endl; // X-axis
        cout <<setw(8) << " " 
        << centerText("BILLS", catagoryWidth) 
        << centerText("FOOD", catagoryWidth) 
        << centerText("GROCERIES", catagoryWidth) 
        << centerText("SHOPPING", catagoryWidth) 
        << centerText("TRANSPORT", catagoryWidth) 
        << centerText("OTHER", catagoryWidth) << endl;
    }
    void AppMenu::printDoubleBarGraph(const int year) const {
        std::vector<int> indices = filterByYear(year);
        if (indices.empty()) {
            cout << "No transactions found for " << year << "." << endl;
            return;
        }
        std::vector<int> january = filterByMonth(1, year, indices);
        std::vector<int> february = filterByMonth(2, year, indices);
        std::vector<int> march = filterByMonth(3, year, indices);
        std::vector<int> april = filterByMonth(4, year, indices);
        std::vector<int> may = filterByMonth(5, year, indices);
        std::vector<int> june = filterByMonth(6, year, indices);
        std::vector<int> july = filterByMonth(7, year, indices);
        std::vector<int> august = filterByMonth(8, year, indices);
        std::vector<int> september = filterByMonth(9, year, indices);
        std::vector<int> october = filterByMonth(10, year, indices);
        std::vector<int> november = filterByMonth(11, year, indices);
        std::vector<int> december = filterByMonth(12, year, indices);
        std::vector<std::vector<int>> monthlyIndices = {january, february, march, april, may, june, july, august, september, october, november, december};
        std::vector<double> monthlyExpenses(12, 0.0);
        std::vector<double> monthlyIncome(12, 0.0);
        for (int i = 0; i < 12; i++) {
            monthlyExpenses[i] = abs(parser.findExpenses(monthlyIndices[i]));
            monthlyIncome[i] = abs(parser.findIncome(monthlyIndices[i]));
        }
        char graphSymbol = '%';
        int catagoryWidth = 7; // Width for category labels (use odd number for allignment)
        int graphTop = 0;
        for (int i = 0; i < 12; i++) {
            if (monthlyExpenses[i] > graphTop) {
                graphTop = static_cast<int>(monthlyExpenses[i]); // Find the maximum value for the graph top
            }
        }
        for (int i = 0; i < 12; i++) {
            if (monthlyIncome[i] > graphTop) {
                graphTop = static_cast<int>(monthlyIncome[i]); // Find the maximum value for the graph top
            }
        }
        if (((static_cast<int>(graphTop)) % 100) > 50)
            graphTop = (static_cast<int>(graphTop) / 100 + 1) * 100; // Round up to the next hundred if over 50
        else
            graphTop = (static_cast<int>(graphTop) / 100) * 100; // Round down to the nearest hundred if 50 or below
        int graphStep = graphTop / 50;// 50 steps on the y-axis
       // cout << graphTop << " " << graphStep << " " << total << endl;
        cout << "*****Spending vs Expenses for the year of " << year << "*****" << endl;
        for (int i = graphTop; i >= 0; i -= graphStep) {
            cout << "$" << setw(6) << static_cast<int>(i) << " |"; // Y-axis label
            for (int j = 0; j < 12; j++) {
                if (monthlyExpenses[j] >= i) cout << setw((catagoryWidth-3)/2) << " " << "\033[31m" << graphSymbol << "\033[0m";
                else cout << setw((catagoryWidth)/2) << " ";
                cout << " ";
                if (monthlyIncome[j] >= i) cout << "\033[32m" << graphSymbol << "\033[0m" << setw((catagoryWidth-3)/2) << " ";
                else cout << setw((catagoryWidth)/2) << " ";
        } cout << endl;
        }
        cout << setw(catagoryWidth * 12 + 9) << setfill('-') << "-" << setfill(' ') << endl; // X-axis
        cout <<setw(9) << " " 
        << centerText("Jan", catagoryWidth) 
        << centerText("Feb", catagoryWidth) 
        << centerText("Mar", catagoryWidth) 
        << centerText("Apr", catagoryWidth) 
        << centerText("May", catagoryWidth) 
        << centerText("Jun", catagoryWidth) 
        << centerText("Jul", catagoryWidth) 
        << centerText("Aug", catagoryWidth) 
        << centerText("Sep", catagoryWidth) 
        << centerText("Oct", catagoryWidth) 
        << centerText("Nov", catagoryWidth) 
        << centerText("Dec", catagoryWidth) << endl;
    }