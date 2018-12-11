#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "11";
	static const char MONTH[] = "12";
	static const char YEAR[] = "2018";
	static const char UBUNTU_VERSION_STYLE[] =  "18.12";
	
	//Software Status
	static const char STATUS[] =  "Release";
	static const char STATUS_SHORT[] =  "r";
	
	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 0;
	static const long BUILD  = 847;
	static const long REVISION  = 4771;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 2454;
	#define RC_FILEVERSION 1,0,847,4771
	#define RC_FILEVERSION_STRING "1, 0, 847, 4771\0"
	static const char FULLVERSION_STRING [] = "1.0.847.4771";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 0;
	

}
#endif //VERSION_H
