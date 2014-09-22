/*********************************************

    FinallyHome Server "core" module source

    copyright: KuszkiDevGroup

    license: GNU GPL v2

*********************************************/

/*! \file
    \brief Plik zawierający definicje dla modułu "core".

Plik jest dołączany przez server.hpp.
*/

#include <KuszkAPI.hpp>

#include "..\obj\FinallyHome_Server_private.h"

#include "macros.hpp"

#include "callbacks.hpp"
#include "server.hpp"

ServerCore::ServerCore(bool bDbg)
: Console(true), bDebug(bDbg)
{

	Console.SetTitle(STR(T(PRODUCT_NAME)) + STR(T(" v")) + STR(T(VER_STRING)));

	IF_DEBUG Console << T("\n >> Inicjuje sekcje krytyczne\t");

	tThr.Add(CONS_SECTION, ConsoleHandler);

	sSec.Add(CORE_SECTION);
	sSec.Add(CONS_SECTION);
	sSec.Add(SOCK_SECTION);

	IF_DEBUG Console << T("[OK]\n");

     LoadSettings();

     Start();

     IF_DEBUG Console << T("\n >> Tworze watek wejscia konsoli\t");

     tThr.Start(CONS_SECTION, this);

     IF_DEBUG Console << T("[OK]\n");

     Sleep(100);

}

ServerCore::~ServerCore(void)
{

	SaveSettings();

}

void ServerCore::EnterSection(unsigned uSection)
{

	sSec.Enter(uSection);

}

void ServerCore::LeaveSection(unsigned uSection)
{

	sSec.Leave(uSection);

}

bool ServerCore::Start(void)
{

	Console << T("\n >> Uruchamiam serwer\t");

	bool bOK = sSrv.Listen(mSets[S T("port")], ServerHandler);

	Console << (bOK ? T("[OK]\n") : T("[FAIL]\n"));

	return bOK;

}

bool ServerCore::Stop(void)
{

	Console << T("\n >> Wylaczam serwer...\t");

	EnterSection(CORE_SECTION);

	sSrv.Shutdown();

	LeaveSection(CORE_SECTION);

	Console << T("[OK]\n");

	return true;

}

void ServerCore::Disconnect(SOCKET sClient)
{

	OnDisconnect(sClient);

	sSrv.Disconnect(sClient);

}

void ServerCore::Shutdown(void)
{

	Stop();

	ExitProcess(0);

}

void ServerCore::LoadSettings(const STR& sFile)
{

	INI iConfig(sFile);

	IF_DEBUG Console << T("\n >> Wczytuje wartosci zmiennych z pliku '") << sFile << T("'\n");

	IF_DEBUG Console << T("\nZmienne projektu:\n\n");

	mVars = iConfig.GetIntValues(T("VARS"));

	IF_DEBUG for (int i = 1; i <= mVars.Capacity(); i++) Console << T("\t") << mVars.GetKey(i) << T(" = ") << mVars.GetDataByInt(i) << T("\n");

	IF_DEBUG Console << T("\nZmienne programu:\n\n");

	mSets = iConfig.GetIntValues(T("SRV"));

	IF_DEBUG for (int i = 1; i <= mSets.Capacity(); i++) Console << T("\t") << mSets.GetKey(i) << T(" = ") << mSets.GetDataByInt(i) << T("\n");

	IF_DEBUG Console << T("\n << Wczytywanie zakonczone\n");
	else Console << T("\n >> Wczytuje wartosci zmiennych z pliku '") << sFile << T("'\t[OK]");

}

void ServerCore::SaveSettings(const STR& sFile)
{

	INI iConfig(sFile);

	IF_DEBUG Console << T("\n >> Zapisuje wartosci zmiennych do pliku '") << sFile << T("\n");

	IF_DEBUG Console << T("\nZmienne projektu:\n\n");

	iConfig.SetIntValues(mVars, T("VARS"));

	IF_DEBUG for (int i = 1; i <= mVars.Capacity(); i++) Console << T("\t") << mVars.GetKey(i) << T(" = ") << mVars.GetDataByInt(i) << T("\n");

	IF_DEBUG Console << T("\nZmienne programu:\n\n");

	iConfig.SetIntValues(mSets, T("SRV"));

	IF_DEBUG for (int i = 1; i <= mSets.Capacity(); i++) Console << T("\t") << mSets.GetKey(i) << T(" = ") << mSets.GetDataByInt(i) << T("\n");

	IF_DEBUG Console << T("\n << Zapisywanie zakonczone\n"); else Console << T("\n >> Zapisuje wartosci zmiennych do pliku '") << sFile << T("'\t[OK]");

}

void ServerCore::OnConnect(SOCKET sClient)
{

	Console << T("\n >> Polaczono z klientem\n\tID: ") << (int) sClient << T("\n\tAdres: ") << sSrv[sClient].GetAddress() << T("\n");

}

void ServerCore::OnDisconnect(SOCKET sClient)
{

	Console << T("\n >> Rozlaczono Klienta\n\tID: ") << (int) sClient << T("\n");

}

