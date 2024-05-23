// Author: Sean Pesce


#include "SP_IO.hpp"
#include <clocale>


// Writes a string to a text file:
int file_write_text(const char *file, const char *msg)
{
    std::ofstream out_stream;
	out_stream.open(file);
	if(out_stream.fail())
	{
		// Failed to open the output file
		return 1;
	}
	out_stream << msg << std::endl;
	out_stream.close();
	return 0;
}

// Appends a string to a text file:
int file_append_text(const char *file, const char *msg)
{
	std::ofstream out_stream;
	out_stream.open(file, std::ofstream::out | std::ofstream::app);
	if (out_stream.fail())
	{
		// Failed to open the output file
		return 1;
	}
	out_stream << msg << std::endl;
	out_stream.close();
	return 0;
}


// Obtains a list of all key/value pairs from the specified section of a configuration file
int get_private_profile_string_section(const char *file, const char *section, std::list<std::string> *keys)
{
	std::ifstream in_stream;
	in_stream.open(file, std::ifstream::in); // Open file
	std::string line;

	if (!in_stream.is_open())
	{
		// Handle error
		return -1;
	}

	// Find section header
	std::string section_start = std::string("[").append(section).append("]");
	while (std::getline(in_stream, line))
	{
		int index = -1;
		if((index = (int)line.find(section_start.c_str(), 0, section_start.length())) < 0)
		{
			// Current line does not contain the given section header
			line.clear();
			continue;
		}

		if (index == line.find_first_not_of(" \t"))
		{
			// Found desired section
			break;
		}
		else
		{
			// Line contains section header, but header is commented out
			line.clear();
		}
	}

	// Reached end of file without finding the section
	if (line.length() == 0)
	{
		in_stream.close();
		return -1;
	}

	int key_pair_count = 0; // Number of valid key/value pairs parsed from the file

	// Continue reading each line until EOF or another line that starts with "[", and store all valid keys
	while (std::getline(in_stream, line))
	{
		// If line starts with '[', new section header was found
		int index = (int)line.find_first_of("[", 0);
		if (index >= 0 && index == line.find_first_not_of(" \t"))
		{
			// Found next section; stop reading values
			break;
		}
		else if((index = (int)line.find_first_not_of(" \t")) >= 0)
		{
			char key_c_str[_SP_MAX_PP_KEY_LENGTH_ + 1];
			char value_c_str[_SP_MAX_PP_STRING_VAL_LENGTH_ + 1];
			key_c_str[0] = '\0';
			value_c_str[0] = '\0';
			key_c_str[_SP_MAX_PP_KEY_LENGTH_] = '\0';
			value_c_str[_SP_MAX_PP_STRING_VAL_LENGTH_] = '\0';

			//std::regex regex("^[a-zA-Z_]+[a-zA-Z0-9_]*[= ][^ \r\n\t]+");
			// If line starts with [a-zA-Z0-9_], get all characters until "=" or " " and store as key name, and get next word token and store as key value
			#ifdef _WIN32
				int count = sscanf_s(&line.c_str()[index], "%[a-zA-Z0-9_]%*[= \t]%[^\t\r\n ]", key_c_str, _SP_MAX_PP_KEY_LENGTH_, value_c_str, _SP_MAX_PP_STRING_VAL_LENGTH_);
			#else
				int count = sscanf(&line.c_str()[index], "%[a-zA-Z0-9_]%*[= \t]%[^\t\r\n ]", key_c_str, value_c_str);
			#endif // _WIN32
			if (count < 2)
			{
				// Check for and handle errors
			}
			else
			{
				// Add the key/value pair to the list
				keys->push_back(std::string(key_c_str));
				keys->push_back(std::string(value_c_str));
				key_pair_count++;
			}
		}
	}
	in_stream.close();

	// Return number of key/value pairs that were successfully read from the file
	return key_pair_count;
}




/////////////////////// General Text Data I/O ///////////////////////


