#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <limits>
using namespace std;
// ==========================================
// SIMPLE COLOR MACROS (ANSI)
// ==========================================
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define RESET   "\033[0m"

// ==========================================
//E HANDLING FUNCTIONS
// ==========================================
void saveData(string filename, string data)
{
    // Check if new file to add headers
    ifstream check(filename);
    bool isNew = !check.good() || (check.peek() == EOF);
    check.close();

    ofstream file(filename, ios::app);
    if (file.is_open())
    {
        // 1. Write Header MANUALLY if new file
        if (isNew) {
            if (filename == "prisoners.txt")
                file << "ID      Name           Age     Crime         Years     Cell    Officer" << endl;
            else if (filename == "officers.txt")
                file << "ID        Name                 Rank" << endl;
            else if (filename == "cells.txt")
                file << "ID        Capacity" << endl;
        }

        // 2. Format the CSV data into Fixed Width Columns MANUALLY
        // We will loop through the data string, find commas, and print words with manual spaces

        string word = "";
        int col = 0;
        string processData = data + ","; // Add comma to process last word

        for (int i = 0; i < processData.length(); i++) {
            char c = processData[i];

            if (c == ',') {
                // Determine width based on file and column index
                int w = 10; // default
                if (filename == "prisoners.txt") {
                    if (col == 0) w = 8;        // ID
                    else if (col == 1) w = 15;  // Name
                    else if (col == 2) w = 8;   // Age
                    else if (col == 3) w = 15;  // Crime
                    else if (col == 4) w = 10;  // Sentence
                    else if (col == 5) w = 8;   // Cell
                    else if (col == 6) w = 15;  // Officer
                }
                else if (filename == "officers.txt") {
                    if (col == 0) w = 10;       // ID
                    else if (col == 1) w = 20;  // Name
                    else if (col == 2) w = 15;  // Rank
                }
                else if (filename == "cells.txt") {
                    if (col == 0) w = 10;       // ID
                    else if (col == 1) w = 15;  // Capacity
                }

                // Write the word
                file << word;

                // Manually write spaces
                int spaces = w - word.length();
                if (spaces < 1) spaces = 1; // Minimum 1 space
                for (int s = 0; s < spaces; s++) {
                    file << " ";
                }

                word = "";
                col++;
            }
            else {
                word += c;
            }
        }
        file << endl; // End the line in the file

        file.close();
        cout << "                              " << GREEN << "[SUCCESS] Data saved to file!" << RESET << endl;
    }
    else
    {
        cout << "                              " << RED << "[ERROR] Could not save to file!" << RESET << endl;
    }
}

void loadData(string filename)
{
    ifstream file(filename);
    if (file.is_open())
    {
        string line;
        cout << "\n";
        cout << "                              " << CYAN << "=== VIEWING FILE: " << filename << " ===" << RESET << endl;
        // The file is now pre-formatted, so we just print line by line
        while (getline(file, line))
        {
            cout << "                              " << line << endl;
        }
        file.close();
    }
    else
    {
        cout << "                              " << RED << "[ERROR] File not found!" << RESET << endl;
    }
}

// ==========================================
// UI HELPER FUNCTIONS
// ==========================================
void printHeader(string title)
{
    cout << "\n";
    cout << "                         =========================================" << endl;
    cout << "                         " << CYAN << title << RESET << endl;
    cout << "                         =========================================" << endl;
    cout << "\n";
}

void printWelcome()
{
    cout << "\n\n";
    cout << "                         =========================================" << endl;
    cout << "                         " << CYAN << "      WELCOME TO SHAWSHANK     " << RESET << endl;
    cout << "                         =========================================" << endl;
    cout << "                         " << YELLOW << "\"The Hardest Prison to Escape is from your Mind.\"" << RESET << endl;
    cout << "\n";
}

void printSuccess(string msg) {
    cout << "                              " << GREEN << "[SUCCESS] " << msg << RESET << endl;
}

