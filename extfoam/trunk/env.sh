## VulaSHAKA (Simultaneous Neutronic, Fuel Performance, Heat And Kinetics Analysis)
## Copyright (C) 2009-2010 Pebble Bed Modular Reactor (Pty) Limited (PBMR)
## 
## This program is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with this program.  If not, see <http://www.gnu.org/licenses/>.
## 
## See https://vulashaka.svn.sourceforge.net/svnroot/vulashaka
##
## Author : Alexey PETROV
##


#--------------------------------------------------------------------------------------
# Source the conffoam environment
source /opt/conffoam/r0.2/bashrc


#--------------------------------------------------------------------------------------
# Source the OpenFOAM environment
which_open_foam="1.6"
source_openfoam /opt/OpenFOAM/r${which_open_foam}/debug/bashrc

# Source the SALOME environment
source_salome /opt/SALOME/r5.1.3/r1.0/from_sources/env_products.sh


#--------------------------------------------------------------------------------------
