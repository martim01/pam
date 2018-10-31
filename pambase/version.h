#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "31";
	static const char MONTH[] = "10";
	static const char YEAR[] = "2018";
	static const char UBUNTU_VERSION_STYLE[] =  "18.10";
	
	//Software Status
	static const char STATUS[] =  "Release";
	static const char STATUS_SHORT[] =  "r";
	
	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 0;
	static const long BUILD  = 196;
	static const long REVISION  = 1067;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 213;
	#define RC_FILEVERSION 1,0,196,1067
	#define RC_FILEVERSION_STRING "1, 0, 196, 1067\0"
	static const char FULLVERSION_STRING [] = "1.0.196.1067";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 0;
	

}
#endif //VERSION_H
