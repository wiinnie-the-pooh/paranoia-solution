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


#------------------------------------------------------------------------------
# set compilation and dependency building rules
#------------------------------------------------------------------------------
GENERAL_RULES = $(WM_DIR)/rules/General
RULES         = $(WM_DIR)/rules/$(WM_ARCH)$(WM_COMPILER)
BIN           = $(WM_DIR)/bin/$(WM_ARCH)$(WM_COMPILER)

include $(GENERAL_RULES)/general
include $(RULES)/general
include $(RULES)/$(WM_LINK_LANGUAGE)

c++FLAGS:=$(shell echo $(c++FLAGS) | sed -e"s%-Wold-style-cast %%g")


#------------------------------------------------------------------------------
# declare default paths
#------------------------------------------------------------------------------
LIB_SRC            = $(WM_PROJECT_DIR)/src
LIB_DIR            = $(WM_PROJECT_DIR)/lib
LIB_WM_OPTIONS_DIR = $(LIB_DIR)/$(WM_OPTIONS)
OBJECTS_DIR        = $(MAKE_DIR)/$(WM_OPTIONS)
CLASSES_DIR        = $(MAKE_DIR)/classes

SYS_INC            =
SYS_LIBS           =

PROJECT_INC        = -I$(LIB_SRC)/$(WM_PROJECT)/lnInclude -I$(LIB_SRC)/OSspecific/$(WM_OSTYPE)/lnInclude
PROJECT_LIBS       = -l$(WM_PROJECT)


#------------------------------------------------------------------------------
CPPFLAGS := $(CPPFLAGS) $(c++FLAGS) $(PROJECT_INC) \
	-I$(WM_PROJECT_DIR)/src/finiteVolume/lnInclude \
	-I$(WM_PROJECT_DIR)/src/sampling/lnInclude

LDFLAGS := $(LDFLAGS) -L$(LIB_WM_OPTIONS_DIR) $(PROJECT_LIBS) -ldl \
	-L$(WM_PROJECT_DIR)/lib/$(WM_OPTIONS) -lfiniteVolume -lsampling

ifneq "$(parallel_foam_root_dir)" "."
  LDFLAGS := -lparallel_foam $(LDFLAGS)
endif


#--------------------------------------------------------------------------------------
parallel_dev_root_dir := $(parallel_foam_root_dir)/../dev

include $(parallel_dev_root_dir)/include.makefile

CPPFLAGS:=$(shell echo $(CPPFLAGS) | sed -e"s%-Wunused%%g")


#--------------------------------------------------------------------------------------
