//============================================================================
// Name        : Course Planner.cpp
// Author      : Brian Fulton
// Version     : 2.03
// Description : read from .txt
// issues: Need to remove Header from CSV when printing 
//       : Add Course names to preReqs when displaying case 3
//       : Couldn't use csvParser do to return error when using under "LoadCourse()"
//============================================================================
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>


using namespace std;

//displays menu
void Menu() {
    cout << "1: Load File" << endl;
    cout << "2: Print Course List" << endl;
    cout << "3: Print Course" << endl;
    cout << "4: Test <<<what is being tested>>>" << endl;
    cout << "9: Exit" << endl;

}

//defines a structure to hold course information
struct Course {
    string courseId;
    string courseName;
    vector<string> preReq;
};

//Dev structure for bst node
struct Node {
    Course course;
    Node* left;
    Node* right;

    //Default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }
    //initialize with a given course
    Node(Course aCourse) : Node() {
        this->course = aCourse;
    }

};

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:

    void Destruct(Node* node);

public:
    Node* root;
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void Insert(BinarySearchTree* tree, Node* node);
    void Search(string courseId);
    void PrintCourse(Node* node);
    void PrintCourseID(Node* node);
};

/**
 * Default constructor
 */

BinarySearchTree::BinarySearchTree() {
    root = nullptr;
};

/**
 * Destructor
 */

BinarySearchTree::~BinarySearchTree() {
    Destruct(root);
};

// recurse from root deleting every node
void BinarySearchTree::Destruct(Node* node) {
    if (node != nullptr) {
        Destruct(node->left);
        node->left = nullptr;
        Destruct(node->right);
        node->right = nullptr;
        delete node;
    }
};


/**
 * Search for a course
 */

void BinarySearchTree::Search(string courseId) {
    Node* currentNode = root;

    while (currentNode != nullptr) {
        if (currentNode->course.courseId == courseId) {
            cout << currentNode->course.courseId << ", " << currentNode->course.courseName << endl;
            cout << "Prerequisites for " << currentNode->course.courseName << ": " << endl;

            //Display preRequisites if any, comma seperated
            for (string preReq : currentNode->course.preReq) {
                if (preReq == currentNode->course.preReq.back()) {
                    cout << preReq << endl; //FIXME need more lines to add course names to found preReqs ***currentNode->course.courseName <<***
                }
                else {
                    cout << preReq << ", "; //<< preReq
                }
            }
            return;
        }
        else if (courseId < currentNode->course.courseId) {

            if (currentNode->left != nullptr) {
                currentNode = currentNode->left;
            }
        }
        else {
            currentNode = currentNode->right;
        }
    }

    //if not found
    cout << "Course " << courseId << "could not be located. " << endl;

    return;
}

/**
 * Insert a course
 */

void BinarySearchTree::Insert(BinarySearchTree* tree, Node* node) {

    if (tree->root == nullptr) {
        tree->root = node;
    }
    else {
        Node* curr = tree->root;
        while (curr != nullptr) {

            if (node->course.courseId < curr->course.courseId) {
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

//BST Function to traverse and display nodes
void BinarySearchTree::PrintCourse(Node* node) {

    //Traverse bst
    if (node == nullptr) {
        return;
    }

    //Display from left node and moves to next node
    PrintCourse(node->left);
    cout << node->course.courseId << ", " << node->course.courseName << endl;
    PrintCourse(node->right);
};

void BinarySearchTree::PrintCourseID(Node* node) {

    //Traverse tree to display options for user input on courseId
    if (node == nullptr) {
        return;
    }
    PrintCourseID(node->left);
    cout << node->course.courseId << endl;
    PrintCourseID(node->right);

};



//using ifstream to read && write from csv file
//FIXME ran out of time to fix issue with trying to use parser from previous bst lesson > ifstream was quick fix from stack overflow

void loadCourse(string csvPath, BinarySearchTree* bst) {
    cout << "Loading CSV File from Dir: " << "Dir path here" << endl; //find code to insert where dir came from

    ifstream file("courses.csv");
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
                    node->course.courseId = word;
                }
                else {
                    node->course.courseName = word;
                }
                ++num;
            }
            while (getline(str, word, ',')) {
                node->course.preReq.push_back(word);
            }

            //inserts node into bst
            bst->Insert(bst, node);
        }
    }
    //file error if file not found
    else {
        cout << "File error, please try again. " << endl;
        return;
    }

}

int main(int argc, char* argv[]) {
    string csvPath, courseKey;


    BinarySearchTree* bst = new BinarySearchTree();

    string fileChoice;
    string courseChoice;

    int i = 0;

    cout << "Welcome to ABCU Course planner." << endl << endl;
    // Menu loop exit case is 9
    while (i != 9) {
        Menu();
        cin >> i;

        switch (i) {

        case 1: // case one loads csv file

            loadCourse(csvPath, bst);
            cout << endl;
            break;

        case 2:
            //prints course starting at root   	FIXME prints courses in alphanumeric order need to drop the headder 
            bst->PrintCourse(bst->root);
            cout << endl;
            break;


        case 3:  //Display options for course look up && Display course w/preRequisite info
            cout << endl;
            bst->PrintCourseID(bst->root);
            cout << " \n";
            cout << "Enter a course to display \n" << endl;
            cin >> courseChoice;
            cout << endl;

            std::transform(courseChoice.begin(), courseChoice.end(), courseChoice.begin(), ::toupper);
            bst->Search(courseChoice);

            cout << endl;
            break;

            //exits the program
        case 9:
            cout << "Thank you! Have a great semester!" << endl;
            break;

            //default message if one of the above choices is not entered
        default:
            // cout << userInput << " is not a valid option." << endl << endl;
            break;
        }
    }
}
