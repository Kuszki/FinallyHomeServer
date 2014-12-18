/*********************************************

    FinallyHome Server "core" module header

    copyright: KuszkiDevGroup

    license: GNU GPL v2

*********************************************/

/*! \file
    \brief Plik zawierający deklaracje dla modułu "core".

Zawiera deklaracje klasy ServerCore.
*/

#ifndef _FINALLYHOMESERVER_CORE_HPP
#define _FINALLYHOMESERVER_CORE_HPP

using namespace KuszkAPI;

// ----- CLASS DECLARATION -----

/*! \brief Klasa odpowiedzialna za funkcjonowanie aplikacji.
 *  \warning Klasa powinna być singletonem, nie zaimplementowano tego rozwiązania z oszczędności czasu. Nie zaleca się jednak wielokrotnego instancjowania tej klasy.
 *
 *  Klasa zarządza serwerem aplikacji, organizuje zmienne używane w projekcie, obsługuje zdarzenia wywoływane przez klienta i wymienia istatne informacje z podłączonym do komputera sterownikiem.
 *
 */ class ServerCore
{

	protected:

		typedef void		(*COMPORT_SET)(unsigned uDev, unsigned uSet);	//!< Prototyp funkcji ustawiającej urządzenie.
		typedef unsigned	(*COMPORT_START)(unsigned uPort, void* pvProc);	//!< Prototyp funkcji rozpoczynającej nasłuchiwanie.
		typedef void		(*COMPORT_STOP)(void);						//!< Prototyp funkcji kończącej nasłuchiwanie.

        /*! \brief Biblioteki.
         *
         *  Odpowiada za ładowanie zewnętrznych bibliotek.
         *
         */ DLL dDll;

        /*! \brief Kontener na zmienne projektu.
         *
         *  Mapa przechowująca wszystkie dostępne w projekcie zmienne dotyczące działania systemu.
         *
         */ MAP<INT, STR> mVars;

        /*! \brief Kontener na zmienne programowe.
         *
         *  Mapa przechowująca wszystkie dostępne w programie zmienne dotyczące ustawień aplikacji.
         *
         */ MAP<INT, STR> mSets;

        /*! \brief Kontener na identyfikatory kontrolek.
         *
         *  Mapa przechowująca wszystkie dostępne w programie kontrolki powiązane z podstawowymi zmiennymi.
         *
         */ MAP<INT, STR> mCtrls;

        /*! \brief Kontener na identyfikatory urządzeń.
         *
         *  Mapa przechowująca wszystkie dostępne w programie urządzenia powiązane z podstawowymi zmiennymi.
         *
         */ MAP<INT, STR> mDevs;

        /*! \brief Instancja serwera.
         *
         *  Odpowiada za nadzorowanie ruchu sieciowego.
         *
         */ SRV sSrv;

        /*! \brief Wątki.
         *
         *  Odpowiada za nadzorowanie wszystkich wątków w aplikacji.
         *
         */ THR tThr;

        /*! \brief Sekcje krytyczne.
         *
         *  Odpowiada za synchronizowanie wszystkich wątków w aplikacji.
         *
         */ SEC sSec;

        /*! \brief Plik konfiguracji.
         *
         *  Ścieżka do pliku konfiguracyjnego.
         *
         */ STR sConfig;

        /*! \brief Wyjście z informacjami o debugowaniu.
         *
         *  Odpowiada za włączenie lub wyłączenie szczegółowych komunikatów aplikacji.
         *
         */ BOOL bDebug;

    public:

        /*! \brief Konsola.
         *
         *  Odpowiada za wyświetlanie i pobieranie danych.
         *
         */ CON& Console;

        /*! \brief Okno programu.
         *
         *  Odpowiada za interfejs graficzny.
         *
         */ WND& Window;

        /*! \brief Domyślny konstruktor klasy.
         *  \param [in, out] cCon Referencja do konsoli użytej w programie.
         *  \param [in, out] wWnd Referencja do okna użytego w programie.
         *
         *  Tworzy sekcje krytyczne i obiekty potrzebne do pracy serwera.
         *
         */ ServerCore(CON& cCon, WND& wWnd);

        /*! \brief Destruktor.
         *
         *  Zwalnia użyte zasoby.
         *
         */ ~ServerCore(void);

        /*! \brief Inicjacja interfejsu na podstawie wejścia.
         *  \param [in] sParams Parametry do przetworzenia.
         *
         *  Parsuje wejście i przetwarza podane parametry.
         *
         */ void Initiate(Containers::Strings sParams);

        /*! \brief Początek synchronizacji wątku.
         *  \param [in] uSection Numer sekcji do wejścia.
         *
         *  Wchodzi do sekcji krytycznej w celu synchronizacji wątków.
         *
         */ void EnterSection(unsigned uSection);

        /*! \brief Koniec synchronizacji wątku.
         *  \param [in] uSection Numer sekcji do wyjścia.
         *
         *  Wychodzi z sekcji krytycznej.
         *
         */ void LeaveSection(unsigned uSection);

        /*! \brief Uruchamia usługę serwera.
         *  \return Powodzenie operacji.
         * 		true - operacja powiodła się,
         * 		false - nastąpił błąd.
         *
         *  Na podstawie wczytanych wcześniej danych uruchamia serwer aplikacji.
         *
         */ bool Start(void);

        /*! \brief Wyłącza usługę serwera.
         *  \return Powodzenie operacji.
         * 		true - operacja powiodła się,
         * 		false - nastąpił błąd.
         *
         *  Kończy działanie serwera i zamyka wszystkie połączenia.
         *
         */ bool Stop(void);

        /*! \brief Rozłącza wybranego klienta.
         *  \param [in] sClient ID klienta do odłączenia.
         *
         *  Kończy połączenie z klientem na życzenie klienta.
         *
         */ void Disconnect(SOCKET sClient);

        /*! \brief Wyłącza serwer.
         *
         *  Kończy połączenia z klientami i wyłącza serwer.
         *
         */ void Shutdown(void);

        /*! \brief Wczytuje ustawienia z pliku ini.
         *  \param [in] sFile Nazwa pliku.
         *  \return Powodzenia operacji.
         *
         *  Powoduje, że wszystkie wartości zmiennych serwera zostają wczytane z pliku konfiguracyjnego.
         *
         */ bool LoadSettings(const STR& sFile);

        /*! \brief Zapisuje bierzące ustawienia do pliku ini.
         *  \param [in] sFile Nazwa pliku.
         *
         *  Powoduje, że wszystkie wartości zmiennych serwera zostają zapisane do pliku konfiguracyjnego tak, aby w przyszłości można było je odtworzyć.
         *
         */ void SaveSettings(const STR& sFile);

        /*! \brief Zdarzenie wywoływane gdy nastąpi połączenie klienta.
         *  \param [in] sClient Identyfikator gniazda klienta.
         *
         *  Zapisuje parametry połączenia z klientem.
         *
         */ void OnConnect(SOCKET sClient);

        /*! \brief Zdarzenie wywoływane gdy nastąpi rozłączenie klienta.
         *  \param [in] sClient Identyfikator gniazda klienta.
         *
         *  Zamyka gniazdo połączenia z klientem.
         *
         */ void OnDisconnect(SOCKET sClient);

        /*! \brief Zdarzenie wywoływane gdy nastąpi zmiana wartości zmiennej projektu.
         *  \param [in] sVar Nazwa zmiennej.
         *  \param [in] iValue Nowa wartość zmiennej.
         *  \param [in] uFlags Określa flagi wywołania zdarzenia.
         *
         *  Metoda parsuje nazwę zmiennej, a następnie przekazuje jej identyfikator i nową wartość do odpowiedniej funkcji z biblioteki zewnętrznej.
         *
         */ void OnVarChange(const STR& sVar, int iValue, unsigned uFlags = SET_REMOTE);

        /*! \brief Zdarzenie wywoływane gdy serwer odbierze komunikat od klienta.
         *  \param [in] sMessage Odebrane dane.
         *  \param [in, out] sClient Identyfikator klienta, który wywołał zdarzenie lub 0 gdy zostało one wywołane lokalnie.
         *
         *  Odbiera dane od klienta i przekazuje je do parsera.
         *
         */ void OnRead(const STR& sMessage, SOCKET sClient = 0);

        /*! \brief Zdarzenie wywoływane gdy serwer odbierze komunikat od urządzenia.
         *  \param [in] uDev ID urządzenia.
         *  \param [in] uSet Wartość do nastawienia.
         *
         *  Odbiera dane od urządzenia i przekazuje je do parsera.
         *
         */ void OnSet(unsigned uDev, unsigned uSet);

        /*! \brief Zwraca nazwę zmiennej do której przyporządkowany jest podany identyfikator widgetu.
         *  \param [in] iControl ID kontrolki.
         *  \return Nazwa zmiennej odpowiadająca kontrolce.
         *
         *  Wyszukuje nazwy zmiennej przyporządkowanej do podanej kontrolki.
         *
         */ const STR& GetControlVar(INT iControl) const;

        /*! \brief Parsuje wejście z konsoli lub gniazda i przekazuje wynik parsingu do interpretera.
         *  \tparam tnTerminal Rodzaj terminala.
         *  \param [in] sInput Wejście danych.
         *  \param [in, out] tTerminal Terminal na którym zostaną przeprowadzone operacje wejścia i wyjścia.
         *  \return Kod polecenia.
         *  \warning Terminal musi posiadać przeciążony operator strumienia wejścia.
         *
         *  Odbiera dane od klienta i przekazuje je do parsera.
         *
         */ template<typename tnTerminal> unsigned Parse(const STR& sInput, tnTerminal& tTerminal);

        /*! \brief Interpretuje dane przekazane przez parser.
         *  \tparam tnTerminal Rodzaj terminala.
         *  \param [in] uCode Kod polecenia.
         *  \param [in] sParams Parametry polecenia.
         *  \param [in, out] tTerminal Terminal na którym zostaną przeprowadzone operacje wejścia i wyjścia.
         *  \warning Terminal musi posiadać przeciążony operator strumienia weyjścia.
         *
         *  Wykonuje odpowiednie akcje na podstawie podanego polecenia i parametrów.
         *
         */ template<typename tnTerminal> void Interpret(unsigned uCode, Containers::Strings& sParams, tnTerminal& tTerminal);

};

#endif