void printError(string msg) {
    cout << "                              " << RED << "[ERROR] " << msg << RESET << endl;
}

string getPassword()
{
    string password = "";
    char ch;
    while (true)
    {
        ch = _getch();
        if (ch == 13)
        {
            cout << endl;
            break;
        }
        else if (ch == 8)
        {
            if (password.length() > 0)
            {
                cout << "\b \b";
                password.pop_back();
            }
        }
        else
        {
            cout << "*";
            password += ch;
        }
    }
    return password;
}

// ==========================================
// DATA STRUCTURES (NODES)
// ==========================================

struct Officer
{
    int id;
    string name;
    string rank;
    Officer* next;
};

struct Cell
{
    int id;
    int capacity;
    int currentCount;
    Cell* next;
};

struct Prisoner
{
    int id;
    string name;
    int age;
    string crime;
    int sentence;
    int cellID;
    int officerID;
    Prisoner* next;
};

struct Visitor
{
    string name;
    int prisonerID;
    Visitor* next;
};

// ==========================================
// STACK
// ==========================================
class IDStack
{
    struct Node
    {
        int id;
        Node* next;
    };
    Node* top;

public:
    IDStack() : top(NULL) {}

    void push(int id)
    {
        Node* newNode = new Node();
        newNode->id = id;
        newNode->next = top;
        top = newNode;
    }

    int pop()
    {
        if (isEmpty()) return -1;
        int id = top->id;
        Node* temp = top;
        top = top->next;
        delete temp;
        return id;
    }

    bool isEmpty()
    {
        return top == NULL;
    }
};

// ==========================================
// QUEUE
// ==========================================
class VisitorQueue
{
    Visitor* front;
    Visitor* rear;

public:
    VisitorQueue() : front(NULL), rear(NULL) {}

    void enqueue(string name, int pID)
    {
        Visitor* newVisitor = new Visitor();
        newVisitor->name = name;
        newVisitor->prisonerID = pID;
        newVisitor->next = NULL;

        if (rear == NULL)
        {
            front = rear = newVisitor;
        }
        else
        {
            rear->next = newVisitor;
            rear = newVisitor;
        }
        printSuccess("Visitor added to queue.");
    }

    void dequeue()
    {
        if (front == NULL)
        {
            printError("Queue is empty.");
            return;
        }
        Visitor* temp = front;
        cout << "                              Processing Visitor: " << temp->name << " for Prisoner ID: " << to_string(temp->prisonerID) << endl;
        front = front->next;
        if (front == NULL) rear = NULL;
        delete temp;
    }

    void display() {
        if (front == NULL) {
            cout << "                              No visitors in queue." << endl;
            return;
        }
        Visitor* temp = front;
        int count = 1;
        cout << "                              " << CYAN << "Visitor Queue:" << RESET << endl;
        while (temp != NULL) {
            cout << "                              " << count << ". " << temp->name << " (Visiting ID: " << to_string(temp->prisonerID) << ")" << endl;
            temp = temp->next;
            count++;
        }
    }
};

// ==========================================
// LINKED LISTS
// ==========================================

class OfficerList {
    Officer* head;

public:
    OfficerList() : head(NULL) {}

    bool exists(int id) {
        Officer* temp = head;
        while (temp != NULL) {
            if (temp->id == id) return true;
            temp = temp->next;
        }
        return false;
    }

    bool isEmpty() {
        return head == NULL;
    }

    string getName(int id) {
        Officer* temp = head;
        while (temp != NULL) {
            if (temp->id == id) return temp->name;
            temp = temp->next;
        }
        return "Unknown";
    }

