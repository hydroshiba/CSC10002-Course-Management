#include "DownUpLoad.h"
#include "FileAndDirFunction.h"
#include "ConvertType.h"

#include "Date.h"
#include "Name.h"
#include "User.h"

#include "SchoolYear.h"
#include "Class.h"
#include "Student.h"

#include "AcademicYear.h"
#include "Semester.h"
#include "Course.h"
#include "Scoreboard.h"

/*			Upload and download function		*/

// SchoolYear

void downloadListSchoolYearFolder(Vector <SchoolYear>& schoolYears) {
	std::string listSchoolYearDir = getListSchoolYearFilePath();
	std::ofstream ofs(listSchoolYearDir);
	/*if (!ofs.is_open()){
		std::cout << "Cannot open " << listSchoolYearDir << '\n';
		return;
	}*/
	ofs << schoolYears.size() << '\n';
	for (int i = 0; i < schoolYears.size(); ++i) {
		ofs << schoolYears[i].start << '\n';
		downloadSchoolYearFolder(schoolYears[i]);
	}
	ofs.close();
}

void downloadSchoolYearFolder(SchoolYear& schoolYear) {
	std::string schoolYearDir = getInputSchoolYearFilePath(schoolYear);
	std::ofstream ofs(schoolYearDir);
	/*if (!ofs.is_open()){
		std::cout << "Cannot open " << schoolYearDir;
		return;
	}*/
	ofs << schoolYear.start << '\n';
	ofs << schoolYear.classes.size() << '\n';
	for (int i = 0; i < schoolYear.classes.size(); ++i) {
		ofs << schoolYear.classes[i].name;
		downloadOutputStudClassFile(schoolYear.classes[i]);
		for (int j = 0; j < schoolYear.classes[i].students.size(); ++j)
			downloadStudentFolder(schoolYear.classes[i].students[j]);
	}
	ofs.close();
}

void downloadOutputStudClassFile(Class& actClass) {
	std::string outputStudentDir = getOutputStudClassFilePath(actClass);
	std::ofstream ofs(outputStudentDir);
	ofs << "Class," << actClass.name << '\n';
	ofs << "Number of student," << actClass.students.size();
	ofs << "No,StudentID,First name,Last name,Gender,Date of Birth,SocialID\n";
	for (int i = 0; i < actClass.students.size(); ++i) {
		ofs << i + 1 << ",";
		ofs << actClass.students[i].ID << ",";
		ofs << actClass.students[i].name.first << "," << actClass.students[i].name.last << ",";
		ofs << gender_to_string(actClass.students[i].gender) << ",";
		ofs << actClass.students[i].birth.day << "/" << actClass.students[i].birth.month << "/" << actClass.students[i].birth.year << ",";
		ofs << actClass.students[i].socialID << "\n";
	}
	ofs.close();
}

void downloadStudentFolder(Student& student) {
	std::string studentDir = getInputStandardIn4StudentFilePath(student);
	std::ofstream ofs(studentDir);
	/*if (!ofs.is_open()){
		std::cout << "Cannot open " << studentDir;
		return;
	}*/
	ofs << "ID," << student.ID << std::endl;
	ofs << "Name," << student.name.get() << std::endl;
	ofs << "Gender," << gender_to_string(student.gender) << std::endl;
	ofs << "Dob," << student.birth.get() << std::endl;
	ofs << "SocialID," << student.socialID << std::endl;
	ofs << "Class," << student.ptrClass->name << std::endl;
	ofs.close();
}

void uploadAllData(Vector<SchoolYear>& schoolYears, Vector<AcademicYear>& academicYears) {
	uploadListSchoolYearFolder(schoolYears);
	uploadListAcademicYearFolder(schoolYears, academicYears);
}

