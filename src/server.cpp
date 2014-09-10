/*********************************************

    FinallyHome Server "core" module source

    copyright: KuszkiDevDroup

    license: GNU GPL v2

*********************************************/

/*! \file
    \brief Plik zawierający definicja dla modułu "core".

Plik jest dołączany przez server.hpp.
*/

ServerCore::ServerCore(bool bDbg)
: Console(true), bDebug(bDbg)
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
	tThr.Start(0);

     LoadSettings();

}

ServerCore::~ServerCore(void)
{

	SaveSettings();

}

bool ServerCore::Start(void)
{

	IF_DEBUG Console << T("\n >> Uruchamiam serwer\t");

	bool bOK = sSrv.Listen((int) mVars[S T("port")], ServerHandler);

	IF_DEBUG Console << bOK ? T("[OK]\n") : T("[FAIL]\n");

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

	IF_DEBUG Console << T("\n >> Polaczono z klientem\n\tnumer: '") << (int) sClient << ";\n\tAdres: " << sSrv.Clients[sClient].GetAddress() << T("\n");

}

void ServerCore::OnRead(const ARA<char>& aData)
{

	IF_DEBUG Console << T("\n >> Odebrano wiadomosc: '") << aData.GetBegin() << T("'\n");

}
