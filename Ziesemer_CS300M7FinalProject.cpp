// Ziesemer_CS300M7FinalProject.cpp : This file contains the 'main' function. Program execution begins and ends there.


#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>


using namespace std;

//Object to contain course data for each course 
class Course {
    //Basic course number and name are contained as stings
private:
    string courseNum;
    string courseName;
    //Vector to hold all prerequisit course numbers
    vector<string> preReqs;
public:
    //Data is initialized to blank for easier checking to see if the data has been filled in
    Course() {
        courseNum = "";
        courseName = "";
    }
    //Setters and getters for the variables of the class
    void setCourseNum(string courseNum) {
        this->courseNum = courseNum;
    }

    void setCourseName(string courseName) {
        this->courseName = courseName;
    }

    void addPreReq(string preReq) {
        this->preReqs.push_back(preReq);
    }

    string getCourseNum() {
        return this->courseNum;
    }

    string getCourseName() {
        return this->courseName;
    }

    vector<string> getPreReqs() {
        return this->preReqs;
    }
    //print function built into the class for easier and safer access to private variables
    void printCourse() {

        cout << courseNum << ", " << courseName << endl;
        //Checks if there are any preReqs
        if (preReqs.size() > 0) {

            cout << "Prerequisites: ";
            //for loop to run through all the preReqs and print them
            for (int i = 0; i < preReqs.size(); i++) {

                cout << preReqs.at(i);

                if ((i + 1) < preReqs.size()) {
                    cout << ", ";
                }
            }
            cout << endl;
        }
    }
    //Less detailed print function used when printing all courses
    void printCourseBasic() {
        cout << courseNum << ", " << courseName << endl;
    }

};

//Partition function used by the quickSort
int partition(vector<Course>& courses, int left, int right) {
    
    //Set pivot index to the midpoint between left and right indexes
    int pivotIndex = left + (right - left) / 2;

    //Set pivot value to the courseNum at the pivotIndex
    string pivotValue = courses.at(pivotIndex).getCourseNum();

    //Initialize tempValue to later hold course to be moved
    Course tempValue;

    //While loop to go until left index equals or surpases right index
    while (left <= right) {

        //increase left index until the courseNum at that index is larger than pivotValue
        while (courses.at(left).getCourseNum().compare(pivotValue) < 0) {
            ++left;
        }

        //Decrease right index until the courseNum at that index is less than pivotValue
        while (courses.at(right).getCourseNum().compare(pivotValue) > 0) {
            --right;
        }

        //check to make sure that left index is still less than right
        if (left <= right) {

            //set tempValue to the course at the left index
            tempValue = courses.at(left);

            //set the left index to hold the course currently at the right index
            courses.at(left) = courses.at(right);

            //set right index to hold course in tempValue
            courses.at(right) = tempValue;

            //Increase left and decrease right index to keep process moving
            left++;
            right--;
        }
    }
    //return left index as pivot index for quickSort
    return left;

}


//quicksort function for vector of courses
void quickSort(vector<Course>& courses, int left, int right) {

    //Check if left index is smaller than right. if it is not then vector section is considered sorted 
    //and exit back to calling function
    if (left < right) {

        //call partition to sort vector section and use the return as the new pivotIndex
        int pivotIndex = partition(courses, left, right);

        //call recursive quicksort for left and right half of the current vector section
        quickSort(courses, left, pivotIndex - 1);
        quickSort(courses, pivotIndex, right);

    }
}

//function to print all courses held in the vector courses
void printCourseList(vector<Course> courses) {

    //First call quicksort to put courses in alphanumeric order least to greatest
    quickSort(courses, 0, courses.size() - 1);

    //for loop to go through every course and call the basic print function
    for (int i = 0; i < courses.size(); i++) {
        courses.at(i).printCourseBasic();
    }
}


//search function that uses the courseNum to find the course
//at current size this works best but for much larger system consider sorting list immediatly after creation
Course findCourse(vector<Course> courses, string courseNum) {

    //For loop to search through all elements of the vector
    for (int i = 0; i < courses.size(); i++) {

        //if courseNum found then return course at that index
        if (courses.at(i).getCourseNum() == courseNum) {
            return courses.at(i);
        }
    }

    //create blankCourse to return so system knows the course couldnt be found
    Course blankCourse;
    return blankCourse;
}


