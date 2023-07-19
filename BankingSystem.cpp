#include<iostream>
#include<vector>
#include<unordered_set>
#include<utility>
#include<map>
using namespace std;

// Data Strucure to store customer details in map
map<string, pair<vector<string>, vector<string> > > personalDetails;
map<string, vector<string> > accountDetails;

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

};

class Account{
public:
    enum Type { Saving, Current, Loan };
    enum Method {ATM, Direct};

private:
    int account_number;
    Type type;
    Method method;
    string opening_date;
    double balance;
    int customer_id;
    vector<Transaction*> transactions;

public:
    Account(int customer_id, int account_number, Type type, double balance, string opening_date)
        : customer_id(customer_id), account_number(account_number), type(type), balance(balance), opening_date(opening_date) {}

    // deposit
    int getAccountNumber(){
        return account_number;
    }
    Type getType(){
        return type;
    }
    double getBalance(){
        return balance;
    }

    void deposit(double amount){
        this->balance = this->balance + amount;

        // Add transactions into the vector

        cout << "---------------------- Deposit successful----------------------\n";
        cout << "Amount " << amount << " added in your bank account. Current balance is " << balance << "\n";
    }

    void withdraw(Type type, double amount, Method method){
        // Withdraw from Saving account
        if(type == 0){
            if(amount > 20000){
                cout << "------------------------ Transaction error! ----------------------\n";
                cout << "Amount greater than Rs 20000 cant be withdrawn in one transaction!\n";
                return;
            }
            else{
                if(method == 0){
                    
                }
                else{

                }
            }
        }
        else if(type == 1){

        }
        else{

        }


    }



};

class User{
private:
    string firstName, lastName, email, address, phone;
    int customer_id, age; 
    vector<Account*> accounts;

public:
    User(int customer_id, string firstName, string lastName, string email, string address, string phone, int age)
        : customer_id(customer_id), firstName(firstName), lastName(lastName), email(email), address(address), phone(phone), age(age) {}

    int getCustomerId(){
        return customer_id;
    }
    int getAge(){
        return age;
    }

    void getDetails(){
        cout << "Customer Id : " << customer_id << "\n";
        cout << "Full Name : " <<firstName << " "<< lastName << "\n";
        cout << "Email : " << email << "\n";
        cout << "Address : " << address << "\n";
        cout << "Phone Number : " << phone << "\n";
    }

    double getTotalAmount(){
        // Run loop over accounts and calculate total amount
        double totalBalance = 0;
        for(auto p : accounts){
            totalBalance += p->getBalance();
        }
        return totalBalance;
    }

    void getAccount(){
        cout << "--------------------- User Accounts -----------------------\n";
        for(auto account : accounts){
            cout << "Account Number : " << account->getAccountNumber() << ", Balance : " << account->getBalance() << "\n";
        }
        return;
    }
};

class Bank{
private:
    vector<User*> users;
    vector<Account*> accounts;
    int customer_id;
    int account_number;

public:
    // As soon as the bank created it will initialise customerID and accountNumber
    Bank(): customer_id(1), account_number(1) {}

    // create user
    User* create_user(string firstName, string lastName, string email, string address, string phone, int age){
        User* user = new User(customer_id++, firstName, lastName, email, address, phone, age);
        users.push_back(user);
        return user;
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
        // Loan account
        else{
            // create loan account

        }
        return temp;
        
    }

};


void signUp(){
    // customerId = generate_customer_id();
    // // Create User
    // personalDetails.emplace(customerId, make_pair(vector<string>{firstName, lastName, email, address, phone, age}, vector<string>{accountNumber}));
    // // Account details
    // accountDetails[accountNumber] = {customerId,firstName, lastName, email, address, phone, age, accountType, balance, openingDate};

    // cout << "Account Created succesfully!\nYour Customer Id = " << customerId << "\nYour Account Number = " << accountNumber << "\n";

}

int main(){
    // Create Bank
    Bank bank;

    // Create Users
    User* alice = bank.create_user("Alice", "Babu", "alice@google.com", "alice home address", "9999999999", 23);
    User* bob = bank.create_user("Bob", "builder", "bob@builder.com", "Bob home address", "1234567890", 34);
    
    // Create accounts
    Account* alice_saving = bank.create_account(alice->getCustomerId(), Account::Saving, 50000.0, "19/07/2023", alice->getAge());
    Account* bob_current = bank.create_account(bob->getCustomerId(), Account::Current, 4000.0, "19/07/2023", bob->getAge());

    // Transactions
    alice_saving->deposit(500);
    alice_saving->deposit(300);
    alice_saving->withdraw(alice_saving->Account::getType(), 20001, Account::ATM);
    alice_saving->withdraw(alice_saving->Account::getType(), 300, Account::Direct);




    //alice->getDetails();

    return 0;

}