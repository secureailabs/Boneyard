$(PY_BUILD)/$(COVERSRCDIR)/io.o: io.c $(COVERSRCDIR)/../Include/enclave_wrapper/io.h \
 $(COVERSRCDIR)/../../include/data_management.h $(COVERSRCDIR)/../../include/enclave_def.h \
 $(COVERSRCDIR)/../../include/../gt_enclave_t.h \
 $(COVERSRCDIR)/../../../gt_app/include/msg_def.h \
  $(COVERSRCDIR)/../Include/Python.h \
 $(COVERSRCDIR)/../Include/./enclave_wrapper/io.h \
 $(COVERSRCDIR)/../Include/./enclave_wrapper/string_sgx.h \
 $(COVERSRCDIR)/../Include/./enclave_wrapper/signal.h $(COVERSRCDIR)/../Include/./enclave_wrapper/sys.h \
  $(COVERSRCDIR)/../Include/patchlevel.h \
 $(COVERSRCDIR)/../Include/pyconfig.h $(COVERSRCDIR)/../Include/pymacconfig.h \
  $(COVERSRCDIR)/../Include/pyport.h \
  $(COVERSRCDIR)/../Include/pymath.h \
 $(COVERSRCDIR)/../Include/pymem.h $(COVERSRCDIR)/../Include/object.h $(COVERSRCDIR)/../Include/objimpl.h \
 $(COVERSRCDIR)/../Include/pydebug.h $(COVERSRCDIR)/../Include/unicodeobject.h \
  $(COVERSRCDIR)/../Include/intobject.h \
 $(COVERSRCDIR)/../Include/boolobject.h $(COVERSRCDIR)/../Include/longobject.h $(COVERSRCDIR)/../Include/floatobject.h \
 $(COVERSRCDIR)/../Include/complexobject.h $(COVERSRCDIR)/../Include/rangeobject.h \
 $(COVERSRCDIR)/../Include/stringobject.h $(COVERSRCDIR)/../Include/memoryobject.h \
 $(COVERSRCDIR)/../Include/bufferobject.h $(COVERSRCDIR)/../Include/bytesobject.h \
 $(COVERSRCDIR)/../Include/bytearrayobject.h $(COVERSRCDIR)/../Include/tupleobject.h \
 $(COVERSRCDIR)/../Include/listobject.h $(COVERSRCDIR)/../Include/dictobject.h $(COVERSRCDIR)/../Include/enumobject.h \
 $(COVERSRCDIR)/../Include/setobject.h $(COVERSRCDIR)/../Include/methodobject.h \
 $(COVERSRCDIR)/../Include/moduleobject.h $(COVERSRCDIR)/../Include/funcobject.h \
 $(COVERSRCDIR)/../Include/classobject.h $(COVERSRCDIR)/../Include/fileobject.h $(COVERSRCDIR)/../Include/cobject.h \
 $(COVERSRCDIR)/../Include/pycapsule.h $(COVERSRCDIR)/../Include/traceback.h $(COVERSRCDIR)/../Include/sliceobject.h \
 $(COVERSRCDIR)/../Include/cellobject.h $(COVERSRCDIR)/../Include/iterobject.h $(COVERSRCDIR)/../Include/genobject.h \
 $(COVERSRCDIR)/../Include/descrobject.h $(COVERSRCDIR)/../Include/warnings.h \
 $(COVERSRCDIR)/../Include/weakrefobject.h $(COVERSRCDIR)/../Include/codecs.h $(COVERSRCDIR)/../Include/pyerrors.h \
 $(COVERSRCDIR)/../Include/pystate.h $(COVERSRCDIR)/../Include/pyarena.h $(COVERSRCDIR)/../Include/modsupport.h \
 $(COVERSRCDIR)/../Include/pythonrun.h $(COVERSRCDIR)/../Include/ceval.h $(COVERSRCDIR)/../Include/sysmodule.h \
 $(COVERSRCDIR)/../Include/intrcheck.h $(COVERSRCDIR)/../Include/import.h $(COVERSRCDIR)/../Include/abstract.h \
 $(COVERSRCDIR)/../Include/compile.h $(COVERSRCDIR)/../Include/code.h $(COVERSRCDIR)/../Include/eval.h \
 $(COVERSRCDIR)/../Include/pyctype.h $(COVERSRCDIR)/../Include/pystrtod.h $(COVERSRCDIR)/../Include/pystrcmp.h \
 $(COVERSRCDIR)/../Include/dtoa.h $(COVERSRCDIR)/../Include/pyfpe.h
$(PY_BUILD)/$(COVERSRCDIR)/locale.o: locale.c $(COVERSRCDIR)/../Include/enclave_wrapper/locale.h
$(PY_BUILD)/$(COVERSRCDIR)/random.o: random.c $(COVERSRCDIR)/../Include/enclave_wrapper/random.h 
$(PY_BUILD)/$(COVERSRCDIR)/string_sgx.o: string_sgx.c $(COVERSRCDIR)/../Include/enclave_wrapper/string_sgx.h 
$(PY_BUILD)/$(COVERSRCDIR)/sys.o: sys.c $(COVERSRCDIR)/../Include/enclave_wrapper/sys.h \
 $(COVERSRCDIR)/../Include/enclave_wrapper/signal.h 
$(PY_BUILD)/$(COVERSRCDIR)/datetime.o: datetime.c $(COVERSRCDIR)/../Include/enclave_wrapper/time.h \
