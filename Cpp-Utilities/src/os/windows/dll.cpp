/*
    CONTRIBUTORS:
        Sean Pesce

*/

#include "sp/os/windows/dll.h"
#include "sp/preferences/user_setting.h"
#include "sp/log.h"
#include <map>


__SP_NAMESPACE
namespace lib {


std::map<std::string, HMODULE> _loaded;


size_t load_user_specified()
{
    std::string name = user_setting<std::string>("DLL", "LoadLibrary", "").read();
    bool entries_exist = !name.empty();
    HMODULE hmodule = name.empty() ? NULL : LoadLibrary(name.c_str());
    if (hmodule)
    {
        _loaded[name] = hmodule;
    }
    name = user_setting<std::string>("DLL", "LoadLibrary0", "").read();
    entries_exist = entries_exist || !name.empty();
    hmodule = (name.empty() || _loaded.find(name) != _loaded.end()) ? NULL : LoadLibrary(name.c_str());
    if (hmodule)
    {
        _loaded[name] = hmodule;
    }
    if (entries_exist)
    {
        for (int i = 1; i < SP_MAX_LOAD_LIBS_; i++)
        {
            name = user_setting<std::string>("DLL", std::string("LoadLibrary").append(std::to_string(i)).c_str(), "").read();
            if (name.empty())
            {
                break;
            }
            hmodule = (_loaded.find(name) != _loaded.end()) ? NULL : LoadLibrary(name.c_str());
            if (hmodule)
            {
                _loaded[name] = hmodule;
            }
        }
    }
    if (!_loaded.empty())
    {
        if (_loaded.size() == 1)
        {
            SP_LOG("Loaded %d generic library\n", _loaded.size());
        }
        else
        {
            SP_LOG("Loaded %d generic libraries\n", _loaded.size());
        }
    }
    return _loaded.size();
}

} // namespace lib
__SP_NAMESPACE_CLOSE // namespace sp
