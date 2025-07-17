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

#include <pybind11/pybind11.h>
#include <string>

#include "ttrack/local_logger.hpp"
#include "ttrack/uuid.hpp"

namespace py = pybind11;

PYBIND11_MODULE(ttrack_cpp, m) {
    py::class_<ttrack::LocalLogger>(m, "LocalLogger")
        .def(py::init<const std::string&>(), py::arg("logging_dir"))
        .def("log_param", &ttrack::LocalLogger::log_param, py::arg("key"), py::arg("value"))
        .def("log_metric", &ttrack::LocalLogger::log_metric, py::arg("key"), py::arg("value"));
    m.def("uuid_v4", &ttrack::uuid_v4);
}
