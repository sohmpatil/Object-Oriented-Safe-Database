#include <windows.h>
#include <stdio.h>

#include "employee_info.hpp"
#include "state_functions.hpp"

bool SetState (eState pFileState)
{
        HANDLE          state_file;
        tState          state_object;
        DWORD           bytes_written;
        DWORD           bytes_read;

    state_file  =  CreateFileA (
                                STATE_FILE_NAME,
                                GENERIC_WRITE | GENERIC_READ,
                                0,
                                NULL,
                                OPEN_ALWAYS,
                                FILE_ATTRIBUTE_NORMAL,
                                NULL);

    if (state_file == INVALID_HANDLE_VALUE) {

        printf ("Cannot open state_file.dat\n");
        return 1;
    }

    if (!ReadFile (state_file, &state_object, sizeof (tState), &bytes_read, NULL)) {

        printf ("Error while reading\n");
        CloseHandle (state_file);
        return 1;
    }

    state_object.uState = pFileState;
    SetFilePointer (state_file, 0, NULL, FILE_BEGIN);

    // Storing State of the file
    if (!WriteFile (state_file, &state_object, sizeof(tState), &bytes_written, NULL)) {

        printf ("Error while writing\n");
        CloseHandle (state_file);
        return 1;
    }

    CloseHandle (state_file);
    return 0;
}

eState GetState ()
{
        HANDLE          state_file;
        tState          state_object;
        DWORD           bytes_read;
        int             read_status;

    state_file  =  CreateFileA (
                                STATE_FILE_NAME,
                                GENERIC_READ,
                                0,
                                NULL,
                                OPEN_ALWAYS,
                                FILE_ATTRIBUTE_NORMAL,
                                NULL);

    if (state_file == INVALID_HANDLE_VALUE) {

        printf ("Cannot open state_file.dat\n");
        return STATE_ERROR;
    }

    if (!ReadFile (state_file, &state_object, sizeof (tState), &bytes_read, NULL)) {

        printf ("Error while reading\n");
        CloseHandle (state_file);
        return STATE_ERROR;
    }

    CloseHandle (state_file);
    return (state_object.uState);
}

bool CreateRollBackRecord (int pEmployeeId, HANDLE * pFileHandle)
{
        tEmployeeInfo       employee_object;
        HANDLE              rollback_file;
        DWORD               file_pointer    = 0;
        DWORD               bytes_read;
        DWORD               bytes_written;
        char                string_buffer[sizeof (tEmployeeInfo)];

    rollback_file  =  CreateFileA (
                                    ROLLBACK_NAME,
                                    GENERIC_WRITE | GENERIC_READ,
                                    0,
                                    NULL,
                                    OPEN_ALWAYS,
                                    FILE_ATTRIBUTE_NORMAL,
                                    NULL);

    if (rollback_file == INVALID_HANDLE_VALUE) {

        printf ("Cannot open rollback file\n");
        return 1;
    }

    SetFilePointer ((* pFileHandle), 0, NULL, FILE_BEGIN);

    while (1) {

        if (!ReadFile ((* pFileHandle), &employee_object, sizeof(tEmployeeInfo), &bytes_read, NULL)) {

            printf ("Error while reading\n");
            return 1;
        }

        if (bytes_read == 0) {

            printf ("Reached file end cannot find the ID\n");
            return 1;
        }

        file_pointer += bytes_read;

        if (employee_object.uID == pEmployeeId) {

            break;
        }
    }

    if (!WriteFile (rollback_file, &pEmployeeId, sizeof (int), &bytes_written, NULL)) {

        printf ("Error while writing\n");
        return 1;
    }

    SetFilePointer ((*pFileHandle), file_pointer, NULL, FILE_BEGIN);

    if (!ReadFile ((*pFileHandle), string_buffer, sizeof(tEmployeeInfo), &bytes_read, NULL)) {

        printf ("Error while reading\n");
        return 1;
    }

    if (!WriteFile (rollback_file, string_buffer, sizeof(tEmployeeInfo), &bytes_written, NULL)) {

        printf ("Error while writing\n");
        return 1;
    }

    CloseHandle (rollback_file);
    return 0;
}

bool DoRollBackRecord (HANDLE * pFileHandle)
{
        tEmployeeInfo           employee_object;
        HANDLE                  rollback_file;
        int                     employee_id;
        DWORD                   file_pointer    = 0;
        DWORD                   bytes_read;
        DWORD                   bytes_written;
        char                    string_buffer[sizeof (tEmployeeInfo)];

    rollback_file  =  CreateFileA (
                                    ROLLBACK_NAME,
                                    GENERIC_WRITE,
                                    0,
                                    NULL,
                                    OPEN_ALWAYS,
                                    FILE_ATTRIBUTE_NORMAL,
                                    NULL);

    if (rollback_file == INVALID_HANDLE_VALUE) {

        printf ("Cannot open file for reading\n");
        return 1;
    }

    if (!ReadFile (rollback_file, &employee_id, sizeof (int), &bytes_read, NULL)) {

        printf ("Error while reading\n");
        return 1;
    }

    while (1) {

        if (!ReadFile ((*pFileHandle), &employee_object, sizeof (tEmployeeInfo), &bytes_read, NULL)) {

            printf ("Error while reading\n");
            return 1;
        }

        if (bytes_read == 0) {

            printf ("Reached file end cannot find the ID\n");
            return 1;
        }

        file_pointer += bytes_read;

        if (employee_object.uID == employee_id) {

            break;
        }
    }

    if (!ReadFile (rollback_file, string_buffer, sizeof (tEmployeeInfo), &bytes_read, NULL)) {

        printf ("Error while reading\n");
        return 1;
    }

    SetFilePointer ((*pFileHandle), file_pointer, NULL, FILE_BEGIN);

    if (!WriteFile ((*pFileHandle), string_buffer, sizeof (tEmployeeInfo), &bytes_written, NULL)) {

        printf ("Error while writing\n");
        return 1;
    }

    CloseHandle(rollback_file);
    return 0;
}
