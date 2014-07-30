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
         *  Mapa przechowująca wszystkie dostępne w projekcie zmienne.
         *
         */ MAP<STR, INT> mVars;

        /*! \brief Plik konfiguracyjny serwera.
         *
         *  Przechowuje informacje na temat ostatniego stanu zmiennych i portu gniazda słuchającego.
         *
         */ INI iConfig;

         /*! \brief Instancja serwera.
         *
         *  Odpowiada za nadzorowanie ruchu sieciowego.
         *
         */ SRV sSrv;

         /*! \brief Konsola.
         *
         *  Odpowiada za wyświetlanie i pobieranie danych.
         *
         */ CON cCon;

         /*! \brief Wątki.
         *
         *  Odpowiada za nadzorowanie wszystkich wątków w aplikacji.
         *
         */ THR tThr;

    public:

        /*! \brief Domyślny konstruktor klasy.
         *
         *  Dokonuje inicjacji mapy ServerCore::mVars tak, aby przechowywała ona wszystkie zmienne używane w projekcie.
         *
         */ ServerCore(void);

        /*! \brief Destruktor.
         *
         *  Zwalnia użyte zasoby.
         *
         */ ~ServerCore(void);

};

#include "server.cpp"

#endif
