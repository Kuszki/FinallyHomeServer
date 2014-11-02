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

HBRUSH g_hBkgnd = CreateSolidBrush(RGB(255,255,255));

MAP<STR, SOCKET> mBuffer;

LRESULT ServerHandler(SRV& srv, UINT event, SOCKET id)
{

     switch (event){

          case FD_ACCEPT:
          {

               Eng.EnterSection(SOCK_SECTION);

               Eng.OnConnect(id);

               char pcEcho[] = {255, 252, 1};
               char pcBin[] = {255, 251, 0};

               srv[id].Send(ARA<char>(pcEcho, 3));
               Sleep(10);
               srv[id].Send(ARA<char>(pcBin, 3));
               Sleep(10);
               srv[id] << PROMPT;

               mBuffer.Add(T(""), id);

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

				if (mBuffer[id]) Eng.OnRead(mBuffer[id], srv[id]); else srv[id] << PROMPT;

				mBuffer[id].Clean();

			}

		}
		break;

		case FD_CLOSE:

			Eng.EnterSection(SOCK_SECTION);

			Eng.OnDisconnect(id);

			mBuffer.Delete(id);

			Eng.LeaveSection(SOCK_SECTION);

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

		if (sMessage) psSrv->OnRead(sMessage); else psSrv->Console << PROMPT;

	}

	return 0;
}

LRESULT WINAPI WindowHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
     WND* fWnd = (WND*) GetWindowLong(hWnd, 0);

     switch (uMsg){
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
                              case CTR_BTN_ABOUT:
                              case CTR_BTN_HELP:
                              {
							Containers::Strings sMessage;

							sMessage.LoadFromFile(LOWORD(wParam) == CTR_BTN_HELP ? T("../doc/info.txt") : T("../doc/license.txt"));

							MessageBox(hWnd, sMessage.All().Str(), T("Informacje"), 0);
                              }
                              break;
                              case CTR_BTN_KICK:
                              {
							unsigned uSel = fWnd->Widgets.Tables[CTR_TABLE_CLI].GetIndex();

							if (!uSel) break;

							STR sMessage = STR(T("rcon kick ")) + fWnd->Widgets.Tables[CTR_TABLE_CLI].GetItem();

							Eng.OnRead(sMessage);
						}
						break;
						case CTR_BTN_RESET:
						{
							STR sMessage = T("rcon shutdown");

							Eng.OnRead(sMessage);

							sMessage = T("rcon listen");

							Eng.OnRead(sMessage);
						}
						break;
						case CTR_BTN_LOAD:
						{
							Forms::CommonDialog dDialog(hWnd);

							if (dDialog.GetFileOpen(NULL, T("Pliki konfiguracyjne\0*.ini\0"))) if (!Eng.LoadSettings(dDialog.GetLastFile().Full)) MessageBox(hWnd, T("Błąd przy wczytywaniu wartości z pliku"), NULL, MB_OK | MB_ICONERROR);
						}
						break;
						case CTR_BTN_SAVE:
						{
							Forms::CommonDialog dDialog(hWnd);

							if (dDialog.GetFileSave(T(".ini"), T("Pliki konfiguracyjne\0*.ini\0"))) Eng.SaveSettings(dDialog.GetLastFile().Full);
						}
						break;
						case CTR_CHK_CONS:
							Eng.Console.Show(fWnd->Widgets.Checks[LOWORD(wParam)].GetCheck());
						break;
						case CTR_CHK_DEBUG:
						{
							STR sMessage = T("rcon debug ");

							sMessage += S (unsigned) fWnd->Widgets.Checks[LOWORD(wParam)].GetCheck();

							Eng.OnRead(sMessage);
						}
						break;
						case CTR_CHK_LISTEN:
						{
							STR sMessage = T("rcon ");

							sMessage += fWnd->Widgets.Checks[LOWORD(wParam)].GetCheck() ? T("listen") : T("shutdown");

							Eng.OnRead(sMessage);
						}
						break;
						case CTR_CHK_SLO:
						case CTR_CHK_SBO:
						case CTR_CHK_SHO:
						case CTR_CHK_PLO:
						case CTR_CHK_PDC:
						{
							STR sMessage = T("set ");

							sMessage += Eng.GetControlVar(LOWORD(wParam));
							sMessage += fWnd->Widgets.Checks[LOWORD(wParam)].GetCheck() ? T(" 1") : T(" 0");

							Eng.OnRead(sMessage);
						}
						break;
                         }
                    break;
               }
          break;

          case WM_HSCROLL:
			switch (LOWORD(wParam)){
				case SB_ENDSCROLL:
				{
					STR sMessage = T("set ");

					sMessage += Eng.GetControlVar((INT) GetMenu((HWND) lParam)) + T(" ");
					sMessage += fWnd->Widgets.Tracks[(INT) GetMenu((HWND) lParam)].GetValue();

					Eng.OnRead(sMessage);
				}
				break;
			}
          break;

          case WM_CTLCOLORSTATIC:
		{
			HDC hdcStatic = (HDC) wParam;
			SetTextColor(hdcStatic, RGB(0, 0, 0));
			SetBkColor(hdcStatic, RGB(255, 255, 255));

			return (INT_PTR) g_hBkgnd;
		}
		break;

          default: return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}

unsigned ComportHandler(unsigned uDev, unsigned uSet)
{
	//STR sMsg = S uDev + S ":" + S uSet;

	//MessageBox(NULL, sMsg.Str(), "CALLBACK", 0);

	Eng.OnSet(uDev, uSet);

	return 0;
}
