# install g++
install_g++:
	@ sudo apt-get update
	@ sudo apt-get install g++ -y

# create .venv, install dependencies and ttrack in dev mode
venv: 
	@ python3 -m venv .venv
	@ bash -c "source .venv/bin/activate && pip install -r requirements.txt && pip install -e ."

# build ttrack.cpp.so
_build:
	@ g++ -O3 -Wall -shared -std=c++17 \
	      -fPIC ttrack/bindings.cpp \
		  -Iinclude \
		  $(shell python3 -m pybind11 --includes) \
		  -o ttrack/ttrack.cpp.so

# activate .venv and build ttrack.cpp.so
build:
	@ bash -c "source .venv/bin/activate && make _build --no-print-directory"
