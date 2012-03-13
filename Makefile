all:
	g++ -shared -fPIC -Wall -g mysql_geo_distance.cc -o mysql_geo_distance.so