void ServerCore::OnVarChange(const STR& sVar, int iValue)
{

	STR sMessage = S T("set ") + sVar + S T(" ") + S iValue + S T("\n");

	mVars[sVar] = iValue;

	for (int i = 1; i <= sSrv.Capacity(); i++) sSrv.GetClient(i) << sMessage;

}

void ServerCore::OnRead(const STR& sMessage, SOCKET sClient)
{

	STR sBuffer;

	IF_DEBUG Console << T("\n >> Odebrano wiadomosc: '") << sMessage << T("'\n");

	unsigned uAction = Parse<STR>(sMessage, sBuffer);

	sBuffer << PROMPT;

	if (sClient){

		if (uAction != CMD_BYE) sSrv[sClient] << sBuffer;
		else Disconnect(sClient);

	} else Console << sBuffer;

}

template<typename tnTerminal>
unsigned ServerCore::Parse(const STR& sInput, tnTerminal& tTerminal)
{

	Containers::Strings sAction(sInput, T(' '));

	STR sCommand = sAction[1];

	sAction.Delete(1);

	unsigned uCode = CMD_UNKNOWN;

	if (sCommand == T("rcon")) uCode = CMD_RCON;
	else if (sCommand == T("set")) uCode = CMD_SET;
	else if (sCommand == T("get")) uCode = CMD_GET;
	else if (sCommand == T("bye")) uCode = CMD_BYE;

	IF_DEBUG {

		Console << T("\r\n >> Interpretuje polecenie: ") << sCommand << T("\r\n");

		for (int i = 1; i <= sAction.Capacity(); i++) Console << T("\r\n\tParametr ") << i << T(": '") << sAction[i] << T("'");

		Console << T("\r\n");

	}

	Interpret<tnTerminal>(uCode, sAction, tTerminal);

	IF_DEBUG Console << T("\r\n << Parsowanie zakonczone\r\n");

	return uCode;

}

template<typename tnTerminal>
void ServerCore::Interpret(unsigned uCode, Containers::Strings& sParams, tnTerminal& tTerminal)
{

	switch (uCode){

		case CMD_RCON:

			if (!sParams) return;

			else if (sParams[1] == T("listen")) Start();
			else if (sParams[1] == T("shutdown")) Stop();
			else if ((sParams[1] == T("quit")) || (sParams[1] == T("exit"))) Shutdown();

			else if (sParams[1] == T("save")) {if (sParams.Capacity() == 2) SaveSettings(sParams[2]); else SaveSettings();}

			else if (sParams[1] == T("set") && sParams.Capacity() == 3) mSets[sParams[2]] = (int) sParams[3];

			else if (sParams[1] == T("debug") && sParams.Capacity() == 2) bDebug = (int) sParams[2];

			else if (sParams[1] == T("kick") && sParams.Capacity() == 2) sSrv.Disconnect((int) sParams[2]);

			else if (sParams[1] == T("show") && sParams.Capacity() == 2){

				if (sParams[2] == T("config")){

					tTerminal << T("\r\nZmienne projektu:\r\n\r\n");

					for (int i = 1; i <= mVars.Capacity(); i++) tTerminal << S T("\t") << S mVars.GetKey(i) << S T(" = ") << S mVars.GetDataByInt(i) << S T("\r\n");

					tTerminal << S T("\r\nZmienne programu:\r\n\r\n");

					for (int i = 1; i <= mSets.Capacity(); i++) tTerminal << S T("\t") << S mSets.GetKey(i) << S T(" = ") << S mSets.GetDataByInt(i) << S T("\r\n");

					tTerminal << S T("\r\n");

				}

				if (sParams[2] == T("clients")){

					if (sSrv.Capacity()){

						tTerminal << S T("\r\n >> Lista klientow:\r\n\r\n");

						for (int i = 1; i <= sSrv.Capacity(); i++) tTerminal << S T("\t") << S i << S T(": ID: [") << S (int) sSrv.GetClient(i).GetSocket() << S T("] @: [") << S sSrv.GetClient(i).GetAddress() << S T("]\r\n");

						tTerminal << S T("\r\n");

					} else tTerminal << T("\r\n >> Brak polaczonych klientow\r\n");

				}

			}

			else tTerminal << S T(" >> Nieznane parametry polecenia lub nieprawidlowa liczba parametrow\n\t");

		break;

		case CMD_SET:

			if (!sParams) return;

			else if (sParams.Capacity() == 2) if (mVars.Contain(sParams[1])) OnVarChange(sParams[1], (int) sParams[2]);

			else tTerminal << S T(" >> Nieprawidlowa liczba parametrow lub niezdefiniowana zmienna\n\t");

		break;

		case CMD_GET:

			if (!sParams) return;

			else if (sParams[1] == T("*")) for (int i = 1; i <= mVars.Capacity(); i++) tTerminal << S T("set ") << mVars.GetKey(i) << S T(" ") << S mVars.GetDataByInt(i) << S T("\r\n");

			else if (mVars.Contain(sParams[1])) tTerminal << S T("set ") << sParams[1] << S T(" ") << S mVars[sParams[1]] << S T("\r\n");

			else tTerminal << S T(" >> Niezdefiniowana zmienna\r\n");

		break;

		default: tTerminal << S T(" >> Nieznane polecenie\r\n");

	}

}
