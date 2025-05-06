/*
    Copyright (C) 2024-2025 Gerwaric

    This file is part of Acquisition.

    Acquisition is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Acquisition is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Acquisition.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <utils/json_struct_qt.h>

#include <optional>

namespace poe{

    // https://www.pathofexile.com/developer/docs/reference#type-LeagueRule
    
    struct LeagueRule {
        QString                              id;             // string examples : Hardcore, NoParties(SSF)
        QString                              name;           // string
        std::optional<QString>                    description;    // ? string
        JS_OBJ(id, name, description);
    };

}
