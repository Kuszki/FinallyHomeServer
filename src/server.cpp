/*********************************************

    FinallyHome Server "core" module source

    copyright: KuszkiDevGroup

    license: GNU GPL v2

*********************************************/

/*! \file
    \brief Plik zawierający definicje dla modułu "core".

Zawiera implementacje klasy ServerCore.
*/

#include KUSZKAPI_PATH

#include "..\FinallyHome_Server_private.h"

#include "macros.hpp"

#include "callbacks.hpp"
#include "server.hpp"

ServerCore::ServerCore(CON& cCon, WND& wWnd)
: Console(cCon), Window(wWnd), bDebug(false)
{

	Console.SetTitle(APP_TITLE);

	tThr.Add(CONS_SECTION, ConsoleHandler);

	sSec.Add(CORE_SECTION);
	sSec.Add(CONS_SECTION);
	sSec.Add(WIND_SECTION);
	sSec.Add(SOCK_SECTION);

	sConfig = T("../conf/config.ini");

}

ServerCore::~ServerCore(void)
{

	Stop();

	SaveSettings(sConfig);

}

void ServerCore::Initiate(Containers::Strings sParams)
{

	if (!sParams.ParseQuotes()) return;

	bool bInterface	=	false;
	bool bWindow		=	false;
	bool bConsole		=	false;

	foreach(sParams){

		if (sParams[i] == T("--debug")) bDebug = true;

		if (sParams[i].Find(T("--config="))) sConfig = Containers::Strings(sParams[i], T('='))[2];

		if (sParams[i] == T("--console")) Console.Show(), bConsole = bInterface = true;

		if (sParams[i] == T("--window")) bWindow = bInterface = true;

	}

	if (bWindow){

		Window.New(T("FINALLYHOME_SERVER"), APP_TITLE, WindowHandler);

		Window.Class.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MAIN));
		Window.Class.hIconSm = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MAIN));

		Window.Register();

		Window.Create(0, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME, NULL, 450, 550);

		auto& Buttons	= Window.Widgets.Buttons;
		auto& Checks	= Window.Widgets.Checks;
		auto& Bars	= Window.Widgets.Tracks;
		auto& Edits	= Window.Widgets.Edits;
		auto& Labels	= Window.Widgets.Labels;
		auto& Tables	= Window.Widgets.Tables;
		auto& Tabs	= Window.Widgets.Tabs;

		unsigned puVarSize[] = {120, 70};
		unsigned puCliSize[] = {80, 200};

		Tables.Add(CTR_TABLE_VAR).Create(5, 5, 200, 250).SetHeader(Containers::Strings(T("Zmienna\nWartość")), Containers::Vector<unsigned>(puVarSize, 2));

		Tables.Add(CTR_TABLE_CLI).Create(5, 260, 300, 255).SetHeader(Containers::Strings(T("ID\nAdres połączenia")), Containers::Vector<unsigned>(puCliSize, 2));

		const TCHAR* ppcButtons[] = {T("Wczytaj zmienne"), T("Zapisz zmienne"), T("Rozłącz klienta"), T("Restart serwera"), T("Pomoc"), T("O programie")};

		for (int i = 0; i < 6; i++) Buttons.Add(CTR_BTN_ID + i + 1).Create(ppcButtons[i], 310, 260 + i*40 + (i > 3 ? 20 : 0), 130, 35);

		Tabs.Add(CTR_TAB_CATS).Create(210, 5, 230, 250, Containers::Strings("Sterowanie\nSalon\nPrzedpokój"));

		Tabs[CTR_TAB_CATS].AddControl(Checks.Add(CTR_CHK_LISTEN).Create(T("Nasłuchiwanie"), 220, 35, 210, 15, BS_RIGHTBUTTON | BS_LEFT), 1);
		Tabs[CTR_TAB_CATS].AddControl(Checks.Add(CTR_CHK_CONS).Create(T("Konsola"), 220, 60, 210, 15, BS_RIGHTBUTTON | BS_LEFT), 1);
		Tabs[CTR_TAB_CATS].AddControl(Checks.Add(CTR_CHK_DEBUG).Create(T("Debugowanie"), 220, 85, 210, 15, BS_RIGHTBUTTON | BS_LEFT), 1);

		Tabs[CTR_TAB_CATS].AddControl(Checks.Add(CTR_CHK_SLO).Create(T("Światło"), 220, 35, 210, 15, BS_RIGHTBUTTON | BS_LEFT), 2);
		Tabs[CTR_TAB_CATS].AddControl(Bars.Add(CTR_BAR_SLP).Create(220, 55, 210, 25, 1, 9), 2);
		Tabs[CTR_TAB_CATS].AddControl(Checks.Add(CTR_CHK_SBO).Create(T("Zasłony"), 220, 95, 210, 15, BS_RIGHTBUTTON | BS_LEFT), 2);
		Tabs[CTR_TAB_CATS].AddControl(Bars.Add(CTR_BAR_SBP).Create(220, 115, 210, 25, 1, 9), 2);
		Tabs[CTR_TAB_CATS].AddControl(Checks.Add(CTR_CHK_SHO).Create(T("Ogrzewanie"), 220, 155, 210, 15, BS_RIGHTBUTTON | BS_LEFT), 2);
		Tabs[CTR_TAB_CATS].AddControl(Bars.Add(CTR_BAR_SHP).Create(220, 175, 210, 25, 15, 30), 2);

		Tabs[CTR_TAB_CATS].AddControl(Checks.Add(CTR_CHK_PLO).Create(T("Światło"), 220, 35, 210, 15, BS_RIGHTBUTTON | BS_LEFT), 3);
		Tabs[CTR_TAB_CATS].AddControl(Bars.Add(CTR_BAR_PLP).Create(220, 55, 210, 25, 1, 9), 3);
		Tabs[CTR_TAB_CATS].AddControl(Checks.Add(CTR_CHK_PDC).Create(T("Blokada drzwi"), 220, 95, 210, 15, BS_RIGHTBUTTON | BS_LEFT), 3);

		Checks[CTR_CHK_CONS].SetCheck(bConsole);
		Checks[CTR_CHK_DEBUG].SetCheck(bDebug);

		INI iConfig(T("../conf/controls.ini"));

		mCtrls = iConfig.GetIntValues(T("IDS"));

		Tabs[CTR_TAB_CATS].SetTab();

		Window.Show();

	}

	if (!bInterface) Console.Show();

	if (LoadSettings(sConfig)) Start();

	if (Console.Visible()) tThr.Start(CONS_SECTION, this);

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

	if (bOK && Window) Window.Widgets.Checks[CTR_CHK_LISTEN].SetCheck(true);

	return bOK;

}

