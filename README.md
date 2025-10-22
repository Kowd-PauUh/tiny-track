# tiny-track

A minimalist, MLFlow-compatible, header-only C++ experiment tracking library with Python bindings.

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
