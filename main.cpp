#include <bits/stdc++.h>
#include <mysql>
#include <iomanip> 
using namespace std;

const char *DB_HOST = "localhost";
const char *DB_USER = "root";
const char *DB_PASSWORD = ""; 
const char *DB_NAME = "db";

class University {
    int studentID;
    string studentName, course;
    float cgpa;

public:
    University() : studentID(0), studentName(""), course(""), cgpa(0.0) {}

    void setStudentID(int id) { studentID = id; }
    void setStudentName(string name) { studentName = name; }
    void setCourse(string c) { course = c; }
    void setCGPA(float marks) { cgpa = marks; }

    int getStudentID() { return studentID; }
    string getStudentName() { return studentName; }
    string getCourse() { return course; }
    float getCGPA() { return cgpa; }
};

// Insert student record
void insertStudent(MYSQL *connection, University &student) {
    int id;
    string name, course;
    float cgpa;

    cout << "Enter Student ID: ";
    cin >> id;
    student.setStudentID(id);

    cout << "Enter Student Name: ";
    cin.ignore();
    getline(cin, name);
    student.setStudentName(name);

    cout << "Enter Student Course: ";
    getline(cin, course);
    student.setCourse(course);

    cout << "Enter Student CGPA: ";
    cin >> cgpa;
    student.setCGPA(cgpa);

    stringstream query;
    query << "INSERT INTO student(Id, Name, Course, CGPA) VALUES ('"
          << student.getStudentID() << "', '"
          << student.getStudentName() << "', '"
          << student.getCourse() << "', '"
          << student.getCGPA() << "')";

    if (mysql_query(connection, query.str().c_str())) {
        cerr << "Error: " << mysql_error(connection) << endl;
    } else {
        cout << "Data inserted successfully." << endl;
    }
}

// Display all records in tabular format
void displayRecords(MYSQL *connection) {
    string query = "SELECT * FROM student";

    if (mysql_query(connection, query.c_str())) {
        cerr << "Error: " << mysql_error(connection) << endl;
    } else {
        MYSQL_RES *result = mysql_store_result(connection);
        if (result) {
            int numFields = mysql_num_fields(result);
            MYSQL_ROW row;

            // Display column headers
            MYSQL_FIELD *fields = mysql_fetch_fields(result);
            cout << left << setw(10) << "ID"
                 << setw(20) << "Name"
                 << setw(20) << "Course"
                 << setw(10) << "CGPA" << endl;
            cout << string(60, '-') << endl;

            // Display rows of data
            while ((row = mysql_fetch_row(result))) {
                cout << left << setw(10) << row[0]  
                     << setw(20) << row[1]          
                     << setw(20) << row[2]          
                     << setw(10) << row[3]       
                     << endl;
            }

            mysql_free_result(result);
        } else {
            cerr << "Error fetching result: " << mysql_error(connection) << endl;
        }
    }
}

// Search for a student record by ID
void searchStudent(MYSQL *connection) {
    int id;
    cout << "Enter Student ID: ";
    cin >> id;

    stringstream query;
    query << "SELECT * FROM student WHERE Id='" << id << "'";

    if (mysql_query(connection, query.str().c_str())) {
        cerr << "Error: " << mysql_error(connection) << endl;
    } else {
        MYSQL_RES *result = mysql_store_result(connection);
        if (result) {
            MYSQL_ROW row = mysql_fetch_row(result);
            if (row) {
                cout << left << setw(10) << "ID"
                     << setw(20) << "Name"
                     << setw(20) << "Course"
                     << setw(10) << "CGPA" << endl;
                cout << string(60, '-') << endl;
                cout << left << setw(10) << row[0]   
                     << setw(20) << row[1]          
                     << setw(20) << row[2]         
                     << setw(10) << row[3]         
                     << endl;
            } else {
                cout << "No record found." << endl;
            }
            mysql_free_result(result);
        }
    }
}

// Update student record
void updateStudent(MYSQL *connection) {
    int id;
    cout << "Enter Student ID to update: ";
    cin >> id;

    int choice;
    cout << "Choose field to update:\n1. Name\n2. Course\n3. CGPA\n";
    cin >> choice;

    stringstream query;
    switch (choice) {
    case 1: {
        string newName;
        cout << "Enter new Name: ";
        cin.ignore();
        getline(cin, newName);
        query << "UPDATE student SET Name='" << newName << "' WHERE Id=" << id;
        break;
    }
    case 2: {
        string newCourse;
        cout << "Enter new Course: ";
        cin.ignore();
        getline(cin, newCourse);
        query << "UPDATE student SET Course='" << newCourse << "' WHERE Id=" << id;
        break;
    }
    case 3: {
        float newCGPA;
        cout << "Enter new CGPA: ";
        cin >> newCGPA;
        query << "UPDATE student SET CGPA=" << newCGPA << " WHERE Id=" << id;
        break;
    }
    default:
        cout << "Invalid choice." << endl;
        return;
    }

    if (mysql_query(connection, query.str().c_str())) {
        cerr << "Error: " << mysql_error(connection) << endl;
    } else {
        cout << "Record updated successfully." << endl;
    }
}

// Delete student record
void deleteStudent(MYSQL *connection) {
    int id;
    cout << "Enter Student ID to delete: ";
    cin >> id;

    stringstream query;
    query << "DELETE FROM student WHERE Id=" << id;

    if (mysql_query(connection, query.str().c_str())) {
        cerr << "Error: " << mysql_error(connection) << endl;
    } else {
        cout << "Record deleted successfully." << endl;
    }
}

// Main program
int main() {
    MYSQL *connection;
    connection = mysql_init(NULL);

    if (!mysql_real_connect(connection, DB_HOST, DB_USER, DB_PASSWORD, DB_NAME, 3306, NULL, 0)) {
        cerr << "Error connecting to database: " << mysql_error(connection) << endl;
        return 1;
    } else {
        cout << "Connected to database successfully." << endl;
    }

    University student;
    int choice;
    bool exitFlag = false;

    do {
        cout << "\nUNIVERSITY MANAGEMENT SYSTEM\n";
        cout << "1. Insert Student Data\n";
        cout << "2. Display Student Data\n";
        cout << "3. Search Student Data\n";
        cout << "4. Update Student Data\n";
        cout << "5. Delete Student Data\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            insertStudent(connection, student);
            break;
        case 2:
            displayRecords(connection);
            break;
        case 3:
            searchStudent(connection);
            break;
        case 4:
            updateStudent(connection);
            break;
        case 5:
            deleteStudent(connection);
            break;
        case 6:
            exitFlag = true;
            cout << "Exiting program..." << endl;
            break;
        default:
            cout << "Invalid choice. Try again." << endl;
        }
    } while (!exitFlag);

    mysql_close(connection);
    return 0;
}