    void addOfficer(int id, string name, string rank) {
        if (exists(id)) {
            printError("Officer ID already exists!");
            return;
        }
        Officer* newOfficer = new Officer();
        newOfficer->id = id;
        newOfficer->name = name;
        newOfficer->rank = rank;
        newOfficer->next = head;
        head = newOfficer;
        printSuccess("Officer added successfully.");

        // Save to file (Comma separation for logic but saveData will format it)
        saveData("officers.txt", to_string(id) + "," + name + "," + rank);
    }

    void display() {
        if (head == NULL) {
            cout << "                              No officers found." << endl;
            return;
        }
        printHeader("OFFICER LIST");

        cout << "                              " << CYAN;
        cout << "ID        Name                 Rank" << RESET << endl;
        cout << "                              ---------------------------------------------" << endl;

        Officer* temp = head;
        while (temp != NULL) {
            cout << "                              ";

            // Manual space printing
            string s = to_string(temp->id);
            cout << s; for (int i = 0; i < 10 - s.length(); i++) cout << " ";

            s = temp->name;
            cout << s; for (int i = 0; i < 20 - s.length(); i++) cout << " ";

            s = temp->rank;
            cout << s; for (int i = 0; i < 15 - s.length(); i++) cout << " ";

            cout << endl;
            temp = temp->next;
        }
    }
};

class CellList {
    Cell* head;

public:
    CellList() : head(NULL) {}

    bool exists(int id) {
        Cell* temp = head;
        while (temp != NULL) {
            if (temp->id == id) return true;
            temp = temp->next;
        }
        return false;
    }

    bool isEmpty() {
        return head == NULL;
    }

    bool hasSpace(int id) {
        Cell* temp = head;
        while (temp != NULL) {
            if (temp->id == id) {
                return temp->currentCount < temp->capacity;
            }
            temp = temp->next;
        }
        return false;
    }

    void incrementCount(int id) {
        Cell* temp = head;
        while (temp != NULL) {
            if (temp->id == id) {
                temp->currentCount++;
                return;
            }
            temp = temp->next;
        }
    }

    void decrementCount(int id) {
        Cell* temp = head;
        while (temp != NULL) {
            if (temp->id == id) {
                if (temp->currentCount > 0) temp->currentCount--;
                return;
            }
            temp = temp->next;
        }
    }

    void addCell(int id, int capacity) {
        if (exists(id)) {
            printError("Cell ID already exists!");
            return;
        }
        Cell* newCell = new Cell();
        newCell->id = id;
        newCell->capacity = capacity;
        newCell->currentCount = 0;
        newCell->next = head;
        head = newCell;
        printSuccess("Cell added successfully.");

        // Save to file
        saveData("cells.txt", to_string(id) + "," + to_string(capacity));
    }

    void display() {
        if (head == NULL) {
            cout << "                              No cells found." << endl;
            return;
        }
        printHeader("CELL LIST");

        cout << "                              " << CYAN;
        cout << "ID        Occupied    Capacity" << RESET << endl;
        cout << "                              ----------------------------------" << endl;

        Cell* temp = head;
        while (temp != NULL) {
            cout << "                              ";

            string s = to_string(temp->id);
            cout << s; for (int i = 0; i < 10 - s.length(); i++) cout << " ";

            s = to_string(temp->currentCount);
            cout << s; for (int i = 0; i < 12 - s.length(); i++) cout << " ";

            s = to_string(temp->capacity);
            cout << s; for (int i = 0; i < 12 - s.length(); i++) cout << " ";

            cout << endl;
            temp = temp->next;
        }
    }
};

class PrisonerList {
    Prisoner* head;
    IDStack recycledIDs;
    int nextID;

public:
    PrisonerList() : head(NULL), nextID(1) {}

