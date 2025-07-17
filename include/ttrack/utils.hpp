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

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <filesystem>
#include <map>

namespace ttrack {

/**
 * @brief Returns a map of experiment UUIDs to their names.
 * @param logging_dir Logging dir where to search for experiments.
 */
std::map<std::string, std::string> get_experiments(const std::string& logging_dir) {
    std::map<std::string, std::string> uuid_name_map;

    // return empty map if logging dir does not exist yet
    if (!std::filesystem::exists(logging_dir)) {
        return uuid_name_map;
    }

    // iterate over subdirectories (experiments) in logging dir
    for (const auto& entry : std::filesystem::directory_iterator(logging_dir)) {
        if (!entry.is_directory()) continue;

        std::string uuid = entry.path().filename().string();
        std::string meta_file_path = entry.path().string() + "/meta.yaml";

        // open meta.yaml file
        std::ifstream meta_file(meta_file_path);
        if (!meta_file) continue;

        // get experiment name
        std::string line;
        while (std::getline(meta_file, line)) {
            if (line.rfind("name: ", 0) == 0) {
                uuid_name_map[uuid] = line.substr(6);
                break;
            }
        }
    }

    return uuid_name_map;
}

/**
 * @brief Saves meta.yaml file under <logging_dir>/<uuid>.
 * @param uuid Experiment UUID.
 * @param experiment_name Experiment name to appear in meta.yaml.
 * @param logging_dir Logging dir where to create an experiment.
 */
void create_experiment(
    const std::string& uuid,
    const std::string& experiment_name,
    const std::string& logging_dir
) {
    // create experiment dir
    std::string experiment_dir = logging_dir + "/" + uuid;
    std::filesystem::create_directories(experiment_dir);

    std::string meta_file_path = experiment_dir + "/meta.yaml";

    int now = static_cast<unsigned int>(time(nullptr));

    // open file
    std::ofstream meta_file(meta_file_path);
    if (!meta_file) {
        std::cerr << "Failed to create file: " << meta_file_path << std::endl;
        return;
    }

    // write metadata
    meta_file << "artifact_location: mlflow-artifacts:/" << uuid << "\n";
    meta_file << "creation_time: " << now << "\n";
    meta_file << "experiment_id: '" << uuid << "'\n";
    meta_file << "last_update_time: " << now << "\n";
    meta_file << "lifecycle_stage: active\n";
    meta_file << "name: " << experiment_name << "\n";

    meta_file.close();
}

}  // ttrack namespace
