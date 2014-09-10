/*********************************************

    FinallyHome Server "callback" module source

    copyright: KuszkiDevDroup

    license: GNU GPL v2

*********************************************/

/*! \file
    \brief Plik zawierający definicje dla modułu "callback".

Plik jest dołączany przez callbacks.hpp.
*/

extern ServerCore Server;

LRESULT ServerHandler(SRV* srv, UINT event, SOCKET id)
{
     switch (event){

          case FD_ACCEPT:
               Server.OnConnect(id);
          break;

		case FD_READ:
		{
			Containers::Array<char> aData;

			srv->Clients[id].Recv(aData);

			aData.Add(0);

			Server.OnRead(aData);
		}
		break;

		case FD_CLOSE:
			Server.Console << T("Polaczenie zakonczone, ID klienta: ") << id << T('\n');

		break;
	}

	return 0;
}

DWORD WINAPI ConsoleHandler(LPVOID pvArgs)
{
	while (true){

		Containers::String sTmp;
		Server.Console >> sTmp;

		Containers::Array<char> aData(sTmp.Str(), sTmp.Capacity());

		aData.Add(T("\n\0"), 2);

		//Server.Announce(aData);

		Sleep(100);
	}

	return 0;
}
