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

#include <acquisition/common/json_struct_qt.h>

#include <acquisition/poe_api/displaymode.h>

#include <optional>
#include <vector>

namespace poe {
    
    // https://www.pathofexile.com/developer/docs/reference#type-ItemProperty

    struct ItemProperty {
        QString name; // string
        std::vector<std::tuple<QString, int>> values; // array of array
		std::optional<poe::DisplayMode> displayMode; // ? uint as DisplayMode
        std::optional<double>progress; // ? double rounded to 2 decimal places
        std::optional<unsigned> type; // ? uint
        std::optional<QString> suffix; // ? string
        JS_OBJ(name, values, displayMode, progress, type, suffix);
    };

}
