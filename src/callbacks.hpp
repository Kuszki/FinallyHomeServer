/*********************************************

    FinallyHome Server "callback" module header

    copyright: KuszkiDevGroup

    license: GNU GPL v2

*********************************************/

/*! \file
    \brief Plik zawierający deklaracje dla modułu "callback".

Zawiera deklaracje funkcji ServerHandler oraz ConsoleHandler.
*/

#ifndef _FINALLYHOMESERVER_CALLBACK_HPP
#define _FINALLYHOMESERVER_CALLBACK_HPP

/*! \brief Funkcja zwrotna odpowiadająca za obsługę zdarzeń serwera.
 *  \param [in] srv Wskaźnik do instancji serwera, który obsługuje to zdarzenie.
 *  \param [in] event Identyfikator obsługiwanego zdarzenia.
 *  \param [in] socket Identyfikator klienta.
 *  \return Kod operacji. Domyślnie 0.
 *
 *  Obsługuje dodatkowe zdarzenia przy połączeniu, odbieraniu informacji i zakończaniu połączenia.
 *
 */ LRESULT ServerHandler(SRV& srv, UINT event, SOCKET socket);

/*! \brief Funkcja wątku konsoli.
 *  \param [in] pvArgs Wskaźnik na dodatkowe parametry.
 *  \return Kod operacji. Domyślnie 0.
 *
 *  Pozwala obsługiwać wejście konsoli w osobnym wątku.
 *
 */ DWORD WINAPI ConsoleHandler(LPVOID pvArgs);

/*! \brief Funkcja zwrotna okna programu.
 *  \param [in] hWnd Uchwyt okna.
 *  \param [in] uMsg Kod wiadomości.
 *  \param [in] wParam Pierwszy parametr.
 *  \param [in] lParam Drugi parametr.
 *  \return Kod operacji. Domyślnie 0.
 *
 *  Pozwala obsługiwać zdarzenia okna aplikacji.
 *
 */ LRESULT WINAPI WindowHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif
