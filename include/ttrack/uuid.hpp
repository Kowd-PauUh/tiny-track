/*
 * Copyright 2025 Ivan Danylenko
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <cstdlib>
#include <string>
#include <ctime>

namespace ttrack {

std::string uuid_v4() {
    // seed rand
    static bool seeded = false;
    if (!seeded) {
        srand(static_cast<unsigned int>(time(nullptr)));
        seeded = true;
    }

    static const char* hex_chars = "0123456789abcdef";
    char uuid[37];

    // generate uuid
    for (int i = 0; i < 36; ++i) {
        switch (i) {
            case 8: case 13: case 18: case 23:
                uuid[i] = '-';
                break;
            case 14:
                uuid[i] = '4';
                break;
            case 19:
                uuid[i] = hex_chars[(rand() % 4) + 8];
                break;
            default:
                uuid[i] = hex_chars[rand() % 16];
        }
    }
    uuid[36] = '\0';
    return std::string(uuid);
}

}  // ttrack namespace
