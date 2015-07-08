TOPDIR := ../..

#inclue system configure
include $(TOPDIR)/make.conf

src-all := $(wildcard *.cpp)
obj-all := $(patsubst %.cpp,%.o,$(src-all))
inc-all := $(wildcard *.h)
bin-all := $(patsubst %.cpp,%.e,$(src-all))

target  := test_osa.e

all : $(target)

$(target) : $(obj-all) $(inc-all)
	$(CXX) -o $(target) $(obj-all) $(ALL_LDFLAGS)

%.e:%.cpp $(inc-y)
	$(CXX) $< -o $@ $(ALL_CFLAGS) $(ALL_LDFLAGS)

%.o:%.cpp
	$(CXX) -c $< -o $@ $(ALL_CFLAGS)

clean :
	rm -f *.e *.o *.a *.so

