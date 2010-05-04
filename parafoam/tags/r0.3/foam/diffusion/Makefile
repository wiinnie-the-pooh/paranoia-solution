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
include $(EXTFOAM_ROOT_DIR)/common/foam.version.makefile


#--------------------------------------------------------------------------------------
ifeq "$(shell if [ ${__FOAM_VERSION__} -eq 010401 ]; then echo 'true'; else echo 'false'; fi )" "true" 
#--------------------------------------------------------------------------------------

sexe = parallel_diffusion

CPPFLAGS := -I$(DIFFUSION_ROOT_DIR)/diffusionFoamLib/lnInclude
LDFLAGS := -L$(DIFFUSION_ROOT_DIR)/lib -lblockMatrix -ldiffusionFoam

parallel_foam_root_dir := ..

include $(parallel_foam_root_dir)/include.makefile

#--------------------------------------------------------------------------------------
else
#--------------------------------------------------------------------------------------

$(warning Compilable only for OpenFOAM 1.4.1-dev)

all:
clean:


#--------------------------------------------------------------------------------------
endif
#--------------------------------------------------------------------------------------
