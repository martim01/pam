#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "18";
	static const char MONTH[] = "05";
	static const char YEAR[] = "2020";
	static const char UBUNTU_VERSION_STYLE[] =  "20.05";
	
	//Software Status
	static const char STATUS[] =  "Release";
	static const char STATUS_SHORT[] =  "r";
	
	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 0;
	static const long BUILD  = 367;
	static const long REVISION  = 2012;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 392;
	#define RC_FILEVERSION 1,0,367,2012
	#define RC_FILEVERSION_STRING "1, 0, 367, 2012\0"
	static const char FULLVERSION_STRING [] = "1.0.367.2012";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 0;
	

}
#endif //VERSION_H
