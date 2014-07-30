/*********************************************

    FinallyHome Server "callback" module source

    copyright: KuszkiDevDroup

    license: GNU GPL v2

*********************************************/

/*! \file
    \brief Plik zawierający definicje dla modułu "callback".

Plik jest dołączany przez callbacks.hpp.
*/

LRESULT ServerHandler(SRV* srv, UINT event, SOCKET id)
{
    switch (event){
        case FD_ACCEPT:
            //Server.Console << "Polaczenie nawiazane, ID klienta: " << id << "; Adress: " << srv->Clients[id].GetAddress() << '\n';
        break;
        case FD_READ:
        {
            Containers::Array<char> aData;
            unsigned uParam = srv->Clients[id].Recv(aData);
            aData.Add(0);
            //Server.Console << "Odebrano bajtow: " << uParam << "; Tresc: \"" << aData.GetBegin() << "\"\n";
        }
        break;
        case FD_CLOSE:
            //Server.Console << "Polaczenie zakonczone, ID klienta: " << id << '\n';
        break;
    }

    return 0;
}

DWORD WINAPI ConsoleHandler(LPVOID pvArgs)
{
    while (true){
        Containers::String sTmp;
        //Server.Console >> sTmp;
        Containers::Array<char> aData(sTmp.Str(), sTmp.Capacity() + 1);
        //Server.Console << "Wyslano bajtow: " << aData.Capacity() + 1 << "; Dane: \"" << aData.GetBegin() << "\"\n";
        aData.Add('\n', -1);
        //Server.Announce(aData);
    }

    return 0;
}