// Constructs a timestamp string for the current 24-hour time and stores it in the specified buffer.
int get_current_timestamp_string(char *timestamp_string_buff, bool surround_with_brackets)
{
	// Get the current time and store it in a data structure for easier parsing
	time_t current_time_t = time(0);
	struct tm current_time;
	errno_t err = 0;
	if (err = localtime_s(&current_time, &current_time_t))
	{
		// Function failed; return error code
		return (int)err;
	}

	int index = 0; // Stores the current character index in the timestamp string buffer

	if (surround_with_brackets)
	{
		// Open bracket
		timestamp_string_buff[index++] = '[';
	}
	// Hour
	if (current_time.tm_hour < 10)
	{
		timestamp_string_buff[index++] = '0';
		timestamp_string_buff[index++] = std::to_string(current_time.tm_hour).c_str()[0];
	}
	else
	{
		timestamp_string_buff[index++] = std::to_string(current_time.tm_hour).c_str()[0];
		timestamp_string_buff[index++] = std::to_string(current_time.tm_hour).c_str()[1];
	}
	timestamp_string_buff[index++] = ':';
	// Minute
	if (current_time.tm_min < 10)
	{
		timestamp_string_buff[index++] = '0';
		timestamp_string_buff[index++] = std::to_string(current_time.tm_min).c_str()[0];
	}
	else
	{
		timestamp_string_buff[index++] = std::to_string(current_time.tm_min).c_str()[0];
		timestamp_string_buff[index++] = std::to_string(current_time.tm_min).c_str()[1];
	}
	timestamp_string_buff[index++] = ':';
	// Second
	if (current_time.tm_sec < 10)
	{
		timestamp_string_buff[index++] = '0';
		timestamp_string_buff[index++] = std::to_string(current_time.tm_sec).c_str()[0];
	}
	else
	{
		timestamp_string_buff[index++] = std::to_string(current_time.tm_sec).c_str()[0];
		timestamp_string_buff[index++] = std::to_string(current_time.tm_sec).c_str()[1];
	}
	if (surround_with_brackets)
	{
		// Close bracket
		timestamp_string_buff[index++] = ']';
	}

	timestamp_string_buff[index++] = '\0'; // Null-terminate string
	
	return 0;
}


// Constructs a timestamp string for the current 24-hour time.
int get_current_timestamp_string(std::string *timestamp_string, bool surround_with_brackets)
{
	timestamp_string->clear();

	// Get the current time and store it in a data structure for easier parsing
	time_t current_time_t = time(0);
	struct tm current_time;
	errno_t err = 0;
	if (err = localtime_s(&current_time, &current_time_t))
	{
		// Function failed; return error code
		return (int)err;
	}

	if (surround_with_brackets)
	{
		// Open bracket
		(*timestamp_string) += '[';
	}
	// Hour
	if (current_time.tm_hour < 10)
	{
		(*timestamp_string) += '0';
		timestamp_string->append(std::to_string(current_time.tm_hour));
	}
	else
	{
		timestamp_string->append(std::to_string(current_time.tm_hour));
	}
	(*timestamp_string) += ':';
	// Minute
	if (current_time.tm_min < 10)
	{
		(*timestamp_string) += '0';
		timestamp_string->append(std::to_string(current_time.tm_min));
	}
	else
	{
		timestamp_string->append(std::to_string(current_time.tm_min));
	}
	(*timestamp_string) += ':';
	// Second
	if (current_time.tm_sec < 10)
	{
		(*timestamp_string) += '0';
		timestamp_string->append(std::to_string(current_time.tm_sec));
	}
	else
	{
		timestamp_string->append(std::to_string(current_time.tm_sec));
	}
	if (surround_with_brackets)
	{
		// Close bracket
		(*timestamp_string) += ']';
	}

	return 0;
}