bool ServerCore::Stop(void)
{

	Console << T("\n >> Wylaczam serwer...\t");

	EnterSection(CORE_SECTION);

	sSrv.Shutdown();

	LeaveSection(CORE_SECTION);

	Console << T("[OK]\n");

	if (Window) Window.Widgets.Tables[CTR_TABLE_CLI].CleanItems(), Window.Widgets.Checks[CTR_CHK_LISTEN].SetCheck(false);

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

bool ServerCore::LoadSettings(const STR& sFile)
{

	INI iConfig(sFile);

	IF_DEBUG Console << T("\n >> Wczytuje wartosci zmiennych z pliku '") << sFile << T("'\n");

	auto mTmpVars = iConfig.GetIntValues(T("VARS"));
	auto mTmpSets = iConfig.GetIntValues(T("SRV"));

	if (!mTmpVars || !mTmpSets){

		IF_DEBUG Console << T("\n << Blad przy wczytywaniu wartosci\n");

		return false;

	}

	IF_DEBUG Console << T("\nZmienne projektu:\n\n");

	mVars = mTmpVars;

	IF_DEBUG foreach(mVars) Console << T("\t") << mVars.GetKey(i) << T(" = ") << mVars.GetDataByInt(i) << T("\n");

	IF_DEBUG Console << T("\nZmienne programu:\n\n");

	mSets = mTmpSets;

	IF_DEBUG foreach(mSets) Console << T("\t") << mSets.GetKey(i) << T(" = ") << mSets.GetDataByInt(i) << T("\n");

	IF_DEBUG Console << T("\n << Wczytywanie zakonczone\n");
	else Console << T("\n >> Wczytuje wartosci zmiennych z pliku '") << sFile << T("'\t[OK]");

	if (Window){

		Window.Widgets.Tables[CTR_TABLE_VAR].CleanItems();

		foreach(mVars){

			Containers::Strings sTmp;

			sTmp << mVars.GetKey(i) << S mVars.GetDataByInt(i);

			Window.Widgets.Tables[CTR_TABLE_VAR].AddItem(sTmp);

		}

	}

	foreach(mVars) OnVarChange(mVars.GetKey(i), mVars.GetDataByInt(i), false);

	return mSets && mVars;

}

void ServerCore::SaveSettings(const STR& sFile)
{

	if (!mSets.Capacity() || !mVars.Capacity()) return;

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

	if (Window) Window.Widgets.Tables[CTR_TABLE_CLI].AddItem(Containers::Strings(STR(sSrv[sClient].GetSocket()) + STR(T('\n')) + sSrv[sClient].GetAddress()));

}

void ServerCore::OnDisconnect(SOCKET sClient)
{

	Console << T("\n >> Rozlaczono Klienta\n\tID: ") << (int) sClient << T("\n");

	if (Window) Window.Widgets.Tables[CTR_TABLE_CLI].DeleteItem(Window.Widgets.Tables[CTR_TABLE_CLI].GetItems().Find(STR(sClient)));

}

void ServerCore::OnVarChange(const STR& sVar, int iValue, bool bRemote)
{

	STR sMessage = S T("set ") + sVar + S T(" ") + S iValue + S T("\r\n");

	if (bRemote) mVars[sVar] = iValue;

	for (int i = 1; i <= sSrv.Capacity(); i++) sSrv.GetClient(i) << sMessage;

	if (Window){

		if (bRemote) Window.Widgets.Tables[CTR_TABLE_VAR].SetItemData(S iValue, 2, mVars.FindKey(sVar));

		if (mCtrls) if (mCtrls.Contain(sVar)) if (mCtrls[sVar] > 500) Window.Widgets.Tracks[mCtrls[sVar]].SetValue(iValue);
		else Window.Widgets.Checks[mCtrls[sVar]].SetCheck(iValue);

	}

	if (bRemote) Console << sMessage;

}

void ServerCore::OnRead(const STR& sMessage, SOCKET sClient)
{

	STR sBuffer;

	EnterSection(CORE_SECTION);

	IF_DEBUG Console << T("\n >> Odebrano wiadomosc: '") << sMessage << T("'\n");

	unsigned uAction = Parse<STR>(sMessage, sBuffer);

	sBuffer << PROMPT;

	if (sClient){

		if (uAction != CMD_BYE) sSrv[sClient] << sBuffer;
		else Disconnect(sClient);

	} else Console << sBuffer;

	LeaveSection(CORE_SECTION);

}

const STR& ServerCore::GetControlVar(INT iControl) const
{
	foreach(mCtrls) if (mCtrls.GetDataByInt(i) == iControl) return mCtrls.GetKey(i);
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

			else if (sParams[1] == T("save")) {if (sParams.Capacity() == 2) SaveSettings(sParams[2]); else SaveSettings(sConfig);}

			else if (sParams[1] == T("load")) {if (sParams.Capacity() == 2) LoadSettings(sParams[2]); else LoadSettings(sConfig);}

			else if (sParams[1] == T("set") && sParams.Capacity() == 3) mSets[sParams[2]] = (int) sParams[3];

			else if (sParams[1] == T("debug") && sParams.Capacity() == 2) { bDebug = (int) sParams[2]; if (Window) Window.Widgets.Checks[CTR_CHK_DEBUG].SetCheck(bDebug); }

			else if (sParams[1] == T("kick") && sParams.Capacity() == 2) Disconnect((int) sParams[2]);

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

			else tTerminal << S T(" >> Nieznane parametry polecenia lub nieprawidlowa liczba parametrow\r\n");

		break;

		case CMD_SET:

			if (!sParams) return;

			else if (sParams.Capacity() == 2) if (mVars.Contain(sParams[1])) OnVarChange(sParams[1], (int) sParams[2]);

			else tTerminal << S T(" >> Nieprawidlowa liczba parametrow lub niezdefiniowana zmienna\r\n");

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
