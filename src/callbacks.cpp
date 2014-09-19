/*********************************************

    FinallyHome Server "callback" module source

    copyright: KuszkiDevGroup

    license: GNU GPL v2

*********************************************/

/*! \file
    \brief Plik zawierający definicje dla modułu "callback".

Plik jest dołączany przez callbacks.hpp.
*/

#include <KuszkAPI.hpp>

#include "..\obj\FinallyHome_Server_private.h"

#include "macros.hpp"

#include "callbacks.hpp"
#include "server.hpp"

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

               Eng.EnterSection(SOCK_SECTION);

               srv[id].Send(ARA<char>(pcEcho, 3));
               Sleep(10);
               srv[id].Send(ARA<char>(pcBin, 3));
               Sleep(10);
               srv[id] << PROMPT;

               mBuffer.Add("", id);

               Eng.LeaveSection(SOCK_SECTION);
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

			Eng.EnterSection(SOCK_SECTION);

			if (mBuffer[id][0] == T('\n')){

				mBuffer[id].Delete(T('\n'), true);
				mBuffer[id].Delete(T('\r'), true);

				if (mBuffer[id]) Eng.OnRead(mBuffer[id], srv[id]); else srv[id] << PROMPT;

				mBuffer[id].Clean();

			}

			Eng.LeaveSection(SOCK_SECTION);

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

	psSrv->Console << PROMPT;

	while (true){

		STR sMessage;

		psSrv->Console >> sMessage;

		psSrv->EnterSection(CONS_SECTION);

		if (sMessage) psSrv->OnRead(sMessage); else psSrv->Console << PROMPT;

		psSrv->LeaveSection(CONS_SECTION);

	}

	return 0;
}
