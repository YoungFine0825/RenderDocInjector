/*
    CONTRIBUTORS:
        Sean Pesce

*/

#include "sp/string.h"
#include <clocale>

__SP_NAMESPACE
namespace str {

std::string LOCALE = "en_US.utf8";


// Converts a wide character string (wchar_t) to a multibyte string (char)
std::string wide_to_mb(const std::wstring &in, const std::string &locale)
{
    std::string out;
    char *buffer = NULL;
    size_t chars_converted;
    errno_t ret_val = 0;
    size_t len = in.length();

    if (len == 0)
        return out.c_str();

    buffer = (char*)malloc(sizeof(char) * (len + 1));

    if (buffer == NULL) {
        sp::err::set(SP_ERR_NOT_ENOUGH_MEMORY);
        return "";
    }

    std::setlocale(LC_ALL, locale.c_str());
    if ((ret_val = wcstombs_s(&chars_converted, buffer, len + 1, in.c_str(), sizeof(wchar_t)*(len + 1))) || chars_converted == (size_t)-1) {
        // Error converting from wide char to char
        free(buffer);

        if (ret_val != 0) {
            sp::err::set(ret_val);
        } else {
            sp::err::set(SP_ERR_BAD_FORMAT);
        }
        return "";
    }
    out.append(buffer);
    free(buffer);
    sp::err::set(SP_ERR_SUCCESS);
    return out;
}


// Converts a multibyte string (char) to a wide character string (wchar_t)
std::wstring mb_to_wide(const std::string &in, const std::string &locale)
{
    std::wstring out;
    wchar_t *buffer = NULL;
    size_t chars_converted;
    errno_t ret_val = 0;
    size_t len = in.length();

    if (len == 0)
        return out.c_str();

    buffer = (wchar_t*)malloc(sizeof(wchar_t) * (len + 1));

    if (buffer == NULL) {
        sp::err::set(SP_ERR_NOT_ENOUGH_MEMORY);
        return L"";
    }

    std::setlocale(LC_ALL, locale.c_str());
    if ((ret_val = mbstowcs_s(&chars_converted, buffer, len + 1, in.c_str(), sizeof(char)*(len + 1))) || chars_converted == (size_t)-1) {
        // Error converting from char to wide char
        free(buffer);

        if (ret_val != 0) {
            sp::err::set(ret_val);
        } else {
            sp::err::set(SP_ERR_BAD_FORMAT);
        }
        return L"";
    }
    out.append(buffer);
    free(buffer);
    sp::err::set(SP_ERR_SUCCESS);
    return out;
}


// Formats a string like sprintf
std::string format(const std::string format, ...)
{
    int buff_size = (int)format.length() + 20;
    std::string buff;
    va_list args;
    while (true) {
        buff.resize(buff_size);
        va_start(args, format);
        int n = vsnprintf((char*)buff.data(), buff_size, format.c_str(), args);
        va_end(args);
        if (n > -1 && n < buff_size) {
            buff.resize(n);
            return buff;
        }
        if (n > -1) {
            buff_size = n + 1; // +1 for null terminator
        } else {
            buff_size *= 2; // Guess a new size
        }
    }
}

// Formats a wide-character string like swprintf
std::wstring format_w(const std::wstring format, ...)
{
    int buff_size = (int)format.length() + 20;
    std::wstring buff;
    va_list args;
    while (true) {
        buff.resize(buff_size);
        va_start(args, format);
        int n = vswprintf((wchar_t*)buff.data(), buff_size, format.c_str(), args);
        va_end(args);
        if (n > -1 && n < buff_size) {
            buff.resize(n);
            return buff;
        } else {
            buff_size *= 2; // Guess a new size
        }
    }
}


} // namespace str
__SP_NAMESPACE_CLOSE // namespace sp
