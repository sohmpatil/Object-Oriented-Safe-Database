#ifndef     STATE_FUNCTIONS_HPP
#define     STATE_FUNCTIONS_HPP

bool SetState (eState pFileState);
eState GetState ();
bool CreateRollBackRecord (int pEmployeeId, HANDLE * pFileHandle);
bool DoRollBackRecord (HANDLE * pFileHandle);

#endif      //STATE_FUNCTIONS_HPP
