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


AC_DEFUN([EXTFOAM_CHECK_LOKI],dnl
[
AC_CHECKING(for Loki library)

AC_LANG_SAVE
AC_LANG_CPLUSPLUS

LOKI_CPPFLAGS=""
AC_SUBST(LOKI_CPPFLAGS)

LOKI_CXXFLAGS=""
AC_SUBST(LOKI_CXXFLAGS)

LOKI_LDFLAGS=""
AC_SUBST(LOKI_LDFLAGS)

LOKI_LIBS=""
AC_SUBST(LOKI_LIBS)

AC_ARG_WITH([loki],
            AC_HELP_STRING([--with-loki=<path>],
	                   [use <path> to look for Loki installation]),
	    [],
	    [withval=yes])

loki_ok=no

if test ! "x${withval}" = "xno" ; then
   AC_MSG_CHECKING( ${withval} )
   if test "x${withval}" = "xyes" ; then
      if test ! "x${LOKI_ROOT_DIR}" = "x" && test -d ${LOKI_ROOT_DIR} ; then
      	 loki_root_dir=${LOKI_ROOT_DIR}
      else
	 loki_root_dir="/usr"
      fi
   else
      loki_root_dir=${withval}
   fi

   AC_CHECK_FILE( [${loki_root_dir}], [ loki_ok=yes ], [ loki_ok=no ] )

   if test "x${loki_ok}" = "xyes" ; then
      test ! "x${loki_root_dir}" = "x/usr" && LOKI_CPPFLAGS="-I${loki_root_dir}/include"
      CPPFLAGS="${LOKI_CPPFLAGS}"

      LOKI_CXXFLAGS="-DLOKI_OBJECT_LEVEL_THREADING"
      CXXFLAGS="${LOKI_CXXFLAGS}"

      AC_CHECK_HEADERS( [loki/SmartPtr.h], [ loki_ok=yes ], [ loki_ok=no ] )
   fi

   if test "x${loki_ok}" = "xyes" ; then
      AC_MSG_CHECKING( linking to the Loki library )

      test ! "x${loki_root_dir}" = "x/usr" && LOKI_LDFLAGS="-L${loki_root_dir}/lib"
      LDFLAGS="${LOKI_LDFLAGS}"

      LOKI_LIBS="-lloki -lstdc++ -lpthread"
      LIBS="${LOKI_LIBS}"

      AC_LINK_IFELSE( [ AC_LANG_PROGRAM( [ [ #include <loki/SmartPtr.h> ] ],
      		                         [ [ Loki::SmartPtrDef< int >::type ptr ] ] ) ],
					 [ loki_ok=yes ],
					 [ loki_ok=no ] )
      AC_MSG_RESULT( ${loki_ok} )
   fi
fi

if test "x${loki_ok}" = "xno" ; then
   AC_MSG_ERROR([use either LOKI_ROOT_DIR environement varaible or --with-loki=<path>])
fi

AC_LANG_RESTORE
])

