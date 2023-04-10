//
//  LISA_SH_Solver.h
//  LISA_SH_Solver
//
//  Created by Jakub Nowak on 12/03/2023.
//

#ifndef LISA_SH_Solver_h
#define LISA_SH_Solver_h

/* All includes for project */
#include "../src/solver/solver.hpp"
#include "../src/settings/settings.h"
#include "../src/functions/functions.h"
/* Banner */
const std::string banner =
R"(
___       ___  ________  ________          ________  ___  ___
|\  \     |\  \|\   ____\|\   __  \        |\   ____\|\  \|\  \
\ \  \    \ \  \ \  \___|\ \  \|\  \       \ \  \___|\ \  \\\  \
\ \  \    \ \  \ \_____  \ \   __  \       \ \_____  \ \   __  \
\ \  \____\ \  \|____|\  \ \  \ \  \       \|____|\  \ \  \ \  \
 \ \_______\ \__\____\_\  \ \__\ \__\        ____\_\  \ \__\ \__\
  \|_______|\|__|\_________\|__|\|__|       |\_________\|__|\|__|
                \|_________|                \|_________|
                                                                 
                                                                 
________  ________  ___       ___      ___ _______   ________
|\   ____\|\   __  \|\  \     |\  \    /  /|\  ___ \ |\   __  \
\ \  \___|\ \  \|\  \ \  \    \ \  \  /  / | \   __/|\ \  \|\  \
\ \_____  \ \  \\\  \ \  \    \ \  \/  / / \ \  \_|/_\ \   _  _\
\|____|\  \ \  \\\  \ \  \____\ \    / /   \ \  \_|\ \ \  \\  \|
  ____\_\  \ \_______\ \_______\ \__/ /     \ \_______\ \__\\ _\
 |\_________\|_______|\|_______|\|__|/       \|_______|\|__|\|__|
 \|_________|
)";

inline void addBrewToPATH() {setenv("PATH", "/Applications/Xcode.app/Contents/Developer/usr/bin:/usr/bin:/bin:/usr/sbin:/sbin:/opt/homebrew/bin", 1);};

#endif /* LISA_SH_Solver_h */
