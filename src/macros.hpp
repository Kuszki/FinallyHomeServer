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

#define CTR_TABLE_VAR		201
#define CTR_TABLE_CLI		202

#define CTR_BTN_ID			300
#define CTR_BTN_LOAD		301
#define CTR_BTN_SAVE		302
#define CTR_BTN_KICK		303
#define CTR_BTN_RESET		304
#define CTR_BTN_HELP		305
#define CTR_BTN_ABOUT		306

#define CTR_CHK_LISTEN		401
#define CTR_CHK_CONS		402
#define CTR_CHK_DEBUG		403

#define CTR_CHK_SLO			411
#define CTR_CHK_SBO			412
#define CTR_CHK_SHO			413

#define CTR_CHK_PLO			421
#define CTR_CHK_PDC			422

#define CTR_BAR_SLP			511
#define CTR_BAR_SBP			512
#define CTR_BAR_SHP			513

#define CTR_BAR_PLP			521

// Resources ID's
#define IDI_MAIN			1001

#endif
