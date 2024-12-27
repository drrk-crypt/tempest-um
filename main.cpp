#include <Windows.h>
#include <iostream>
#include <conio.h>
#include <string>

#define initialize CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0072, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define custom_code CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0323, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

DWORD bReturned = 0;

struct req_struct {
    const char* serial;
};

std::string custom_serial;

int main()
{
    try {


        HANDLE driver_handle = CreateFileA("\\\\.\\PhysicalDrive0", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0); // The handle to our vuln
        if (driver_handle == INVALID_HANDLE_VALUE)
        {
            throw std::runtime_error(skCrypt("Code 0x555")); // Vuln was not started ;(
        }


        DeviceIoControl(driver_handle, initialize, 0, 0, 0, 0, &bReturned, NULL); // If this is not called the driver will think someone is trying to crack it by sending invalid codes so PC WILL BSOD
        Sleep(1000); // Sleep to make sure req went smoothly


        req_struct req_handle;
        req_handle.serial = custom_serial.c_str(); // This will be the disk serial


        DeviceIoControl(driver_handle, custom_code, &req_handle, sizeof(req_handle), 0, 0, &bReturned, NULL); // Send the serial to the driver
        Sleep(1000); // Sleep to make sure req went smoothly

        // For the love of god restart SMBios cache using these CMDs
        system(skCrypt("net stop winmgmt /y >nul"));
        Sleep(1500); 
        system(skCrypt("net start winmgmt >nul"));


        // Hooray!
    }
    catch (exception e) {
        std::cout << e.what();
    }

    return 0;
}
