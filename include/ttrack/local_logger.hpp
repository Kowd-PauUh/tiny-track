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
#include <ctime>

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

    inline void log_metric(const std::string& key, double value, int step);

    inline void add_tag(const std::string& key, const std::string& value);

private:
    std::string source;
    std::string logging_dir;
    std::string experiment_name;
    std::string experiment_uuid;
    std::string run_name;
    std::string run_uuid;
    std::string run_dir;

    inline void write_to_file(
        const std::string& category,
        const std::string& key,
        const std::string& value,
        std::ios_base::openmode mode
    );

};

/**
 * @brief Writes a key-value pair to a specific file in the run directory.
 *
 * This method constructs the full path as:
 * <run_dir>/<category>/<key>
 * and writes the value using the provided I/O mode.
 *
 * @param category Subdirectory name (e.g., "params", "metrics", "tags").
 * @param key File name within the category directory.
 * @param value Content to write into the file.
 * @param mode File open mode (e.g., std::ios::out or std::ios::app).
 */
inline void LocalLogger::write_to_file(
    const std::string& category,
    const std::string& key,
    const std::string& value,
    std::ios_base::openmode mode
) {
    std::string file_path = run_dir + "/" + category + "/" + key;

    // open file
    std::ofstream file(file_path, mode);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + file_path);
    }

    // write to file
    file << value;
    file.close();
}

/**
 * @brief Logs a parameter as a key-value pair.
 *
 * Writes the parameter to the file:
 * <run_dir>/params/<key>
 * Overwrites if already exists.
 *
 * @param key Name of the parameter.
 * @param value Value of the parameter.
 */
inline void LocalLogger::log_param(const std::string& key, const std::string& value) {
    write_to_file(/*category=*/"params", /*key=*/key, /*value=*/value, /*mode=*/std::ios::out);
}

/**
 * @brief Logs a metric value with timestamp and step.
 *
 * Appends a line in the format:
 * <timestamp(ms)> <value> <step>\n
 * to the file:
 * <run_dir>/metrics/<key>
 *
 * @param key Name of the metric.
 * @param value Metric value.
 * @param step Step index (e.g., epoch or iteration).
 */
inline void LocalLogger::log_metric(const std::string& key, double value, int step) {
    int now = static_cast<unsigned int>(time(nullptr));
    std::string line = std::to_string(now) + "000 " + std::to_string(value) + " " + std::to_string(step) + "\n";
    write_to_file(/*category=*/"metrics", /*key=*/key, /*value=*/line, /*mode=*/std::ios::app);
}

/**
 * @brief Adds a tag to the current run.
 *
 * Writes the tag value to:
 * <run_dir>/tags/<key>
 * Overwrites if already exists.
 *
 * @param key Name of the tag.
 * @param value Value of the tag.
 */
inline void LocalLogger::add_tag(const std::string& key, const std::string& value) {
    write_to_file(/*category=*/"tags", /*key=*/key, /*value=*/value, /*mode=*/std::ios::out);
}

}  // ttrack namespace
