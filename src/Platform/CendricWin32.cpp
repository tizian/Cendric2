#include "Platform/CendricWin32.h"

#ifdef _WIN32

#include <ShlObj.h>
#include <algorithm>

std::string getDocumentPath() {
	std::string resultPath = "";

	TCHAR path[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL | CSIDL_FLAG_CREATE, NULL, SHGFP_TYPE_CURRENT, path))) {
		char* folder = "\\My Games\\Cendric\\";
		strcat(path, folder);

		resultPath = path;

		int wchars_num = MultiByteToWideChar(CP_UTF8, 0, path, -1, NULL, 0);
		wchar_t* wstr = new wchar_t[wchars_num];
		MultiByteToWideChar(CP_UTF8, 0, path, -1, wstr, wchars_num);
		if (SHCreateDirectory(NULL, wstr) == ERROR_SUCCESS) {
			{
				TCHAR* saves = "saves";
				TCHAR pathCopy[MAX_PATH];
				strcpy(pathCopy, path);
				strcat(pathCopy, saves);

				int wchars_num_copy = MultiByteToWideChar(CP_UTF8, 0, pathCopy, -1, NULL, 0);
				wchar_t* wstr_copy = new wchar_t[wchars_num_copy];
				MultiByteToWideChar(CP_UTF8, 0, pathCopy, -1, wstr_copy, wchars_num_copy);
				SHCreateDirectory(NULL, wstr_copy);
				delete[] wstr_copy;
			}
			{
				TCHAR* screenshots = "screenshots";
				TCHAR pathCopy[MAX_PATH];
				strcpy(pathCopy, path);
				strcat(pathCopy, screenshots);

				int wchars_num_copy = MultiByteToWideChar(CP_UTF8, 0, pathCopy, -1, NULL, 0);
				wchar_t* wstr_copy = new wchar_t[wchars_num_copy];
				MultiByteToWideChar(CP_UTF8, 0, pathCopy, -1, wstr_copy, wchars_num_copy);
				SHCreateDirectory(NULL, wstr_copy);
				delete[] wstr_copy;
			}
		}
		delete[] wstr;
	}

	std::replace(resultPath.begin(), resultPath.end(), '\\', '/');

	return resultPath;
}

#endif