// Constructs a timestamp string for the current 24-hour time and appends it to the given string.
int append_current_timestamp_string(std::string *timestamp_string, bool surround_with_brackets)
{
	// Get the current time and store it in a data structure for easier parsing
	time_t current_time_t = time(0);
	struct tm current_time;
	errno_t err = 0;
	if (err = localtime_s(&current_time, &current_time_t))
	{
		// Function failed; return error code
		return (int)err;
	}

	if (surround_with_brackets)
	{
		// Open bracket
		(*timestamp_string) += '[';
	}
	// Hour
	if (current_time.tm_hour < 10)
	{
		(*timestamp_string) += '0';
		timestamp_string->append(std::to_string(current_time.tm_hour));
	}
	else
	{
		timestamp_string->append(std::to_string(current_time.tm_hour));
	}
	(*timestamp_string) += ':';
	// Minute
	if (current_time.tm_min < 10)
	{
		(*timestamp_string) += '0';
		timestamp_string->append(std::to_string(current_time.tm_min));
	}
	else
	{
		timestamp_string->append(std::to_string(current_time.tm_min));
	}
	(*timestamp_string) += ':';
	// Second
	if (current_time.tm_sec < 10)
	{
		(*timestamp_string) += '0';
		timestamp_string->append(std::to_string(current_time.tm_sec));
	}
	else
	{
		timestamp_string->append(std::to_string(current_time.tm_sec));
	}
	if (surround_with_brackets)
	{
		// Close bracket
		(*timestamp_string) += ']';
	}

	return 0;
}


// Constructs a formatted date string for the current day and stores it in the specified buffer.
int get_current_date_string(char *date_string_buff, bool surround_with_brackets, int format, char separator)
{
	// Get the current time and store it in a data structure for easier parsing
	time_t current_time_t = time(0);
	struct tm current_time;
	errno_t err = 0;
	if (err = localtime_s(&current_time, &current_time_t))
	{
		// Function failed; return error code
		return (int)err;
	}

	// Get month string-value
	char month[3];
	if (current_time.tm_mon < 9)
	{
		month[0] = '0';
		strcpy_s(&month[1], 2, std::to_string(current_time.tm_mon+1).c_str());
	}
	else
	{
		strcpy_s(month, 3, std::to_string(current_time.tm_mon+1).c_str());
	}
	month[2] = '\0';

	// Get day string-value
	char day[3];
	if (current_time.tm_mday < 10)
	{
		day[0] = '0';
		strcpy_s(&day[1], 2, std::to_string(current_time.tm_mday).c_str());
	}
	else
	{
		strcpy_s(day, 3, std::to_string(current_time.tm_mday).c_str());
	}
	day[2] = '\0';

	// Get year string-value
	char year[5];
	strcpy_s(year, 5, std::to_string(current_time.tm_year + 1900).c_str());
	year[4] = '\0';

	int index = 0; // Stores the current character index in the date string buffer

	if (surround_with_brackets)
	{
		// Open bracket
		date_string_buff[index++] = '[';
	}

	switch (format)
	{
		case SP_DATE_MMDDYY:
			date_string_buff[index++] = month[0];
			date_string_buff[index++] = month[1];
			date_string_buff[index++] = separator;
			date_string_buff[index++] = day[0];
			date_string_buff[index++] = day[1];
			date_string_buff[index++] = separator;
			date_string_buff[index++] = year[2];
			date_string_buff[index++] = year[3];
			break;
		case SP_DATE_DDMMYY:
			date_string_buff[index++] = day[0];
			date_string_buff[index++] = day[1];
			date_string_buff[index++] = separator;
			date_string_buff[index++] = month[0];
			date_string_buff[index++] = month[1];
			date_string_buff[index++] = separator;
			date_string_buff[index++] = year[2];
			date_string_buff[index++] = year[3];
			break;
		case SP_DATE_MMDDYYYY:
			date_string_buff[index++] = month[0];
			date_string_buff[index++] = month[1];
			date_string_buff[index++] = separator;
			date_string_buff[index++] = day[0];
			date_string_buff[index++] = day[1];
			date_string_buff[index++] = separator;
			date_string_buff[index++] = year[0];
			date_string_buff[index++] = year[1];
			date_string_buff[index++] = year[2];
			date_string_buff[index++] = year[3];
			break;
		case SP_DATE_DDMMYYYY:
		default:
			date_string_buff[index++] = day[0];
			date_string_buff[index++] = day[1];
			date_string_buff[index++] = separator;
			date_string_buff[index++] = month[0];
			date_string_buff[index++] = month[1];
			date_string_buff[index++] = separator;
			date_string_buff[index++] = year[0];
			date_string_buff[index++] = year[1];
			date_string_buff[index++] = year[2];
			date_string_buff[index++] = year[3];
			break;
	}
	if (surround_with_brackets)
	{
		// Close bracket
		date_string_buff[index++] = ']';
	}

	date_string_buff[index++] = '\0'; // Null-terminate string

	return 0;
}

