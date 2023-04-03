#include "Components.h"

#include "AcademicYear.h"
#include "Components.h"
#include "Course.h"
#include "SchoolYear.h"
#include "Class.h"
#include "Name.h"
#include "Semester.h"
#include "Scoreboard.h"

/* Task functions */

// Add a new 1st-year & delete old 4th-year
void addNewSchoolYear(Vector<SchoolYear>& yearList){
    unsigned int startYear;
    std::cout << "Enter the school year: "; std::cin >> startYear;
    SchoolYear newYear = {startYear};
    yearList.append(newYear);
    if (yearList.size() > 4)
        yearList.remove(yearList.begin());
}

// Add several classes for 1st year
void addClasses (Vector<SchoolYear>& yearList){
    Class newClass;
    do {
        std::cout << "Enter class name (enter 0 when done): ";
        std::cin >> newClass.name;
        if (newClass.name == "0") break;
        yearList[yearList.size()-1].addClass(newClass);
    } while (newClass.name != "0");
}

// Add students into a specific class
void addStudToClass(Class &actClass,const std::string inFile){
    Student newStud;
    std::ifstream inF;
    inF.open("Data/"+inFile);
    if (!inF.is_open()){
        std::cout << "Cannot open file!\n";
        return ;
    }
    std::string ignore;
    getline(inF, ignore);
    while (!inF.eof()){
        newStud.setInfoToClass(inF);
        actClass.addStudent(newStud);
        newStud.myClass = &actClass;
    }
    inF.close();
}

// Add a new academic year
void addNewAcademicYear(AcademicYear &newYear){
    unsigned int startyear;
    std::cout << "Enter a new year: ";
    std::cin >> startyear;
    newYear = {startyear};
}

// Add a semester to an academic year

void addSemester(AcademicYear &newYear){
    unsigned short day, month;
    unsigned int year;
    std::string id;
    std::cout << "Enter semester id: "; std::cin >> id;
    Date startDate;
    std::cout << "Enter start date for the semester:\n";
    std::cin >> day >> month >> year;
    startDate.set(day, month, year);

    Date endDate;
    std::cout << "Enter end date for the semester:\n";
    std::cin >> day >> month >> year;
    endDate.set(day, month, year);

    Semester newSem;
    newSem.semesterID = id;
    newSem.startDate = startDate;
    newSem.endDate = endDate;
    newSem.year = &newYear;

    newYear.addSemester(newSem);
}

// Add a new course
void addNewCourse(Semester &semester){
    Course newCourse;
    std::string ID, classID, name, teacher;
    int cre, maxEn;
    int day, ss;

    std::cout << "Enter Course ID: "; std::cin >> newCourse.ID;
    std::cout << "Enter Course Name: "; std::cin >> newCourse.name;
    std::cout << "Enter Teacher's Name: "; std::cin >> newCourse.teacher;
    std::cout << "Enter Class ID: "; std::cin >> newCourse.classID;
    std::cout << "Enter Number of Credits: "; std::cin >> newCourse.credits;
    std::cout << "Enter Maximun students enrolled: "; std::cin >> newCourse.maxEnroll;

    std::cout << "Day of week: "; std::cin >> day;
    std::cout << "Session performed: "; std::cin >> ss;

    newCourse.weekday = static_cast<Weekday>(day);
    newCourse.session = static_cast<Session>(ss);
    newCourse.ptrSemester = &semester;

    semester.addCourse(newCourse);
}

// Get students' info to course
void getStudentToCourse(Vector<SchoolYear> years, Course &course, const std::string stud_input_file){
    std::ifstream inF;
    inF.open("Data/"+stud_input_file);
    if (!inF.is_open()){
        std::cout << "Cannot open file!\n";
        return;
    }
    std::string ignore;
    getline(inF, ignore);
    std::string className;
    Class actClass;
	Class* ptrClass = nullptr;
    Student student;
	Student* ptrStudent = nullptr;
    while (!inF.eof()){
        student.setInfoToCourseCSV(inF, className);
        actClass.name = className;
        for (int i = 0; i<4; ++i)
			if (years[i].classes.find(actClass))
			{
				ptrClass = years[i].classes.find(actClass);
				break;
			} 
		if (ptrClass != nullptr)
			ptrStudent = ptrClass->students.find(student);
		if (ptrStudent != nullptr)
			course.addStudent(*ptrStudent);
    }
    inF.close();
}

