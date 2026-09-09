all:
	g++ ChaCha20.cpp main.cpp portable_endian.cpp -std=c++20 -o MEncryptor -lbe
