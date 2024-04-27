#include <iostream>
#include <unordered_map>
#include <queue>
#include <exception>
#include <string>
using namespace std;

class Action {
    string type;
    string key;
    int value;
public:
    Action(string _type, string _key, int _value) : type(_type), key(_key), value(_value) {}

    string getType() const { return type; }
    string getKey() const { return key; }
    int getValue() const { return value; }
};

class inMemoryDB {
    unordered_map<string, int> database;
    queue<Action> tempActions;
    bool ongoingTransaction = false;

public:
    inMemoryDB() = default;

    void begin_transaction() {
        if (ongoingTransaction) {
            throw runtime_error("Transaction already in progress");
        }
        ongoingTransaction = true;
        cout << "Transaction started." << endl;
    }

    void commit() {
        if (!ongoingTransaction) {
            throw runtime_error("No transaction is in progress");
        }
        while (!tempActions.empty()) {
            Action tempAction = tempActions.front();
            database[tempAction.getKey()] = tempAction.getValue();
            tempActions.pop();
        }
        ongoingTransaction = false;
        cout << "Transaction committed." << endl;
    }

    int get(string key) {
        if (database.find(key) == database.end()) {
            cout << "This key does not exist in the database." << endl;
            return -1; // Indicate key not found
        }
        return database[key];
    }

    void put(string key, int value) {
        if (!ongoingTransaction) {
            throw runtime_error("No transaction in progress");
        }
        tempActions.emplace("put", key, value);
        cout << "Key " << key << " set to " << value << " within transaction." << endl;
    }

    void rollback() {
        if (!ongoingTransaction) {
            throw runtime_error("No transaction to rollback");
        }
        while (!tempActions.empty()) {
            tempActions.pop();
        }
        ongoingTransaction = false;
        cout << "Transaction rolled back." << endl;
    }
};

string printMenu() {
    cout << "Choose an option, ONLY type the number." << endl;
    cout << "1. Start Transaction" << endl;
    cout << "2. Get" << endl;
    cout << "3. Put" << endl;
    cout << "4. Commit" << endl;
    cout << "5. Rollback" << endl;
    cout << "6. Exit Program" << endl;
    cout << "Make your choice: ";
    string option;
    cin >> option;
    return option;
}

int main() {
    inMemoryDB db;
    string currentOption;

    while (currentOption != "6") {
        currentOption = printMenu();
        try {
            if (currentOption == "1") {
                db.begin_transaction();
            } else if (currentOption == "2") {
                cout << "Type a key to retrieve a value: ";
                string key;
                cin >> key;
                int value = db.get(key);
                if (value != -1) {
                    cout << "The value for key '" << key << "' is: " << value << endl;
                }
            } else if(currentOption == "3"){

                    cout << "Choose a key to put " << endl;
                    string newKey;
                    cin >> newKey;
                    cout << "Choose a value to put " << endl;
                    string newValue;
                    cin >> newValue;
                    try{
                        int newValueAsInt = stoi(newValue);
                        db.put(newKey , newValueAsInt);
                    } catch (const std::invalid_argument& e) {
                        // If no valid conversion could be performed
                        std::cout << "Invalid number entered. Please enter valid numeric digits." << std::endl;
                    }
                    catch (const std::out_of_range& e) {
                        // If the converted number is out of range for an int
                        std::cout << "The number is out of range for an int." << std::endl;
                    }


            } else if (currentOption == "4") {
                db.commit();
            } else if (currentOption == "5") {
                db.rollback();
            } else if (currentOption == "6") {
                cout << "Exiting program." << endl;
            } else {
                cout << "Invalid option selected. Please try again." << endl;
            }
        } catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }

    return 0;
}

