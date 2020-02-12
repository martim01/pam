#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "12";
	static const char MONTH[] = "02";
	static const char YEAR[] = "2020";
	static const char UBUNTU_VERSION_STYLE[] =  "20.02";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 0;
	static const long BUILD  = 19;
	static const long REVISION  = 117;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 20;
	#define RC_FILEVERSION 1,0,19,117
	#define RC_FILEVERSION_STRING "1, 0, 19, 117\0"
	static const char FULLVERSION_STRING [] = "1.0.19.117";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 0;
	

}
#endif //VERSION_H
