#include<iostream>
#include<vector>
#include<unordered_set>
#include<utility>
#include<map>
#include<math.h>
using namespace std;

// Functions to create random customer ids and accountNumber
const int ID_LENGTH = 6;                        // the length of the customer ID
unordered_set<string> used_ids;                 // to keep track of used IDs

// generates a random alphanumeric character
char generate_random_char() {
    const string CHARACTERS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int random_index = rand() % CHARACTERS.length();
    return CHARACTERS[random_index];
}
string generate_customer_id() {
    string customer_id(ID_LENGTH, ' '); // initialize the customer ID with spaces
    srand(time(nullptr)); // seed the random number generator with the current time
    
    // loop until a unique ID is generated
    while (used_ids.count(customer_id) > 0 || customer_id[0] == ' ') {
        for (int i = 0; i < ID_LENGTH; i++) {
            customer_id[i] = generate_random_char();
        }
    }
    
    used_ids.insert(customer_id); // add the generated ID to the set of used IDs
    return customer_id;
}



// --------------------------------------Classes----------------------------------------//


class Transaction{
public:
    enum Method {Direct, ATM};
private:
    Method method;
    string date;         
    double amount;
    double transaction_fee;

public:
    Transaction(double amount, string date, Method method)
    : amount(amount), date(date), method(method) {}
    double getAmount(){
        return amount;
    }
    Method getMethod(){
        return method;
    }
    void getDetail(){

    }

};

class LoanAccount{
public:
    enum Type {HOME, CAR, PERSONAL, BUNSINESS};

private:
    // varibles
    int account_number;
    Type type;
    string opening_date;
    double loan_amount;
    double balance;
    int customer_id;
    int interest_percentage;
    int duration_in_years;

public:
    LoanAccount(int customer_id, int account_number, Type type, double loan_amount, double balance, int interest_percentage, int duration_in_years)
        : customer_id(customer_id), account_number(account_number), type(type), loan_amount(loan_amount), balance(balance), interest_percentage(interest_percentage), duration_in_years(duration_in_years) {};

    void payInstallment(int amount){
        if(amount > loan_amount/10){
            cout << "---------------------------- Installedment in loan account failed! -----------------------------\n";
            cout << "Lum sum amount should be less than 10\% of the total loan amoount\n";
        }
        else if(amount > balance){
            cout << "---------------------------- Installedment in loan account failed! -----------------------------\n";
            cout << " Amount of lum sum must be less than the current balance in the loan account\n";
        }
        else{
            this->balance -= amount;
            cout << "---------------------------- Installedment to loan account succed -----------------------------\n";
            cout << "Remaining loan amount in the loan account is : " << balance << "\n";
        }
    }
    // Interest calculation based on interest rate

};

class Account{
public:
    enum Type { Saving, Current, Loan };

private:
    int account_number;
    Type type;
    string opening_date;
    double balance;
    int customer_id;
    map<string, vector<Transaction*>> transactionsbydate;
    map<string, vector<Transaction*>> transactionsbymonth;

public:
    Account(int customer_id, int account_number, Type type, double balance, string opening_date)
        : customer_id(customer_id), account_number(account_number), type(type), balance(balance), opening_date(opening_date) {}

