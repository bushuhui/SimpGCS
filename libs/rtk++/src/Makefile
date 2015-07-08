###############################################################################
#
#  Robot Toolkit ++ (RTK++)

#  Copyright (c) 2007-2013 Shuhui Bu <bushuhui@nwpu.edu.cn>
#  http://www.adv-ci.com

#  ----------------------------------------------------------------------------
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU Affero General Public License as
#  published by the Free Software Foundation, either version 3 of the
#  License, or (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#  GNU Affero General Public License for more details.
#
#  You should have received a copy of the GNU Affero General Public License
#  along with this program. If not, see <http://www.gnu.org/licenses/>.
###############################################################################

TOPDIR := ..

#inclue system configure
include $(TOPDIR)/make.conf

src-all := $(wildcard *.cpp)
obj-all := $(patsubst %.cpp,%.o,$(src-all))
inc-all := $(wildcard *.h)
bin-all := $(patsubst %.cpp,%.e,$(src-all))

all :
	#$(MAKE) -C cv
	#$(MAKE) -C gui
	$(MAKE) -C osa
	#$(MAKE) -C pr
	$(MAKE) -C utils


clean :
	$(RM) -f *.e *.o *.a *.so
	../make_clean ./cv
	../make_clean ./gui
	../make_clean ./osa
	../make_clean ./pr
	../make_clean ./utils