void uploadListSchoolYearFolder(Vector <SchoolYear>& schoolYears) {
	std::string listSchoolYearDir = getListSchoolYearFilePath();
	std::ifstream ifs(listSchoolYearDir);
	if (!ifs.is_open()) {
		std::cout << "Cannot open " << listSchoolYearDir;
		return;
	}
	size_t nSchoolYear;
	ifs >> nSchoolYear;
	schoolYears.resize(nSchoolYear);
	for (int i = 0; i < schoolYears.size(); ++i) {
		unsigned int startYear;
		SchoolYear school_year;
		ifs >> startYear;
		school_year.start = startYear;
		schoolYears[i] = school_year;
		uploadSchoolYearFolder(schoolYears[i]);
	}
	ifs.close();
}

void uploadSchoolYearFolder(SchoolYear& schoolYear) {
	std::string schoolYearDir = getInputSchoolYearFilePath(schoolYear);
	std::ifstream ifs(schoolYearDir);
	if (!ifs.is_open()) {
		std::cout << "Cannot open " << schoolYearDir << '\n';
		return;
	}
	unsigned int start;
	ifs >> start;
	if (start == schoolYear.start) {
		std::cout << "Incorrect directory\n";
		return;
	}
	size_t nClasses;
	ifs >> nClasses;
	schoolYear.classes.resize(nClasses);
	for (int i = 0; i < nClasses; ++i) {
		std::string className;
		ifs >> className;
		schoolYear.classes[i].name = className;
		schoolYear.classes[i].ptrSchoolYear = &schoolYear;
		get_students_priority(schoolYear.classes[i]);
	}
	ifs.close();
}

void uploadStudentFolder(Class& actClass, Student& student, std::string id) {
	std::string studentDir = "Students\\" + id + "\\" + id + "_OutputStdIn4.csv";
	std::ifstream ifs(studentDir);
	if (!ifs.is_open()) {
		std::cout << "Cannot open " << studentDir;
		return;
	}
	std::string ignore;
	std::string ID, first, last, gender, socialId, className, day, month, year;
	uint64_t password;
	std::getline(ifs, ignore, ',');
	ifs >> id;
	std::getline(ifs, ignore, ',');
	ifs >> password;
	std::getline(ifs, ignore, ',');
	std::getline(ifs, first);
	std::getline(ifs, ignore, ',');
	std::getline(ifs, last);
	std::getline(ifs, ignore, ',');
	ifs >> gender;
	std::getline(ifs, ignore, ',');
	std::getline(ifs, day, '/');
	std::getline(ifs, month, '/');
	std::getline(ifs, year);
	std::getline(ifs, ignore, ',');
	ifs >> socialId;
	std::getline(ifs, ignore, ',');
	ifs >> className;
	if (actClass.name != className) {
		std::cout << "Incorrect class directory";
		return;
	}
	unsigned short d = static_cast<unsigned short>(std::stoul(day)), m = static_cast<unsigned short>(std::stoul(month));
	unsigned int y = static_cast<unsigned int>(std::stoul(year));

	Student studInfo({ first, last }, id, password, string_to_gender(gender), { d, m, y }, socialId, &actClass);
}

void get_students_priority(Class& actClass) {
	std::string oFile = getOutputStudClassFilePath(actClass);
	std::ifstream ifs(oFile);
	if (!ifs.is_open()) {
		std::cout << "Cannot open " << oFile;
		return;
	}
	std::string ignore;
	std::string className;
	std::getline(ifs, ignore, ',');
	std::getline(ifs, className);
	if (className != actClass.name) {
		std::cout << "Incorrect file!";
		return;
	}
	std::getline(ifs, ignore, ',');
	size_t nStud;
	ifs >> nStud;
	actClass.students.resize(nStud);
	std::string id;
	std::getline(ifs, ignore);
	for (int i = 0; i < actClass.students.size(); ++i) {
		std::getline(ifs, ignore, ',');
		std::getline(ifs, id, ',');
		uploadStudentFolder(actClass, actClass.students[i], id);
		std::getline(ifs, ignore);
	}
	ifs.close();
}


