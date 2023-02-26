//============================================================================
// Name        : Source.cpp
// Author      : Larry Jones CS-300 
// Version     : 1.0
// Description : ABCU Course Planner
//============================================================================

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <sstream>
#include <fstream>

using namespace std;

// Creates Course, with course ID and name as well as prerequisite
struct Course {
    string id;
    string name;
    vector<string> prerequisite;
};

// Structure for tree node
struct Node {
    Course course;
    Node* left;
    Node* right;

    // Default constructor
    Node() { 
        left = nullptr;
        right = nullptr;
    }
    // Initialize with course
    Node(Course aCourse) : Node() {
        this->course = aCourse;
    }
};

// Defines a class containing data members & methods for binary search tree
class BinarySearchTree {
private:
    void Destruct(Node* node);
public:
    Node* root;
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void Insert(BinarySearchTree* tree, Node* node);
    void Search(string id);
    void PrintCourse(Node* node);
};

// Default constructor
BinarySearchTree::BinarySearchTree() {
    root = nullptr;
};

// The destructor
BinarySearchTree::~BinarySearchTree() {
    Destruct(root);
};

// Destructs left & right pointer nodes, deletes them
void BinarySearchTree::Destruct(Node* node) {
    if (node != nullptr) {
        Destruct(node->left);
        node->left = nullptr;
        Destruct(node->right);
        node->right = nullptr;
        delete node;
    }
};

// Searches for course
void BinarySearchTree::Search(string id) {
    Node* currentNode = root;

    while (currentNode != nullptr) {
        if (currentNode->course.id == id) {
            // Prints course name & ID
            cout << currentNode->course.id << ", ";
            cout << currentNode->course.name;
            cout << endl;
            cout << "Prerequisites: ";
            // Prints any prerequisites found
            for (string prerequisite : currentNode->course.prerequisite) {
                if (prerequisite == currentNode->course.prerequisite.back()) {
                    cout << prerequisite << endl;
                }
                else {
                    cout << prerequisite << ", ";
                }
            }
            return;
        }
        // Searches left pointer if not found
        else if (id < currentNode->course.id) {
            if (currentNode->left != nullptr) {
                currentNode = currentNode->left;
            }
        }
        // Searches right pointer if not found
        else {
            currentNode = currentNode->right;
        }
    }
    // Course is not in list
    cout << "Course " << id << "not found. " << endl;
    return;
}

// Inserts course into course list
void BinarySearchTree::Insert(BinarySearchTree* tree, Node* node) {
    if (tree->root == nullptr) {
        tree->root = node;
    }
    else {
        Node* curr = tree->root;
        while (curr != nullptr) {
            if (node->course.id < curr->course.id) {
                if (curr->left == nullptr) {
                    curr->left = node;
                    curr = nullptr;
                }
                else {
                    curr = curr->left;
                }
            }
            else {
                if (curr->right == nullptr) {
                    curr->right = node;
                    curr = nullptr;
                }
                else {
                    curr = curr->right;
                }
            }
        }
    }
}
// Prints course list
void BinarySearchTree::PrintCourse(Node* node) {
    // Traverse & print entire tree
    if (node == nullptr) {
        return;
    }
    // Looks at left node, prints course name & ID, then at next node
    PrintCourse(node->left);
    cout << node->course.id << ", ";
    cout << node->course.name << endl;
    PrintCourse(node->right);
};

// Loads file & creates course list
void loadCourse(string filename, BinarySearchTree* bst) {
    ifstream file(filename);
    if (file.is_open()) {
        cout << "File loaded." << endl;
        int num;
        string line;
        string word;
        while (getline(file, line)) {
            num = 0;
            Node* node = new Node();
            stringstream str(line);
            while (num < 2) {
                getline(str, word, ',');
                if (num == 0) {
                    node->course.id = word;
                }
                else {
                    node->course.name = word;
                }
                num++;
            }
            while (getline(str, word, ',')) {
                node->course.prerequisite.push_back(word);
            }
            // Inserts node into bst
            bst->Insert(bst, node);
        }
    }
    // Indicates error if file is not found
    else {
        cout << "Error.. file not found. Try again." << endl;
        return;
    }
}

// Displays ABCU Course Planner menu
void DisplayMenu() {
    cout << "1. Load Data Structure" << endl;
    cout << "2. Print Course List" << endl;
    cout << "3. Print Course" << endl;
    cout << "9. Exit" << endl << endl;
    cout << "What would you like to do?";
}

void main() {
    BinarySearchTree* bst = new BinarySearchTree();
    string fin;
    string choice;
    int input = 0;
    cout << "Welcome to the ABCU Course Planner." << endl << endl;
    // Main loop for program while choice is not 9
    while (input != 9) {
        DisplayMenu();
        cin >> input;
        switch (input) {
            // Loads ABCUCourses.csv file
        case 1:
            cout << endl;
            fin = "ABCUCourses.csv";
            loadCourse(fin, bst);
            cout << endl;
            break;
            // Prints courses in alphanumeric order
        case 2:
            cout << endl;
            cout << "Here is a sample schedule: " << endl << endl;
            // Prints course starting at root   	
            bst->PrintCourse(bst->root);
            cout << endl;
            break;
            // Prints courses based on user input and prerequisites if applicable
        case 3:
            cout << endl;
            cout << "What course do you want to know about? ";
            cin >> choice;
            cout << endl;
            std::transform(choice.begin(), choice.end(), choice.begin(), ::toupper);
            bst->Search(choice);
            cout << endl;
            break;
            // Terminates program
        case 9:
            cout << "Thank you for using the ABCU Course Planner!" << endl;
            break;
            // Default catch statement to let user know option is not valid.
        default:
            cout << input << " is not a valid option." << endl << endl;
            break;
        }
    }
}