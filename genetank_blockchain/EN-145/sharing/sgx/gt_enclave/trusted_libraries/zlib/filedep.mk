$(ZLIBSRCDIR)/adler32.o:$(ZLIBSRCDIR)/adler32.c $(ZLIBSRCDIR)/zutil.h $(ZLIBSRCDIR)/zlib.h $(ZLIBSRCDIR)/zconf.h 
$(ZLIBSRCDIR)/compress.o:$(ZLIBSRCDIR)/compress.c $(ZLIBSRCDIR)/zlib.h $(ZLIBSRCDIR)/zconf.h
$(ZLIBSRCDIR)/crc32.o:$(ZLIBSRCDIR)/crc32.c $(ZLIBSRCDIR)/zutil.h $(ZLIBSRCDIR)/zlib.h $(ZLIBSRCDIR)/zconf.h \
 $(ZLIBSRCDIR)/crc32.h
$(ZLIBSRCDIR)/deflate.o:$(ZLIBSRCDIR)/deflate.c $(ZLIBSRCDIR)/deflate.h $(ZLIBSRCDIR)/zutil.h $(ZLIBSRCDIR)/zlib.h \
 $(ZLIBSRCDIR)/zconf.h 
$(ZLIBSRCDIR)/gzclose.o:$(ZLIBSRCDIR)/gzclose.c $(ZLIBSRCDIR)/gzguts.h \
 $(ZLIBSRCDIR)/zlib.h $(ZLIBSRCDIR)/zconf.h 
$(ZLIBSRCDIR)/gzlib.o:$(ZLIBSRCDIR)/gzlib.c $(ZLIBSRCDIR)/gzguts.h \
 $(ZLIBSRCDIR)/zlib.h $(ZLIBSRCDIR)/zconf.h 
$(ZLIBSRCDIR)/gzread.o:$(ZLIBSRCDIR)/gzread.c $(ZLIBSRCDIR)/gzguts.h \
 $(ZLIBSRCDIR)/zlib.h $(ZLIBSRCDIR)/zconf.h
$(ZLIBSRCDIR)/gzwrite.o:$(ZLIBSRCDIR)/gzwrite.c $(ZLIBSRCDIR)/gzguts.h \
 $(ZLIBSRCDIR)/zlib.h $(ZLIBSRCDIR)/zconf.h 
$(ZLIBSRCDIR)/infback.o:$(ZLIBSRCDIR)/infback.c $(ZLIBSRCDIR)/zutil.h $(ZLIBSRCDIR)/zlib.h $(ZLIBSRCDIR)/zconf.h \
 $(ZLIBSRCDIR)/inftrees.h $(ZLIBSRCDIR)/inflate.h \
 $(ZLIBSRCDIR)/inffast.h $(ZLIBSRCDIR)/inffixed.h
$(ZLIBSRCDIR)/inffast.o:$(ZLIBSRCDIR)/inffast.c $(ZLIBSRCDIR)/zutil.h $(ZLIBSRCDIR)/zlib.h $(ZLIBSRCDIR)/zconf.h \
 $(ZLIBSRCDIR)/inftrees.h $(ZLIBSRCDIR)/inflate.h \
 $(ZLIBSRCDIR)/inffast.h
$(ZLIBSRCDIR)/inflate.o:$(ZLIBSRCDIR)/inflate.c $(ZLIBSRCDIR)/zutil.h $(ZLIBSRCDIR)/zlib.h $(ZLIBSRCDIR)/zconf.h \
 $(ZLIBSRCDIR)/inftrees.h $(ZLIBSRCDIR)/inflate.h \
 $(ZLIBSRCDIR)/inffast.h $(ZLIBSRCDIR)/inffixed.h
$(ZLIBSRCDIR)/inftrees.o:$(ZLIBSRCDIR)/inftrees.c $(ZLIBSRCDIR)/zutil.h $(ZLIBSRCDIR)/zlib.h $(ZLIBSRCDIR)/zconf.h \
 $(ZLIBSRCDIR)/inftrees.h
$(ZLIBSRCDIR)/trees.o:$(ZLIBSRCDIR)/trees.c $(ZLIBSRCDIR)/deflate.h $(ZLIBSRCDIR)/zutil.h $(ZLIBSRCDIR)/zlib.h \
 $(ZLIBSRCDIR)/zconf.h \
 $(ZLIBSRCDIR)/trees.h
$(ZLIBSRCDIR)/uncompr.o:$(ZLIBSRCDIR)/uncompr.c $(ZLIBSRCDIR)/zlib.h $(ZLIBSRCDIR)/zconf.h 
$(ZLIBSRCDIR)/zutil.o:$(ZLIBSRCDIR)/zutil.c $(ZLIBSRCDIR)/zutil.h $(ZLIBSRCDIR)/zlib.h $(ZLIBSRCDIR)/zconf.h \
 $(ZLIBSRCDIR)/gzguts.h 
