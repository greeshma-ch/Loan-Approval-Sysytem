#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <limits>
#include <sstream>
#include <queue>

using namespace std;

struct Customer {
    string name;
    int age;
    float annual_income;
    float loan_amount;
    int credit_history_years;
    bool has_existing_loans;
    float monthly_expenses;
    float assets_value;
    float score;

    bool operator<(const Customer& other) const {
        return score < other.score; // For max-heap
    }
};

// Function to calculate credit score
float calculateScore(Customer& c) {
    float score = 0;

    // Income-based score
    if (c.annual_income >= 50000) score += 25;
    else if (c.annual_income >= 30000) score += 15;
    else score += 5;

    // Credit history score
    if (c.credit_history_years >= 5) score += 20;
    else if (c.credit_history_years >= 2) score += 10;

    // Loan to income ratio
    float ratio = c.loan_amount / c.annual_income;
    if (ratio < 0.2) score += 20;
    else if (ratio < 0.5) score += 10;

    // Existing loan status
    score += c.has_existing_loans ? 0 : 15;

    // Asset score
    if (c.assets_value >= 50000) score += 10;

    // Monthly expense score
    if (c.monthly_expenses < 2000) score += 10;
    else if (c.monthly_expenses < 4000) score += 5;

    c.score = score;
    return score;
}

// Function to save data to CSV
void saveToCSV(const Customer& c, bool writeHeader = false) {
    ofstream file("loan_applications.csv", ios::app);
    if (writeHeader) {
        file << "Name,Age,Annual Income,Loan Amount,Credit History,Existing Loans,Monthly Expenses,Assets Value,Credit Score,Status\n";
    }
    file << fixed << setprecision(2);
    file << c.name << ","
         << c.age << ","
         << c.annual_income << ","
         << c.loan_amount << ","
         << c.credit_history_years << ","
         << (c.has_existing_loans ? "Yes" : "No") << ","
         << c.monthly_expenses << ","
         << c.assets_value << ","
         << c.score << ","
         << (c.score >= 70 ? "Approved" : "Rejected") << "\n";
    file.close();
}

// Function to evaluate loan approval
void evaluateCustomer(const Customer& c) {
    cout << "\n--- Evaluation Result ---\n";
    cout << "Customer: " << c.name << endl;
    cout << "Credit Score: " << c.score << endl;
    cout << "Loan Status: " << (c.score >= 70 ? "Approved" : "Rejected") << "\n";
}

// Input customer data
Customer getCustomerData() {
    Customer c;
    cout << "\n--- Enter Customer Data ---\n";
    cout << "Name: ";
    cin.ignore();
    getline(cin, c.name);
    
    cout << "Age: ";
    while (!(cin >> c.age) || c.age <= 0) {
        cin.clear(); cin.ignore(1000, '\n');
        cout << "Enter a valid age: ";
    }

    cout << "Annual Income: ";
    while (!(cin >> c.annual_income) || c.annual_income <= 0) {
        cin.clear(); cin.ignore(1000, '\n');
        cout << "Enter a valid income: ";
    }

    cout << "Loan Amount: ";
    while (!(cin >> c.loan_amount) || c.loan_amount <= 0) {
        cin.clear(); cin.ignore(1000, '\n');
        cout << "Enter a valid loan amount: ";
    }

    cout << "Credit History (years): ";
    while (!(cin >> c.credit_history_years) || c.credit_history_years < 0) {
        cin.clear(); cin.ignore(1000, '\n');
        cout << "Enter a valid credit history: ";
    }

    int hasLoans;
    cout << "Has Existing Loans (0 = No, 1 = Yes): ";
    while (!(cin >> hasLoans) || (hasLoans != 0 && hasLoans != 1)) {
        cin.clear(); cin.ignore(1000, '\n');
        cout << "Enter 0 or 1: ";
    }
    c.has_existing_loans = hasLoans;

    cout << "Monthly Expenses: ";
    while (!(cin >> c.monthly_expenses) || c.monthly_expenses < 0) {
        cin.clear(); cin.ignore(1000, '\n');
        cout << "Enter valid monthly expenses: ";
    }

    cout << "Assets Value: ";
    while (!(cin >> c.assets_value) || c.assets_value < 0) {
        cin.clear(); cin.ignore(1000, '\n');
        cout << "Enter a valid asset value: ";
    }

    calculateScore(c);
    return c;
}

// View application records
void viewApplications() {
    ifstream file("loan_applications.csv");
    if (!file.is_open()) {
        cout << "No application records found.\n";
        return;
    }

    string line;
    cout << "\n--- All Loan Applications ---\n";
    while (getline(file, line)) {
        cout << line << endl;
    }

    file.close();
}

// Show top N applicants
void showTopApplicants(int n) {
    ifstream file("loan_applications.csv");
    if (!file.is_open()) {
        cout << "No application records found.\n";
        return;
    }

    string line;
    getline(file, line); // skip header

    priority_queue<Customer> pq;
    while (getline(file, line)) {
        stringstream ss(line);
        Customer c;
        string temp;

        getline(ss, c.name, ',');
        getline(ss, temp, ','); c.age = stoi(temp);
        getline(ss, temp, ','); c.annual_income = stof(temp);
        getline(ss, temp, ','); c.loan_amount = stof(temp);
        getline(ss, temp, ','); c.credit_history_years = stoi(temp);
        getline(ss, temp, ','); c.has_existing_loans = (temp == "Yes");
        getline(ss, temp, ','); c.monthly_expenses = stof(temp);
        getline(ss, temp, ','); c.assets_value = stof(temp);
        getline(ss, temp, ','); c.score = stof(temp);

        pq.push(c);
    }

    cout << "\n--- Top Applicants ---\n";
    for (int i = 0; i < n && !pq.empty(); i++) {
        Customer top = pq.top();
        pq.pop();
        cout << i + 1 << ". " << top.name << " (Score: " << top.score << ")\n";
    }

    file.close();
}

// Main menu
int main() {
    int choice;
    bool headerWritten = false;

    // Write header only once if file is empty
    ifstream checkFile("loan_applications.csv");
    if (checkFile.peek() == ifstream::traits_type::eof()) {
        headerWritten = true;
    }
    checkFile.close();

    do {
        cout << "\n==== Loan Approval System ====\n";
        cout << "1. New Loan Application\n";
        cout << "2. View All Applications\n";
        cout << "3. Show Top Applicants\n";
        cout << "4. Exit\n";
        cout << "Choose an option: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear(); cin.ignore(1000, '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        switch (choice) {
            case 1: {
                Customer c = getCustomerData();
                evaluateCustomer(c);
                saveToCSV(c, headerWritten);
                headerWritten = false; // Header only once
                break;
            }
            case 2:
                viewApplications();
                break;
            case 3: {
                int n;
                cout << "Enter number of top applicants to display: ";
                cin >> n;
                showTopApplicants(n);
                break;
            }
            case 4:
                cout << "Exiting the system. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Please select from the menu.\n";
        }
    } while (choice != 4);

    return 0;
}
