## Copyright (C) 2009 Pebble Bed Modular Reactor (Pty) Limited (PBMR)
## 
## This library is free software; you can redistribute it and/or 
## modify it under the terms of the GNU Lesser General Public 
## License as published by the Free Software Foundation; either 
## version 2.1 of the License. 
## 
## This library is distributed in the hope that it will be useful, 
## but WITHOUT ANY WARRANTY; without even the implied warranty of 
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
## Lesser General Public License for more details. 
## 
## You should have received a copy of the GNU Lesser General Public 
## License along with this library; if not, write to the Free Software 
## Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
## 
## See http://sourceforge.net/projects/pyfoam
##
## Author : Alexey PETROV
##


#--------------------------------------------------------------------------------------
__FOAM_VERSION__ := $(subst -dev,,$(WM_PROJECT_VERSION))
__FOAM_VERSION__ := $(subst 1.4.1,010401,$(__FOAM_VERSION__))
__FOAM_VERSION__ := $(subst 1.5,010500,$(__FOAM_VERSION__))
__FOAM_VERSION__ := $(subst 1.6,010600,$(__FOAM_VERSION__))

#--------------------------------------------------------------------------------------
ifdef WM_PROJECT_VERSION
	__OS_INCLUDES__ = $(shell if [ ${__FOAM_VERSION__} -lt 010600 ]; then echo Unix; else echo POSIX; fi )
endif

__CXX_INCLUDES__ := $(__CXX_INCLUDES__)\
	-I$(WM_PROJECT_DIR)/src/OpenFOAM/lnInclude\
	-I$(WM_PROJECT_DIR)/src/OSspecific/$(__OS_INCLUDES__)/lnInclude


#--------------------------------------------------------------------------------------
__LIB_FLAGS__ := $(__LIB_FLAGS__) -lstdc++

ifdef WM_PROJECT_VERSION
	__LIB_FLAGS__ +=\
		-lpthread -ldl -lm\
		-L$(WM_PROJECT_DIR)/lib/$(WM_OPTIONS) -lOpenFOAM
endif


#--------------------------------------------------------------------------------------
__OBJ_FLAGS__ = $(__CXX_INCLUDES__)\
	-g -Wall -Wno-strict-aliasing -Wextra -Wno-unused-parameter -march=opteron -ftemplate-depth-40

ifdef WM_PROJECT_VERSION
	__PRECISION_OPTION__ = $(shell if [ ${__FOAM_VERSION__} -lt 010600 ]; then echo ${WM_PRECISION_OPTION}; else echo WM_$(WM_PRECISION_OPTION); fi )
	__OBJ_FLAGS__ += $(WM_CFLAGS) -D$(WM_ARCH) -D$(__PRECISION_OPTION__) -D__FOAM_VERSION__=$(__FOAM_VERSION__) -DNoRepository
endif


#--------------------------------------------------------------------------------------
$(EXTFOAM_ROOT_DIR)/common/lib/lib$(target).so: $(notdir $(patsubst %.C,%.o,$(sources)))
	c++ -shared $^ $(__LIB_FLAGS__) -o $@

source_dirs = $(local_dir)/lnInclude $(external_dir)/lnInclude 

VPATH := $(source_dirs)


#--------------------------------------------------------------------------------------
%.o: %.C
	gcc -c -MD $(__OBJ_FLAGS__) $(addprefix -I,$(source_dirs)) $<


#--------------------------------------------------------------------------------------
include $(wildcard *.d) 


#--------------------------------------------------------------------------------------
clean: 
	rm -fr *.o *.d* *~


#--------------------------------------------------------------------------------------
