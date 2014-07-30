#include "..\..\KuszkAPI\KuszkAPI.hpp"

using namespace KuszkAPI;

SRV Server;
CON Terminal(true);

LRESULT ServerHandler(SRV* srv, UINT event, SOCKET id)
{
    switch (event){
        case FD_ACCEPT:
            Terminal << "Polaczenie nawiazane, ID klienta: " << id << "; Adress: " << srv->Clients[id].GetAddress() << '\n';
        break;
        case FD_READ:
        {
            Containers::Array<char> aData;
            unsigned uParam = srv->Clients[id].Recv(aData);
            aData.Add(0);
            Terminal << "Odebrano bajtow: " << uParam << "; Tresc: \"" << aData.GetBegin() << "\"\n";
        }
        break;
        case FD_CLOSE:
            Terminal << "Polaczenie zakonczone, ID klienta: " << id << '\n';
        break;
    }
              
    return 0;
}

DWORD WINAPI ConsoleHandler(LPVOID pvArgs)
{
    while (true){
        Containers::String sTmp;
        Terminal >> sTmp;
        Containers::Array<char> aData(sTmp.Str(), sTmp.Capacity() + 1);
        Terminal << "Wyslano bajtow: " << aData.Capacity() + 1 << "; Dane: \"" << aData.GetBegin() << "\"\n";
        aData.Add('\n', -1);
        Server.Announce(aData);
    }
    
    return 0;
}

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nFunsterStil)

{    
    MSG mMsg;
    
    Server.Listen(9096, ServerHandler);
    
    Core::Threads<int> ConsThr;
    ConsThr.Add(0, ConsoleHandler);
    ConsThr.Start(0);

    while (GetMessage(&mMsg, NULL, 0, 0)){
        TranslateMessage(&mMsg);
        DispatchMessage(&mMsg);
    }

    return mMsg.wParam;
}