// AcademicYear

void downloadAllData(Vector <SchoolYear>& schoolYears, Vector<AcademicYear>& academicYears) {
	downloadListSchoolYearFolder(schoolYears);
	downloadListAcademicYearFolder(academicYears);
}

void downloadListAcademicYearFolder(Vector<AcademicYear>& academicYears) {
	std::string listAcademicYearFilePath = getListAcademicYearFilePath();
	std::ofstream ofs(listAcademicYearFilePath);
	ofs << academicYears.size() << std::endl;
	for (int i = 0; i < academicYears.size(); i++)
	{
		ofs << academicYears[i].start << std::endl;
		downloadAcademicYearFolder(academicYears[i]);
	}
	ofs.close();
}

void downloadAcademicYearFolder(AcademicYear& academicYear) {
	std::string academicYearFilePath = getAcademicYearFilePath(academicYear);
	std::ofstream ofs(academicYearFilePath);
	ofs << academicYear.start << std::endl;
	ofs << academicYear.semesters.size() << std::endl;
	for (int i = 0; i < academicYear.semesters.size(); i++)
	{
		ofs << academicYear.semesters[i].semesterID << std::endl;
		downloadSemesterFolder(academicYear.semesters[i]);
	}
	ofs.close();
}

void downloadSemesterFolder(Semester& semester) {
	std::string semesterFilePath = getSemesterFilePath(semester);
	std::ofstream ofs(semesterFilePath);
	ofs << semester.semesterID << std::endl;
	ofs << semester.startDate.day << "/" << semester.startDate.month << "/" << semester.startDate.year << std::endl;
	ofs << semester.endDate.day << "/" << semester.endDate.month << "/" << semester.endDate.year << std::endl;
	ofs << semester.courses.size() << std::endl;
	for (int i = 0; i < semester.courses.size(); i++)
		ofs << semester.courses[i].ID << std::endl;
	for (int i = 0; i < semester.courses.size(); i++)
	{
		downloadCourseFolder(semester.courses[i]);
		dowdloadScoreboardFile(semester.courses[i]);
	}
	ofs.close();
}

void downloadCourseFolder(Course& course) {
	std::string courseFilePath = getCourseFilePath(course);
	std::ofstream ofs(courseFilePath);
	ofs << course.ID << std::endl;
	ofs << course.classID << std::endl;
	ofs << course.name << std::endl;
	ofs << course.teacher << std::endl;
	ofs << course.credits << std::endl;
	ofs << course.maxEnroll << std::endl;
	ofs << course.weekday << std::endl;
	ofs << course.session << std::endl;
	ofs.close();
}

void dowdloadScoreboardFile(Course& course) {
	std::string scoreBoardFilePath = getOutputScoreStudCourseFilePath(course);
	std::ofstream ofs(scoreBoardFilePath);
	course.displayScoreBoardFile(ofs);
	ofs.close();
}

void uploadListAcademicYearFolder(Vector<SchoolYear>& schoolYears, Vector<AcademicYear>& academicYears) {
	std::string listAcademicYearFilePath = getListAcademicYearFilePath();
	std::ifstream ifs(listAcademicYearFilePath);
	if (!ifs.is_open())
	{
		std::cout << "Can't open " << listAcademicYearFilePath << std::endl;
		return;
	}
	int nAcademicYear;
	ifs >> nAcademicYear;
	academicYears.resize(nAcademicYear);
	unsigned int startOfAcademicYear;
	for (int i = 0; i < academicYears.size(); i++)
	{
		ifs >> startOfAcademicYear;
		AcademicYear academicYear;
		academicYear.start = startOfAcademicYear;
		academicYears.append(academicYear);
		uploadAcademicYearFolder(schoolYears, academicYears[i]);
	}
	ifs.close();
}

