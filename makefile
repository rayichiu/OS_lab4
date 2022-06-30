iosched: iosched.cpp
	g++ -std=c++11 iosched.cpp IOSchedulerAlgo.cpp -o iosched

clean:
	rm -f iosched *~