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

bool UpdateFile (int pEmployeeID, HANDLE * pHandleFile)
{
        tEmployeeInfo       employee_object;
        HANDLE              file_handle;
        int                 user_choice         = 0;
        int                 update_value        = 0;
        DWORD               file_pointer        = 0;
        DWORD               bytes_read;
        DWORD               bytes_written;

    file_handle  =  (* pHandleFile);

    if (SetState (ROLLBACK)) {

        printf ("Error in SetState function\n");
        return 1;
    }

    if (CreateRollBackRecord (pEmployeeID, &file_handle)) {

        printf ("Error while creating rollback record\n");
        return 1;
    }

    if (SetState (OK)) {

        printf ("Error in SetState function\n");
        return 1;
    }

    SetFilePointer (file_handle, 0, NULL, FILE_BEGIN);

    while (1) {

        if (!ReadFile (file_handle, &employee_object, sizeof(tEmployeeInfo), &bytes_read, NULL)) {

            printf ("Error while reading\n");
            return 1;
        }

        file_pointer += bytes_read;

        if (employee_object.uID == pEmployeeID) {

            printf ("What field you want to update\n");
            printf (" 1. Salary \n 2. Joining Date \n 3. Medical Expense\n 4. Conveyance \n");
            user_choice = GetInputAsInteger ();

            if (user_choice == ERROR_NUMBER) {

                printf ("Invalid Input\n");
                return 1;
            }

            switch (user_choice) {
                case 1:

                    printf ("Enter the salary: ");
                    update_value = GetInputAsInteger ();

                    if (update_value == ERROR_NUMBER) {

                        printf ("Invalid input\n");
                        return 1;
                    }

                    employee_object.uSalary = update_value;
                    break;
                case 2:

                    printf ("Enter the joining date: ");
                    update_value = GetInputAsInteger ();

                    if (update_value == ERROR_NUMBER) {

                        printf ("Invalid input\n");
                        return 1;
                    }

                    employee_object.uJoiningDate = update_value;
                    break;
                case 3:

                    printf ("Enter Medical Expense : ");

                    update_value = GetInputAsInteger ();
                    if (update_value == ERROR_NUMBER) {

                        printf ("Invalid input\n");
                        return 1;
                    }

                    employee_object.uMedicalExpense = update_value;
                    break;
                case 4:

                    printf ("Enter conveyance: ");
                    update_value = GetInputAsInteger ();

                    if (update_value == ERROR_NUMBER) {

                        printf ("Invalid input\n");
                        return 1;
                    }

                    employee_object.uConveyance = update_value;
                    break;
                default:

                    printf ("Invalid input\n");
                    break;
            }
            break;
        }
    }

    if (SetState (TRANS_START)) {

        printf ("Error in SetState Function\n");
        return 1;
    }

    file_pointer -= bytes_read;

    SetFilePointer (file_handle, file_pointer, NULL, FILE_BEGIN);

    if (!WriteFile (file_handle, &employee_object, sizeof(tEmployeeInfo), &bytes_written, NULL)) {

        printf ("Error while writing");
        return 1;
    }

    if (SetState (OK)) {

        printf ("Error in SetState Function\n");
        return 1;
    }

    return 0;
}

int main ()
{
        HANDLE      file_handle;
        int         employee_id;

    //create file with win 32 api
    file_handle = CreateFileA (
                                FILE_NAME,
                                GENERIC_READ | GENERIC_WRITE,
                                0,
                                NULL,
                                OPEN_ALWAYS,
                                FILE_ATTRIBUTE_NORMAL,
                                NULL);

    if (file_handle == INVALID_HANDLE_VALUE) {

        printf("Cannot open binary_file.dat\n");
        return 1;
    }

    //Update the employee using ID
    printf ("Enter the employee id : ");
    employee_id = GetInputAsInteger ();

    if (employee_id == ERROR_NUMBER) {

        printf ("Invalid Input\n");
        return 1;
    }

    if (UpdateFile (employee_id, &file_handle)) {

        printf ("Error in updating records\n");
        return 1;
    }

    // close file
    CloseHandle (file_handle);
    return 0;
}

