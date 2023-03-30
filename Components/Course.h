#ifndef COURSE_H
#define COURSE_H

#include "Components.h"

#include "Scoreboard.h"
#include "Student.h"
#include "Semester.h"
struct Course {
    std::string ID, classID;
    std::string name, teacher;

    int credits, maxEnroll = 50;
    Weekday weekday;
    Session session;

    Semester* ptrSemester = nullptr;
    Vector<Scoreboard*> scoreboards;

    void addStudent(Student& student);  
    // already set double linke between Course & Score, Score & Student
    void removeStudent(Student& student);

    Scoreboard* getScoreboard(Student& student);
    
    void displayInfo(std::ostream& outDev);

    void displayInfoScreen();

    void displayInfoFile(std::ostream& ofs);

    void importScoreBoards(std::ifstream& ifs) {
        const char delimiter = ',';
        const int nSkipLine = 8;
        const int nSkipInfo = 3;
        std::string s = "";
        for (int i = 0; i < nSkipLine; i++)
            std::getline(ifs, s);
        int No = 0;
        while (!ifs.eof())
        {
            float midterm, final, other, total;
            for (int i = 0; i < nSkipInfo; i++)
                std::getline(ifs, s, delimiter);
            std::getline(ifs, s, delimiter);
            midterm = std::stof(s);
            std::getline(ifs, s, delimiter);
            final = std::stof(s);
            std::getline(ifs, s, delimiter);
            other = std::stof(s);
            std::getline(ifs, s, delimiter);
            total = std::stof(s);
            scoreboards[No]->setScore(midterm, final, other, total);
        }
    }

    void displayScoreBoards(std::ostream& outDev = std::cout) {
        if (&outDev == &std::cout)
            displayScoreBoardScreen();
        else
            displayScoreBoardFile(outDev);
    }

    void displayScoreBoardFile(std::ostream& outDev){
        displayInfoFile(outDev);
        outDev << "No," << "ID," << "Fullname," << "Midterm," << "Final," << "Other," << "Total";
        outDev << std::endl;
        for (int i = 0; i < scoreboards.size(); i++)
        {
            outDev << i + 1 << "," << scoreboards[i]->ptrStudent->ID << "," << scoreboards[i]->ptrStudent->name.get() << ",";
            outDev << scoreboards[i]->midterm << "," << scoreboards[i]->final << "," << scoreboards[i]->other << ",";
            outDev << scoreboards[i]->total << std::endl;
        }
    }

    void displayScoreBoardScreen() {
        displayInfoScreen();
        const int noMaxLength = 3;
        const int idMaxLength = 10;
        const int fullNameMaxLength = 20;
        const int midtermMaxLength = 10;
        const int finalMaxLength = 8;
        const int totalMaxLength = 8;
        const int otherMaxLength = 8;
        std::cout << std::setw(noMaxLength) << std::left << "No";
        std::cout << std::setw(idMaxLength) << std::left << "ID";
        std::cout << std::setw(fullNameMaxLength) << std::left << "Fullname";
        std::cout << std::setw(midtermMaxLength) << std::left << "Midterm";
        std::cout << std::setw(finalMaxLength) << std::left << "Final";
        std::cout << std::setw(otherMaxLength) << std::left << "Other";
        std::cout << std::setw(totalMaxLength) << std::left << "Total";
        std::cout << std::endl;
        for (int i = 0; i < scoreboards.size(); i++)
        {
            std::cout << std::setw(noMaxLength) << std::left << i + 1;
            std::cout << std::setw(idMaxLength) << std::left << scoreboards[i]->ptrStudent->ID;
            std::cout << std::setw(fullNameMaxLength) << std::left << scoreboards[i]->ptrStudent->name.get();
            std::cout << std::setw(midtermMaxLength) << std::left << scoreboards[i]->midterm;
            std::cout << std::setw(finalMaxLength) << std::left << scoreboards[i]->final;
            std::cout << std::setw(otherMaxLength) << std::left << scoreboards[i]->other;
            std::cout << std::setw(totalMaxLength) << std::left << scoreboards[i]->total;
            std::cout << std::endl;
        }
    }
};

bool operator==(const Course& courseA, const Course& courseB);

#endif // !COURSE_H