/*********************************************

    FinallyHome Server "core" module source

    copyright: KuszkiDevGroup

    license: GNU GPL v2

*********************************************/

/*! \file
    \brief Plik zawierający definicja dla modułu "core".

Plik jest dołączany przez server.hpp.
*/

ServerCore::ServerCore(void)
: Console(true), bDebug(true)
{

	if (bDebug) Console << T("\n >> Inicjuje liste zmiennych\t");

	mVars.Add(0, T("porch.light.on"));
	mVars.Add(0, T("porch.light.power"));

	mVars.Add(0, T("doors.lock"));

	mVars.Add(0, T("salon.light.on"));
	mVars.Add(0, T("salon.light.power"));

	mVars.Add(0, T("salon.blinds.on"));
	mVars.Add(0, T("salon.blinds.power"));

	mVars.Add(0, T("salon.heat.on"));
	mVars.Add(0, T("salon.heat.power"));

	mSets.Add(9096, T("port"));
	mSets.Add(bDebug, T("debug"));

	if (bDebug) Console << T("[OK]\n");

	tThr.Add(0, ConsoleHandler);

     LoadSettings();

     tThr.Start(0, this);

     Sleep(100);

}

ServerCore::~ServerCore(void)
{

	SaveSettings();

}

bool ServerCore::Start(void)
{

	IF_DEBUG Console << T("\n >> Uruchamiam serwer\t");

	bool bOK = sSrv.Listen(9096, ServerHandler); //(short unsigned) mVars[S T("port")]

	IF_DEBUG Console << (bOK ? T("[OK]\n") : T("[FAIL]\n"));

	return bOK;

}

bool ServerCore::Stop(void)
{

	IF_DEBUG Console << T("\n >> Wylaczam serwer...\t");

	sSrv.Shutdown();

	IF_DEBUG Console << T("[OK]\n");

	return true;

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

	IF_DEBUG Console << T("\n >> Wczytywanie zakonczone\n");

	if (!bDebug && (bDebug = mSets[S T("debug")])) Console << T("\n >> Komunikaty debugowania wlaczone\n");

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

	IF_DEBUG Console << T("\n >> Zapisywanie zakonczone\n");

}

void ServerCore::OnConnect(SOCKET sClient)
{

	IF_DEBUG Console << T("\n >> Polaczono z klientem\n\tnumer: ") << (int) sClient << T("\n\tAdres: ") << sSrv.Clients[sClient].GetAddress() << T("\n");

}

void ServerCore::OnDisconnect(SOCKET sClient)
{

	IF_DEBUG Console << T("\n >> Rozlaczono Klienta\n\tnumer: ") << (int) sClient << T("\n");

}

void ServerCore::OnRead(const ARA<char>& aData)
{

	IF_DEBUG Console << T("\n >> Odebrano wiadomosc: '") << aData.GetBegin() << T("'\n");

}

void ServerCore::Parse(const STR& sInput)
{

	// TODO V

	// DEBUG VERSION

	IF_DEBUG Console << T(" >> Parsuje dane\n\n\tWejscie: '") << sInput << T("'\n");

	Containers::Strings sAction(sInput, T(' '));

	STR sCommand = sAction[1];

	IF_DEBUG Console << T("\tPolecenie: '") << sCommand << T("'\n");

	sAction.Delete(1);

	IF_DEBUG {

		Console << T("\tparametry: '");

		for (int i = 1; i <= sAction.Capacity(); i++){

			Console << sAction[i];

			if (i != sAction.Capacity()) Console << T(", ");

		}

		Console << T("'\n");

	}

	if (sCommand == T("rcon")) Interpret(CMD_RCON, sAction);

	IF_DEBUG Console << T("\n << Parsowanie zakonczone\n");

}

void ServerCore::Interpret(unsigned uCode, Containers::Strings& sParams)
{



	switch (uCode){

		case CMD_RCON:

			IF_DEBUG {

				Console << T("\n >> Interpretuje polecenie: CMD_RCON\n");

				for (int i = 1; i <= sParams.Capacity(); i++) Console << T("\n\tParametr ") << i << T(": '") << sParams[i] << T("'");

				Console << T("\n");

			}

			if (!sParams) return;

			if (sParams[1] == T("listen")) Start();
			if (sParams[1] == T("shutdown")) Stop();
			if ((sParams[1] == T("quit")) || (sParams[1] == T("exit"))) ExitProcess(0);

			if (sParams[1] == T("save")) if (sParams.Capacity() == 2) SaveSettings(sParams[2]); else SaveSettings();

			if (sParams[1] == T("set")) if (sParams.Capacity() == 3) mSets[sParams[2]] = (int) sParams[3];

		break;

	}

}
