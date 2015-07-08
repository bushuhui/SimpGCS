TOPDIR := ../..

#inclue system configure
include $(TOPDIR)/make.conf

ALL_CFLAGS += -DDSFMT_MEXP=19937 -DHAVE_SSE2 -msse4


src-all := $(wildcard *.cpp)
obj-all := $(patsubst %.cpp,%.o,$(src-all))
inc-all := $(wildcard *.h)
bin-all := $(patsubst %.cpp,%.e,$(src-all))

target  := librtk_utils.a

all : $(target)

$(target) : $(obj-all) $(inc-all)
	$(AR) rcs $@ $(obj-all) 
	$(CP) -f $(target) $(TOPDIR)/lib/$(target)

%.e:%.cpp $(inc-y)
	$(CXX) $< -o $@ $(ALL_CFLAGS) $(ALL_LDFLAGS)

%.o:%.cpp
	$(CXX) -c $< -o $@ $(ALL_CFLAGS)

clean :
	rm -f *.e *.o *.a *.so