    int getAccountNumber(){
        return account_number;
    }
    Type getType(){
        return type;
    }
    double getBalance(){
        return balance;
    }
    void setBalance(double amount){
        this->balance = amount;
    }
    double totalAmountOfDay(string date){
        double amount = 0;
        for(auto transaction : transactionsbydate[date]){
            amount += transaction->getAmount();
        }
        return amount;
    }
    int numberOfTransMonth(string monthYear){
        int cnt = 0;
        for(auto transaction : transactionsbymonth[monthYear]){
            if(transaction->getMethod() == 1) cnt++;
        }
        return cnt;
    }
    void* withdraw(double amount, string date, Transaction::Method method, Account::Type type){
        // Saving Account
        double totalWithdrawedInDay = totalAmountOfDay(date);
        int numberoftransactions = numberOfTransMonth(date.substr(3));
        if( type == 0 ){
            if(amount > 20000){
                cout << "------------------- Transaction Failed! --------------------\n";
                cout << "Single transaction can't be of more than 20,000\n";
            }
            else if(totalWithdrawedInDay + amount > 50000){
                cout << "------------------- Transaction Failed! --------------------\n";
                cout << "Maximum of 50000 can be withdrawed in a single day\n";
            }
            else if(amount > getBalance()){
                cout << "------------------- Transaction Failed! --------------------\n";
                cout << "Account have insufficient Balance\n";
            }
            else{
                // Direct from bank
                if(method == 0){
                    this->balance = this->balance - amount;

                    // Add transaction
                    Transaction* transaction = new Transaction(amount, date, method);
                    transactionsbydate[date].push_back(transaction);
                    transactionsbymonth[date.substr(3)].push_back(transaction);
                    cout << "------------------------ Transaction Successful ----------------------\n";
                    cout << amount << " Amount withdrawn, Current Balance : " << this->balance << "\n";
                }
                // With Draw from ATM
                else{
                    if(numberoftransactions >= 5){
                        cout << "-------------- ATM transaction of a month Exceeded -------------------\n";
                        cout << "Penalty of 500 will be imposed\n";
                        this->balance = this->balance - amount - 500;
                        
                        // Add transaction
                        Transaction* transaction = new Transaction(amount, date, method);
                        transactionsbydate[date].push_back(transaction);
                        transactionsbymonth[date.substr(3)].push_back(transaction);
                        cout << amount << " Amount withdrawn, Current Balance : " << this->balance << "\n";

                    }
                    else{
                        this->balance = this->balance - amount;
                        
                        // Add transaction
                        Transaction* transaction = new Transaction(amount, date, method);
                        transactionsbydate[date].push_back(transaction);
                        transactionsbymonth[date.substr(3)].push_back(transaction);
                        cout << "------------------------ Transaction Successful ----------------------\n";
                        cout << amount << " Amount withdrawn, Current Balance : " << this->balance << "\n";
                    }

                }
            }
        }
        
        // Current Account
        else if(type == 1){
            // Implement logic for penalty if minimum transaction not completed in month
            double maxpenalty = 500.0;
            double transactionfee = max(amount/200, maxpenalty);
            this->balance = this->balance - transactionfee - amount;
            Transaction* transaction = new Transaction(amount, date, method);
            transactionsbydate[date].push_back(transaction);
            transactionsbymonth[date.substr(3)].push_back(transaction);
            cout << "------------------------ Transaction Successful ----------------------\n";
            cout << amount + transactionfee << " Amount withdrawn, Current Balance : " << this->balance << "\n";

        }
    }
    void deposit(double amount){
        this->balance = this->balance + amount;

        // Add transactions into the vector

        cout << "---------------------- Deposit successful----------------------\n";
        cout << "Amount " << amount << " added in your bank account. Current balance is " << balance << "\n";
    }

};

class User{
private:
    string firstName, lastName, email, address, phone;
    int customer_id, age, account_number; 
    vector<Account*> accounts;
    vector<LoanAccount*> laccounts;

public:
    User(int customer_id, string firstName, string lastName, string email, string address, string phone, int age)
        : customer_id(customer_id), firstName(firstName), lastName(lastName), email(email), address(address), phone(phone), age(age), account_number(1) {}

