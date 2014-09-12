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
          {
               Eng.OnConnect(id);

               char pcEcho[] = {255, 252, 1};
               char pcBin[] = {255, 251, 0};

               Eng.EnterSection();

               srv->Clients[id].Send(ARA<char>(pcEcho, 3));
               Sleep(10);
               srv->Clients[id].Send(ARA<char>(pcBin, 3));

               Eng.LeaveSection();
		}
          break;

		case FD_READ:
		{
			STR sMessage;

			srv->Clients[id] >> sMessage;

			if (sMessage[1] == (char) 255) return 0;

			sMessage.Delete(T('\n'), true);
			sMessage.Delete(T('\r'), true);

			Eng.EnterSection();

			if (sMessage) Eng.OnRead<CLI>(sMessage, srv->Clients[id]);

			Eng.LeaveSection();

			srv->Clients[id] << T("\n\r$: ");
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

		STR sMessage;

		psSrv->Console << T("\n$: ") >> sMessage << T("\n");

		psSrv->EnterSection();

		if (sMessage) psSrv->OnRead<CON>(sMessage, psSrv->Console);

		psSrv->LeaveSection();

	}

	return 0;
}
