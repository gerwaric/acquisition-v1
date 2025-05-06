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

#include <vector>

namespace poe{

    // https://www.pathofexile.com/developer/docs/reference#type-LeagueAccount           

    struct LeagueAccount {

        struct AtlasPassiveTree {
            QString name; // string
            std::vector<unsigned> hashes; // array of uint
            JS_OBJ(name, hashes);
        };

		std::vector<poe::LeagueAccount::AtlasPassiveTree> atlas_passive_trees; // array of object
        JS_OBJ(atlas_passive_trees);

    };

}
