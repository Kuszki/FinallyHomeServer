/*********************************************

    FinallyHome Server "core" module source

    copyright: KuszkiDevDroup

    license: GNU GPL v2

*********************************************/

/*! \file
    \brief Plik zawierający definicja dla modułu "core".

Plik jest dołączany przez server.hpp.
*/

ServerCore::ServerCore(void)
: iConfig(S"../conf/config.ini"), Console(true)
{

    mVars.Add(S"porch.light.on", iConfig.GetInt(S"VARS", S"porch.light.on"));
	mVars.Add(S"porch.light.power", iConfig.GetInt(S"VARS", S"porch.light.power"));

	mVars.Add(S"doors.lock", iConfig.GetInt(S"VARS", S"doors.lock"));

	mVars.Add(S"salon.light.on", iConfig.GetInt(S"VARS", S"salon.light.on"));
	mVars.Add(S"salon.light.power", iConfig.GetInt(S"VARS", S"salon.light.power"));

	mVars.Add(S"salon.blinds.on", iConfig.GetInt(S"VARS", S"salon.blinds.on"));
	mVars.Add(S"salon.blinds.power", iConfig.GetInt(S"VARS", S"salon.blinds.power"));

	mVars.Add(S"salon.heat.on", iConfig.GetInt(S"VARS", S"salon.heat.on"));
	mVars.Add(S"salon.heat.power", iConfig.GetInt(S"VARS", S"salon.heat.power"));

}

ServerCore::~ServerCore(void)
{

}

