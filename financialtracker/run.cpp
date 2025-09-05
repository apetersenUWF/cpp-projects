#include "reports.hpp"
#include <iostream>
using namespace std;
int main() {
    AppMenu menu;
    menu.run();
    return EXIT_SUCCESS;
}//g++ transaction.cpp parser.cpp reports.cpp run.cpp -o bank_app
//add subscription finder, add fraud finder, add bar graph to reports, more robust sorting, real data