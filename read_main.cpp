#include <windows.h>
#include <stdio.h>

#include "employee_info.hpp"
#include "state_functions.hpp"

bool ReadFromFile ()
{
        tEmployeeInfo       info;
        HANDLE              file_handle;
        DWORD               bytes_read      = 0;
        int                 employee_count  = 0;
        int                 total_expenses  = 0;
        int                 total_salary    = 0;

    //create file with win 32 api
    file_handle = CreateFileA (
                                FILE_NAME,
                                GENERIC_READ,
                                0,
                                NULL,
                                OPEN_ALWAYS,
                                FILE_ATTRIBUTE_NORMAL,
                                NULL);

    if (file_handle == INVALID_HANDLE_VALUE) {

        printf("Cannot open binary_file.dat\n");
        return false;
    }

    while (1) {

        //read from file
        if (!ReadFile (file_handle, &info, sizeof (tEmployeeInfo), &bytes_read, NULL)) {

            printf ("Error while reading\n");
            CloseHandle (file_handle);
            return false;
        }

        if (bytes_read == 0) {

            break;
        }

        employee_count++;
        total_salary += info.uSalary;

        if (info.uTeamType == TEAM_SALES) {

            total_expenses += info.uConveyance + info.uMedicalExpense;
        }
    }

    printf ("The count of employees is %d\n", employee_count);
    printf ("The sales team expenses are %d\n", total_expenses);
    printf ("The total salary by company is %d\n", total_salary);

    // close file
    CloseHandle (file_handle);
    return true;
}

int main ()
{
    if (!ReadFromFile ()) {

        printf ("Error while reading\n");
        return 1;
    }

    return 0;
}