    void addPrisoner(string name, int age, string crime, int sentence, int cellID, int officerID, CellList& cells, OfficerList& officers) {
        // Validation
        if (!cells.exists(cellID)) {
            printError("Cell ID does not exist!");
            return;
        }
        if (!cells.hasSpace(cellID)) {
            printError("Cell is full!");
            return;
        }
        if (!officers.exists(officerID)) {
            printError("Officer ID does not exist!");
            return;
        }

        int id;
        if (!recycledIDs.isEmpty()) {
            id = recycledIDs.pop();
        }
        else {
            id = nextID++;
        }

        Prisoner* newPrisoner = new Prisoner();
        newPrisoner->id = id;
        newPrisoner->name = name;
        newPrisoner->age = age;
        newPrisoner->crime = crime;
        newPrisoner->sentence = sentence;
        newPrisoner->cellID = cellID;
        newPrisoner->officerID = officerID;
        newPrisoner->next = head;
        head = newPrisoner;

        cells.incrementCount(cellID);
        printSuccess("Prisoner added successfully. Assigned ID: " + to_string(id));

        // Save to file
        saveData("prisoners.txt", to_string(id) + "," + name + "," + to_string(age) + "," + crime + "," + to_string(sentence) + "," + to_string(cellID) + "," + to_string(officerID));
    }

    void deletePrisoner(int id, CellList& cells) {
        Prisoner* temp = head;
        Prisoner* prev = NULL;

        while (temp != NULL && temp->id != id) {
            prev = temp;
            temp = temp->next;
        }

        if (temp == NULL) {
            printError("Prisoner not found.");
            return;
        }

        // Update Cell count
        cells.decrementCount(temp->cellID);

        // Remove from list
        if (prev == NULL) {
            head = temp->next;
        }
        else {
            prev->next = temp->next;
        }

        recycledIDs.push(id); // Recycle ID
        delete temp;
        printSuccess("Prisoner deleted successfully.");
    }

    void editPrisoner(int id) {
        Prisoner* temp = head;
        while (temp != NULL) {
            if (temp->id == id) {
                cout << "                              Editing Prisoner ID: " << id << endl;
                cout << "                              Enter new Name: "; cin >> temp->name;
                cout << "                              Enter new Age: "; cin >> temp->age;
                cout << "                              Enter new Crime: "; cin >> temp->crime;
                cout << "                              Enter new Sentence (years): "; cin >> temp->sentence;
                printSuccess("Prisoner updated.");
                return;
            }
            temp = temp->next;
        }
        printError("Prisoner not found.");
    }

    void searchPrisoner(int id, OfficerList& officers) {
        Prisoner* temp = head;
        while (temp != NULL) {
            if (temp->id == id) {
                printHeader("PRISONER DETAILS");
                cout << "                              ID: " << temp->id << endl;
                cout << "                              Name: " << temp->name << endl;
                cout << "                              Age: " << temp->age << endl;
                cout << "                              Crime: " << temp->crime << endl;
                cout << "                              Sentence: " << temp->sentence << " years" << endl;
                cout << "                              Cell ID: " << temp->cellID << endl;
                cout << "                              Officer: " << officers.getName(temp->officerID) << " (ID: " << temp->officerID << ")" << endl;
                return;
            }
            temp = temp->next;
        }
        printError("Prisoner not found.");
    }

    void display(OfficerList& officers) {
        if (head == NULL) {
            cout << "                              No prisoners found." << endl;
            return;
        }
        printHeader("PRISONER LIST");

        cout << "                              " << CYAN;
        cout << "ID    Name           Age     Crime           Sent.     Cell    Officer" << RESET << endl;
        cout << "                              -----------------------------------------------------------------------" << endl;

        Prisoner* temp = head;
        while (temp != NULL) {
            cout << "                              ";

            string s = to_string(temp->id);
            cout << s; for (int i = 0; i < 8 - s.length(); i++) cout << " ";

            s = temp->name;
            cout << s; for (int i = 0; i < 15 - s.length(); i++) cout << " ";

            s = to_string(temp->age);
            cout << s; for (int i = 0; i < 8 - s.length(); i++) cout << " ";

            s = temp->crime;
            cout << s; for (int i = 0; i < 15 - s.length(); i++) cout << " ";

            s = to_string(temp->sentence);
            cout << s; for (int i = 0; i < 10 - s.length(); i++) cout << " ";

            s = to_string(temp->cellID);
            cout << s; for (int i = 0; i < 8 - s.length(); i++) cout << " ";

            s = officers.getName(temp->officerID);
            cout << s; for (int i = 0; i < 15 - s.length(); i++) cout << " ";

            cout << endl;
            temp = temp->next;
        }
    }
};

