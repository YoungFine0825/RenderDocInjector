// Made by Sean P

#include "SP_AoB_Scan_Example.hpp"


int main()
{
    sp_print_intro("\nAoB Scan Demo Program");
    enter_to_continue("Press enter to continue...");

    uint8_t buff[255];
    bool mask[255];
    size_t len = 0;

    reset_aob_scan_settings();

	#ifdef _WIN32
	// This is just a way to stop the compiler from removing dummy_function (as an optimization)
	std::cout << "Address of dummy_function:  " << &dummy_function << std::endl;
	#endif

    if(is_64bit()) // this is a 64-bit process
    {
        // Define array to search for based on architecture and OS:
        #ifdef _WIN32
            bool m[10] = { true, true, true, true, true, true, true, true, true, true };  // Mask array
			uint8_t aob[10] = { 0xbb, 0x01, 0x00, 0x00, 0x00, 0x66, 0x90, 0x48, 0x8b, 0x0d }; // starts at &dummy_function() + 29
			std::cout << "Address of byte array in main:  " << (void*)aob << std::endl;
			const char *str_aob = "bb 01 00 00 00 66 90 48 8b 0d";
			char str_aob2[30] = { 'b', 'b', ' ', '0', '1', ' ', '0', '0', ' ', '0', '0', ' ', '0', '0', ' ', '6', '6', ' ', '9', '0', ' ', '4', '8', ' ', '8', 'b', ' ', '0', 'd', '\0' };
			std::string str_aob3("bb 01 00 00 00 66 90 48 8b 0d");
        #else
            bool m[10] = { true, true, true, true, true, true, true, true, true, true };  // Mask array
            uint8_t aob[10] = { 0xbf, 0xa0, 0x62, 0x60, 0x00, 0xe8, 0x7c, 0xcc, 0xff, 0xff }; // starts at &dummy_function() + 104
            const char *str_aob = "bf a0 62 60 00 e8 7c cc ff ff";
            char str_aob2[30] = { 'b', 'f', ' ', 'a', '0', ' ', '6', '2', ' ', '6', '0', ' ', '0', '0', ' ', 'e', '8', ' ', '7', 'c', ' ', 'c', 'c', ' ', 'f', 'f', ' ', 'f', 'f', '\0' };
            std::string str_aob3("bf a0 62 60 00 e8 7c cc ff ff");
        #endif // _WIN32


        // Test aob_scan using byte array:
        print_aob(aob, (size_t)10);
        std::cout << "   found at   " << aob_scan(aob, 10, NULL) << std::endl;

        // Test aob_scan using strings:
        std::cout << str_aob << "   found at   " << aob_scan(str_aob) << std::endl;
        std::cout << str_aob2 << "   found at   " << aob_scan(str_aob2) << std::endl;
        std::cout << str_aob3 << "   found at   " << aob_scan(&str_aob3) << std::endl;
    }
    else // this is a 32-bit process
    {
        // Define array to search for based on architecture and OS:
        #ifdef _WIN32
            bool m[10] = { true, true, true, true, true, true, true, true, true, true };  // Mask array
            uint8_t aob[10] = { 0xbe, 0x01, 0x00, 0x00, 0x00, 0x66, 0x66, 0x0f, 0x1f, 0x84 }; // starts at &dummy_function() + 26
			std::cout << "Address of byte array in main:  " << (void*)aob << std::endl;
            const char *str_aob = "BE 01 00 00 00 66 66 0F 1F 84";
            char str_aob2[30] = { 'b', 'e', ' ', '0', '1', ' ', '0', '0', ' ', '0', '0', ' ', '0', '0', ' ', '6', '6', ' ', '6', '6', ' ', '0', 'f', ' ', '1', 'f', ' ', '8', '4', '\0' };
            std::string str_aob3("BE 01 00 00 00 66 66 0F 1F 84");
        #else
            bool m[10] = { true, true, true, true, true, true, true, true, true, true };  // Mask array
            uint8_t aob[10] = { 0xe8, 0xd9, 0xd1, 0xff, 0xff, 0x83, 0xc4, 0x10, 0x83, 0x45 }; // starts at &dummy_function() + 102
            const char *str_aob = "e8 d9 d1 ff ff 83 c4 10 83 45";
            char str_aob2[30] = { 'e', '8', ' ', 'd', '9', ' ', 'd', '1', ' ', 'f', 'f', ' ', 'f', 'f', ' ', '8', '3', ' ', 'c', '4', ' ', '1', '0', ' ', '8', '3', ' ', '4', '5', '\0' };
            std::string str_aob3("e8 d9 d1 ff ff 83 c4 10 83 45");
        #endif // _WIN32


        // Test aob_scan using byte array:
        print_aob(aob, (size_t)10);
        std::cout << "   found at   " << aob_scan(aob, 10, NULL) << std::endl;

        // Test aob_scan using strings:
        std::cout << str_aob << "   found at   " << aob_scan(str_aob) << std::endl;
        std::cout << str_aob2 << "   found at   " << aob_scan(str_aob2) << std::endl;
        std::cout << str_aob3 << "   found at   " << aob_scan(&str_aob3) << std::endl;
    }

    return 0;
}

// Prints a formatted byte array with masked bytes:
void print_aob(uint8_t *aob, bool *mask, size_t len)
{
    for(int b = 0; b < len; b++)
    {
        if(mask[b])
        {
            char cb[5] = { '\0', '\0', '\0', '\0', '\0' };
            sprintf(cb, "%02x", aob[b]);
            std::cout << cb;
        }
        else
        {
            std::cout << "??";
        }
        if(b != len-1)
        {
            std::cout << " ";
        }
    }
}

// Prints a formatted byte array (without mask):
void print_aob(uint8_t *aob, size_t len)
{
    for(int b = 0; b < len; b++)
    {
        char cb[5] = { '\0', '\0', '\0', '\0', '\0' };
        sprintf(cb, "%02x", aob[b]);
        std::cout << cb;
        if(b != len-1)
        {
            std::cout << " ";
        }
    }
}

// Prints formatted byte mask:
void print_mask(bool *mask, size_t len)
{
    for(int b = 0; b < len; b++)
    {
        if(mask[b])
        {
            std::cout << "bb";
        }
        else
        {
            std::cout << "??";
        }
        if(b != len-1)
        {
            std::cout << " ";
        }
    }
}


// This function isn't called; it's just used to test AoB Scanning:
void dummy_function()
{
    std::cout << "This is a dummy function. It should never be called." << std::endl;
    for(int i = 1; i <= 20; i++)
    {
        std::cout << "i = " << i << std::endl;
    }
    std::cout << "We have counted to 20. Congratulations." << std::endl;
}
