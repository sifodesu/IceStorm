#pragma once
#include "Generic_Object.h"
#include <vector>
#include <ostream>
#include "Paths.h"
#include "Engine_Manager.h"
#include "Textures_Manager.h"
#include <vector>
using namespace std;

class Objects_Manager {
public:
	Objects_Manager() {
		Init();
	}

	static void Init();

	static GObject* findObject(string target) {
		for (auto i = objects.begin(); i != objects.end(); ++i) {
			if (!(*i)->target.compare(target))
				return *i;
		}
		return NULL;
	}

	static bool identify(string target, string wanted);

	static vector<GObject*> objects;
};

/*
Data structure of the object database :
ID: <int ID>
target: <string target>
targetnames: <strings targetnames>
texture ID: <int texture id>
type: <string type>
flags: <strings flags>
crect: <h, w>
content: <string content>

Example :

ID: 0
target: wall_test
targetnames: wall_test
texture: pannel.png
type: BUTTON
flags: SOLID
crect: 20, 20
content: Vive la bite !
*/