# USAA Parser and Financial Tracker
This is a personal C++ program I wrote to take a csv file of USAA transaction history and analyze spending trends
To use:
- Download your csv file from USAA.com
- Copy and paste into provided USAAtransactionreport.csv file
- Build executable using "g++ transaction.cpp parser.cpp reports.cpp run.cpp -o bank_app" in terminal
- Run executable using "./bank_app"
- Use CLI to print, sort, filter, and analyze spending data

# Important Information:
- This program uses pre-determined filter names to sort transactions into catagories (bills, food, groceries, auto, shopping, misc)
- Add keywords to each respective txt file to add that into the catagory search
- Be careful not to add the same keyword twice and dont include it as a substring of another word ex: truck and truckstop, this will cause problems
- Capitalization and punctuation are not included in the search