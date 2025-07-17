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
        const std::string& run_name,
        const std::string& source = std::string(__FILE__)
    ) : logging_dir(logging_dir),
        experiment_name(experiment_name),
        run_name(run_name)
    {
        // get existing experiments
        std::map<std::string, std::string> uuid_name_map = ttrack::get_experiments(logging_dir);

        // prepare new uuid in case experiment doesn't exist
        std::string experiment_id = ttrack::uuid_v4();

        // get experiment uuid by name if exists
        bool exists = false;
        for (const auto& pair : uuid_name_map) {
            if (experiment_name == pair.second) {
                experiment_id = pair.first;
                exists = true;
                break;
            }
        }

        // create experiment if not exists
        if (!exists) {
            ttrack::create_experiment(/*uuid=*/experiment_id, /*experiment_name=*/experiment_name, /*logging_dir=*/logging_dir);
        }

        // create run
        std::string run_id = ttrack::uuid_v4();
        ttrack::create_run(/*uuid=*/run_id, /*run_name=*/run_name, /*experiment_uuid=*/experiment_id, /*logging_dir=*/logging_dir);
        
        experiment_uuid = experiment_id;
        run_uuid = run_id;
        run_dir = logging_dir + "/" + experiment_uuid + "/" + run_uuid;

        // add basic tags to the run
        add_tag(/*key=*/"mlflow.runName",           /*value=*/run_name);
        add_tag(/*key=*/"mlflow.source.git.commit", /*value=*/"unknown");
        add_tag(/*key=*/"mlflow.source.name",       /*value=*/source);
        add_tag(/*key=*/"mlflow.source.type",       /*value=*/"LOCAL");
        add_tag(/*key=*/"mlflow.user",              /*value=*/"cpp");
    }

    inline void log_param(const std::string& key, const std::string& value);

    inline void log_metric(const std::string& key, double value);

    inline void add_tag(const std::string& key, const std::string& value);

private:
    std::string source;
    std::string logging_dir;
    std::string experiment_name;
    std::string experiment_uuid;
    std::string run_name;
    std::string run_uuid;
    std::string run_dir;
};

inline void LocalLogger::log_param(const std::string& key, const std::string& value) {
    return;  // TODO
}

inline void LocalLogger::log_metric(const std::string& key, double value) {
    return;  // TODO
}

inline void LocalLogger::add_tag(const std::string& key, const std::string& value) {
    std::string tag_file_path = run_dir + "/tags/" + key;

    // open file
    std::ofstream tag_file(tag_file_path);
    if (!tag_file) {
        throw std::runtime_error("Failed to create file: " + tag_file_path);
    }

    // write to file
    tag_file << value;
    tag_file.close();
}

}  // ttrack namespace
