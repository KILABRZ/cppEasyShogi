.SILENT: shogi_manual shogi_speed python clean shogi_strmanual shogi_strspeed shogi_strsl

all: python shogi_speed shogi_manual shogi_strmanual shogi_strspeed shogi_strsl

python:
	g++ -O3 -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` ./src/core.cpp ./src/viewing.cpp ./src/extract.cpp ./src/describe.cpp ./src/pythonbinder.cpp -o ShogiAgent.so
shogi_manual:
	g++ ./src/core.cpp ./src/viewing.cpp ./src/main_manual.cpp ./src/extract.cpp -o shogi_manual -O3
shogi_speed:
	g++ ./src/core.cpp ./src/viewing.cpp ./src/main_speed.cpp -o shogi_speed -O3
shogi_strmanual:
	g++ ./src/core.cpp ./src/viewing.cpp ./src/describe.cpp ./src/main_strmanual.cpp -o shogi_strmanual -O3
shogi_strspeed:
	g++ ./src/core.cpp ./src/viewing.cpp ./src/describe.cpp ./src/main_strspeed.cpp -o shogi_strspeed -O3
shogi_strsl:
	g++ ./src/core.cpp ./src/viewing.cpp ./src/describe.cpp ./src/main_strsl.cpp -o shogi_strsl -O3

clean:
	rm ./shogi_manual ./shogi_speed ./ShogiAgent.so ./shogi_strmanual ./shogi_strspeed ./shogi_strsl