CPPFLAGS=$(COUPLER_BUILD_CPPFLAGS) -I./ -g -D_DEBUG_ -Wno-deprecated -Wparentheses -Wreturn-type -Wunused
LDFLAGS=$(COUPLER_BUILD_LDFLAGS)

workshop: $(patsubst %.cpp, %.o, $(wildcard *.cpp))
	gcc $^ -o $@ $(LDFLAGS)
	gcc -shared $^ $(LDFLAGS) -o libcoupler.so

%.o: %.cpp
	gcc -x c++ $(CPPFLAGS) -fPIC -c -MMD $<

include $(wildcard *.d) 

clean:
	rm -f *.o *.d *~ 
