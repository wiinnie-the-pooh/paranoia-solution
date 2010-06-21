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
## See https://vulashaka.svn.sourceforge.net/svnroot/vulashaka/parafoam_testing
##
## Author : Alexey PETROV


#------------------------------------------------------------------------------
initilize ()
{
    dir=`dirname ${0}`
    dir=`(cd ${dir} && pwd)`
    export thisScript=${dir}/`basename ${0}`
}


#------------------------------------------------------------------------------
runRecursive ()
{
    engine=$1
    # If an argument is supplied do not execute ./Allrun to avoid recursion
    if [ -f "./${engine}" ] ; then
        # Specialised script.
	echo ------------------------------------------------------------------------------
	echo
	./${engine}
	echo
	echo ------------------------------------------------------------------------------
    else
        # Recurse to subdirectories
	for case in * ; do
	    if [ -d $case ] ; then
		(cd $case && echo Visiting : `pwd` && $thisScript ${engine})
	    fi
	done
    fi
}


#------------------------------------------------------------------------------
get_source_case ()
{
    dir=`pwd` 
    if [ $# -gt 0 ] ; then
       dir=${dir}/${1}
    fi
    echo ${dir}
}


#------------------------------------------------------------------------------
get_reference_case ()
{
   source_case=`get_source_case $*`
   echo ${source_case}_reference
}


#------------------------------------------------------------------------------
diff_two_cases ()
{
   source_case=${1}
   reference_case=${2}
   echo ------------- differencing with the referenced C++ solver -------------
   diff -r -q -I "case" -x ".svn" -x "All*" -x "log.*" ${reference_case} ${source_case} 
   echo -----------------------------------------------------------------------
}


#------------------------------------------------------------------------------
