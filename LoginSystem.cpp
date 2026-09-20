// CodeAlpha C++ Internship - Task 2: Login and Registration System
// Credentials are stored in files: users/<username>.txt (one file per user)
// Passwords are stored as a hash (not plain text).
// Note: std::hash is used for simplicity in this demo. Real systems should
// use bcrypt / Argon2 with salts.

#include <iostream>
#include <fstream>
#include <string>
#include <functional>
#include <limits>
#include <cctype>
#include <sys/stat.h>

#ifdef _WIN32
#include <direct.h>
#define MAKE_DIR(d) _mkdir(d)
#else
#define MAKE_DIR(d) mkdir(d, 0755)
#endif

using namespace std;

const string USERS_DIR = "users";

void ensureUsersDir() {
    MAKE_DIR(USERS_DIR.c_str());
}

string userFilePath(const string &username) {
    return USERS_DIR + "/" + username + ".txt";
}

bool userExists(const string &username) {
    ifstream file(userFilePath(username));
    return file.good();
}

string hashPassword(const string &password) {
    hash<string> hasher;
    return to_string(hasher(password));
}

// Username: 3-20 chars, letters/digits/underscore only
bool isValidUsername(const string &username) {
    if (username.size() < 3 || username.size() > 20) return false;
    for (char c : username) {
        if (!isalnum(static_cast<unsigned char>(c)) && c != '_') return false;
    }
    return true;
}

// Password: min 6 chars, at least one letter and one digit
bool isValidPassword(const string &password) {
    if (password.size() < 6) return false;
    bool hasLetter = false, hasDigit = false;
    for (char c : password) {
        if (isalpha(static_cast<unsigned char>(c))) hasLetter = true;
        if (isdigit(static_cast<unsigned char>(c))) hasDigit = true;
    }
    return hasLetter && hasDigit;
}

void registerUser() {
    string username, password, confirm;

    cout << "\n--- REGISTRATION ---\n";
    cout << "Enter username (3-20 chars, letters/digits/_): ";
    cin >> username;

    if (!isValidUsername(username)) {
        cout << "Error: Invalid username format.\n";
        return;
    }

    if (userExists(username)) {
        cout << "Error: Username already taken. Try another one.\n";
        return;
    }

    cout << "Enter password (min 6 chars, letters + digits): ";
    cin >> password;

    if (!isValidPassword(password)) {
        cout << "Error: Password must be at least 6 characters and contain a letter and a digit.\n";
        return;
    }

    cout << "Confirm password: ";
    cin >> confirm;

    if (password != confirm) {
        cout << "Error: Passwords do not match.\n";
        return;
    }

    ofstream file(userFilePath(username));
    if (!file) {
        cout << "Error: Could not save user data.\n";
        return;
    }

    file << username << "\n" << hashPassword(password) << "\n";
    file.close();

    cout << "Success: Registration completed! You can now login.\n";
}

void loginUser() {
    string username, password;

    cout << "\n--- LOGIN ---\n";
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    if (!isValidUsername(username) || !userExists(username)) {
        cout << "Error: User not found.\n";
        return;
    }

    ifstream file(userFilePath(username));
    string storedUser, storedHash;
    getline(file, storedUser);
    getline(file, storedHash);
    file.close();

    if (storedUser == username && storedHash == hashPassword(password)) {
        cout << "Success: Login successful! Welcome, " << username << "!\n";
    } else {
        cout << "Error: Incorrect password.\n";
    }
}

int main() {
    ensureUsersDir();
    int choice = 0;

    while (true) {
        cout << "\n=====================================\n";
        cout << "   LOGIN & REGISTRATION SYSTEM (C++) \n";
        cout << "=====================================\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter 1, 2 or 3.\n";
            continue;
        }

        switch (choice) {
            case 1: registerUser(); break;
            case 2: loginUser(); break;
            case 3: cout << "Goodbye!\n"; return 0;
            default: cout << "Invalid choice. Try again.\n";
        }
    }
}
