all: vv_chess_phone_numbers permute_string

vv_chess_phone_numbers: vv_chess_phone_numbers.cpp
	g++ -g -std=c++11  -o vv_chess_phone_numbers vv_chess_phone_numbers.cpp

permute_string: permute_string.cpp
	g++ -g -std=c++11  -o permute_string permute_string.cpp

clean:
	rm vv_chess_phone_numbers permute_string
