#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "08";
	static const char MONTH[] = "10";
	static const char YEAR[] = "2018";
	static const char UBUNTU_VERSION_STYLE[] =  "18.10";
	
	//Software Status
	static const char STATUS[] =  "Release";
	static const char STATUS_SHORT[] =  "r";
	
	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 0;
	static const long BUILD  = 680;
	static const long REVISION  = 3897;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 1972;
	#define RC_FILEVERSION 1,0,680,3897
	#define RC_FILEVERSION_STRING "1, 0, 680, 3897\0"
	static const char FULLVERSION_STRING [] = "1.0.680.3897";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 0;
	

}
#endif //VERSION_H