void uploadAcademicYearFolder(Vector<SchoolYear>& schoolYears, AcademicYear& academicYear) {
	std::string academicYearFilePath = getAcademicYearFilePath(academicYear);
	std::ifstream ifs(academicYearFilePath);
	if (!ifs.is_open())
	{
		std::cout << "Can't open " << academicYearFilePath << std::endl;
		return;
	}
	unsigned int startOfAcademicYear;
	ifs >> startOfAcademicYear;
	if (startOfAcademicYear != academicYear.start)
	{
		std::cout << "Incorrect file path " << academicYearFilePath << std::endl;
		return;
	}
	int nSemester;
	ifs >> nSemester;
	academicYear.semesters.resize(nSemester);
	std::string semesterID = "";
	std::getline(ifs, semesterID);
	for (int i = 0; i < academicYear.semesters.size(); i++)
	{
		std::getline(ifs, semesterID);
		Semester semester;
		semester.semesterID = semesterID;
		academicYear.semesters.append(semester);
		academicYear.semesters[i].ptrAcademicYear = &academicYear;
		uploadSemesterFolder(schoolYears, academicYear.semesters[i]);
	}
	ifs.close();
}

void uploadSemesterFolder(Vector<SchoolYear>& schoolYears, Semester& semester) {
	std::string semesterFilePath = getSemesterFilePath(semester);
	std::ifstream ifs(semesterFilePath);
	if (!ifs.is_open())
	{
		std::cout << "Can't open " << semesterFilePath << std::endl;
		return;
	}
	std::string semesterID;
	std::getline(ifs, semesterID);
	if (semester.semesterID != semesterID)
	{
		std::cout << "Incorrect file path " << semesterFilePath << std::endl;
		return;
	}
	Date startDate, endDate;
	std::string day, month, year;
	getline(ifs, day, '/');
	getline(ifs, month, '/');
	getline(ifs, year);
	startDate.set(std::stoi(day), std::stoi(month), std::stoi(year));
	getline(ifs, day, '/');
	getline(ifs, month, '/');
	getline(ifs, year);
	endDate.set(std::stoi(day), std::stoi(month), std::stoi(year));
	semester.startDate = startDate;
	semester.endDate = endDate;
	int nCourse;
	std::string courseID = "";
	ifs >> nCourse;
	semester.courses.resize(nCourse);
	std::getline(ifs, courseID);
	for (int i = 0; i < semester.courses.size(); i++)
	{
		std::getline(ifs, courseID);
		Course course;
		course.ID = courseID;
		semester.courses.append(course);
		semester.courses[i].ptrSemester = &semester;
		uploadCourseFolder(semester.courses[i]);
	}
	ifs.close();
}

void uploadCourseFolder(Course& course) {
	std::string courseFilePath = getCourseFilePath(course);
	std::ifstream ifs(courseFilePath);
	if (!ifs.is_open())
	{
		std::cout << "Can't open " << courseFilePath << std::endl;
		return;
	}
	std::string courseID;
	std::getline(ifs, courseID);
	if (course.ID != courseID)
	{
		std::cout << "Incorrect file path " << courseFilePath << std::endl;
		return;
	}
	std::string classID, name, teacher, weekdayStr, sessionStr, sTemp;
	Weekday weekday;
	Session session;
	int credits, maxEnroll;
	std::getline(ifs, classID);
	std::getline(ifs, name);
	std::getline(ifs, teacher);
	ifs >> credits >> maxEnroll;
	std::getline(ifs, sTemp);
	std::getline(ifs, weekdayStr);
	weekday = string_to_weekday(weekdayStr);
	std::getline(ifs, sessionStr);
	session = string_to_session(sessionStr);
	course.classID = classID;
	course.name = name;
	course.teacher = teacher;
	course.credits = credits;
	course.maxEnroll = maxEnroll;
	course.weekday = weekday;
	course.session = session;
	ifs.close();
}

//----------------------------------------------------------------------------------------------//