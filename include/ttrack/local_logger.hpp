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

#include <string>

#include "ttrack/utils.hpp"
#include "ttrack/uuid.hpp"

namespace ttrack {

class LocalLogger {
public:
    explicit LocalLogger(
        const std::string& logging_dir,
        const std::string& experiment_name,
        const std::string& run_name
    ) : logging_dir(logging_dir),
        experiment_name(experiment_name),
        run_name(run_name)
    {
        // get existing experiments
        std::map<std::string, std::string> uuid_name_map = ttrack::get_experiments(logging_dir);

        // prepare new uuid in case experiment doesn't exist
        std::string uuid = ttrack::uuid_v4();

        // get experiment uuid by name if exists
        for (const auto& pair : uuid_name_map) {
            if (experiment_name == pair.second) {
                uuid = pair.first;
                break;
            }
        }
        
        experiment_uuid = uuid;
    }

    inline void log_param(const std::string& key, const std::string& value);

    inline void log_metric(const std::string& key, double value);

private:
    std::string logging_dir;
    std::string experiment_name;
    std::string experiment_uuid;
    std::string run_name;
};

inline void LocalLogger::log_param(const std::string& key, const std::string& value) {
    return;  // TODO
}

inline void LocalLogger::log_metric(const std::string& key, double value) {
    return;  // TODO
}

}  // ttrack namespace
