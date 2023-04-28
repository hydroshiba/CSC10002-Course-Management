#ifndef REGISTRY_H
#define REGISTRY_H

#include "Scene.h"

class Registry {
public:
	Scene* blank;
	Scene* login;
	Scene* courseScene;
	Scene* editScene;
	Scene* editSchoolYearScene;
	Scene* studentScene;
	Scene* staffScene;
	Scene* addStudentScene;
	Registry(): 
	blank(nullptr), 
	login(nullptr), 
	studentScene(nullptr),
	staffScene(nullptr),
	addStudentScene(nullptr),
	courseScene(nullptr), 
	editScene(nullptr), 
	editSchoolYearScene(nullptr) {}
};

extern Registry registry;

#endif