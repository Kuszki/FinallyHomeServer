/*********************************************

    FinallyHome Server "core" module source

    copyright: KuszkiDevGroup

    license: GNU GPL v2

*********************************************/

/*! \file
    \brief Plik zawierający definicje dla modułu "core".

Plik jest dołączany przez server.hpp.
*/

ServerCore::ServerCore(bool bDebug)
: Console(true)
{

	Console << T("\n >> Inicjuje liste zmiennych\t");

	mVars.Add(0, T("porch.light.on"));
	mVars.Add(0, T("porch.light.power"));

	mVars.Add(0, T("doors.lock"));

	mVars.Add(0, T("salon.light.on"));
	mVars.Add(0, T("salon.light.power"));

	mVars.Add(0, T("salon.blinds.on"));
	mVars.Add(0, T("salon.blinds.power"));

	mVars.Add(0, T("salon.heat.on"));
	mVars.Add(0, T("salon.heat.power"));

	mSets.Add(0, T("port"));
	mSets.Add(bDebug, T("debug"));

	Console << T("[OK]\n");

	tThr.Add(0, ConsoleHandler);
	sSec.Add(0);

     LoadSettings();

     Start();

     tThr.Start(0, this);

     Sleep(100);

}

ServerCore::~ServerCore(void)
{

	SaveSettings();

}

void ServerCore::EnterSection(void)
{

	sSec.Enter(0);

}

void ServerCore::LeaveSection(void)
{

	sSec.Leave(0);

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

	sSrv.Shutdown();

	Console << T("[OK]\n");

	return true;

}

void ServerCore::Disconnect(CLI& cClient)
{

	SOCKET sTmp = cClient;

	sSrv.Disconnect(sTmp);

	OnDisconnect(sTmp);

}

void ServerCore::Disconnect(CON& cTerminal)
{

	Stop();

	ExitProcess(0);

}

void ServerCore::LoadSettings(const STR& sFile)
{

	INI iConfig(sFile);

	IF_DEBUG Console << T("\n >> Wczytuje wartosci zmiennych z pliku '") << sFile << T("'\n");

	IF_DEBUG Console << T("\nZmienne projektu:\n\n");

	for (int i = 1; i <= mVars.Capacity(); i++){

		mVars.SetDataByInt(iConfig.GetInt(T("VARS"), mVars.GetKey(i)), i);

		IF_DEBUG Console << T("\t") << mVars.GetKey(i) << T(" = ") << mVars.GetDataByInt(i) << T("\n");

	}

	IF_DEBUG Console << T("\nZmienne programu:\n\n");

	for (int i = 1; i <= mSets.Capacity(); i++){

		mSets.SetDataByInt(iConfig.GetInt(T("SRV"), mSets.GetKey(i)), i);

		IF_DEBUG Console << T("\t") << mSets.GetKey(i) << T(" = ") << mSets.GetDataByInt(i) << T("\n");

	}

	IF_DEBUG Console << T("\n >> Wczytywanie zakonczone\n"); else Console << T("\n >> Wczytuje wartosci zmiennych z pliku '") << sFile << T("'\t[OK]");

}

void ServerCore::SaveSettings(const STR& sFile)
{

	INI iConfig(sFile);

	IF_DEBUG Console << T("\n >> Zapisuje wartosci zmiennych do pliku '") << sFile << T("\n");

	IF_DEBUG Console << T("\nZmienne projektu:\n\n");

	for (int i = 1; i <= mVars.Capacity(); i++){

		iConfig.SetInt(T("VARS"), mVars.GetKey(i), mVars.GetDataByInt(i));

		IF_DEBUG Console << T("\t") << mVars.GetKey(i) << T(" = ") << mVars.GetDataByInt(i) << T("\n");

	}

	IF_DEBUG Console << T("\n >> Zapisywanie zakonczone\n"); else Console << T("\n >> Zapisuje wartosci zmiennych do pliku '") << sFile << T("'\t[OK]");

}

void ServerCore::OnConnect(SOCKET sClient)
{

	Console << T("\n >> Polaczono z klientem\n\tID: ") << (int) sClient << T("\n\tAdres: ") << sSrv.Clients[sClient].GetAddress() << T("\n");

}

void ServerCore::OnDisconnect(SOCKET sClient)
{

	Console << T("\n >> Rozlaczono Klienta\n\tID: ") << (int) sClient << T("\n");

}

void ServerCore::OnVarChange(const STR& sVar, int iValue)
{

	// TODO

}

