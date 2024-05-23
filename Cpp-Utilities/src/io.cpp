/*
    CONTRIBUTORS:
        Sean Pesce

*/

#include "sp/io.h"

#ifdef _WIN32
#include "Objbase.h" // CoTaskMemAlloc, CoTaskMemFree
#else // UNIX
#include <fcntl.h> // ioctl
#include <linux/kd.h> // KIOCSOUND
#include <thread> // std::thread
#include <iostream> // std::cout
#endif // _WIN32


__SP_NAMESPACE
namespace io {


uint32_t BEEP_FREQ_DEFAULT = 500,
         BEEP_DUR_DEFAULT  = 500;


#ifdef _WIN32
DWORD WINAPI _beep(LPVOID lpParam)
{
    uint32_t *beep_data = (uint32_t*)lpParam;
    if (lpParam != NULL)
    {
        Beep(beep_data[0], beep_data[1]);
        CoTaskMemFree(lpParam);
    }
    else
    {
        Beep(BEEP_FREQ_DEFAULT, BEEP_DUR_DEFAULT);
    }
    return 0;
}
#else // UNIX
void _beep(uint32_t frequency, uint32_t duration)
{
    // Reference: https://ubuntuforums.org/showthread.php?t=873679
    int fd = open("/dev/console", O_WRONLY);
    if (fd == -1)
    {
        std::cout << "\a";
        usleep(duration);
        return;
    }
    ioctl(fd, KIOCSOUND, (int)(1193180 / frequency));
    usleep(duration);
    ioctl(fd, KIOCSOUND, 0);
    close(fd);
}
#endif // _WIN32


bool beep(uint32_t frequency, uint32_t duration, bool async)
{
#ifdef _WIN32
    if (!async)
    {
        return !!Beep(frequency, duration);
    }
    uint32_t *beep_data = (uint32_t*)CoTaskMemAlloc(sizeof(uint32_t) * 2);
    if (beep_data != NULL)
    {
        beep_data[0] = frequency;
        beep_data[1] = duration;
    }
    return (CreateThread(NULL, 0, _beep, beep_data, 0, NULL) != NULL);
#else // UNIX
    std::thread t(_beep, frequency, duration);
    if (!async)
    {
        t.join();
    }
    else
    {
        t.detach();
    }
    return true;
#endif // _WIN32
}


} // namespace io
__SP_NAMESPACE_CLOSE // namespace sp
