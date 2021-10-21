#include <windows.h>
#include <stdio.h>

#include "employee_info.hpp"
#include "state_functions.hpp"

int GetInputAsInteger ()
{
        int     number          = 0;
        int     symbol_count    = 0;
        bool    is_negative     = 0;
        char    user_input;

    while (true) {

        user_input = getchar ();

        if (user_input >= ZERO && user_input <= NINE) {

            number = number * TEN + (user_input - ZERO);
        } else if (user_input == MINUS) {

            is_negative = true;
            symbol_count++;
            break;
        } else if (user_input == PLUS) {

            symbol_count++;
        } else if (user_input == NEW_LINE || user_input == 0) {

            break;
        } else {

            return ERROR_NUMBER;
        }
    }

    if (is_negative == true || symbol_count > 1) {

        return ERROR_NUMBER;
    }

    return number;
}

//update the db by adding the employee info to the data
bool WriteToFile (tEmployeeInfo * pInfo, int pEmployeeNumber)
{
        HANDLE      file_handle;
        DWORD       byte_written    = 0;

    //create file with win 32 api
    file_handle = CreateFileA (
                                FILE_NAME,
                                GENERIC_WRITE,
                                FILE_SHARE_WRITE,
                                NULL,
                                OPEN_ALWAYS,
                                FILE_ATTRIBUTE_NORMAL,
                                NULL);

    if (file_handle == INVALID_HANDLE_VALUE) {

        printf("Cannot open binary_file.dat\n");
        return false;
    }

    printf (" Enter the employee details of employee %d\n Name:\n", pEmployeeNumber + 1);
    scanf ("%s", pInfo->uName);
    getchar ();

    printf("Employee ID: \n");
    pInfo->uID = GetInputAsInteger ();

    if (pInfo->uID == ERROR_NUMBER) {

       printf ("Error while reading input as integer\n");
       return false;
    }

    printf ("Salary:\n");
    pInfo->uSalary = GetInputAsInteger ();

    if (pInfo->uSalary == ERROR_NUMBER) {

        printf ("Error while reading input as integer\n");
        return false;
    }

    printf ("Joining Date(format: yyyymmdd):\n");
    pInfo->uJoiningDate = GetInputAsInteger ();

    if (pInfo->uJoiningDate == ERROR_NUMBER) {

        printf ("Error while reading input as integer\n");
        return false;
    }

    printf ("Conveyance:\n");
    pInfo->uConveyance = GetInputAsInteger ();

    if (pInfo->uConveyance == ERROR_NUMBER) {

        printf ("Error while reading input as integer\n");
        return false;
    }

    printf ("Medical expenses:\n");
    pInfo->uMedicalExpense = GetInputAsInteger ();

    if (pInfo->uMedicalExpense == ERROR_NUMBER) {

        printf ("Error while reading input as integer\n");
        return false;
    }

    printf ("Enter team type 1. Engineering, 2. Sales, 3. HR, 4. PM\n");
    scanf("%d", &pInfo->uTeamType);

    if (pInfo->uTeamType < 1 || pInfo->uTeamType > 4) {

        printf ("Error while reading input as integer\n");
        return false;
    }

    SetFilePointer (file_handle, 0, NULL, FILE_END);

    //write numbers into the file
    if (!WriteFile (file_handle, pInfo, sizeof(tEmployeeInfo), &byte_written, NULL)) {

        printf ("Error while writing\n");
        CloseHandle (file_handle);
        return false;
    }

    CloseHandle (file_handle);
    return true;
}

int main ()
{
        tEmployeeInfo *     write_employee;
        int                 employee_count;
        int                 counter;

    printf("Enter number of employees to enter\n");
    employee_count = GetInputAsInteger ();

    if (employee_count == ERROR_NUMBER) {

        printf ("Error while taking input\n");
        return 1;
    }

    write_employee = (tEmployeeInfo *) malloc (sizeof(tEmployeeInfo) * employee_count);

    for (counter = 0; counter < employee_count; counter++) {

        if (!WriteToFile (write_employee + counter, counter)) {

            printf ("Error while writing\n");
            return 1;
        }
    }

    return 0;
}
