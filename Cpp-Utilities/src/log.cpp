/*
    CONTRIBUTORS:
        Sean Pesce

*/

#include "sp/log.h"



__SP_NAMESPACE


bool log::_overwrite_on_init = log::defaults::overwrite_on_init; // Overwrite log file during initialization
std::string log::_file = log::defaults::file;
io::ps_ostream log::_ostream;
bool log::_initialized = log::initialize();
const log::printer log::out;

bool log::initialize(bool open_output_window)
{
    long err = SP_ERR_SUCCESS;
    if (log::_overwrite_on_init)
    {
        err = sp::file::append_text(log::_file, std::string());
    }
    else
    {
        err = sp::file::write_text(log::_file, std::string());
    }
    if (open_output_window)
    {
        log::_ostream.start();
    }
    log::_initialized = (err == SP_ERR_SUCCESS);
    return log::_initialized;
}


__SP_NAMESPACE_CLOSE // namespace sp
