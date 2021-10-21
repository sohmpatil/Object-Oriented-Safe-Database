#ifndef     EMPLOYEE_INFO_HPP
#define     EMPLOYEE_INFO_HPP

#define     SIZE                (50)
#define     SECTOR_SIZE         (512)
#define     TOTAL_EMPLOYEES     (5)
#define     ZERO                ('0')
#define     NINE                ('9')
#define     TEN                 (10)
#define     MINUS               ('-')
#define     PLUS                ('+')
#define     START               (true)
#define     END                 (false)
#define     ERROR_NUMBER        (-9999999)
#define     NEW_LINE            ('\n')
#define     FILE_NAME           ("..//files//binary_file.dat")
#define     STATE_FILE_NAME     ("..//files//state_file.dat")
#define     ROLLBACK_NAME       ("..//files//rollback_file.dat")

enum eTeam {
    TEAM_ENG = 1,
    TEAM_SALES,
    TEAM_HR,
    TEAM_PM
};

enum eState {
    OK = 1,
    TRANS_START,
    ROLLBACK,
    STATE_ERROR
};

struct tState {
    eState  uState;
};

struct tEmployeeInfo {
    eTeam   uTeamType;
    int     uID;
    int     uSalary;
    int     uJoiningDate;
    int     uConveyance;
    int     uMedicalExpense;
    char    uName[SIZE];
};

#endif      //EMPLOYEE_INFO_HPP