// View list of courses
void viewCourses(Semester sem, std::ostream& outDev){
    std::cout << "List of courses: ";
    for (size_t i = 0; i<sem.courses.size(); ++i)
        sem.courses[i].displayInfo(outDev);
}

// Update course informations
void updateCourse(Course &course){
    int option;
    do {
        std::cout << "Update course informations:\n";
        std::cout << "0. Back\n";
        std::cout << "1. Change course id\n";
        std::cout << "2. Change teacher's name\n";
        std::cout << "3. Change course name\n";
        std::cout << "4. Change class id\n";
        std::cout << "5. Change number of credits\n";
        std::cout << "6. Change max students enrolled\n";
        std::cout << "7. Change session\n";
		std::cout << "Choose the following option 0-7: ";
		std::cin >> option;
		while (option < 0 || option > 7)
		{
			std::cout << "Invalid option, pls try again: ";
			std::cin >> option;
		}
        switch (option){
            case 1: 
                std::cout << "Enter Course ID: "; std::cin >> course.ID;
                break;
            case 2: 
                std::cout << "Enter Teacher's Name: "; std::cin >> course.teacher;
                break;
            case 3: 
                std::cout << "Enter Course Name: "; std::cin >> course.name; 
                break;
            case 4: 
                std::cout << "Enter Class ID: "; std::cin >> course.classID;
                break;
            case 5:
                std::cout << "Enter Number of Credits: "; std::cin >> course.credits;
                break;
            case 6:
                std::cout << "Enter Maximun students enrolled: "; std::cin >> course.maxEnroll;
                break;
            case 7: 
                int in, day, ss;
                std::cout << "0. Change weekday\n";
                std::cout << "1. Change session\n";
                std::cin >> in;
                if (!in){
                    std::cout << "Enter weekday: "; std::cin >> day;
                    course.weekday = static_cast<Weekday>(day); 
                }
                else{
                    std::cout << "Enter Session performed: "; std::cin >> ss;
                    course.session = static_cast<Session>(ss);
                }
                break;
            default: 
				std::cout << "Exit update course information!" << std::endl;
				break;
        }
    }while (option);
}

// Add student to course
void addNewStudToCourse(Vector <SchoolYear>& yearlist, Course &course, std::istream& inDev){
    Student newStud;
    std::string className;
    Class myClass;
	Class* ptrClass = nullptr;
	Student* ptrStudent = nullptr;
    if (&inDev == &std::cin)
        newStud.setInfoCourseConsole(className);
    else{
        std::ifstream ifs;
        std::string file;
        std::cout << "Enter file name: "; std::cin >> file;
        ifs.open(file);
        newStud.setInfoToCourseCSV(ifs, className);
        ifs.close();
    }
    myClass.name = className;
    for (int i = 0; i<4; ++i)
		if (yearlist[i].classes.find(myClass)) {
			ptrClass = yearlist[i].classes.find(myClass);
			break;
		}
	if (ptrClass != nullptr)
		ptrStudent = ptrClass->students.find(newStud);
	if (ptrStudent != nullptr)
		course.addStudent(*ptrStudent);
}

void removeStudFromCourse(Course &course, std::istream &inDev){
    Student student;
    std::string className;
	Student* pStud = nullptr;
    if (&inDev == &std::cin)
        student.setInfoCourseConsole(className);
    else{
        std::ifstream ifs;
        std::string file;
        std::cout << "Enter file name: "; std::cin >> file;
        ifs.open(file);
        student.setInfoToCourseCSV(ifs, className);
        ifs.close();
    }
    for (int i = 0; i<course.scoreboards.size(); ++i)
        if (*(course.scoreboards[i]->ptrStudent) == student){
            pStud = course.scoreboards[i]->ptrStudent;
            break;
        }
	if (pStud != nullptr)
		course.removeStudent(*pStud);
}

