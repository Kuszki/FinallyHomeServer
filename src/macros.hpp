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

// Common macros
#define PROMPT 			T("\r\n$: ")										//!< Znak zachęty.
#define IF_DEBUG 			if (bDebug)										//!< Skrócone wywołanie konstrukcji warunkowej.
#define APP_TITLE			STR(T(PRODUCT_NAME)) + STR(T(" v")) + STR(T(VER_STRING))	//!< Tytuł aplikacji.

// Critical sections ID's
#define SOCK_SECTION		1												//!< Identyfikator sekcji krytycznej gniazda.
#define CONS_SECTION		2												//!< Identyfikator sekcji krytycznej konsoli.
#define WIND_SECTION		3												//!< Identyfikator sekcji krytycznej okna.
#define CORE_SECTION		4												//!< Identyfikator sekcji krytycznej jądra.

// Commands ID's
#define CMD_UNKNOWN			0												//!< Identyfikator nieznanego polecenia.

#define CMD_RCON			1												//!< Identyfikator polecenia "rcon".
#define CMD_SET			2												//!< Identyfikator polecenia "set".
#define CMD_GET			3												//!< Identyfikator polecenia "get".

#define CMD_BYE			10												//!< Identyfikator polecenia "bye".

// Controls ID's
#define CTR_TAB_CATS		101												//!< Identyfikator kontrolki zakładek.

#define CTR_TABLE_VAR		201												//!< Identyfikator kontrolki tabeli zmiennych.
#define CTR_TABLE_CLI		202												//!< Identyfikator kontrolki tabeli klientów.

#define CTR_BTN_ID			300												//!< Identyfikator początku indeksów przycisków.
#define CTR_BTN_LOAD		301												//!< Identyfikator kontrolki przycisku "Wczytaj".
#define CTR_BTN_SAVE		302												//!< Identyfikator kontrolki przycisku "Zapisz".
#define CTR_BTN_KICK		303												//!< Identyfikator kontrolki przycisku "Rozłącz".
#define CTR_BTN_RESET		304												//!< Identyfikator kontrolki przycisku "Restart".
#define CTR_BTN_HELP		305												//!< Identyfikator kontrolki przycisku "Pomoc".
#define CTR_BTN_ABOUT		306												//!< Identyfikator kontrolki przycisku "O programie".

#define CTR_CHK_LISTEN		401												//!< Identyfikator kontrolki pola "Nasłuchiwanie".
#define CTR_CHK_CONS		402												//!< Identyfikator kontrolki pola "Konsola".
#define CTR_CHK_DEBUG		403												//!< Identyfikator kontrolki pola "Debug".

#define CTR_CHK_SLO			411												//!< Identyfikator kontrolki pola "salon.lights.on".
#define CTR_CHK_SBO			412												//!< Identyfikator kontrolki pola "salon.blinds.on".
#define CTR_CHK_SHO			413												//!< Identyfikator kontrolki pola "salon.heat.on".

#define CTR_CHK_PLO			421												//!< Identyfikator kontrolki pola "porch.light.on".
#define CTR_CHK_PDC			422												//!< Identyfikator kontrolki pola "doors.lock".

#define CTR_BAR_SLP			511												//!< Identyfikator kontrolki paska "salon.light.power".
#define CTR_BAR_SBP			512												//!< Identyfikator kontrolki paska "salon.blinds.power".
#define CTR_BAR_SHP			513												//!< Identyfikator kontrolki paska "salon.heat.set".

#define CTR_BAR_PLP			521												//!< Identyfikator kontrolki paska "porch.light.power".

// Resources ID's
#define IDI_MAIN			1001												//!< Identyfikator zasobu ikony programu.

#endif
