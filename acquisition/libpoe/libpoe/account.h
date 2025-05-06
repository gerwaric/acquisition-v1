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

#include <libpoe/guild.h>

#include <optional>

namespace poe {

    // https://www.pathofexile.com/developer/docs/reference#type-Account

    struct Account {

        struct Challenges {
            QString set; // string the challenge set
            unsigned completed; // uint
            unsigned max; // uint
            JS_OBJ(set, completed, max);
        };

        struct TwitchStream {
            QString name; // string
            QString image; // string
            QString status; // string
            JS_OBJ(name, image, status);
        };

        struct Twitch {
            QString name; // string
			std::optional<poe::Account::TwitchStream> stream; // ? object
            JS_OBJ(name, stream);
        };

        QString name; // string
        std::optional<QString> realm; // ? string pc, xbox, or sony
		std::optional<poe::Guild> guild; // ? Guild
		std::optional<poe::Account::Challenges> challenges; // ? object
		std::optional<poe::Account::Twitch> twitch; // ? object
        JS_OBJ(name, realm, guild, challenges, twitch);
    };
}
