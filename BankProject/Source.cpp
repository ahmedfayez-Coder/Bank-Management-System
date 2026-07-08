    #include <iostream> 
    #include <string>
    #include <vector>
    #include <fstream>
    #include <iomanip>

    using namespace std;

    class Client {
    private:
        string _accountNumber;
        string _pinCode;
        string _name;
        string _phone;
        double _accountBalance;
        bool _markForDelete = false;

    public:
        Client() {
            _accountNumber = "";
            _pinCode = "";
            _name = "";
            _phone = "";
            _accountBalance = 0.0;
        }

        Client(string accountNumber, string pinCode, string name, string phone, double accountBalance) {
            _accountNumber = accountNumber;
            _pinCode = pinCode;
            _name = name;
            _phone = phone;
            _accountBalance = accountBalance;
        }

        void setAccountNumber(string accountNumber) { _accountNumber = accountNumber; }
        string getAccountNumber() { return _accountNumber; }

        void setPinCode(string pinCode) { _pinCode = pinCode; }
        string getPinCode() { return _pinCode; }

        void setName(string name) { _name = name; }
        string getName() { return _name; }

        void setPhone(string phone) { _phone = phone; }
        string getPhone() { return _phone; }

        void setAccountBalance(double accountBalance) { _accountBalance = accountBalance; }
        double getAccountBalance() { return _accountBalance; }

        void setMarkForDelete(bool mark) { _markForDelete = mark; }
        bool isMarkedForDelete() { return _markForDelete; }

        void readClientInput() {
            cout << "Enter Account Number: ";
            getline(cin >> ws, _accountNumber);
            cout << "Enter PinCode: ";
            getline(cin, _pinCode);
            cout << "Enter Name: ";
            getline(cin, _name);
            cout << "Enter Phone: ";
            getline(cin, _phone);
            cout << "Enter Account Balance: ";
            cin >> _accountBalance;
        }

        void updateClientInput() {
            cout << "Enter PinCode: ";
            getline(cin >> ws, _pinCode);
            cout << "Enter Name: ";
            getline(cin, _name);
            cout << "Enter Phone: ";
            getline(cin, _phone);
            cout << "Enter Account Balance: ";
            cin >> _accountBalance;
        }

        void printClientCard() {
            cout << "\n Your Account Number Is : " << _accountNumber << endl;
            cout << " Your Pin Code Is       : " << _pinCode << endl;
            cout << " Your Name Is           : " << _name << endl;
            cout << " Your Phone Is          : " << _phone << endl;
            cout << " Your Account Balance Is: " << _accountBalance << endl;
        }

        void printClientRecordRow() {
            cout << "| " << setw(15) << left << _accountNumber;
            cout << "| " << setw(10) << left << _pinCode;
            cout << "| " << setw(40) << left << _name;
            cout << "| " << setw(12) << left << _phone;
            cout << "| " << setw(12) << left << _accountBalance;
        }

        void printBalanceRow() {
            cout << "| " << setw(15) << left << _accountNumber;
            cout << "| " << setw(40) << left << _name;
            cout << "| " << setw(12) << left << _accountBalance;
        }

        string convertRecordToLine(string separator = "//#//") {
            return _accountNumber + separator + _pinCode + separator + _name + separator + _phone + separator + to_string(_accountBalance);
        }

        static vector<string> splitString(string s, string delim) {
            vector<string> vString;
            short pos = 0;
            string sWord;
            while ((pos = s.find(delim)) != string::npos) {
                sWord = s.substr(0, pos);
                if (sWord != "") vString.push_back(sWord);
                s.erase(0, pos + delim.length());
            }
            if (s != "") vString.push_back(s);
            return vString;
        }

        static Client convertLineToRecord(string line, string separator = "//#//") {
            vector<string> vData = splitString(line, separator);
            return Client(vData[0], vData[1], vData[2], vData[3], stod(vData[4]));
        }
    };

    class BankManager {
    private:
        string _fileName;

        vector<Client> _loadClientsFromFile() {
            vector<Client> vClients;
            fstream myFile;
            myFile.open(_fileName, ios::in);
            if (myFile.is_open()) {
                string line;
                while (getline(myFile, line)) {
                    vClients.push_back(Client::convertLineToRecord(line));
                }
                myFile.close();
            }
            return vClients;
        }

        void _saveClientsToFile(vector<Client>& vClients) {
            fstream myFile;
            myFile.open(_fileName, ios::out);
            if (myFile.is_open()) {
                for (Client& c : vClients) {
                    if (!c.isMarkedForDelete()) {
                        myFile << c.convertRecordToLine() << endl;
                    }
                }
                myFile.close();
            }
        }

        void _addDataLineToFile(string dataLine) {
            fstream myFile;
            myFile.open(_fileName, ios::out | ios::app);
            if (myFile.is_open()) {
                myFile << dataLine << endl;
                myFile.close();
            }
        }

    public:
        BankManager(string fileName) {
            _fileName = fileName;
        }

        bool checkAccountNum(string accountNum) {
            vector<Client> vClients = _loadClientsFromFile();
            for (Client& c : vClients) {
                if (c.getAccountNumber() == accountNum) return true;
            }
            return false;
        }

        void showAllClientsScreen() {
            system("cls");
            vector<Client> vClients = _loadClientsFromFile();
            cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
            cout << "\n_________________________________________________________________________________\n" << endl;
            cout << "| " << left << setw(15) << "Account Number" << "| " << left << setw(10) << "Pin Code"
                << "| " << left << setw(40) << "Client Name" << "| " << left << setw(12) << "Phone"
                << "| " << left << setw(12) << "Balance";
            cout << "\n_________________________________________________________________________________\n" << endl;

            for (Client& c : vClients) {
                c.printClientRecordRow();
                cout << endl;
            }
            cout << "_________________________________________________________________________________\n" << endl;
            cout << "Press Any Key To back to main menu...";
            system("pause > nul");
        }

        void addNewClientsScreen() {
            char addMore = 'y';
            do {
                system("cls");
                cout << "-----------------------------------\n";
                cout << "\tAdd New Clients Screen\n";
                cout << "-----------------------------------\n";

                Client client;
                client.readClientInput();
                _addDataLineToFile(client.convertRecordToLine());

                cout << "\nClient Added Successfully :) " << endl;
                cout << "Do You Need To Add more Clients? Y/N: ";
                cin >> addMore;
            } while (toupper(addMore) == 'Y');
        }

        void deleteClientScreen() {
            system("cls");
            cout << "-----------------------------------\n";
            cout << "\tDelete Client Screen\n";
            cout << "-----------------------------------\n";

            string accountNum;
            cout << "Please enter Account Number: ";
            cin >> accountNum;

            vector<Client> vClients = _loadClientsFromFile();
            bool found = false;

            for (Client& c : vClients) {
                if (c.getAccountNumber() == accountNum) {
                    c.printClientCard();
                    found = true;

                    char answer;
                    cout << "\nAre you sure you want to delete this client? (y/n): ";
                    cin >> answer;
                    if (toupper(answer) == 'Y') {
                        c.setMarkForDelete(true);
                        _saveClientsToFile(vClients);
                        cout << "\nClient Deleted Successfully.";
                    }
                    else {
                        cout << "\nTransaction Cancelled.";
                    }
                    break;
                }
            }
            if (!found) cout << "\nClient With Account Number " << accountNum << " Not Found!";

            cout << "\n\nPress any key to go back to Main Menu...";
            system("pause > nul");
        }

        void updateClientScreen() {
            system("cls");
            cout << "-----------------------------------\n";
            cout << "\tUpdate Client Info Screen\n";
            cout << "-----------------------------------\n";

            string accountNum;
            cout << "Please enter Account Number: ";
            cin >> accountNum;

            vector<Client> vClients = _loadClientsFromFile();
            bool found = false;

            for (Client& c : vClients) {
                if (c.getAccountNumber() == accountNum) {
                    c.printClientCard();
                    found = true;

                    char answer;
                    cout << "\nAre You Sure To Update This Client? y/n: ";
                    cin >> answer;
                    if (toupper(answer) == 'Y') {
                        c.updateClientInput();
                        _saveClientsToFile(vClients);
                        cout << "\nClient Updated Successfully.";
                    }
                    break;
                }
            }
            if (!found) cout << "\nUser NOT Found";

            cout << "\n\nPress any key to go back to Main Menu...";
            system("pause > nul");
        }

        void findClientScreen() {
            system("cls");
            cout << "-----------------------------------\n";
            cout << "\tFind Client Screen\n";
            cout << "-----------------------------------\n";

            string accountNum;
            cout << "Please enter Account Number: ";
            cin >> accountNum;

            vector<Client> vClients = _loadClientsFromFile();
            bool found = false;

            for (Client& c : vClients) {
                if (c.getAccountNumber() == accountNum) {
                    c.printClientCard();
                    found = true;
                    break;
                }
            }
            if (!found) cout << "\nClient With Account Number " << accountNum << " Not Found";

            cout << "\nPress any key to continue...";
            system("pause > nul");
        }

        void transactionDeposit() {
            string accountNum;
            cout << "Enter Account Number: ";
            cin >> accountNum;

            vector<Client> vClients = _loadClientsFromFile();
            bool found = false;
            for (Client& c : vClients) {
                if (c.getAccountNumber() == accountNum) {
                    c.printClientCard();
                    found = true;
                    double amount;
                    cout << "\nEnter Amount You Need To Deposit: ";
                    cin >> amount;

                    if (amount > 0) {
                        char choice;
                        cout << "Are You Sure To Do This Transaction? Y/N: ";
                        cin >> choice;

                        if (toupper(choice) == 'Y') {
                            c.setAccountBalance(c.getAccountBalance() + amount);
                            _saveClientsToFile(vClients);
                            cout << "Balance Added Successfully\n";
                            cout << "Your Balance Now is: " << c.getAccountBalance() << endl;
                        }
                        else {
                            cout << "Transaction Cancelled.\n";
                        }
                    }
                    else {
                        cout << "Invalid Amount, please enter a positive number.\n";
                    }
                    break;
                }
            }
            if (!found) cout << "Account Not Found.\n";
            cout << "\nPress Any Key to return to Menu...";
            system("pause > nul");
        }

        void transactionWithdraw() {
            string accountNum;
            cout << "Enter Account Number: ";
            cin >> accountNum;

            vector<Client> vClients = _loadClientsFromFile();
            bool found = false;
            for (Client& c : vClients) {
                if (c.getAccountNumber() == accountNum) {
                    c.printClientCard();
                    found = true;
                    double amount;
                    cout << "\nEnter Amount You Need To Withdraw: ";
                    cin >> amount;

                    if (amount > 0) {
                        char choice;
                        cout << "Are You Sure To Do this Transaction? Y/N: ";
                        cin >> choice;

                        if (toupper(choice) == 'Y') {
                            if (c.getAccountBalance() >= amount) {
                                c.setAccountBalance(c.getAccountBalance() - amount);
                                _saveClientsToFile(vClients);
                                cout << "Balance Withdraw Successfully\n";
                                cout << "Your Balance Now is: " << c.getAccountBalance() << endl;
                            }
                            else {
                                cout << "Insufficient Balance!\n";
                            }
                        }
                        else {
                            cout << "Transaction Cancelled.\n";
                        }
                    }
                    else {
                        cout << "Invalid Amount, please enter a positive number.\n";
                    }
                    break;
                }
            }
            if (!found) cout << "Account Not Found.\n";
            cout << "\nPress Any Key to return to Menu...";
            system("pause > nul");
        }

        void transActionTotalBalances() {
            system("cls");
            vector<Client> vClients = _loadClientsFromFile();
            cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
            cout << "\n_________________________________________________________________________________\n" << endl;
            cout << "| " << left << setw(15) << "Account Number" << "| " << left << setw(40) << "Client Name" << "| " << left << setw(12) << "Balance";
            cout << "\n_________________________________________________________________________________\n" << endl;

            double totalBalance = 0;
            for (Client& c : vClients) {
                totalBalance += c.getAccountBalance();
                c.printBalanceRow();
                cout << endl;
            }
            cout << "\n_________________________________________________________________________________\n" << endl;
            cout << "\t\t\tYour Total Balance In Database Is: " << fixed << setprecision(2) << totalBalance << endl;
            cout << "_________________________________________________________________________________\n" << endl;
            cout << "Press Any Key To back to main menu";
            system("pause > nul");
        }

        void showTransactionsMenu() {
            system("cls");
            cout << "\n===========================================\n";
            cout << "\t\tTransactions Menu\n";
            cout << "===========================================\n";
            cout << "\t[1] Deposit\n";
            cout << "\t[2] Withdraw\n";
            cout << "\t[3] Total Balance\n";
            cout << "\t[4] Back To Main Menu\n";
            cout << "===========================================\n";
        }

        void showTransactionScreen() {
            showTransactionsMenu();
            short choice;
            cout << "Enter Your Service You Need: ";
            cin >> choice;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(1000, '\n');
                choice = 0;
            }
            switch (choice) {
            case 1:
                transactionDeposit();
                break;
            case 2:
                transactionWithdraw();
                break;
            case 3:
                transActionTotalBalances();
                break;
            case 4:
                return;
            default:
                cout << "Invalid Choice !!!!" << endl;
                cout << "Press Any Key to back to Main Menu" << endl;
                system("pause > nul");
            }
        }
    };

    class MenuController {
    public:
        static void showMainMenu() {
            system("cls");
            cout << "===========================================\n";
            cout << "\t\tMain Menu Screen\n";
            cout << "===========================================\n";
            cout << "\t[1] Show Client List.\n";
            cout << "\t[2] Add New Client.\n";
            cout << "\t[3] Delete Client.\n";
            cout << "\t[4] Update Client Info.\n";
            cout << "\t[5] Find Client.\n";
            cout << "\t[6] Transaction.\n";
            cout << "\t[7] Exit.\n";
            cout << "===========================================\n";
        }

        static void viewTheDataOfBank(BankManager& manager) {
            short choice;
            cout << "Enter Your Service You Need: ";
            cin >> choice;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(1000, '\n');
                choice = 0;
            }

            switch (choice) {
            case 1:
                manager.showAllClientsScreen();
                break;
            case 2:
                manager.addNewClientsScreen();
                break;
            case 3:
                manager.deleteClientScreen();
                break;
            case 4:
                manager.updateClientScreen();
                break;
            case 5:
                manager.findClientScreen();
                break;
            case 6:
                manager.showTransactionScreen();
                break;
            case 7:
                exit(0);
                break;
            default:
                cout << "\nInvalid Choice! Please enter a number from [1] to [7].\n";
                system("pause");
            }
        }
    };

    int main() {
        BankManager manager("Client.txt");

        while (true) {
            MenuController::showMainMenu();
            MenuController::viewTheDataOfBank(manager);
        }
        return 0;
    }