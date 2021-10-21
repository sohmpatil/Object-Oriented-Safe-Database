# Object-Oriented-Safe-Database
The database creates backup of records if there are multiple entries are written in the database simultaneously.
It is prone to the failure while writing to the database.
It stops multiple writers from writing into the same file.


How to run on Windows machine\
\
mkdir -p bin/\
cl /D_USING_V110_SDK71_ /EHsc /Fobin/ /c src/read_main.cpp\
cl /D_USING_V110_SDK71_ /EHsc /Fobin/ /c src/write_main.cpp\
cl /D_USING_V110_SDK71_ /EHsc /Fobin/ /c src/update_main.cpp\
cl /D_USING_V110_SDK71_ /EHsc /Fobin/ /c src/state_functions.cpp\
cl /EHsc bin/state_functions.obj bin/write_main.obj /Febin/write_main.exe\
cl /EHsc bin/state_functions.obj bin/read_main.obj /Febin/read_main.exe\
cl /EHsc bin/state_functions.obj bin/update_main.obj /Febin/update_main.exe\
