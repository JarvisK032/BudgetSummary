#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

struct Transaction {
    string date;
    double amount;
    bool isExpense;
    string description;
};

void getTransactions(vector<Transaction>& transactions) {
    string date;
    double amount;
    char type;
    string description;
    bool isExpense;
    
    while (true) {
        cout << "Enter the transaction date (MM/DD/YYYY) or q when finish: ";
        cin >> date;
        
        if (date == "q") {
            break;
        }
        
        cout << "Enter the transaction amount: $";
        cin >> amount;
        
        cout << "Enter the transaction type (E for expense, I for income): ";
        cin >> type;
        isExpense = (type == 'E' || type == 'e');
        
        cout << "Enter the transaction description: ";
        cin >> description;
        
        transactions.push_back({date, amount, isExpense, description});
    }
}

void calculateSummary(const vector<Transaction>& transactions, double& totalExpense, double& totalIncome, double& netIncome, Transaction& mostExpensiveExpense) {
    totalExpense = 0.0;
    totalIncome = 0.0;
    netIncome = 0.0;
    mostExpensiveExpense = {"", 0.0, true, ""};
    
    for (const auto& transaction : transactions) {
        if (transaction.isExpense) {
            totalExpense += transaction.amount;
            if (mostExpensiveExpense.amount < transaction.amount) {
                mostExpensiveExpense = transaction;
            }
        } else {
            totalIncome += transaction.amount;
        }
    }
    
    netIncome = totalIncome - totalExpense;
}

void saveSummary(const vector<Transaction>& transactions, const double totalExpense, const double totalIncome, const double netIncome, const Transaction mostExpensiveExpense, const string& fileName) {
    ofstream file(fileName);
    
    if (file.is_open()) {
        // Write summary information to file
        file << "Summary Information:\n";
        file << "Total Income:\t" << totalIncome << endl;
        file << "Total Expense:\t" << totalExpense << endl;
        file << "Net Income:\t" << netIncome << endl;
        file << "Most Expensive Expense:\t" << mostExpensiveExpense.amount << "\t" << mostExpensiveExpense.description << endl << endl;
        
        // Write transactions to file
        file << "Transaction Date\tAmount\tType\tDescription\n";
        for (const auto& transaction : transactions) {
            file << transaction.date << "\t" << transaction.amount << "\t" << (transaction.isExpense ? "Expense" : "Income") << "\t" << transaction.description << endl;
        }
    } else {
        cerr << "Error opening file " << fileName << " for writing." << endl;
    }
    
    file.close();
}

bool compareTransactions(const Transaction& t1, const Transaction& t2) {
    return t1.date < t2.date;
}

void sortTransactions(vector<Transaction>& transactions) {
    sort(transactions.begin(), transactions.end(), compareTransactions);
}

int main() {
    vector<Transaction> transactions;
    double totalExpense, totalIncome, netIncome;
    Transaction mostExpensiveExpense;
    
    getTransactions(transactions);
    calculateSummary(transactions, totalExpense, totalIncome, netIncome, mostExpensiveExpense);
    sortTransactions(transactions);

    string fileName = "BudgetSummary_" + transactions[0].date.substr(6) + "_" + transactions[0].date.substr(0,2) + ".txt";
    
    saveSummary(transactions, totalExpense, totalIncome, netIncome, mostExpensiveExpense, fileName);
    
    cout << "Summary information saved to " << fileName << endl;
    
    return 0;
}