template<typename tnTerminal>
void ServerCore::OnRead(const STR& sMessage, tnTerminal& tTerminal)
{

	IF_DEBUG Console << T("\n >> Odebrano wiadomosc: '") << sMessage << T("'\n");

	Parse<tnTerminal>(sMessage, tTerminal);

}

template<typename tnTerminal>
void ServerCore::Parse(const STR& sInput, tnTerminal& tTerminal)
{

	IF_DEBUG Console << T(" >> Parsuje dane\r\n\r\n\tWejscie: '") << sInput << T("'\r\n");

	Containers::Strings sAction(sInput, T(' '));

	STR sCommand = sAction[1];

	IF_DEBUG Console << T("\tPolecenie: '") << sCommand << T("'\r\n");

	sAction.Delete(1);

	IF_DEBUG {

		Console << T("\tParametry: '");

		for (int i = 1; i <= sAction.Capacity(); i++){

			Console << sAction[i];

			if (i != sAction.Capacity()) Console << T(", ");

		}

		Console << T("'\r\n");

	}

	unsigned uCode = CMD_UNKNOWN;

	if (sCommand == T("rcon")) uCode = CMD_RCON;
	else if (sCommand == T("set")) uCode = CMD_SET;
	else if (sCommand == T("get")) uCode = CMD_GET;
	else if (sCommand == T("bye")) uCode = CMD_BYE;

	Interpret<tnTerminal>(uCode, sAction, tTerminal);

	IF_DEBUG Console << T("\r\n << Parsowanie zakonczone\r\n");

	if (uCode != CMD_BYE) tTerminal << PROMPT;

}

template<typename tnTerminal>
void ServerCore::Interpret(unsigned uCode, Containers::Strings& sParams, tnTerminal& tTerminal)
{

	switch (uCode){

		case CMD_RCON:

			IF_DEBUG {

				Console << T("\r\n >> Interpretuje polecenie: CMD_RCON\r\n");

				for (int i = 1; i <= sParams.Capacity(); i++) Console << T("\r\n\tParametr ") << i << T(": '") << sParams[i] << T("'");

				Console << T("\r\n");

			}

			if (!sParams) return;

			else if (sParams[1] == T("listen")) Start();
			else if (sParams[1] == T("shutdown")) Stop();
			else if ((sParams[1] == T("quit")) || (sParams[1] == T("exit"))) Disconnect(tTerminal);

			else if (sParams[1] == T("save")) {if (sParams.Capacity() == 2) SaveSettings(sParams[2]); else SaveSettings();}

			else if (sParams[1] == T("set") && sParams.Capacity() == 3) mSets[sParams[2]] = (int) sParams[3];

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

					tTerminal << S T("\r\nLista klientow:\r\n\r\n");

					for (int i = 1; i <= sSrv.Clients.Capacity(); i++) tTerminal << S T("\t") << S i << S T(": ID: [") << S (int) sSrv.Clients.GetDataByInt(i).GetSocket() << S T("] @: [") << S sSrv.Clients.GetDataByInt(i).GetAddress() << S T("]\r\n");

					tTerminal << S T("\r\n");

				}

			}

			else tTerminal << S T("Nieznane parametry polecenia lub nieprawidlowa liczba parametrow\n\t");

		break;

		case CMD_SET:

			IF_DEBUG {

				Console << T("\r\n >> Interpretuje polecenie: CMD_SET\r\n");

				for (int i = 1; i <= sParams.Capacity(); i++) Console << T("\r\n\tParametr ") << i << T(": '") << sParams[i] << T("'");

				Console << T("\r\n");

			}

			if (!sParams) return;

			else if (sParams.Capacity() == 2 && mVars.Contain(sParams[1])) mVars[sParams[1]] = (int) sParams[2];

			else tTerminal << S T("Nieprawidlowa liczba parametrow lub niezdefiniowana zmienna\n\t");

		break;

		case CMD_GET:

			IF_DEBUG {

				Console << T("\r\n >> Interpretuje polecenie: CMD_GET\r\n");

				for (int i = 1; i <= sParams.Capacity(); i++) Console << T("\r\n\tParametr ") << i << T(": '") << sParams[i] << T("'");

				Console << T("\r\n");

			}

			if (!sParams) return;

			else if (mVars.Contain(sParams[1])) tTerminal << S T("set ") << sParams[1] << S T(" ") << S mVars[sParams[1]] << S T("\r\n");

			else tTerminal << S T("Niezdefiniowana zmienna\n\t");

		break;

		case CMD_BYE:

			IF_DEBUG Console << T("\r\n >> Interpretuje polecenie: CMD_BYE\r\n");

			Disconnect(tTerminal);

		break;

		default: tTerminal << S T("Nieznane polecenie\r\n");

	}

}
