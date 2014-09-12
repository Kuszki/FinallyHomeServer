/*********************************************

    FinallyHome Server "core" module header

    copyright: KuszkiDevGroup

    license: GNU GPL v2

*********************************************/

/*! \file
    \brief Plik zawierający deklaracje dla modułu "core".

Po dołączeniu go do projektu automatycznie dołączony zostanie też plik server.cpp.
*/

#ifndef _FINALLYHOMESERVER_CORE_HPP
#define _FINALLYHOMESERVER_CORE_HPP

#define IF_DEBUG if (mSets[S T("debug")]) //!< Skrócone wywołanie konstrukcji warunkowej.

#define CMD_UNKNOWN 0 //!< Identyfikator nieznanego polecenia.

#define CMD_RCON 1 //!< Identyfikator polecenia "rcon".
#define CMD_SET 2 //!< Identyfikator polecenia "set".
#define CMD_GET 3 //!< Identyfikator polecenia "get".

using namespace KuszkAPI;

// ----- CLASS DECLARATION -----

/*! \brief Klasa odpowiedzialna za funkcjonowanie aplikacji.
 *  \warning Klasa powinna być singletonem, nie zaimplementowano tego rozwiązania z oszczędności czasu. Nie zaleca się jednak wielokrotnego instancjowania tej klasy.
 *
 *  Zwalnia użyte zasoby.
 *
 */ class ServerCore
{

    protected:

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

    public:

        /*! \brief Konsola.
         *
         *  Odpowiada za wyświetlanie i pobieranie danych.
         *
         */ CON Console;

        /*! \brief Domyślny konstruktor klasy.
         *  \param [in] bDebug Włącza komunikaty debugowania.
         *
         *  Dokonuje inicjacji mapy ServerCore::mVars tak, aby przechowywała ona wszystkie zmienne używane w projekcie.
         *
         */ ServerCore(bool bDebug);

        /*! \brief Destruktor.
         *
         *  Zwalnia użyte zasoby.
         *
         */ ~ServerCore(void);

        /*! \brief Początek synchronizacji wątku.
         *
         *  Wchodzi do sekcji krytycznej w celu synchronizacji wątków.
         *
         */ void EnterSection(void);

        /*! \brief Koniec synchronizacji wątku.
         *
         *  Wychodzi z sekcji krytycznej.
         *
         */ void LeaveSection(void);

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

        /*! \brief Wczytuje ustawienia z pliku ini.
         *  \param [in] sFile Nazwa pliku.
         * 		    Domyślnie: "/conf/config.ini"
         *
         *  Powoduje, że wszystkie wartości zmiennych serwera zostają wczytane z pliku konfiguracyjnego.
         *
         */ void LoadSettings(const STR& sFile = T("../conf/config.ini"));

        /*! \brief Zapisuje bierzące ustawienia do pliku ini.
         *  \param [in] sFile Nazwa pliku.
         * 		    Domyślnie: "/conf/config.ini"
         *
         *  Powoduje, że wszystkie wartości zmiennych serwera zostają zapisane do pliku konfiguracyjnego tak, aby w przyszłości można było je odtworzyć.
         *
         */ void SaveSettings(const STR& sFile = T("../conf/config.ini"));

        /*! \brief Zdarzenie wywoływane gdy nastąpi połączenie klienta.
         *  \param [in] sClient Identyfikator gniazda klienta.
         *  \todo Należy przemyśleć działanie programu w przypadku połączenia więcej niż jednego klienta.
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
         *  \todo Zaimplementować biblioteke zewnętrzną.
         *
         *  Metoda parsuje nazwę zmiennej, a następnie przekazuje jej identyfikator i nową wartość do odpowiedniej funkcji z biblioteki zewnętrznej.
         *
         */ void OnVarChange(const STR& sVar, int iValue);

        /*! \brief Zdarzenie wywoływane gdy serwer odbierze komunikat od klienta.
         *  \tparam tnTerminal Rodzaj terminala.
         *  \param [in] sMessage Odebrane dane.
         *  \param [in, out] tTerminal Terminal na którym zostaną przeprowadzone operacje wejścia i wyjścia.
         *  \warning Terminal musi posiadać przeciążony operator strumienia weyjścia.
         *
         *  Odbiera dane od klienta i przekazuje je do parsera.
         *
         */ template<typename tnTerminal> void OnRead(const STR& sMessage, tnTerminal& tTerminal);

        /*! \brief Parsuje wejście z konsoli lub gniazda i przekazuje wynik parsingu do interpretera.
         *  \tparam tnTerminal Rodzaj terminala.
         *  \param [in] sInput Wejście danych.
         *  \param [in, out] tTerminal Terminal na którym zostaną przeprowadzone operacje wejścia i wyjścia.
         *  \warning Terminal musi posiadać przeciążony operator strumienia weyjścia.
         *
         *  Odbiera dane od klienta i przekazuje je do parsera.
         *
         */ template<typename tnTerminal> void Parse(const STR& sInput, tnTerminal& tTerminal);

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