//Function to read file and fill vector courses
int createDataStructure(vector<Course>& courses, string fileName) { 

    //Create in file stream 
    ifstream courseFile;

    //Create vectors to hold all courseNums and preReqs for late comparison
    vector<string> preReqs;
    vector<string> courseNums;

    //open file
    courseFile.open(fileName);

    //if the file has failed to open let user know and exit program
    if (!courseFile) {
        cout << "File does not exist!" << endl;
        return -1;
    }

    //initialize line to hold each line of the file
    string line;


    //while loop to go through each line of the file
    while (getline(courseFile, line)) {

        // items to make sure each line has two parts at minimum for courseNum and courseName
        int items = 0;

        //value used to hold current section of line
        string value;

        //course object to hold data to be added to the courses vector
        Course tempCourse;

        //turn line into stringstream
        stringstream s(line);

        //check and get string up to endline or ,
        if (getline(s, value, ',')) {

            //set first value to courseNum
            tempCourse.setCourseNum(value);

            //Add value to courseNums vector
            courseNums.push_back(value);

            //increase items by one
            items++;
        }

        //chek and get string to endline or ,
        if (getline(s, value, ',')) {

            //set second value to courseName
            tempCourse.setCourseName(value);

            //increase items by one
            items++;
        }

        //while loop to check for any more items in the current line until endline is reached
        while (getline(s, value, ',')) {
            //add each item to current course preReqs vector
            tempCourse.addPreReq(value);

            //add each item to the preReq vector for later comparison
            preReqs.push_back(value);
            }

        //if there are not at least two items in current line then file is missing information
        //or has not been formatted correctly. let user know and exit program
        if (items < 2) {
            cout << "course name or number missing for course: " << tempCourse.getCourseNum() << endl;
            return -1;
        }

        //If line is good them add tempCourse to the courses vector
        courses.push_back(tempCourse);
    }

    //Once all lines have been read close file
    courseFile.close();

    //for loop to itterate through all preReqs
    for (int i = 0; i < preReqs.size(); i++) {

        bool exists = false;

        //for loop to itterate through all courseNums
        for (int j = 0; j < courseNums.size(); j++) {

            //check that each preReq has a corrisponding courseNum
            if (courseNums.at(j) == preReqs.at(i)) {
                exists = true;
            }
        }

        //If there is not a courseNum for any of the prereqs than information is missing.
        //let user know and exit program
        if (!exists) {
            cout << "PreReq: " << preReqs.at(i) << " does not exist in course list!" << endl;
            return -1;
        }
    }

    //If file read is succesful return 0 
    return 0;
}

//menu for user to make choices and call other functions
 int menu() {

     //initialize courses vector to hold all courses
    vector<Course> courses;
    
    bool done = false;

    //while loop to continue until user exits or program self exits
    while (!done) {

        //Print out menu options
        cout << "Main Menu" << endl;
        cout << "1. Load Data Structure." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "4. Exit" << endl;

        int userChoice = -99;

        //Prompt user for choice and store choise in userChoice variable
        cout << "What would you like to do? ";
        cin >> userChoice;

        //Initialize needed variables for each choice
        string courseNum;
        string fileName;
        Course foundCourse;


        //switch to react to userChoice
        switch (userChoice) {

        case 1:

            //Prompt user for filename to read courses from
            cout << "Please enter the filename with extension: ";
            cin >> fileName;

            //Call createdatastructure and check for file read success or failure
            if (createDataStructure(courses, fileName) == -1) {
                
                //If fileRead fails exit menu function
                return -1;
            }
            break;

        case 2:

            //call printCourseList to display all courses in courses vector
            //prints nothing if no courses loaded
            printCourseList(courses);
            break;

        case 3:

            //Prompt user for course number to print full course details
            cout << "Please input course number: ";
            cin >> courseNum;

            //call findCourse first to find and store course with courseNum that matches user input
            foundCourse = findCourse(courses, courseNum);

            //if the course retrieved is not empty call the full print function for that course
            if (foundCourse.getCourseNum() != "") {
                foundCourse.printCourse();
            }

            //if the course retrieved is blank then let user know course could not be founc
            else {
                cout << "Course could not be found!" << endl;
            }
            break;

        case 4:

            //Set done to true to exit menu
            cout << "Thank you for useing the course planner. Have a great day!" << endl;
            done = true;
            break;

        //If user does not enter valid option let them know and return to beggining of menu
        default:
            cout << "Please choose from the available options." << endl;

        }
    }

    //If no issues during menu operation return 0
    return 0;
}


//Main function to initiate system
int main()
{
    cout << "Welcome to the course planner!" << endl << endl;
    
    // run menu and check if it exits with an error
    if (menu() == -1) {

        //if it exits with an error let user know
        cout << "Program closing due to file error!" << endl;
    }
}