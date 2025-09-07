bin/flight_management:src/*.c  include/*.h 
	gcc -w -o $@ $^ 

clean:
	rm bin/flight_management 