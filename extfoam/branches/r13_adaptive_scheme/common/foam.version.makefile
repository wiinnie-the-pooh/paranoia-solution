#--------------------------------------------------------------------------------------
__FOAM_VERSION__ := $(subst -dev,,$(WM_PROJECT_VERSION))
__FOAM_VERSION__ := $(subst 1.4.1,010401,$(__FOAM_VERSION__))
__FOAM_VERSION__ := $(subst 1.5,010500,$(__FOAM_VERSION__))
__FOAM_VERSION__ := $(subst 1.6,010600,$(__FOAM_VERSION__))


#--------------------------------------------------------------------------------------