// Constructs a formatted date string for the current day and stores it in the specified buffer.
int get_current_date_string(char *date_string_buff, bool surround_with_brackets, int format)
{
	return get_current_date_string(date_string_buff, surround_with_brackets, format, '/');
}

// Constructs a formatted date string for the current day.
int get_current_date_string(std::string *date_string, bool surround_with_brackets, int format, char separator)
{
	date_string->clear();

	// Get the current time and store it in a data structure for easier parsing
	time_t current_time_t = time(0);
	struct tm current_time;
	errno_t err = 0;
	if (err = localtime_s(&current_time, &current_time_t))
	{
		// Function failed; return error code
		return (int)err;
	}

	// Get month string-value
	char month[3];
	if (current_time.tm_mon < 9)
	{
		month[0] = '0';
		strcpy_s(&month[1], 2, std::to_string(current_time.tm_mon + 1).c_str());
	}
	else
	{
		strcpy_s(month, 3, std::to_string(current_time.tm_mon + 1).c_str());
	}
	month[2] = '\0';

	// Get day string-value
	char day[3];
	if (current_time.tm_mday < 10)
	{
		day[0] = '0';
		strcpy_s(&day[1], 2, std::to_string(current_time.tm_mday).c_str());
	}
	else
	{
		strcpy_s(day, 3, std::to_string(current_time.tm_mday).c_str());
	}
	day[2] = '\0';

	// Get year string-value
	char year[5];
	strcpy_s(year, 5, std::to_string(current_time.tm_year + 1900).c_str());
	year[4] = '\0';

	if (surround_with_brackets)
	{
		// Open bracket
		(*date_string) += '[';
	}

	switch (format)
	{
		case SP_DATE_MMDDYY:
			date_string->append(month);
			(*date_string) += separator;
			date_string->append(day);
			(*date_string) += separator;
			date_string->append(&year[2]);
			break;
		case SP_DATE_DDMMYY:
			date_string->append(day);
			(*date_string) += separator;
			date_string->append(month);
			(*date_string) += separator;
			date_string->append(&year[2]);
			break;
		case SP_DATE_MMDDYYYY:
			date_string->append(month);
			(*date_string) += separator;
			date_string->append(day);
			(*date_string) += separator;
			date_string->append(year);
			break;
		case SP_DATE_DDMMYYYY:
		default:
			date_string->append(day);
			(*date_string) += separator;
			date_string->append(month);
			(*date_string) += separator;
			date_string->append(year);
			break;
	}
	if (surround_with_brackets)
	{
		// Close bracket
		(*date_string) += ']';
	}

	return 0;
}

// Constructs a formatted date string for the current day.
int get_current_date_string(std::string *date_string, bool surround_with_brackets, int format)
{
	return get_current_date_string(date_string, surround_with_brackets, format, '/');
}

// Constructs a formatted date string for the current day and appends it to the given string.
int append_current_date_string(std::string *date_string, bool surround_with_brackets, int format, char separator)
{
	// Get the current time and store it in a data structure for easier parsing
	time_t current_time_t = time(0);
	struct tm current_time;
	errno_t err = 0;
	if (err = localtime_s(&current_time, &current_time_t))
	{
		// Function failed; return error code
		return (int)err;
	}

	// Get month string-value
	char month[3];
	if (current_time.tm_mon < 9)
	{
		month[0] = '0';
		strcpy_s(&month[1], 2, std::to_string(current_time.tm_mon + 1).c_str());
	}
	else
	{
		strcpy_s(month, 3, std::to_string(current_time.tm_mon + 1).c_str());
	}
	month[2] = '\0';

	// Get day string-value
	char day[3];
	if (current_time.tm_mday < 10)
	{
		day[0] = '0';
		strcpy_s(&day[1], 2, std::to_string(current_time.tm_mday).c_str());
	}
	else
	{
		strcpy_s(day, 3, std::to_string(current_time.tm_mday).c_str());
	}
	day[2] = '\0';

	// Get year string-value
	char year[5];
	strcpy_s(year, 5, std::to_string(current_time.tm_year + 1900).c_str());
	year[4] = '\0';

	if (surround_with_brackets)
	{
		// Open bracket
		(*date_string) += '[';
	}

	switch (format)
	{
		case SP_DATE_MMDDYY:
			date_string->append(month);
			(*date_string) += separator;
			date_string->append(day);
			(*date_string) += separator;
			date_string->append(&year[2]);
			break;
		case SP_DATE_DDMMYY:
			date_string->append(day);
			(*date_string) += separator;
			date_string->append(month);
			(*date_string) += separator;
			date_string->append(&year[2]);
			break;
		case SP_DATE_MMDDYYYY:
			date_string->append(month);
			(*date_string) += separator;
			date_string->append(day);
			(*date_string) += separator;
			date_string->append(year);
			break;
		case SP_DATE_DDMMYYYY:
		default:
			date_string->append(day);
			(*date_string) += separator;
			date_string->append(month);
			(*date_string) += separator;
			date_string->append(year);
			break;
	}
	if (surround_with_brackets)
	{
		// Close bracket
		(*date_string) += ']';
	}

	return 0;
}

