
SUBDIRS = appl main nmath randomForest stats 

.PHONY: all $(SUBDIRS)

all: $(SUBDIRS)

$(SUBDIRS):
	(@echo make $@; cd $@ && $(MAKE) all) || exit 1; \


