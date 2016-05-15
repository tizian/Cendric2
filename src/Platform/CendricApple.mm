#include "Platform/CendricApple.h"

#ifdef __APPLE__

#import <Foundation/Foundation.h>
#include <iostream>

std::string getDocumentPath() {
	std::string resultPath = "";

	NSArray* pathsArray = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES);
	NSString* path = [pathsArray firstObject];
	resultPath = std::string([path UTF8String]) + std::string("/Cendric/");

	NSString* savesPath = [path stringByAppendingString:@"/Cendric/saves"];
	NSString* screenshotsPath = [path stringByAppendingString:@"/Cendric/screenshots"];

	[[NSFileManager defaultManager] createDirectoryAtPath:savesPath
							  withIntermediateDirectories: YES
											   attributes: nil
													error: nil];

	[[NSFileManager defaultManager] createDirectoryAtPath:screenshotsPath
							  withIntermediateDirectories: YES
											   attributes: nil
													error: nil];

	[pathsArray release];
	[path release];
	[savesPath release];
	[screenshotsPath release];

	return resultPath;
}

#endif