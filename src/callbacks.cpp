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

MAP<STR, SOCKET> mBuffer;

LRESULT ServerHandler(SRV& srv, UINT event, SOCKET id)
{

     switch (event){

          case FD_ACCEPT:
          {
               Eng.OnConnect(id);

               char pcEcho[] = {255, 252, 1};
               char pcBin[] = {255, 251, 0};

               Eng.EnterSection();

               srv[id].Send(ARA<char>(pcEcho, 3));
               Sleep(10);
               srv[id].Send(ARA<char>(pcBin, 3));
               Sleep(10);
               srv[id] << PROMPT;

               mBuffer.Add("", id);

               Eng.LeaveSection();
		}
          break;

		case FD_READ:
		{
			STR sMessage;

			srv[id] >> sMessage;

			if (sMessage[1] == (char) 255){

				// DEBUG!

				Eng.Console << T("\nTELNET MESSAGE: ");

				for (int i = 1; i <=3; i++) Eng.Console << T("[") << (unsigned char) sMessage[i] << T("]");

				Eng.Console << T(" EOM;\n");

				return 0;

			}

			mBuffer[id] += sMessage;

			Eng.EnterSection();

			if (mBuffer[id][0] == T('\n')){

				mBuffer[id].Delete(T('\n'), true);
				mBuffer[id].Delete(T('\r'), true);

				if (mBuffer[id]) Eng.OnRead<CLI>(mBuffer[id], srv[id]); else srv[id] << PROMPT;

				mBuffer[id].Clean();

			}

			Eng.LeaveSection();

		}
		break;

		case FD_CLOSE:
			Eng.OnDisconnect(id);

			mBuffer.Delete(id);
		break;
	}

	return 0;
}

DWORD WINAPI ConsoleHandler(LPVOID pvArgs)
{
	ServerCore* psSrv = (ServerCore*) pvArgs;

	psSrv->Console << T("\n$:");

	while (true){

		STR sMessage;

		psSrv->Console >> sMessage;

		psSrv->EnterSection();

		if (sMessage) psSrv->OnRead<CON>(sMessage, psSrv->Console); else psSrv->Console << PROMPT;

		psSrv->LeaveSection();

	}

	return 0;
}