// 13. delete course in current semester 
void deleteCourse(Semester& semester, const std::string& courseID) {
	bool found = false;
	Course* course = nullptr;
	for (int i = 0; i < semester.courses.size(); i++)
		if (semester.courses[i].ID == courseID)
		{
			found = true;
			course = &semester.courses[i];
			break;
		}
	if (!found)
		std::cout << "Can't find a course with ID " << courseID;
	else
	{
		course->ptrSemester = nullptr;
		for (int i = 0; i < course->scoreboards.size(); i++)
		{
			Student* student = course->scoreboards[i]->ptrStudent;
			student->scoreboards.remove(course->scoreboards[i]); // delete link from student->scoreboard
			course->scoreboards[i]->ptrStudent = nullptr; // delete link sb->student
			course->scoreboards[i]->ptrCourse = nullptr; // delete link sb->course
			delete course->scoreboards[i]; // delete scoreboard
		}
		semester.removeCourse(*course); // delete course
		std::cout << "Course with ID " << courseID << " have been removed from semester!";
	}
	std::cout << std::endl;
}

// 14. view list of courses at current student
void viewListOfCoursesOfStudent(const Student& student, std::ostream& outDev) {
	outDev << "List of courses: ";
	for (int i = 0; i < student.scoreboards.size(); i++)
	{
		outDev << "No " << i + 1 << ": " << std::endl;
		student.scoreboards[i]->ptrCourse->displayInfoScreen();
		outDev << "--------------------------------------" << std::endl;
	}
}

// 15. view list of classes 
void viewListOfClasses(const SchoolYear& schoolYear, std::ostream& outDev) {
	outDev << "List of classes: " << std::endl;
	for (int i = 0; i < schoolYear.classes.size(); i++)
		outDev << i + 1 << ". " << schoolYear.classes[i].name << std::endl;
}

// 16. view list of students in a class
void viewListOfStudentsInClass(const Class& curClass, std::ostream& outDev) {
	outDev << "List of students in class " << curClass.name << ": " << std::endl;
	for (int i = 0; i < curClass.students.size(); i++)
	{
		outDev << i + 1 << ". " << curClass.students[i].ID << " - " << curClass.students[i].name.get();
		outDev << std::endl;
	}
}

// 17. view list of course
void viewListOfCoursesInSemester(const Semester& semester, std::ostream& outDev) {
	outDev << "List of course in this semester: " << std::endl;
	outDev << "No  " << "CourseID   " << "ClassID   " << std::endl;
	for (int i = 0; i < semester.courses.size(); i++)
	{
		outDev << "No " << i + 1 << ": " << std::endl;
		semester.courses[i].displayInfo(outDev);
	}
}

// 18. view list of students in course
void viewListOfStudentsInCourse(const Course& course, std::ostream& outDev) {
	outDev << "List of students in course " << course.ID << " - " << course.name << ": " << std::endl;
	for (int i = 0; i < course.scoreboards.size(); i++)
	{
		Student* student = course.scoreboards[i]->ptrStudent;
		outDev << i + 1 << ". " << student->ID << " - " << student->name.get() << std::endl;
	}
}

// 19. export list of students in course to csv file
void exportListOfStudent(Course& course, const std::string& fileName) {
	std::ofstream ofs(fileName, std::ios::out);
	course.displayInfo(ofs);
	ofs << "No,ID,Fullname" << std::endl;
	for (int i = 0; i < course.scoreboards.size(); i++)
	{
		Student* student = course.scoreboards[i]->ptrStudent;
		ofs << i + 1 << "," << student->ID << "," << student->name.get() << std::endl;
	}
	ofs.close();
}

// 20. import scoreboard of course
void importScoreBoardOfCourse(Course& course) {
	std::cout << "Input filename: ";
	std::string fileName;
	std::getline(std::cin, fileName, '\n');
	fileName = "Data\\" + fileName;
	std::ifstream ifs(fileName);
	if (!ifs.is_open())
	{
		std::cout << "Can't find file with that name, pls try again!" << std::endl;
		return;
	}
	course.importScoreBoards(ifs);
	std::cout << "Complete importion!" << std::endl;
	ifs.close();
}

