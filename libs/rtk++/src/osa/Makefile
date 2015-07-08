###############################################################################
#
#  Robot Toolkit ++ (RTK++)
#
#  Copyright (c) 2007-2013 Shuhui Bu <bushuhui@nwpu.edu.cn>
#  http://www.adv-ci.com
#
#  ----------------------------------------------------------------------------
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program. If not, see <http://www.gnu.org/licenses/>.
#
###############################################################################

TOPDIR := ../..

#inclue system configure
include $(TOPDIR)/make.conf

src-all := $(wildcard *.cpp)
obj-all := $(patsubst %.cpp,%.o,$(src-all))
inc-all := $(wildcard *.h)
bin-all := $(patsubst %.cpp,%.e,$(src-all))

obj-all := linux/osa_cv_linux.o linux/osa_mq_linux.o linux/osa_semaphore_linux.o \
           linux/osa_tm_linux.o linux/osa_event_linux.o linux/osa_mutex_linux.o \
           linux/osa_thread_linux.o linux/osa_tsd_linux.o \
           linux/osa_utils_linux.o \
           _osa_in.o osa_main.o osa_mem.o rtk_osa++.o

target := librtk_osa.a


all : $(target)

$(target) : $(obj-all) $(inc-all)
	$(AR) rcs $@ $(obj-all) 
	$(CP) -f $(target) $(TOPDIR)/lib/$(target)

%.e:%.cpp $(inc-y)
	$(CXX) $< -o $@ $(ALL_CFLAGS) $(ALL_LDFLAGS) -lm

%.o:%.cpp
	$(CXX) -c $< -o $@ $(ALL_CFLAGS)

clean :
	rm -f *.e *.o *.a *.so linux/*.o

