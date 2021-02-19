.SILENT: shogi_manual shogi_speed

all: shogi_speed shogi_manual

shogi_manual:
	g++ ./src/core.cpp ./src/viewing.cpp ./src/main_manual.cpp -o shogi_manual -O3
shogi_speed:
	g++ ./src/core.cpp ./src/viewing.cpp ./src/main_speedtest.cpp -o shogi_speedtest -O3
clean:
	rm ./shogi_manual ./shogi_speedtest