// 21. view scoreboard of course
void viewScoreBoardOfCourse(Course& course) {
	std::cout << "1. View on screen" << std::endl;
	std::cout << "2. View on file" << std::endl;
	std::cout << "Choose one of following options to view: ";
	int option;
	std::cin >> option;
	while (option <= 0 || option > 2)
	{
		std::cout << "Invalid option! Pls input 1 or 2: ";
		std::cin >> option;
	}
	switch (option)
	{
	case 1:
	{
		course.displayScoreBoardScreen();
		break;
	}
	case 2:
	{
		std::cout << "Input filename: ";
		std::string fileName;
		std::getline(std::cin, fileName, '\n');
		fileName = "Data\\" + fileName;
		std::ofstream ofs(fileName);
		if (!ofs.is_open())
		{
			std::cout << "Can't find file with that name, pls try again!" << std::endl;
			return;
		}
		course.displayScoreBoardFile(ofs);
		break;
	}
	}
}

// 22. Update student result
void updateStudentResult(Student& student) {
	viewListOfCoursesOfStudent(student);
	const int nCourse = student.scoreboards.size();
	std::cout << "Choose the course to update result (1" << " - " << nCourse << "): ";
	int option;
	std::cin >> option;
	while (option < 1 || option > nCourse)
	{
		std::cout << "Invalid option, pls try again: ";
		std::cin >> option;
	}
	Scoreboard* updateSB = student.scoreboards[option - 1];
	int updateOption;
	do
	{
		std::cout << "1. Midterm" << std::endl;
		std::cout << "2. Final" << std::endl;
		std::cout << "3. Other" << std::endl;
		std::cout << "4. Total" << std::endl;
		std::cout << "0. Exit" << std::endl;
		std::cout << "Choose the following option (0 - 4): ";
		std::cin >> updateOption;
		while (updateOption < 0 || updateOption > 4)
		{
			std::cout << "Invalid option, pls try again: ";
			std::cin >> updateOption;
		}
		if (updateOption == 0)
		{
			std::cout << "Finish update result!";
			return;
		}
		int updateScore;
		switch (updateOption)
		{
		case 1:
		{
			std::cout << "Input updated midterm score: ";
			std::cin >> updateScore;
			updateSB->midterm = updateScore;
			break;
		}
		case 2:
		{
			std::cout << "Input updated final score: ";
			std::cin >> updateScore;
			updateSB->final = updateScore;
			break;
		}
		case 3:
		{
			std::cout << "Input updated other score: ";
			std::cin >> updateScore;
			updateSB->other = updateScore;
			break;
		}
		case 4:
		{
			std::cout << "Input updated total score: ";
			std::cin >> updateScore;
			updateSB->total = updateScore;
			break;
		}
		}
	} while (updateOption != 0);
}

// 23. View scoreboard of class
void viewScoreboardOfClass(Class& curClass, const Semester& curSemester) {
	std::cout << "1. View on screen" << std::endl;
	std::cout << "2. View on file" << std::endl;
	std::cout << "Choose one of following options to view: ";
	int option;
	std::cin >> option;
	while (option <= 0 || option > 2)
	{
		std::cout << "Invalid option! Pls input 1 or 2: ";
		std::cin >> option;
	}
	switch (option)
	{
	case 1:
	{
		curClass.displayScoreboardScreen(curSemester);
		break;
	}
	case 2:
	{
		std::cout << "Input filename: ";
		std::string fileName;
		std::getline(std::cin, fileName, '\n');
		fileName = "Data\\" + fileName;
		std::ofstream ofs(fileName);
		if (!ofs.is_open())
		{
			std::cout << "Can't find file with that name, pls try again!" << std::endl;
			return;
		}
		curClass.displayScoreboardFile(curSemester, ofs);
		break;
	}
	}
}

