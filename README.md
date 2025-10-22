# tiny-track

A minimalist, MLFlow-compatible, header-only C++ experiment tracking library with Python bindings.

## C++

### Usage

```c++
#include <string>
#include <iostream>

#include "ttrack/local_logger.hpp"
#include "ttrack/utils.hpp"

int main() {
    // start run
    ttrack::LocalLogger logger(
        /*logging_dir=*/"mlruns",  // default logging dir used by mlflow is "mlruns"
        /*experiment_name=*/"My experiment name",
        /*run_name=*/"My run name",
        /*source=*/std::string(__FILE__)
    );

    // add tags
    logger.add_tag(/*key=*/"myTagName", /*value=*/"myTagValue");

    // add params
    logger.log_param(/*key=*/"myParamName", /*value=*/"myParamValue");

    // log metrics
    for (int i = 0; i < 10; ++i) {
        logger.log_metric(/*key=*/"myMetric", /*value=*/i*i, /*step=*/i+1);
    }

    // print existing experiments
    auto experiments = ttrack::get_experiments(/*logging_dir=*/"mlruns");
    for (const auto& [experiment_uuid, experiment_name] : experiments) {
        std::cout << "'" << experiment_uuid << "': '" << experiment_name << "'" << std::endl;
    }

    return 0;
}
```

## Python

### Prerequisites

- Linux (Ubuntu/Debian tested)
- Python 3.10, 3.11 or 3.12

### Installation

Install latest release from PyPI:

```sh
pip install -U tiny-track
```

Or build from source:

```sh
# clone the repository
git clone https://github.com/Kowd-PauUh/tiny-track.git
cd tiny-track

# install make
sudo apt-get update && sudo apt-get install -y make

# setup .venv
make venv

# build .so files
make install_g++
make build

# activate .venv
source .venv/bin/activate
```

### Usage

Logging via Python API:

```python
from ttrack.ttrack_cpp import LocalLogger, get_experiments

# start run
logger = LocalLogger(
    logging_dir='mlruns',  # default logging dir used by mlflow is "mlruns"
    experiment_name='My experiment name',
    run_name='My run name',
    source=__file__,
)

# add tags
logger.add_tag(key='myTagName', value='myTagValue')

# add params
logger.log_param(key='myParamName', value='myParamValue')

# log metrics
for i in range(10):
    logger.log_metric(key='myMetric', value=i**2, step=i+1)

# print existing experiments
print(get_experiments(logging_dir='mlruns'))
```

## Limitations

- The UUID generator for runs and experiments (`ttrack::uuid_v4` in `include/ttrack/uuid.hpp`) uses a simple implementation that seeds `rand()` with the current time in seconds. Although this is sufficient for most ML experiment tracking purposes, it **results in one experiment or run overwriting other if both were created within the same second.**
