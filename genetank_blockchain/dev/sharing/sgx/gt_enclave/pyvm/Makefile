SUBDIRS = Python Parser Objects cover Modules
.PHONY: all $(SUBDIRS)

all: $(SUBDIRS)

$(SUBDIRS):
	(@echo make $@; cd $@ && $(MAKE) all) || exit 1; \

