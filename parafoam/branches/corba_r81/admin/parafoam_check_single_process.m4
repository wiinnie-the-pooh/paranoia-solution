dnl VulaSHAKA (Simultaneous Neutronic, Fuel Performance, Heat And Kinetics Analysis)
dnl Copyright (C) 2009-2010 Pebble Bed Modular Reactor (Pty) Limited (PBMR)
dnl 
dnl This program is free software: you can redistribute it and/or modify
dnl it under the terms of the GNU General Public License as published by
dnl the Free Software Foundation, either version 3 of the License, or
dnl (at your option) any later version.
dnl
dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl GNU General Public License for more details.
dnl 
dnl You should have received a copy of the GNU General Public License
dnl along with this program.  If not, see <http://www.gnu.org/licenses/>.
dnl 
dnl See https://vulashaka.svn.sourceforge.net/svnroot/vulashaka
dnl
dnl Author : Alexey PETROV
dnl


dnl --------------------------------------------------------------------------------
AC_DEFUN([PARAFOAM_CHECK_CORBA_SINGLE_PROCESS],
[
AC_MSG_CHECKING(whether to run all the CORBA servers & client within one single process)

AC_SUBST(USE_CORBA_SINGLE_PROCESS_CPPFLAGS)

AC_ARG_ENABLE( [corba_single_process],
               AC_HELP_STRING( [--enable-corba-single-process ],
                               [ enable "corba-single-process" to provide all CORBA advantages ( disabled by default ) ]),
               [ corba_single_process=${enableval} ],
               [ corba_single_process="no" ] )

if test ! "${corba_single_process}xx" != "noxx" ;  then
   USE_CORBA_SINGLE_PROCESS_CPPFLAGS="-D __USE_CORBA_SINGLE_PROCESS__"
fi

AC_MSG_RESULT(${corba_single_process})
])


dnl --------------------------------------------------------------------------------
