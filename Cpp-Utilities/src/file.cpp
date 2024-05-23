/*
    CONTRIBUTORS:
        Sean Pesce

*/

#include "sp/file.h"
#include "sp/string.h"
#include "Objbase.h" // CoTaskMemAlloc

__SP_NAMESPACE
namespace file {


// Returns the size (in bytes) of the specified file
size_t size(const std::string &file)
{
    struct stat stat_buffer;
    int return_val = stat(file.c_str(), &stat_buffer);
    if (return_val == 0) {
        sp::err::set(SP_ERR_SUCCESS);
        return stat_buffer.st_size;
    } else {
        switch (errno)
        {
            case EACCES:
                sp::err::set(SP_ERR_ACCESS_DENIED);
                break;
            case EBADF:
                sp::err::set(SP_ERR_BAD_FILE_TYPE);
                break;
            case EOVERFLOW:
                sp::err::set(SP_ERR_BUFFER_OVERFLOW);
                break;
            case ENOTDIR:
                sp::err::set(SP_ERR_PATH_NOT_FOUND);
                break;
            case ENOMEM:
                sp::err::set(SP_ERR_OUT_OF_MEMORY);
                break;
            case ENOENT:
                sp::err::set(SP_ERR_INVALID_NAME);
                break;
            case ENAMETOOLONG:
                sp::err::set(SP_ERR_FILE_NAME_TOO_LONG);
                break;
            case ELOOP:
                sp::err::set(SP_ERR_TOO_MANY_LINKS);
                break;
            case EFAULT:
            default:
                sp::err::set(SP_ERR_INVALID_ADDRESS);
                break;
        }
        return 0;
    }
}


// Returns the size (in bytes) of the specified file
size_t size(const std::wstring &file)
{
    std::string f = sp::str::wide_to_mb(file);
    if (sp::err::get() != SP_ERR_SUCCESS) {
        sp::err::set(SP_ERR_BAD_FORMAT);
        return 0;
    }
    return size(f);
}


} // namespace file
__SP_NAMESPACE_CLOSE // namespace sp
