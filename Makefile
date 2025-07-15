# install g++
install_g++:
	@ sudo apt-get update
	@ sudo apt-get install g++ -y

# install g++
install_uuid:
	@ sudo apt-get update
	@ sudo apt-get install uuid-dev -y

# create .venv, install dependencies and ttrack in dev mode
venv: 
	@ python3 -m venv .venv
	@ bash -c "source .venv/bin/activate && pip install -r requirements.txt && pip install -e ."

# build ttrack.cpp.so
_build:
	@ dpkg -s uuid-dev >/dev/null 2>&1 || ( $(MAKE) --no-print-directory install_uuid )
	@ g++ -O3 -Wall -shared -std=c++17 \
	      -fPIC ttrack/bindings.cpp \
		  -Iinclude -I/usr/include/uuid \
		  $(shell python3 -m pybind11 --includes) \
		  -o ttrack/ttrack_cpp.so

# activate .venv and build ttrack.cpp.so
build:
	@ bash -c "source .venv/bin/activate && make _build --no-print-directory"
