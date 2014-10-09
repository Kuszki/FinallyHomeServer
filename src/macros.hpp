/*********************************************

    FinallyHome Server macros headers

    copyright: KuszkiDevGroup

    license: GNU GPL v2

*********************************************/

/*! \file
    \brief Plik zawierający macra użyte w projekcie.

Zawiera deklaracje wszystkich ważnych macr.
*/

#ifndef _FINALLYHOMESERVER_MACROS_HPP
#define _FINALLYHOMESERVER_MACROS_HPP

#define PROMPT 		T("\r\n$: ")	//!< Znak zachęty.

#define SOCK_SECTION	1			//!< Identyfikator sekcji krytycznej gniazda.
#define CONS_SECTION	2			//!< Identyfikator sekcji krytycznej konsoli.
#define WIND_SECTION	3			//!< Identyfikator sekcji krytycznej okna.
#define CORE_SECTION	4			//!< Identyfikator sekcji krytycznej jądra.

#define IF_DEBUG 		if (bDebug)	//!< Skrócone wywołanie konstrukcji warunkowej.

#define CMD_UNKNOWN		0			//!< Identyfikator nieznanego polecenia.

#define CMD_RCON		1			//!< Identyfikator polecenia "rcon".
#define CMD_SET		2			//!< Identyfikator polecenia "set".
#define CMD_GET		3			//!< Identyfikator polecenia "get".

#define CMD_BYE		10			//!< Identyfikator polecenia "bye".

#endif