    int getCustomerId(){
        return customer_id;
    }
    int getAge(){
        return age;
    }
    bool haveSavingorCurrent(){
        if(accounts.size() > 0) return true;
        return false;
    }
    void getDetails(){
        cout << "------------------ User Details --------------------\n";
        cout << "Customer Id : " << customer_id << "\n";
        cout << "Full Name : " <<firstName << " "<< lastName << "\n";
        cout << "Email : " << email << "\n";
        cout << "Address : " << address << "\n";
        cout << "Phone Number : " << phone << "\n";
    }
    double totalDeposit(){
        // Run loop over accounts and calculate total amount
        double totalBalance = 0;
        for(auto p : accounts){
            totalBalance += p->getBalance();
        }
        return totalBalance;
    }
    void getTotalAmount(){
        // Run loop over accounts and calculate total amount
        double totalBalance = 0;
        for(auto p : accounts){
            totalBalance += p->getBalance();
        }
        cout << "------------------ Total Balance in all accounts of a User ---------------------------\n";
        cout << "Total Balance is : " << totalBalance << "\n";
    }
    void getAccount(){
        cout << "--------------------- User Accounts -----------------------\n";
        for(auto account : accounts){
            cout << "Account Number : " << account->getAccountNumber() << ", Balance : " << account->getBalance() << "\n";
        }
        return;
    }
    vector<Account*> getuserAccounts(){
        return accounts;
    }
    // create account
    Account* create_account(int customer_id, Account::Type type, double balance, string opening_date, int age) {

        // Saving account
        Account* temp;
        if(type == 0){
            if(balance < 10000){
                cout << "---------------- Account creation failed! ----------------\n";
                cout << "Minimum amount require for saving account is Rs 10000\n";
                cout << "----------------------------------------------------------\n";
                
            }
            else{
                Account* account = new Account(customer_id, account_number++, type, balance, opening_date);
                cout << "---------------- New Saving account added! ----------------\n";
                cout << "Customer id : " << customer_id << "\n";
                cout << "Account Number : " << account_number << "\n";
                cout << "Balance : " << balance << "\n";
                cout << "-----------------------------------------------------------\n";
                accounts.push_back(account);
                return account;
            }
            return temp;
        }

        // Current account
        else if(type == 1){
            if(balance < 100000){
                cout << "---------------- Account creation failed! ----------------\n";
                cout << "Minimum amount require for current account is Rs 100000\n";
                cout << "----------------------------------------------------------\n";
                
            }
            else if(age < 18){
                cout << "---------------- Account creation failed! ----------------\n";
                cout << "Minimum Age require for current account opening is 18";
                cout << "----------------------------------------------------------\n";
                
            }
            else{
                Account* account = new Account(customer_id, account_number++, type, balance, opening_date);
                cout << "---------------- New Current account added! ----------------\n";
                cout << "Customer id : " << customer_id << "\n";
                cout << "Account Number : " << account_number << "\n";
                cout << "Balance : " << balance << "\n";
                cout << "------------------------------------------------------------\n";
                accounts.push_back(account);
                return account;
            }
            return temp;
        }
        return temp;
    }
    LoanAccount* create_loan_account(int customer_id, LoanAccount::Type type, int duration_in_years, double balance, string opening_date){
        // Does user saving account/current account or not
        if(!haveSavingorCurrent()){
            cout << "--------------------- Account creation failed! -------------------------\n";
            cout << "User dont have saving or current account to open a loan account\n";
        }
        else if(getAge() < 25){
            cout << "--------------------- Account creation failed! -------------------------\n";
            cout << "User dont have meet age limit criteria\n";
        }
        else if(duration_in_years < 2){
            cout << "--------------------- Account creation failed! -------------------------\n";
            cout << "Minimum loan account duration is of 2 years\n";
        }
        else if(balance < 500000){
            cout << "--------------------- Account creation failed! -------------------------\n";
            cout << "Minimum loan amount will be issued Rs 500000\n";
        }
        else if(balance > 0.4 * totalDeposit()){
            cout << "--------------------- Account creation failed! -------------------------\n";
            cout << "Maximum Loan amount can be 40\% of the total deposits\n";
        }
        else{
            if(type == 0){
                LoanAccount* loan_account = new LoanAccount(customer_id, account_number, LoanAccount::HOME, balance, balance, 7, duration_in_years);
                cout << "---------------- New Loan account added! ----------------\n";
                cout << "Customer id : " << customer_id << "\n";
                cout << "Account Number : " << account_number << "\n";
                cout << "Balance : " << balance << "\n";
                cout << "-----------------------------------------------------------\n";
                laccounts.push_back(loan_account);
                return loan_account;

            }
            else if(type == 1){
                LoanAccount* loan_account = new LoanAccount(customer_id, account_number, LoanAccount::CAR, balance, balance, 8, duration_in_years);
                cout << "---------------- New Loan account added! ----------------\n";
                cout << "Customer id : " << customer_id << "\n";
                cout << "Account Number : " << account_number << "\n";
                cout << "Balance : " << balance << "\n";
                cout << "-----------------------------------------------------------\n";
                laccounts.push_back(loan_account);
                return loan_account;
            }
            else if(type == 2){
                LoanAccount* loan_account = new LoanAccount(customer_id, account_number, LoanAccount::PERSONAL, balance, balance, 12, duration_in_years);
                cout << "---------------- New Loan account added! ----------------\n";
                cout << "Customer id : " << customer_id << "\n";
                cout << "Account Number : " << account_number << "\n";
                cout << "Balance : " << balance << "\n";
                cout << "-----------------------------------------------------------\n";
                laccounts.push_back(loan_account);
                return loan_account;
            }
            else{
                LoanAccount* loan_account = new LoanAccount(customer_id, account_number, LoanAccount::BUNSINESS, balance, balance, 15, duration_in_years);
                cout << "---------------- New Loan account added! ----------------\n";
                cout << "Customer id : " << customer_id << "\n";
                cout << "Account Number : " << account_number << "\n";
                cout << "Balance : " << balance << "\n";
                cout << "-----------------------------------------------------------\n";
                laccounts.push_back(loan_account);
                return loan_account;
            }
            

        }
    }

    
};

