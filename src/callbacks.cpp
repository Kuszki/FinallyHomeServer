/*********************************************

    FinallyHome Server "callback" module source

    copyright: KuszkiDevGroup

    license: GNU GPL v2

*********************************************/

/*! \file
    \brief Plik zawierający definicje dla modułu "callback".

Plik jest dołączany przez callbacks.hpp.
*/

extern ServerCore Eng;

LRESULT ServerHandler(SRV* srv, UINT event, SOCKET id)
{
     switch (event){

          case FD_ACCEPT:
               Eng.OnConnect(id);
          break;

		case FD_READ:
		{
			Containers::Array<char> aData;

			srv->Clients[id].Recv(aData);

			aData.Add(0);

			Eng.OnRead(aData);
		}
		break;

		case FD_CLOSE:
			Eng.OnDisconnect(id);
		break;
	}

	return 0;
}

DWORD WINAPI ConsoleHandler(LPVOID pvArgs)
{
	ServerCore* psSrv = (ServerCore*) pvArgs;

	while (true){

		Containers::String sTmp;

		psSrv->Console << T("\n$: ") >> sTmp << T("\n");


		if (sTmp) psSrv->Parse(sTmp);

	}

	return 0;
}
