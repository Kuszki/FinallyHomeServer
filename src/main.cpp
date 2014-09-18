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

#ifndef _FINALLYHOMESERVER_MAIN
#define _FINALLYHOMESERVER_MAIN

#include "..\..\KuszkAPI\KuszkAPI.hpp"

#define PROMPT T("\r\n$: ")

#define SOCK_SECTION 1
#define CONS_SECTION 2
#define CORE_SECTION 3

#include "..\obj\FinallyHome_Server_private.h"

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

     while (GetMessage(&mMsg, NULL, 0, 0)){
          TranslateMessage(&mMsg);
          DispatchMessage(&mMsg);
     }

     return mMsg.wParam;
}

#endif