class Bank{
private:
    vector<User*> users;
    int customer_id;

public:
    // As soon as the bank created it will initialise customerID
    Bank(): customer_id(1) {}
    
    // create user
    User* create_user(string firstName, string lastName, string email, string address, string phone, int age){
        User* user = new User(customer_id++, firstName, lastName, email, address, phone, age);
        cout << "------------------------- User Created -----------------------\n";
        cout << firstName << " is created..."<< "\n";
        users.push_back(user);
        return user;
    }
    void monthEnd(){
        // 6% in all saving account
        for(auto user : users){
            for(auto account : user->getuserAccounts()){
                // for each saving account add 6% interest
                
                if(account->getType() == 0){
                    double balance = account->getBalance();
                    double interest = (balance)/200;
                    account->setBalance(balance + interest);

                    // NRV penalty
                    double NRV_rate = 0.01;
                    double NRV_penalty = 100000 > balance ? (100000 - balance)*NRV_rate : 0;
                    account->setBalance(balance - NRV_penalty);
                }
                // current account
                else if(account->getType() == 1){
                    double balance = account->getBalance();
                    
                    // NRV penalty
                    double NRV_rate = 0.01;
                    double NRV_penalty = 5000000 > balance ? (5000000 - balance)*NRV_rate : 0;
                    account->setBalance(balance - NRV_penalty);
                
                }
            }
        }
    }
};

int main(){
    // Create Bank
    Bank bank;

    // Create Users
    User* alice = bank.create_user("Alice", "Babu", "alice@google.com", "alice home address", "9999999999", 25);
    User* bob = bank.create_user("Bob", "builder", "bob@builder.com", "Bob home address", "1234567890", 34);
    User* chaman = bank.create_user("chaman", "kumar", "chaman@kumar", "chaman main road", "9472398472", 25);
    
    LoanAccount* chaman_laon = chaman->create_loan_account(chaman->getCustomerId(), LoanAccount::HOME, 2, 500000, "20/07/2023");

    // Create accounts
    Account* alice_saving = alice->create_account(alice->getCustomerId(), Account::Saving, 10000, "19/07/2023", alice->getAge());
    Account* alice_current = alice->create_account(alice->getCustomerId(), Account::Current, 100001.0, "20/07/2023", alice->getAge());
    Account* bob_current = bob->create_account(bob->getCustomerId(), Account::Current, 4000.0, "19/07/2023", bob->getAge());
    LoanAccount* alice_loan = alice->create_loan_account(alice->getCustomerId(), LoanAccount::CAR, 2, 500000, "20/07/2023");

    bank.monthEnd();

    alice->getDetails();
    alice->getTotalAmount();
    alice->getAccount();


    alice_loan->payInstallment(100000);
    alice_loan->payInstallment(40000);
    alice_loan->payInstallment(10000);

    


    //Transactions
    alice_saving->deposit(500);
    alice_saving->deposit(300);

    alice_saving->withdraw(500, "20/07/2023", Transaction::ATM, alice_saving->Account::getType());
    alice_saving->withdraw(500, "20/07/2023", Transaction::ATM, alice_saving->Account::getType());
    alice_saving->withdraw(500, "20/07/2023", Transaction::ATM, alice_saving->Account::getType());
    alice_saving->withdraw(500, "20/07/2023", Transaction::ATM, alice_saving->Account::getType());
    alice_saving->withdraw(500, "20/07/2023", Transaction::ATM, alice_saving->Account::getType());
    alice_saving->withdraw(500, "20/07/2023", Transaction::ATM, alice_saving->Account::getType());

    // deposit into loan account


    return 0;

}