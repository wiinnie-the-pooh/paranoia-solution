#!/usr/bash

#--------------------------------------------------------------------------------------
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
## See http://sourceforge.net/projects/extfoam
##
## Author : Alexey PETROV
##


#--------------------------------------------------------------------------------------
which_open_foam="1.4.1-dev"

device="/opt/OpenFOAM/r${which_open_foam}/debug/bashrc" && test -f ${device} && source ${device}

device="/opt/SALOME/r5.1.3/r1.0/from_sources/bashrc" && test -f ${device} && source ${device}

device="/opt/extfoam/r2.3/OpenFOAM-${which_open_foam}/bashrc" && test -f ${device} && source ${device}


#--------------------------------------------------------------------------------------
