/*********************************************

    FinallyHome Server "core" main source

    copyright: KuszkiDevGroup

    license: GNU GPL v2

*********************************************/

/*! \file
    \brief Główny plik projektu.

Zawiera pętle obsługi zdarzeń dla głównego wątku oraz globalną instancję klasy ServerCore.
*/

/*! \mainpage FinallyHomeServer

    \section basic_sec O projekcie

    Serwer aplikacji <a target="_blank" href="https://github.com/Kuszki/FinallyHome">FinallyHome</a> zaprojektowany na platformę Win32. Kod źródłowy znajdziesz <a target="_blank" href="https://github.com/Kuszki/FinallyHomeServer">tutaj</a>.

    Jeśli znalazłeś błąd, masz jakieś propozycje lub uwagi, zgłoś to w <a target="_blank" href="https://github.com/Kuszki/FinallyHomeServer/issues">bugtrackerze</a> na GitHubie.

*/

#ifndef _FINALLYHOMESERVER_MAIN
#define _FINALLYHOMESERVER_MAIN

#include KUSZKAPI_PATH

#include "..\FinallyHome_Server_private.h"

#include "macros.hpp"

#include "callbacks.hpp"
#include "server.hpp"

CON Console;
WND Window;

ServerCore Eng(Console, Window);

int WINAPI WinMain (HINSTANCE hThisInstance,
				HINSTANCE hPrevInstance,
				LPSTR lpszArgument,
				int nFunsterStil)

{
     MSG mMsg;

     Eng.Initiate(Containers::Strings(lpszArgument, T(' '), true));

     while (GetMessage(&mMsg, NULL, 0, 0)){
          TranslateMessage(&mMsg);
          DispatchMessage(&mMsg);
     }

     return mMsg.wParam;
}

#endif
