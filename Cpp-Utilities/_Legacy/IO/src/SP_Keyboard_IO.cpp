// Author: Sean Pesce


#include "SP_IO.hpp"


// Parses config file for the specified hotkey (returned as a virtual-key code)
unsigned int get_vk_hotkey(const char *settings_file, const char *section, const char *key_name)
{
	unsigned int vk_hotkey = 0;

	#ifdef _WIN32
		char hotkey_str[3];
		GetPrivateProfileString(section,
								key_name,
								NULL,
								hotkey_str,
								3,
								settings_file);

		std::stringstream hotkey_parse_stream;
		hotkey_parse_stream << std::hex << hotkey_str;
		hotkey_parse_stream >> vk_hotkey;
	#else // _WIN32

		// @TODO: Implement this function for Linux

	#endif // _WIN32

	return vk_hotkey; // Return the hotkey as a virtual key code
}


// Adds a new keybound function to the given list
int add_function_keybind(unsigned int key, int(*function)(), std::list<SP_KEY_FUNCTION> *binds)
{
	if (function == NULL || binds == NULL)
	{
		// Invalid pointer was passed

		// @TODO: Set errno to invalid parameter or invalid pointer
		return -1;
	}

	SP_KEY_FUNCTION new_key_func = { key, function };

	binds->push_back(new_key_func);

	return 0;
}


#ifdef _WIN32
// Gets the async key state for all 256 virtual keys and stores them in a buffer
void get_async_keyboard_state(SHORT *keyboard_state_buffer)
{
	for(int key = 0; key < 256; key++)
	{
		keyboard_state_buffer[key] = GetAsyncKeyState(key);
	}
}
#endif // _WIN32