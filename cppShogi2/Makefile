.SILENT: shogi_manual shogi_speed python clean

all: python shogi_speed shogi_manual 

python:
	g++ -O3 -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` ./src/core.cpp ./src/viewing.cpp ./src/extract.cpp ./src/pythonbinder.cpp -o ShogiAgent.so
shogi_manual:
	g++ ./src/core.cpp ./src/viewing.cpp ./src/main_manual.cpp ./src/extract.cpp -o shogi_manual -O3
shogi_speed:
	g++ ./src/core.cpp ./src/viewing.cpp ./src/main_speedtest.cpp -o shogi_speedtest -O3
clean:
	rm ./shogi_manual ./shogi_speedtest ./ShogiAgent.so