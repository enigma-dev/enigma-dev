#pragma once
#include "filesystem.hpp"
#define SET_LOCALE_ENVIRON(name, default_value) ((!ngs::fs::environment_get_variable(name).empty()) ? ngs::fs::environment_get_variable(name).c_str() : std::string(default_value).c_str())
#ifndef IFD_QUICK_ACCESS
#define IFD_QUICK_ACCESS SET_LOCALE_ENVIRON("IMGUI_QUICK_ACCESS", "Quick Access")
#endif
#ifndef IFD_THIS_PC
#define IFD_THIS_PC SET_LOCALE_ENVIRON("IMGUI_THIS_PC", "This PC")
#endif
#ifndef IFD_ALL_FILES
#define IFD_ALL_FILES SET_LOCALE_ENVIRON("IMGUI_ALL_FILES", "All Files (*.*)")
#endif
#ifndef IFD_NAME
#define IFD_NAME SET_LOCALE_ENVIRON("IMGUI_NAME", "Name")
#endif
#ifndef IFD_DATE_MODIFIED
#define IFD_DATE_MODIFIED SET_LOCALE_ENVIRON("IMGUI_DATE_MODIFIED", "Date modified")
#endif
#ifndef IFD_SIZE
#define IFD_SIZE SET_LOCALE_ENVIRON("IMGUI_SIZE", "Size")
#endif
#ifndef IFD_NEW_FILE
#define IFD_NEW_FILE SET_LOCALE_ENVIRON("IMGUI_NEW_FILE", "New file")
#endif
#ifndef IFD_NEW_DIRECTORY
#define IFD_NEW_DIRECTORY SET_LOCALE_ENVIRON("IMGUI_NEW_DIRECTORY", "New directory")
#endif
#ifndef IFD_DELETE
#define IFD_DELETE SET_LOCALE_ENVIRON("IMGUI_DELETE", "Delete")
#endif
#ifndef IFD_ARE_YOU_SURE
#define IFD_ARE_YOU_SURE SET_LOCALE_ENVIRON("IMGUI_ARE_YOU_SURE", "Are you sure?")
#endif
#ifndef IFD_OVERWRITE_FILE
#define IFD_OVERWRITE_FILE SET_LOCALE_ENVIRON("IMGUI_OVERWRITE_FILE", "Overwrite file?")
#endif
#ifndef IFD_ENTER_FILE_NAME
#define IFD_ENTER_FILE_NAME SET_LOCALE_ENVIRON("IMGUI_ENTER_FILE_NAME", "Enter file name")
#endif
#ifndef IFD_ENTER_DIRECTORU_NAME
#define IFD_ENTER_DIRECTORY_NAME SET_LOCALE_ENVIRON("IMGUI_ENTER_DIRECTORY_NAME", "Enter directory name")
#endif
#ifndef IFD_ARE_YOU_SURE_YOU_WANT_TO_DELETE
#define IFD_ARE_YOU_SURE_YOU_WANT_TO_DELETE SET_LOCALE_ENVIRON("IMGUI_ARE_YOU_SURE_YOU_WANT_TO_DELETE", "Are you sure you want to delete %s?")
#endif
#ifndef IFD_ARE_YOU_SURE_YOU_WANT_TO_OVERWRITE
#define IFD_ARE_YOU_SURE_YOU_WANT_TO_OVERWRITE SET_LOCALE_ENVIRON("IMGUI_ARE_YOU_SURE_YOU_WANT_TO_OVERWRITE", "Are you sure you want to overwrite %s?")
#endif
#ifndef IFD_YES
#define IFD_YES SET_LOCALE_ENVIRON("IMGUI_YES", "Yes")
#endif
#ifndef IFD_NO
#define IFD_NO SET_LOCALE_ENVIRON("IMGUI_NO", "No")
#endif
#ifndef IFD_OK
#define IFD_OK SET_LOCALE_ENVIRON("IMGUI_OK", "OK")
#endif
#ifndef IFD_CANCEL
#define IFD_CANCEL SET_LOCALE_ENVIRON("IMGUI_CANCEL", "Cancel")
#endif
#ifndef IFD_SEARCH
#define IFD_SEARCH SET_LOCALE_ENVIRON("IMGUI_SEARCH", "Search")
#endif
#ifndef IFD_FILE_NAME_WITH_COLON
#define IFD_FILE_NAME_WITH_COLON SET_LOCALE_ENVIRON("IMGUI_FILE_NAME_WITH_COLON", "File name:")
#endif
#ifndef IFD_FILE_NAME_WITHOUT_COLON
#define IFD_FILE_NAME_WITHOUT_COLON SET_LOCALE_ENVIRON("IMGUI_FILE_NAME_WITHOUT_COLON", "File name")
#endif
#ifndef IFD_SAVE
#define IFD_SAVE SET_LOCALE_ENVIRON("IMGUI_SAVE", "Save")
#endif
#ifndef IFD_OPEN
#define IFD_OPEN SET_LOCALE_ENVIRON("IMGUI_OPEN", "Open")
#endif
