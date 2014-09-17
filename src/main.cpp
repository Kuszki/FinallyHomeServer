/*********************************************

    FinallyHome Server "core" main source

    copyright: KuszkiDevGroup

    license: GNU GPL v2

*********************************************/

/*! \file
    \brief Główny plik projektu.

Dołącza automatycznie wszystkie zależności.
*/

/*! \mainpage FinallyHomeServer

    \section basic_sec O projekcie

    Serwer aplikacji <a target="_blank" href="https://github.com/Kuszki/FinallyHome">FinallyHome</a> zaprojektowany na platformę Win32. Kod źródłowy znajdziesz <a target="_blank" href="https://github.com/Kuszki/FinallyHomeServer">tutaj</a>.

    Jeśli znalazłeś błąd, masz jakieś propozycje lub uwagi, zgłoś to w <a target="_blank" href="https://github.com/Kuszki/FinallyHomeServer/issues">bugtrackerze</a> na GitHubie.

*/

#include "..\..\KuszkAPI\KuszkAPI.hpp"

#define PROMPT T("\r\n$: ")

#include "callbacks.hpp"
#include "server.hpp"

#include "callbacks.cpp"
#include "server.cpp"

using namespace KuszkAPI;

ServerCore Eng(true);

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nFunsterStil)

{
    MSG mMsg;
    
    /*
    TCHAR* pcBufor = new TCHAR[MAX_PATH];
    unsigned uTmp = GetPrivateProfileString(NULL, NULL, NULL, pcBufor, MAX_PATH, "c:/config.ini");

    Containers::Vector<char>::Change(pcBufor, uTmp, 0, 10, true);

    pcBufor[uTmp - 1] = 0;

    STR err((unsigned)GetLastError());

    MessageBox(NULL, pcBufor, err.Str(), 0);
	*/
    while (GetMessage(&mMsg, NULL, 0, 0)){
        TranslateMessage(&mMsg);
        DispatchMessage(&mMsg);
    }

    return mMsg.wParam;
}

