$(FT2SRCDIR)/gzip/adler32.o:$(FT2SRCDIR)/gzip/adler32.c zlib.h ftzconf.h
$(FT2SRCDIR)/gzip/ftgzip.o:$(FT2SRCDIR)/gzip/ftgzip.c $(FT2SRCDIR)/gzip/../include/ft2build.h \
 $(FT2SRCDIR)/gzip/../include/freetype/internal/ftmemory.h \
 $(FT2SRCDIR)/gzip/../include/freetype/config/ftconfig.h \
 $(FT2SRCDIR)/gzip/../include/freetype/config/$(FT2SRCDIR)/gzip/../$(FT2SRCDIR)/gzip/../ft2build.h \
 $(FT2SRCDIR)/gzip/../include/freetype/config/ftoption.h \
 $(FT2SRCDIR)/gzip/../include/freetype/config/ftstdlib.h \
 $(FT2SRCDIR)/gzip/../include/freetype/config/$(FT2SRCDIR)/gzip/../$(FT2SRCDIR)/gzip/../$(FT2SRCDIR)/gzip/../$(FT2SRCDIR)/gzip/../pyvm/Include/enclave_wrapper/io.h \
 $(FT2SRCDIR)/gzip/../include/freetype/fttypes.h $(FT2SRCDIR)/gzip/../include/freetype/$(FT2SRCDIR)/gzip/../ft2build.h \
 $(FT2SRCDIR)/gzip/../include/freetype/./config/ftconfig.h $(FT2SRCDIR)/gzip/../include/freetype/ftsystem.h \
 $(FT2SRCDIR)/gzip/../include/freetype/ftimage.h $(FT2SRCDIR)/gzip/../include/freetype/internal/ftstream.h \
 $(FT2SRCDIR)/gzip/../include/freetype/ftsystem.h $(FT2SRCDIR)/gzip/../include/freetype/internal/ftobjs.h \
 $(FT2SRCDIR)/gzip/../include/freetype/ftrender.h $(FT2SRCDIR)/gzip/../include/freetype/ftmodapi.h \
 $(FT2SRCDIR)/gzip/../include/freetype/freetype.h $(FT2SRCDIR)/gzip/../include/freetype/fttypes.h \
 $(FT2SRCDIR)/gzip/../include/freetype/fterrors.h $(FT2SRCDIR)/gzip/../include/freetype/ftmoderr.h \
 $(FT2SRCDIR)/gzip/../include/freetype/fterrdef.h $(FT2SRCDIR)/gzip/../include/freetype/ftglyph.h \
 $(FT2SRCDIR)/gzip/../include/freetype/freetype.h $(FT2SRCDIR)/gzip/../include/freetype/ftsizes.h \
 $(FT2SRCDIR)/gzip/../include/freetype/ftlcdfil.h $(FT2SRCDIR)/gzip/../include/freetype/ftparams.h \
 $(FT2SRCDIR)/gzip/../include/freetype/internal/ftgloadr.h \
 $(FT2SRCDIR)/gzip/../include/freetype/internal/ftdrv.h \
 $(FT2SRCDIR)/gzip/../include/freetype/internal/autohint.h \
 $(FT2SRCDIR)/gzip/../include/freetype/internal/ftserv.h \
 $(FT2SRCDIR)/gzip/../include/freetype/internal/ftcalc.h $(FT2SRCDIR)/gzip/../include/freetype/ftincrem.h \
 $(FT2SRCDIR)/gzip/../include/freetype/internal/ftdebug.h $(FT2SRCDIR)/gzip/../include/freetype/ftgzip.h \
 $(FT2SRCDIR)/gzip/../include/freetype/config/ftstdlib.h $(FT2SRCDIR)/gzip/../include/freetype/ftmoderr.h \
 $(FT2SRCDIR)/gzip/../include/freetype/fterrors.h zlib.h ftzconf.h zutil.h \
$(FT2SRCDIR)/gzip/inftrees.h infblock.h infcodes.h infutil.h zutil.c inftrees.c inffixed.h \
$(FT2SRCDIR)/gzip/infutil.c infcodes.c infblock.c inflate.c adler32.c
$(FT2SRCDIR)/gzip/infblock.o:$(FT2SRCDIR)/gzip/infblock.c zutil.h zlib.h \
 ftzconf.h 
 infblock.h inftrees.h infcodes.h infutil.h
$(FT2SRCDIR)/gzip/infcodes.o:$(FT2SRCDIR)/gzip/infcodes.c zutil.h zlib.h \
 ftzconf.h 
 inftrees.h infblock.h infcodes.h infutil.h
$(FT2SRCDIR)/gzip/inflate.o:$(FT2SRCDIR)/gzip/inflate.c zutil.h zlib.h ftzconf.h \
 infblock.h
$(FT2SRCDIR)/gzip/inftrees.o:$(FT2SRCDIR)/gzip/inftrees.c zutil.h zlib.h \
 ftzconf.h 
 inftrees.h inffixed.h
$(FT2SRCDIR)/gzip/infutil.o:$(FT2SRCDIR)/gzip/infutil.c zutil.h zlib.h ftzconf.h \
 infblock.h inftrees.h infcodes.h infutil.h
$(FT2SRCDIR)/gzip/zutil.o:$(FT2SRCDIR)/gzip/zutil.c zutil.h zlib.h ftzconf.h 
