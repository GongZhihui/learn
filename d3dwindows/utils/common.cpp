#include "common.h"
#include <Windows.h>

std::string GetApplicationDirA()
{
	char current_proc_path[MAX_PATH] = { 0 };
	::GetModuleFileNameA(NULL, current_proc_path, MAX_PATH);

	std::string current_proc_dir;
	current_proc_dir.append(current_proc_path);
	int separator_pos = current_proc_dir.rfind('\\');

	if (std::string::npos == separator_pos)
	{
		current_proc_dir = "";
	}
	else {
		current_proc_dir = current_proc_dir.substr(0, separator_pos);
	}

	return current_proc_dir;
}

std::wstring GetApplicationDirW()
{
	wchar_t current_proc_path[MAX_PATH] = { 0 };
	::GetModuleFileNameW(NULL, current_proc_path, MAX_PATH);

	std::wstring current_proc_dir;
	current_proc_dir.append(current_proc_path);
	int separator_pos = current_proc_dir.rfind('\\');

	if (std::wstring::npos == separator_pos)
	{
		current_proc_dir = L"";
	}
	else {
		current_proc_dir = current_proc_dir.substr(0, separator_pos);
	}

	return current_proc_dir;
}