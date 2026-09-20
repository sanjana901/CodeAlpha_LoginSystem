// CodeAlpha C++ Internship - Task 1: CGPA Calculator
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <cctype>

using namespace std;

struct Course {
    string name;
    string grade;
    double creditHours;
    double gradePoint;
};

// Convert letter grade to grade points (4.0 scale). Returns -1 if invalid.
double gradeToPoint(string grade) {
    transform(grade.begin(), grade.end(), grade.begin(),
              [](unsigned char c) { return toupper(c); });

    if (grade == "A+" || grade == "A") return 4.0;
    if (grade == "A-") return 3.7;
    if (grade == "B+") return 3.3;
    if (grade == "B") return 3.0;
    if (grade == "B-") return 2.7;
    if (grade == "C+") return 2.3;
    if (grade == "C") return 2.0;
    if (grade == "C-") return 1.7;
    if (grade == "D+") return 1.3;
    if (grade == "D") return 1.0;
    if (grade == "F") return 0.0;
    return -1.0;
}

int readPositiveInt(const string &prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value && value > 0) {
            return value;
        }
        cout << "Invalid input. Please enter a positive whole number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

double readPositiveDouble(const string &prompt) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value && value > 0) {
            return value;
        }
        cout << "Invalid input. Please enter a number greater than 0.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

double readNonNegativeDouble(const string &prompt) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= 0) {
            return value;
        }
        cout << "Invalid input. Please enter 0 or a positive number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

int main() {
    cout << "=====================================\n";
    cout << "        CGPA CALCULATOR (C++)        \n";
    cout << "=====================================\n";
    cout << "Grades: A+, A, A-, B+, B, B-, C+, C, C-, D+, D, F\n\n";

    int n = readPositiveInt("Enter number of courses this semester: ");
    vector<Course> courses;

    double totalCredits = 0.0;
    double totalGradePoints = 0.0;

    for (int i = 1; i <= n; i++) {
        Course c;
        cout << "\n--- Course " << i << " ---\n";

        cout << "Course name: ";
        cin >> ws;
        getline(cin, c.name);

        while (true) {
            cout << "Grade (e.g. A, B+): ";
            cin >> c.grade;
            c.gradePoint = gradeToPoint(c.grade);
            if (c.gradePoint >= 0) break;
            cout << "Invalid grade. Try again.\n";
        }

        c.creditHours = readPositiveDouble("Credit hours: ");

        totalCredits += c.creditHours;
        totalGradePoints += c.gradePoint * c.creditHours;
        courses.push_back(c);
    }

    double semesterGPA = totalGradePoints / totalCredits;

    // Overall CGPA (including previous semesters, if any)
    char hasPrev;
    cout << "\nDo you have previous semester records? (y/n): ";
    cin >> hasPrev;

    double cgpa = semesterGPA;
    double overallCredits = totalCredits;

    if (hasPrev == 'y' || hasPrev == 'Y') {
        double prevCGPA = readNonNegativeDouble("Previous CGPA: ");
        double prevCredits = readNonNegativeDouble("Previous total credit hours: ");

        double prevPoints = prevCGPA * prevCredits;
        overallCredits = prevCredits + totalCredits;
        cgpa = (prevPoints + totalGradePoints) / overallCredits;
    }

    // Output
    cout << "\n=====================================\n";
    cout << "            RESULT SUMMARY           \n";
    cout << "=====================================\n";
    cout << left << setw(22) << "Course"
         << setw(8) << "Grade"
         << setw(10) << "Credits"
         << setw(8) << "Points" << "\n";
    cout << string(48, '-') << "\n";

    for (const auto &c : courses) {
        cout << left << setw(22) << c.name
             << setw(8) << c.grade
             << setw(10) << fixed << setprecision(1) << c.creditHours
             << setw(8) << setprecision(2) << c.gradePoint << "\n";
    }

    cout << string(48, '-') << "\n";
    cout << "Total Credits (semester): " << fixed << setprecision(1) << totalCredits << "\n";
    cout << "Total Grade Points      : " << setprecision(2) << totalGradePoints << "\n";
    cout << "Semester GPA            : " << setprecision(2) << semesterGPA << "\n";
    cout << "Overall CGPA            : " << setprecision(2) << cgpa << "\n";
    cout << "=====================================\n";

    return 0;
}
