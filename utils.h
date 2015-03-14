#ifndef UTILS_H
#define UTILS_H

namespace utils {
	/*using namespace std;

	std::wstring s2ws(const std::string& s)
	{
		int len;
		int slength = (int)s.length() + 1;
		len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
		wchar_t* buf = new wchar_t[len];
		MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
		std::wstring r(buf);
		delete[] buf;
		return r;
	}


	
	void GetFilesInDirectory(const string &directory)
	{
#ifdef WINDOWS
		
		WIN32_FIND_DATA search_data;

		memset(&search_data, 0, sizeof(WIN32_FIND_DATA));

		HANDLE handle = FindFirstFile(s2ws(directory).c_str(), &search_data);

		while (handle != INVALID_HANDLE_VALUE)
		{
			std::cout << "\n" << search_data.cFileName;

			if (FindNextFile(handle, &search_data) == FALSE)
				break;
		}

		//Close the handle after use or memory/resource leak
		FindClose(handle);
#else
#include <dirent.h>
		DIR *dir;
		class dirent *ent;
		class stat st;

		dir = opendir(directory);
		while ((ent = readdir(dir)) != NULL) {
			const string file_name = ent->d_name;
			const string full_file_name = directory + "/" + file_name;

			if (file_name[0] == '.')
				continue;

			if (stat(full_file_name.c_str(), &st) == -1)
				continue;

			const bool is_directory = (st.st_mode & S_IFDIR) != 0;

			if (is_directory)
				continue;

			out.push_back(full_file_name);
		}
		closedir(dir);
#endif
	}
	*/
}


#endif