// Constructs a formatted date string for the current day and appends it to the given string.
int append_current_date_string(std::string *date_string, bool surround_with_brackets, int format)
{
	return append_current_date_string(date_string, surround_with_brackets, format, '/');
}



// Converts a wide character string (UTF-16) to a multibyte string (char* string).
int string_wide_to_mb(wchar_t *in_string, std::string &out_string, bool append)
{
	if (in_string == NULL)
	{
#ifdef _WIN32
		return ERROR_INVALID_ADDRESS;
#else
		return EFAULT;
#endif // _WIN32
	}

	if (!append)
		out_string.clear();

	char *buffer = NULL;
	size_t chars_converted;
	errno_t ret_val = 0;

	size_t len = std::wstring(in_string).length();
	if (len == 0)
		return 0;

	buffer = (char*)malloc(sizeof(char) * (len + 1));

	if(buffer == NULL)
#ifdef _WIN32
		return ERROR_NOT_ENOUGH_MEMORY;
#else
		return ENOMEM;
#endif // _WIN32

	std::setlocale(LC_ALL, "en_US.utf8");
	if ((ret_val = wcstombs_s(&chars_converted, buffer, len + 1, in_string, sizeof(wchar_t)*(len+1))) || chars_converted == (size_t)-1)
	{
		// Error converting from wide char to char
		free(buffer);

		if (ret_val != 0)
			return ret_val;
		else
#ifdef _WIN32
			return ERROR_BAD_FORMAT;
#else
			return EBADF;
#endif // _WIN32
	}
	
	out_string.append(buffer);

	free(buffer);
	return 0;
}


// Converts a multibyte string(char* string) to a wide character string(UTF - 16).
int string_mb_to_wide(char *in_string, std::wstring &out_string, bool append)
{
	if (in_string == NULL)
	{
#ifdef _WIN32
		return ERROR_INVALID_ADDRESS;
#else
		return EFAULT;
#endif // _WIN32
	}

	if (!append)
		out_string.clear();

	wchar_t *buffer = NULL;
	size_t chars_converted;
	errno_t ret_val = 0;

	size_t len = std::string(in_string).length();
	if (len == 0)
		return 0;

	buffer = (wchar_t*)malloc(sizeof(wchar_t) * (len + 1));

	if(buffer == NULL)
#ifdef _WIN32
		return ERROR_NOT_ENOUGH_MEMORY;
#else
		return ENOMEM;
#endif // _WIN32

	std::setlocale(LC_ALL, "en_US.utf8");
	if ((ret_val = mbstowcs_s(&chars_converted, buffer, len + 1, in_string, sizeof(char)*(len+1))) || chars_converted == (size_t)-1)
	{
		// Error converting from wide char to char
		free(buffer);

		if (ret_val != 0)
			return ret_val;
		else
#ifdef _WIN32
			return ERROR_BAD_FORMAT;
#else
			return EBADF;
#endif // _WIN32
	}
	
	out_string.append(buffer);

	free(buffer);
	return 0;
}
