/*********************************************

    FinallyHome Server "callback" module source

    copyright: KuszkiDevGroup

    license: GNU GPL v2

*********************************************/

/*! \file
    \brief Plik zawierający definicje dla modułu "callback".

Implementuje funkcje ServerHandler i ConsoleHandler.
*/

#include KUSZKAPI_PATH

#include "..\FinallyHome_Server_private.h"

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

			if (sMessage[1] == (char) 255) return 0;

			mBuffer[id] += sMessage;

			if (mBuffer[id][0] == T('\n')){

				mBuffer[id].Delete(T('\n'), true);
				mBuffer[id].Delete(T('\r'), true);

				Eng.EnterSection(SOCK_SECTION);

				if (mBuffer[id]) Eng.OnRead(mBuffer[id], srv[id]); else srv[id] << PROMPT;

				Eng.LeaveSection(SOCK_SECTION);

				mBuffer[id].Clean();

			}

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

LRESULT WINAPI WindowHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
     WND* fWnd = (WND*) GetWindowLong(hWnd, 0);

     switch (uMsg){
          case WM_CREATEWINDOW:

          break;

          case WM_DESTROY:

               Eng.Stop();

               PostQuitMessage(0);

          break;

          case WM_NOTIFY:
		{
			LPNMHDR nInfo = (LPNMHDR) lParam;

			if (nInfo->code == TCN_SELCHANGE) fWnd->Widgets.Tabs[CTR_TAB_CATS].SetTab();
		}
		break;

          case WM_COMMAND:
               switch (HIWORD(wParam)){
                    case BN_CLICKED:
                         switch (LOWORD(wParam)){
                              case 301:
                              {

                              }
                         }
                    break;
               }
          break;

          case WM_CTLCOLORSTATIC:
		{
			HDC hdcStatic = (HDC) wParam;
			SetTextColor(hdcStatic, 0);
			SetBkColor(hdcStatic, 0xFFFFFF);
		}
		break;

          default: return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0;
}
