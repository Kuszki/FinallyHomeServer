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

#include "server.hpp"
#include "callbacks.hpp"

using namespace KuszkAPI;

ServerCore Server;

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nFunsterStil)

{
    MSG mMsg;

    //Server.Listen(9096, ServerHandler);

    //Core::Threads<int> ConsThr;
    //ConsThr.Add(0, ConsoleHandler);
    //ConsThr.Start(0);

    while (GetMessage(&mMsg, NULL, 0, 0)){
        TranslateMessage(&mMsg);
        DispatchMessage(&mMsg);
    }

    return mMsg.wParam;
}

