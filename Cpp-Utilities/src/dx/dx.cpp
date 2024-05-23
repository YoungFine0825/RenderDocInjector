/*
    CONTRIBUTORS:
        Sean Pesce

*/
#ifdef _WIN32
#include "sp/dx/dx_main.h"
#include "sp/environment.h"
#include "sp/string.h"


__SP_NAMESPACE
namespace dx {


const std::string library_name[SP_DX_LIB_TYPE_COUNT_] = { SP_DX_LIB_NAME_UNKNOWN_, SP_DX_LIB_NAME_DINPUT8_, SP_DX_LIB_NAME_D3D8_, SP_DX_LIB_NAME_D3D9_, SP_DX_LIB_NAME_D3D10_, SP_DX_LIB_NAME_D3D11_, SP_DX_LIB_NAME_D3D12_, SP_DX_LIB_NAME_DXGI_ };


sp::dx::library get_library_type(sp::dx::library default_type)
{
    sp::dx::library type = default_type;
    std::string dll = sp::str::to_lowercase(sp::env::lib_name());
    for (int i = 0; i < SP_DX_LIB_TYPE_COUNT_; i++) {
        if (dll == sp::str::to_lowercase(library_name[i]) + ".dll") {
            type = static_cast<sp::dx::library>(i);
            break;
        }
    }
    return type;
}


std::string get_library_name()
{
    return library_name[static_cast<int>(get_library_type())];
}


sp::dx::library get_library_type_from_name(const std::string &name, bool remove_file_ext)
{
    sp::dx::library type = library::unknown;
    std::string _name = sp::str::to_lowercase(name);
    if (remove_file_ext) {
        _name = sp::str::remove<char>(_name, ".dll");
    }
    for (int i = 0; i < SP_DX_LIB_TYPE_COUNT_; i++) {
        if (_name == sp::str::to_lowercase(library_name[i])) {
            type = static_cast<sp::dx::library>(i);
            break;
        }
    }
    return type;
}


std::string get_library_name_from_type(sp::dx::library type)
{
    return library_name[static_cast<int>(type)];
}


} // namespace dx
__SP_NAMESPACE_CLOSE // namespace sp



#endif // _WIN32