// 24. view scoreboard of student
void viewScoreBoardOfStudent(Student& student) {
	const int noMaxLength = 3;
	const int idSemesterMaxLength = 10;
	const int idCourseNameMaxLength = 20;
	const int midtermMaxLength = 10;
	const int finalMaxLength = 8;
	const int totalMaxLength = 8;
	const int otherMaxLength = 8;
	std::cout << std::setw(noMaxLength) << std::left << "No";
	std::cout << std::setw(idSemesterMaxLength) << std::left << "Semester";
	std::cout << std::setw(idCourseNameMaxLength) << std::left << "Course";
	std::cout << std::setw(midtermMaxLength) << std::left << "Midterm";
	std::cout << std::setw(finalMaxLength) << std::left << "Final";
	std::cout << std::setw(otherMaxLength) << std::left << "Other";
	std::cout << std::setw(totalMaxLength) << std::left << "Total";
	std::cout << std::endl;
	for (int i = 0; i < student.scoreboards.size(); i++)
	{
		std::cout << std::setw(noMaxLength) << std::left << i + 1;
		std::cout << std::setw(idSemesterMaxLength) << std::left << student.scoreboards[i]->ptrCourse->ptrSemester->semesterID;
		std::cout << std::setw(idCourseNameMaxLength) << std::left << student.scoreboards[i]->ptrCourse->ID;
		std::cout << std::setw(midtermMaxLength) << std::left << student.scoreboards[i]->midterm;
		std::cout << std::setw(finalMaxLength) << std::left << student.scoreboards[i]->final;
		std::cout << std::setw(otherMaxLength) << std::left << student.scoreboards[i]->other;
		std::cout << std::setw(totalMaxLength) << std::left << student.scoreboards[i]->total;
		std::cout << std::endl;
	}
}

// Get file path in Data Folder
std::string getListSchoolYearFilePath() {
	return "Data\\SchoolYear\\ListSchoolYear.txt";
}

std::string getListAcademicYearFilePath() {
	return "Data\\AcademicYear\\ListAcademicYear.txt";
}

std::string getSchoolYearFolderPath(const SchoolYear& schoolyear) {
	return "Data\\SchoolYear\\HCMUS_" + std::to_string(schoolyear.start) + "\\";
}

std::string getInputSchoolYearFilePath(const SchoolYear& schoolyear) {
	std::string schoolYearFolderPath = getSchoolYearFolderPath(schoolyear);
	return schoolYearFolderPath + "HCMUS_" + std::to_string(schoolyear.start) + ".txt";
}

std::string getClassFolderPath(const Class& CLASS) {
	std::string schoolYearFolderPath = getSchoolYearFolderPath(*(CLASS.ptrSchoolYear));
	return schoolYearFolderPath + CLASS.name + "\\";
}

std::string getInputStudClassFilePath(const Class& CLASS) {
	std::string classFolderPath = getClassFolderPath(CLASS);
	return classFolderPath + CLASS.name + "_InputStud.csv";
}

std::string getInputScoreClassFilePath(const Class& CLASS) {
	std::string classFolderPath = getClassFolderPath(CLASS);
	return classFolderPath + CLASS.name + "_OutputScore.csv";
}

std::string getStudentFolderPath(const Student& student) {
	std::string classFolderPath = getClassFolderPath(*(student.myClass));
	return classFolderPath + "Students\\" + student.ID + "\\";
}

std::string getOutputScoreStudentFilePath(const Student& student) {
	std::string studentFolderPath = getStudentFolderPath(student);
	return studentFolderPath + student.ID + "_OutputScore.csv";
}

std::string getInputStandardIn4StudentFilePath(const Student& student) {
	std::string studentFolderPath = getStudentFolderPath(student);
	return studentFolderPath + student.ID + "_OutputStdIn4.csv";
}

std::string getAcademicYearFolderPath(const AcademicYear& academicYear) {
	return "Data\\AcademicYear\\" + std::to_string(academicYear.start) + "_" + std::to_string(academicYear.start + 1) + "\\";
}

std::string getAcademicYearFilePath(const AcademicYear& academicYear) {
	std::string academicYearFolderPath = getAcademicYearFolderPath(academicYear);
	return academicYearFolderPath + std::to_string(academicYear.start) + "_" + std::to_string(academicYear.start + 1) + ".txt";
}

std::string getSemesterFolderPath(const Semester& semester) {
	std::string academicYearFolderPath = getAcademicYearFolderPath(*(semester.year));
	return academicYearFolderPath + semester.semesterID + "\\";
}

std::string getSemesterFilePath(const Semester& semester) {
	std::string semesterFolderPath = getSemesterFolderPath(semester);
	return semesterFolderPath + semester.semesterID + ".txt";
}

std::string getCourseFolderPath(const Course& course) {
	std::string semesterFolderPath = getSemesterFolderPath(*(course.ptrSemester));
	return semesterFolderPath + course.ID + "\\";
}

std::string getCourseFilePath(const Course& course) {
	std::string courseFolderPath = getCourseFolderPath(course);
	return courseFolderPath + course.ID + ".txt";
}