/*********************************************

    FinallyHome Server "core" main source

    copyright: KuszkiDevDroup

    license: GNU GPL v2

*********************************************/

/*! \file
    \brief Główny plik projektu.

Dołącza automatycznie wszystkie zależności.
*/

#include "..\..\KuszkAPI\KuszkAPI.hpp"

#include "callbacks.hpp"
#include "server.hpp"

#include "callbacks.cpp"
#include "server.cpp"

using namespace KuszkAPI;

ServerCore Server(true);

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