// ==========================================
// MAIN SYSTEM
// ==========================================
class PrisonSystem {
    PrisonerList prisoners;
    OfficerList officers;
    CellList cells;
    VisitorQueue visitors;

public:
    bool login() {
        string user, pass;
        while (true) {
            system("cls");
            cout << "\n\n\n";
            cout << "                         -----------------------------------------" << endl;
            cout << "                         |              LOGIN PANEL              |" << endl;
            cout << "                         -----------------------------------------" << endl;
            cout << "                         |                                       |" << endl;
            cout << "                              Username: ";
            cin >> user;
            cout << "                         |                                       |" << endl;
            cout << "                              Password: ";
            pass = getPassword();
            cout << "                         |                                       |" << endl;
            cout << "                         -----------------------------------------" << endl;

            if (user == "admin" && pass == "1234") {
                printSuccess("Login Successful!");
                system("timeout /t 1 >nul");
                return true;
            }
            else {
                printError("Invalid Credentials! Try again.");
                system("timeout /t 1 >nul");
            }
        }
    }

    void menu() {
        while (true) {
            system("cls");
            printWelcome();

            cout << "                              1. Manage Prisoners" << endl;
            cout << "                              2. Manage Officers" << endl;
            cout << "                              3. Manage Cells" << endl;
            cout << "                              4. Visitor Queue" << endl;
            cout << "                              5. View Saved Files" << endl;
            cout << "                              6. Logout" << endl;
            cout << "                              7. Exit" << endl;
            cout << "\n";
            cout << "                              Enter choice: ";

            int choice;
            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "\n";
                printError("Invalid input! Please enter a number.");
                system("timeout /t 1 >nul");
                continue;
            }

            switch (choice) {
            case 1: prisonerMenu(); break;
            case 2: officerMenu(); break;
            case 3: cellMenu(); break;
            case 4: visitorMenu(); break;
            case 5: viewFilesMenu(); break;
            case 6: return; // Logout
            case 7: exit(0); // Exit
            default: cout << "\n"; printError("Invalid choice!"); system("timeout /t 1 >nul");
            }
        }
    }

    void viewFilesMenu() {
        while (true) {
            system("cls");
            printHeader("VIEW SAVED FILES");
            cout << "                              1. View Prisoners File" << endl;
            cout << "                              2. View Officers File" << endl;
            cout << "                              3. View Cells File" << endl;
            cout << "                              4. Back" << endl;
            cout << "\n";
            cout << "                              Enter choice: ";

            int choice;
            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            if (choice == 4) return;

            if (choice == 1) {
                loadData("prisoners.txt");
            }
            else if (choice == 2) {
                loadData("officers.txt");
            }
            else if (choice == 3) {
                loadData("cells.txt");
            }

            system("pause");
        }
    }

    void prisonerMenu() {
        while (true) {
            system("cls");
            printHeader("MANAGE PRISONERS");
            cout << "                              1. Add Prisoner" << endl;
            cout << "                              2. Edit Prisoner" << endl;
            cout << "                              3. Delete Prisoner" << endl;
            cout << "                              4. Search Prisoner" << endl;
            cout << "                              5. Display All" << endl;
            cout << "                              6. Back" << endl;
            cout << "\n";
            cout << "                              Choice: ";

            int choice;
            if (!(cin >> choice)) {
                cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            if (choice == 6) return;

            if (choice == 1) {
                if (cells.isEmpty()) {
                    cout << "\n";
                    printError("No Cells found! Please add a cell first.");
                    system("pause");
                    continue;
                }
                if (officers.isEmpty()) {
                    cout << "\n";
                    printError("No Officers found! Please add an officer first.");
                    system("pause");
                    continue;
                }

                string name, crime;
                int age, sentence, cellID, offID;
                cout << "\n";
                cout << "                              Name: "; cin >> name;
                cout << "                              Age: "; cin >> age;
                cout << "                              Crime: "; cin >> crime;
                cout << "                              Sentence (years): "; cin >> sentence;
                cout << "                              Assign Cell ID: "; cin >> cellID;
                cout << "                              Assign Officer ID: "; cin >> offID;
                prisoners.addPrisoner(name, age, crime, sentence, cellID, offID, cells, officers);
            }
            else if (choice == 2) {
                cout << "\n";
                int id;
                cout << "                              Enter ID: ";
                cin >> id;
                prisoners.editPrisoner(id);
            }
            else if (choice == 3) {
                cout << "\n";
                int id;
                cout << "                              Enter ID: ";
                cin >> id;
                prisoners.deletePrisoner(id, cells);
            }
            else if (choice == 4) {
                cout << "\n";
                int id;
                cout << "                              Enter ID: ";
                cin >> id;
                prisoners.searchPrisoner(id, officers);
            }
            else if (choice == 5) {
                prisoners.display(officers);
            }
            system("pause");
        }
    }

    void officerMenu() {
        while (true) {
            system("cls");
            printHeader("MANAGE OFFICERS");
            cout << "                              1. Add Officer" << endl;
            cout << "                              2. Display All" << endl;
            cout << "                              3. Back" << endl;
            cout << "\n";
            cout << "                              Choice: ";

            int choice;
            if (!(cin >> choice)) {
                cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            if (choice == 3) return;

            if (choice == 1) {
                int id; string name, rank;
                cout << "\n";
                cout << "                              ID: "; cin >> id;
                cout << "                              Name: "; cin >> name;
                cout << "                              Rank: "; cin >> rank;
                officers.addOfficer(id, name, rank);
            }
            else if (choice == 2) {
                officers.display();
            }
            system("pause");
        }
    }

    void cellMenu() {
        while (true) {
            system("cls");
            printHeader("MANAGE CELLS");
            cout << "                              1. Add Cell" << endl;
            cout << "                              2. Display All" << endl;
            cout << "                              3. Back" << endl;
            cout << "\n";
            cout << "                              Choice: ";

            int choice;
            if (!(cin >> choice)) {
                cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            if (choice == 3) return;

            if (choice == 1) {
                int id, cap;
                cout << "\n";
                cout << "                              ID: "; cin >> id;
                cout << "                              Capacity: "; cin >> cap;
                cells.addCell(id, cap);
            }
            else if (choice == 2) {
                cells.display();
            }
            system("pause");
        }
    }

    void visitorMenu() {
        while (true) {
            system("cls");
            printHeader("VISITOR QUEUE");
            cout << "                              1. Add Visitor" << endl;
            cout << "                              2. Process Visitor" << endl;
            cout << "                              3. Display Queue" << endl;
            cout << "                              4. Back" << endl;
            cout << "\n";
            cout << "                              Choice: ";

            int choice;
            if (!(cin >> choice)) {
                cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            if (choice == 4) return;

            if (choice == 1) {
                string name; int pid;
                cout << "\n";
                cout << "                              Visitor Name: "; cin >> name;
                cout << "                              Prisoner ID to visit: "; cin >> pid;
                visitors.enqueue(name, pid);
            }
            else if (choice == 2) {
                visitors.dequeue();
            }
            else if (choice == 3) {
                visitors.display();
            }
            system("pause");
        }
    }
};

int main() {
    PrisonSystem sys;
    while (true) {
        if (sys.login()) {
            sys.menu();
        }
    }
    return 0;
}
