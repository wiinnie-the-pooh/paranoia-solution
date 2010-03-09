dnl Copyright (C) 2009 Pebble Bed Modular Reactor (Pty) Limited (PBMR)
dnl 
dnl This library is free software; you can redistribute it and/or 
dnl modify it under the terms of the GNU Lesser General Public 
dnl License as published by the Free Software Foundation; either 
dnl version 2.1 of the License. 
dnl 
dnl This library is distributed in the hope that it will be useful, 
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of 
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
dnl Lesser General Public License for more details. 
dnl 
dnl You should have received a copy of the GNU Lesser General Public 
dnl License along with this library; if not, write to the Free Software 
dnl Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
dnl 
dnl See http://sourceforge.net/projects/pyfoam
dnl
dnl Author : Alexey PETROV
dnl


AC_DEFUN([EXTFOAM_CHECK_OPENFOAM],dnl
[
AC_CHECKING(for OpenFOAM library)

openfoam_ok=no

if test ! "x${withval}" = "xno" ; then
   AC_MSG_CHECKING(location of the OpenFOAM installation)
   if test -d "${WM_PROJECT_DIR}" ; then
      openfoam_ok=yes
   fi
   AC_MSG_RESULT($openfoam_ok)
fi

if test "x${openfoam_ok}" = "xno" ; then
   AC_MSG_ERROR([it is neceesary to source OpenFOAM environment first])
fi

])

