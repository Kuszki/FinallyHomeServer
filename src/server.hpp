/*********************************************

    FinallyHome Server "core" module header

    copyright: KuszkiDevDroup

    license: GNU GPL v2

*********************************************/

/*! \file
    \brief Plik zawierający deklaracje dla modułu "core".

Po dołączeniu go do projektu automatycznie dołączony zostanie też plik server.cpp.
*/

#ifndef _FINALLYHOMESERVER_CORE_HPP
#define _FINALLYHOMESERVER_CORE_HPP

#define IF_DEBUG if (bDebug)

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

        /*! \brief Opcja debugowania.
         *
         *  Włącza lub wyłącza komunikaty programu.
         *
         */ bool bDebug;

    public:

        /*! \brief Konsola.
         *
         *  Odpowiada za wyświetlanie i pobieranie danych.
         *
         */ CON Console;

        /*! \brief Domyślny konstruktor klasy.
         *  \param [in] bDbg Określa czy na wyjściu konsoli mają pojawiać się informacje o działaniu aplikacji zanim ta opcja zostanie wczytana z konfiguracji programu.
         * 		    Wartości:
         * 		    true - informacje będą wypisywane,
         * 		    false - informacje nie będą wypisywane.
         * 		    Domyślnie: false.
         *
         *  Dokonuje inicjacji mapy ServerCore::mVars tak, aby przechowywała ona wszystkie zmienne używane w projekcie.
         *
         */ ServerCore(bool bDbg = false);

        /*! \brief Destruktor.
         *
         *  Zwalnia użyte zasoby.
         *
         */ ~ServerCore(void);

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

        /*! \brief Zdarzenie wywoływane gdy serwer odbierze komunikat od klienta.
         *  \param [in] aData Odebrane dane.
         *
         *  Odbiera dane od klienta i przekazuje je do parsera.
         *
         */ void OnRead(const ARA<char>& aData);

};

#endif
