#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "02";
	static const char MONTH[] = "08";
	static const char YEAR[] = "2019";
	static const char UBUNTU_VERSION_STYLE[] =  "19.08";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 0;
	static const long BUILD  = 31;
	static const long REVISION  = 185;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 45;
	#define RC_FILEVERSION 1,0,31,185
	#define RC_FILEVERSION_STRING "1, 0, 31, 185\0"
	static const char FULLVERSION_STRING [] = "1.0.31.185";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 0;
	

}
#endif //VERSION_H
