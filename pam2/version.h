#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "14";
	static const char MONTH[] = "07";
	static const char YEAR[] = "2021";
	static const char UBUNTU_VERSION_STYLE[] =  "21.07";
	
	//Software Status
	static const char STATUS[] =  "Release";
	static const char STATUS_SHORT[] =  "r";
	
	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 1;
	static const long BUILD  = 43;
	static const long REVISION  = 234;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 3540;
	#define RC_FILEVERSION 1,1,43,234
	#define RC_FILEVERSION_STRING "1, 1, 43, 234\0"
	static const char FULLVERSION_STRING [] = "1.1.43.234";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 0;
	

}
#endif //VERSION_H
