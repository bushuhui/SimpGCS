TOPDIR := .

#inclue system configure
include $(TOPDIR)/make.conf

src-all := $(wildcard *.cpp)
obj-all := $(patsubst %.cpp,%.o,$(src-all))
inc-all := $(wildcard *.h)
bin-all := $(patsubst %.cpp,%.e,$(src-all))

all :
	$(MAKE) -C src
	$(MAKE) -C test

%.e:%.cpp $(inc-y)
	$(CXX) $< -o $@ $(ALL_CFLAGS) $(ALL_LDFLAGS) -lm

%.o:%.cpp
	$(CXX) -c $< -o $@ $(ALL_CFLAGS)

clean :
	$(RM) -f *.e *.o *.a *.so
	./make_clean ./src
	./make_clean ./test
