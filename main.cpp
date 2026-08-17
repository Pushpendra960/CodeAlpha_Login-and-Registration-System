#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <functional>
#include <cctype>

using namespace std;

// ======================================================
// Utility Function: Generate password hash
// ======================================================

string hashPassword(const string& password) {

    // std::hash converts the password into a hash value.
    // This avoids storing the original password directly.
    size_t hashValue = hash<string>{}(password);

    stringstream ss;

    ss << hex << hashValue;

    return ss.str();
}


// ======================================================
// Validate Username
// ======================================================

bool isValidUsername(const string& username) {

    // Username must contain at least 3 characters
    if (username.length() < 3) {
        return false;
    }

    // Username should contain only letters, digits or underscore
    for (char ch : username) {

        if (!isalnum(static_cast<unsigned char>(ch)) && ch != '_') {
            return false;
        }
    }

    return true;
}


// ======================================================
// Validate Password
// ======================================================

bool isValidPassword(const string& password) {

    // Minimum password length
    if (password.length() < 6) {
        return false;
    }

    bool hasLetter = false;
    bool hasDigit = false;

    for (char ch : password) {

        if (isalpha(static_cast<unsigned char>(ch))) {
            hasLetter = true;
        }

        if (isdigit(static_cast<unsigned char>(ch))) {
            hasDigit = true;
        }
    }

    // Password must contain at least one letter and one digit
    return hasLetter && hasDigit;
}


// ======================================================
// Check whether username already exists
// ======================================================

bool usernameExists(const string& username) {

    ifstream file("users.txt");

    string storedUsername;
    string storedPasswordHash;

    while (file >> storedUsername >> storedPasswordHash) {

        if (storedUsername == username) {
            file.close();
            return true;
        }
    }

    file.close();

    return false;
}


// ======================================================
// Registration Function
// ======================================================

void registerUser() {

    string username;
    string password;
    string confirmPassword;

    cout << "\n";
    cout << "========================================\n";
    cout << "           USER REGISTRATION            \n";
    cout << "========================================\n";

    // Username input
    cout << "Enter username: ";
    cin >> username;

    // Validate username
    if (!isValidUsername(username)) {

        cout << "\n[ERROR] Invalid username.\n";
        cout << "Username must contain at least 3 characters\n";
        cout << "and may contain only letters, digits and '_'.\n";

        return;
    }

    // Check duplicate username
    if (usernameExists(username)) {

        cout << "\n[ERROR] Username already exists.\n";
        cout << "Please choose a different username.\n";

        return;
    }

    // Password input
    cout << "Enter password: ";
    cin >> password;

    // Validate password
    if (!isValidPassword(password)) {

        cout << "\n[ERROR] Invalid password.\n";
        cout << "Password must contain at least 6 characters,\n";
        cout << "including at least one letter and one digit.\n";

        return;
    }

    // Confirm password
    cout << "Confirm password: ";
    cin >> confirmPassword;

    if (password != confirmPassword) {

        cout << "\n[ERROR] Passwords do not match.\n";

        return;
    }

    // Open file in append mode
    ofstream file("users.txt", ios::app);

    if (!file) {

        cout << "\n[ERROR] Unable to open user database.\n";

        return;
    }

    // Hash password before storing
    string passwordHash = hashPassword(password);

    // Store username and hashed password
    file << username << " " << passwordHash << endl;

    file.close();

    cout << "\n[SUCCESS] Registration completed successfully!\n";
    cout << "Welcome, " << username << "!\n";
}


// ======================================================
// Login Function
// ======================================================

bool loginUser(string& loggedInUser) {

    string username;
    string password;

    cout << "\n";
    cout << "========================================\n";
    cout << "              USER LOGIN                \n";
    cout << "========================================\n";

    cout << "Enter username: ";
    cin >> username;

    cout << "Enter password: ";
    cin >> password;

    // Generate hash from entered password
    string enteredPasswordHash = hashPassword(password);

    ifstream file("users.txt");

    if (!file) {

        cout << "\n[ERROR] No registered users found.\n";

        return false;
    }

    string storedUsername;
    string storedPasswordHash;

    // Read users one by one
    while (file >> storedUsername >> storedPasswordHash) {

        if (storedUsername == username &&
            storedPasswordHash == enteredPasswordHash) {

            file.close();

            loggedInUser = username;

            cout << "\n[SUCCESS] Login successful!\n";
            cout << "Welcome back, " << username << "!\n";

            return true;
        }
    }

    file.close();

    cout << "\n[ERROR] Invalid username or password.\n";

    return false;
}


// ======================================================
// User Dashboard
// ======================================================

void userDashboard(const string& username) {

    int choice;

    do {

        cout << "\n";
        cout << "========================================\n";
        cout << "             USER DASHBOARD             \n";
        cout << "========================================\n";

        cout << "Logged in as: " << username << "\n\n";

        cout << "1. View Profile\n";
        cout << "2. Logout\n";
        cout << "Enter your choice: ";

        cin >> choice;

        switch (choice) {

            case 1:

                cout << "\n----------------------------------------\n";
                cout << "Username: " << username << "\n";
                cout << "Account Status: Active\n";
                cout << "----------------------------------------\n";

                break;

            case 2:

                cout << "\n[SUCCESS] Logged out successfully.\n";

                break;

            default:

                cout << "\n[ERROR] Invalid choice.\n";
        }

    } while (choice != 2);
}


// ======================================================
// Main Menu
// ======================================================

int main() {

    int choice;
    string loggedInUser;

    cout << "\n";
    cout << "****************************************\n";
    cout << "*                                      *\n";
    cout << "*     LOGIN & REGISTRATION SYSTEM      *\n";
    cout << "*            CODEALPHA TASK 2          *\n";
    cout << "*                                      *\n";
    cout << "****************************************\n";

    do {

        cout << "\n";
        cout << "========================================\n";
        cout << "               MAIN MENU               \n";
        cout << "========================================\n";

        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";

        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {

            case 1:

                registerUser();

                break;


            case 2:

                if (loginUser(loggedInUser)) {

                    userDashboard(loggedInUser);
                }

                break;


            case 3:

                cout << "\nThank you for using the system!\n";
                cout << "Exiting program...\n";

                break;


            default:

                cout << "\n[ERROR] Invalid choice.\n";
                cout << "Please select 1, 2 or 3.\n";
        }

    } while (choice != 3);

    return 0;
}