SOURCES_FENV_C:=$(FENVSRCDIR)/fegetexceptflag.c \
$(FENVSRCDIR)/feholdexcept.c \
$(FENVSRCDIR)/fenv.c \
$(FENVSRCDIR)/fesetexceptflag.c \
$(FENVSRCDIR)/fesetround.c \
$(FENVSRCDIR)/feupdateenv.c \
$(FENVSRCDIR)/__flt_rounds.c

SOURCES_FENV_S:=$(FENVSRCDIR)/fenvasm.s
