/*
    CONTRIBUTORS:
        Sean Pesce

*/
#ifdef _WIN32
#pragma once

#ifndef SP_INPUT_OUTPUT_POWERSHELL_OUTPUT_STREAM_H_
#define SP_INPUT_OUTPUT_POWERSHELL_OUTPUT_STREAM_H_

#include "sp/sp.h"
#include "sp/environment.h"
#include "sp/string.h"
#include "sp/system/process/child.h"

#include <string>
#include <unordered_set>


#define SP_IO_POWERSHELL_OUT_LAUNCH_CMD_ "powershell -NoLogo -Command \"$host.ui.RawUI.WindowTitle = '" + _title + "'; " \
                                         "$host.ui.RawUI.BackgroundColor = '" + _bg_color + "'; " \
                                         "$host.ui.RawUI.ForegroundColor = '" + _fg_color + "'; " \
                                         "cls; " \
                                         "for($i = 0; $i -lt 1; $i+=0) " \
                                         "{ " \
                                         "$in_line = (Read-Host); " \
                                         "Get-Process -Id " + std::to_string(sp::env::process_id()) + " > $null 2> $null; " \
                                         "if (!$?) { Exit }; " \
                                         "}; \""


__SP_NAMESPACE
namespace io {


// Class for printing messages to external PowerShell window
class ps_ostream : public sp::sys::proc::child {
private:
    // Window title
    std::string _title;

    // Console colors
    std::string _bg_color;
    std::string _fg_color;


protected:

    inline void _refresh_command()
    {
        _command = SP_IO_POWERSHELL_OUT_LAUNCH_CMD_;
    }


public:
    class defaults;

    inline static const std::unordered_set<std::string> COLORS =
        {
            "black",
            "white",
            "gray",
            "darkgray",
            "red",
            "darkred",
            "blue",
            "darkblue",
            "green",
            "darkgreen",
            "yellow",
            "darkyellow",
            "cyan",
            "darkcyan",
            "magenta",
            "darkmagenta"
        };

    ps_ostream(const std::string& title = ps_ostream::defaults::title)
            : sp::sys::proc::child("", "", true, 0)
    {
        _title = title;
        _bg_color = ps_ostream::defaults::bg_color;
        _fg_color = ps_ostream::defaults::fg_color;
        _refresh_command();
    }

    ps_ostream(const sp::io::ps_ostream&) = delete; // Disable copy constructor
    ~ps_ostream() { terminate(); }


    bool print(const std::string& message) const
    {
        uint32_t bytes_written = 0;
        if (running())
        {
            if (!WriteFile(io.stdin_write,
                            message.c_str(),
                            static_cast<DWORD>(message.length()),
                            reinterpret_cast<DWORD*>(&bytes_written),
                            NULL))
            {
                // Handle error
                return false;
            }
        }
        else
        {
            return false;
        }
        return true;
    }


    inline const std::string& title() const
    {
        return _title;
    }


    inline const std::string& bg_color() const
    {
        return _bg_color;
    }


    inline const std::string& fg_color() const
    {
        return _fg_color;
    }


    inline void set_title(const std::string& new_title)
    {
        if (_title != new_title)
        {
            _title = new_title;
            _refresh_command();
            restart();
        }
    }


    inline bool set_bg_color(const std::string& new_color)
    {
        std::string color = sp::str::to_lowercase(new_color);

        if (_bg_color != color && ps_ostream::COLORS.find(color) != ps_ostream::COLORS.end())
        {
            _bg_color = color;
            _refresh_command();
            restart();
            return true;
        }
        return false;
    }


    inline bool set_fg_color(const std::string& new_color)
    {
        std::string color = sp::str::to_lowercase(new_color);

        if (_fg_color != color && ps_ostream::COLORS.find(color) != ps_ostream::COLORS.end())
        {
            _fg_color = color;
            _refresh_command();
            restart();
            return true;
        }
        return false;
    }


    inline bool set_colors(const std::string& new_bg_color, const std::string& new_fg_color)
    {
        bool result = false;
        std::string bg_color = sp::str::to_lowercase(new_bg_color);
        std::string fg_color = sp::str::to_lowercase(new_fg_color);

        if (ps_ostream::COLORS.find(bg_color) != ps_ostream::COLORS.end() &&
            ps_ostream::COLORS.find(fg_color) != ps_ostream::COLORS.end()) {

            if (_bg_color != bg_color)
            {
                _bg_color = bg_color;
                result = true;
            }

            if (_fg_color != fg_color)
            {
                _fg_color = fg_color;
                result = true;
            }

            if (result)
            {
                _refresh_command();
                restart();
            }
        }
        return result;
    }

    class defaults {
    public:
        static constexpr const char* title = "PowerShell Output";
        static constexpr const char* bg_color = "black";
        static constexpr const char* fg_color = "white";
    private:
        defaults() = delete;
    };

}; // class ps_ostream


} // namespace io
__SP_NAMESPACE_CLOSE // namespace sp


inline const sp::io::ps_ostream& operator<< (const sp::io::ps_ostream& os, const std::string& message)
{
    os.print(message);
    return os;
}

inline const sp::io::ps_ostream& operator<< (const sp::io::ps_ostream& os, char message)
{
    os.print(std::string(1, message));
    return os;
}


#undef SP_IO_POWERSHELL_OUT_LAUNCH_CMD_

#endif // SP_INPUT_OUTPUT_POWERSHELL_OUTPUT_STREAM_H_


#endif // _WIN32
