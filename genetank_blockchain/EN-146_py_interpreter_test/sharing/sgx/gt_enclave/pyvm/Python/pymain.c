/*
 * pymain.c
 *
 *  Created on: Jan 10, 2019
 *      Author: jingwei
 */

#include "../Include/Python.h"
#include "../Include/pyvm.h"
#include "../../include/vmoutput.h"
#include "../../include/util.h"
//#include "../../include/task_manager_enclave.h"
#include <stdlib.h>
#include <sgx_thread.h>

//TaskTable taskTable;

struct msgbuf inbuf;
struct msgbuf outbuf;
struct msgbuf errsbuf;
//sgx_thread_mutex_t pyVMLock = SGX_THREAD_MUTEX_INITIALIZER;

FILE* stdin;
FILE* stdout;
FILE* stderr;

char** envlist;

jmp_buf top_env;

void add_native(){
	//PyImport_AppendInittab("errno", initerrno);
	PyImport_AppendInittab("signal", initsignal);
	//PyImport_AppendInittab("posix", initposix);
	//PyImport_AppendInittab("imp", initimp);
	PyImport_AppendInittab("_functools", init_functools);
	PyImport_AppendInittab("operator", initoperator);
	PyImport_AppendInittab("_weakref", init_weakref_);
	PyImport_AppendInittab("_collections", init_collections);
	PyImport_AppendInittab("itertools", inititertools);
	PyImport_AppendInittab("_heapq", init_heapq);
	PyImport_AppendInittab("_sre", init_sre);
	PyImport_AppendInittab("cStringIO", initcStringIO);
	PyImport_AppendInittab("math", initmath);
	PyImport_AppendInittab("future_builtins", initfuture_builtins);
	PyImport_AppendInittab("_codecs", init_codecs);
	PyImport_AppendInittab("time", inittime);
	PyImport_AppendInittab("_struct", init_struct);
	PyImport_AppendInittab("_ctypes", init_ctypes);
	PyImport_AppendInittab("_io", init_io);
	PyImport_AppendInittab("cPickle", initcPickle);
	PyImport_AppendInittab("binascii",initbinascii);
	PyImport_AppendInittab("_random",init_random);
	PyImport_AppendInittab("_md5",init_md5);
	PyImport_AppendInittab("_sha",init_sha);
	PyImport_AppendInittab("_sha256",init_sha256);
	PyImport_AppendInittab("_sha512",init_sha512);
	PyImport_AppendInittab("_csv",init_csv);
	PyImport_AppendInittab("datetime", initdatetime);
	PyImport_AppendInittab("unicodedata", initunicodedata);
	PyImport_AppendInittab("array", initarray);
	PyImport_AppendInittab("_federated", init_federated);
	PyImport_AppendInittab("_fileaccess", init_fileaccess);
	PyImport_AppendInittab("_httpsend", init_httpsend);
	PyImport_AppendInittab("zlib", init_zlib);
	PyImport_AppendInittab("_testcapi", init_testcapi);
	PyImport_AppendInittab("cmath", initcmath);
	//PyImport_AppendInittab("select", initselect);
}

void add_numpy(){
	PyImport_AppendInittab("_multiarray_umath", init_multiarray_umath);
	PyImport_AppendInittab("lapack_lite", initlapack_lite);
	PyImport_AppendInittab("_umath_linalg", init_umath_linalg);
	PyImport_AppendInittab("fftpack_lite", initfftpack_lite);
	PyImport_AppendInittab("mtrand", initmtrand);
	PyImport_AppendInittab("_umath_linalg", init_umath_linalg);
}

void add_numpy_tests(){
	PyImport_AppendInittab("_multiarray_tests", init_multiarray_tests);
	PyImport_AppendInittab("_rational_tests", init_rational_tests);
	PyImport_AppendInittab("_umath_tests", init_umath_tests);
	PyImport_AppendInittab("_operand_flag_tests", init_operand_flag_tests);
	PyImport_AppendInittab("_struct_ufunc_tests", init_struct_ufunc_tests);
}

void add_scipy(){
	PyImport_AppendInittab("convolve", initconvolve);
	PyImport_AppendInittab("_fftpack", init_fftpack);

	PyImport_AppendInittab("_ccallback_c", init_ccallback_c);
	PyImport_AppendInittab("_fpumode", init_fpumode);
	PyImport_AppendInittab("messagestream", initmessagestream);

	PyImport_AppendInittab("_fblas", init_fblas);
	PyImport_AppendInittab("_flapack", init_flapack);
	PyImport_AppendInittab("_flinalg", init_flinalg);
	PyImport_AppendInittab("_interpolative", init_interpolative);
	PyImport_AppendInittab("_solve_toeplitz", init_solve_toeplitz);
	PyImport_AppendInittab("cython_blas", initcython_blas);
	PyImport_AppendInittab("cython_lapack", initcython_lapack);
	PyImport_AppendInittab("_decomp_update", init_decomp_update);

	PyImport_AppendInittab("_csparsetools", init_csparsetools);
	PyImport_AppendInittab("_sparsetools", init_sparsetools);
	PyImport_AppendInittab("_arpack", init_arpack);
	PyImport_AppendInittab("_iterative", init_iterative);
	PyImport_AppendInittab("_superlu", init_superlu);
	PyImport_AppendInittab("_traversal", init_traversal);
	PyImport_AppendInittab("_reordering", init_reordering);
	PyImport_AppendInittab("_min_spanning_tree", init_min_spanning_tree);
	PyImport_AppendInittab("_shortest_path", init_shortest_path);
	PyImport_AppendInittab("_tools", init_tools);

	PyImport_AppendInittab("_comb", init_comb);
	PyImport_AppendInittab("cython_special", initcython_special);
	PyImport_AppendInittab("_ellip_harm_2", init_ellip_harm_2);
	PyImport_AppendInittab("specfun", initspecfun);
	PyImport_AppendInittab("_ufuncs", init_ufuncs);
	PyImport_AppendInittab("_ufuncs_cxx", init_ufuncs_cxx);

	PyImport_AppendInittab("_minpack", init_minpack);
	PyImport_AppendInittab("moduleTNC", initmoduleTNC);
	PyImport_AppendInittab("_lbfgsb", init_lbfgsb);
	PyImport_AppendInittab("_zeros", init_zeros);
	PyImport_AppendInittab("_cobyla", init_cobyla);
	PyImport_AppendInittab("minpack2", initminpack2);
	PyImport_AppendInittab("_nnls", init_nnls);
	PyImport_AppendInittab("_slsqp", init_slsqp);
	PyImport_AppendInittab("_group_columns", init_group_columns);
	PyImport_AppendInittab("givens_elimination", initgivens_elimination);
	PyImport_AppendInittab("_trlib", init_trlib);

	PyImport_AppendInittab("qhull", initqhull);
	PyImport_AppendInittab("ckdtree", initckdtree);
	PyImport_AppendInittab("_voronoi", init_voronoi);
	PyImport_AppendInittab("_hausdorff", init_hausdorff);
	PyImport_AppendInittab("_distance_wrap", init_distance_wrap);

//	PyImport_AppendInittab("sigtools", initsigtools);
	PyImport_AppendInittab("_spectral", init_spectral);
	PyImport_AppendInittab("_max_len_seq_inner", init_max_len_seq_inner);
	PyImport_AppendInittab("_peak_finding_utils", init_peak_finding_utils);
	PyImport_AppendInittab("_upfirdn_apply", init_upfirdn_apply);
//	PyImport_AppendInittab("spline", initspline);

	PyImport_AppendInittab("_nd_image", init_nd_image);
	PyImport_AppendInittab("_ni_label", init_ni_label);

	PyImport_AppendInittab("interpnd", initinterpnd);
	PyImport_AppendInittab("_ppoly", init_ppoly);
	PyImport_AppendInittab("_bspl", init_bspl);
	PyImport_AppendInittab("_fitpack", init_fitpack);
	PyImport_AppendInittab("dfitpack", initdfitpack);
	PyImport_AppendInittab("_interpolate", init_interpolate);

//	PyImport_AppendInittab("__odrpack", init__odrpack);

	PyImport_AppendInittab("vode", initvode);
	PyImport_AppendInittab("lsoda", initlsoda);
	PyImport_AppendInittab("_dop", init_dop);
	PyImport_AppendInittab("_odepack", init_odepack);
	PyImport_AppendInittab("_quadpack", init_quadpack);

	PyImport_AppendInittab("_vq", init_vq);
	PyImport_AppendInittab("_hierarchy", init_hierarchy);
	PyImport_AppendInittab("_optimal_leaf_ordering", init_optimal_leaf_ordering);

	PyImport_AppendInittab("statlib", initstatlib);
	PyImport_AppendInittab("mvn", initmvn);
	PyImport_AppendInittab("_stats", init_stats);
}

void add_sklearn(){
	PyImport_AppendInittab("_isotonic", init_isotonic);

	PyImport_AppendInittab("_logistic_sigmoid", init_logistic_sigmoid);
	PyImport_AppendInittab("arrayfuncs", initarrayfuncs);
	PyImport_AppendInittab("graph_shortest_path", initgraph_shortest_path);
	PyImport_AppendInittab("lgamma", initlgamma);
	PyImport_AppendInittab("murmurhash", initmurmurhash);
	PyImport_AppendInittab("seq_dataset", initseq_dataset);
	PyImport_AppendInittab("sparsefuncs_fast", initsparsefuncs_fast);
	PyImport_AppendInittab("weight_vector", initweight_vector);
	PyImport_AppendInittab("_sk_random", init_sk_random);
	PyImport_AppendInittab("fast_dict", initfast_dict);

	PyImport_AppendInittab("libsvm", initlibsvm);
	PyImport_AppendInittab("libsvm_sparse", initlibsvm_sparse);
	PyImport_AppendInittab("liblinear", initliblinear);

	PyImport_AppendInittab("_criterion", init_criterion);
	PyImport_AppendInittab("_splitter", init_splitter);
	PyImport_AppendInittab("_tree", init_tree);
	PyImport_AppendInittab("_tree_utils", inittree_utils);

	PyImport_AppendInittab("_hierarchical", init_hierarchical);
	PyImport_AppendInittab("_dbscan_inner", init_dbscan_inner);
	PyImport_AppendInittab("_k_means", init_k_means);
	PyImport_AppendInittab("_k_means_elkan", init_k_means_elkan);

	PyImport_AppendInittab("sag_fast", initsag_fast);
	PyImport_AppendInittab("sgd_fast", initsgd_fast);
	PyImport_AppendInittab("cd_fast", initcd_fast);

	PyImport_AppendInittab("_check_build", init_check_build);

	PyImport_AppendInittab("pairwise_fast", initpairwise_fast);
	PyImport_AppendInittab("expected_mutual_info_fast", initexpected_mutual_info_fast);

	PyImport_AppendInittab("_gradient_boosting", init_gradient_boosting);

	PyImport_AppendInittab("_barnes_hut_tsne", init_barnes_hut_tsne);
	PyImport_AppendInittab("_mani_utils", init_mani_utils);

	PyImport_AppendInittab("_svmlight_format", init_svmlight_format);

	PyImport_AppendInittab("_hashing", init_hashing);

	PyImport_AppendInittab("ball_tree", initball_tree);
	PyImport_AppendInittab("kd_tree", initkd_tree);
	PyImport_AppendInittab("quad_tree", initquad_tree);
	PyImport_AppendInittab("typedefs", inittypedefs);
	PyImport_AppendInittab("dist_metrics", initdist_metrics);

	PyImport_AppendInittab("_online_lda", init_online_lda);
	PyImport_AppendInittab("cdnmf_fast", initcdnmf_fast);
}

void add_pandas(){
	PyImport_AppendInittab("hashtable", inithashtable);
	PyImport_AppendInittab("indexing", initindexing);
	PyImport_AppendInittab("internals", initinternals);
	PyImport_AppendInittab("sparse", initsparse);
	PyImport_AppendInittab("algos", initalgos);
	PyImport_AppendInittab("reshape", initreshape);
	PyImport_AppendInittab("tslib", inittslib);
	PyImport_AppendInittab("_index", initindex);
	PyImport_AppendInittab("join", initjoin);
	PyImport_AppendInittab("skiplist", initskiplist);
	PyImport_AppendInittab("interval", initinterval);
	PyImport_AppendInittab("window", initwindow);
	PyImport_AppendInittab("properties", initproperties);
	PyImport_AppendInittab("lib", initlib);
	PyImport_AppendInittab("dparsers", initparsers);
	PyImport_AppendInittab("writers", initwriters);
	PyImport_AppendInittab("hashing", inithashing);
	PyImport_AppendInittab("djson", initjson);
	PyImport_AppendInittab("ops", initops);
	PyImport_AppendInittab("missing", initmissing);
	PyImport_AppendInittab("libgroupby", initgroupby);
	PyImport_AppendInittab("reduction", initreduction);

	PyImport_AppendInittab("strptime", initstrptime);
	PyImport_AppendInittab("timedeltas", inittimedeltas);
	PyImport_AppendInittab("parsing", initparsing);
	PyImport_AppendInittab("resolution", initresolution);
	PyImport_AppendInittab("timezones", inittimezones);
	PyImport_AppendInittab("ccalendar", initccalendar);
	PyImport_AppendInittab("period", initperiod);
	PyImport_AppendInittab("offsets", initoffsets);
	PyImport_AppendInittab("timestamps", inittimestamps);
	PyImport_AppendInittab("np_datetime", initnp_datetime);
	PyImport_AppendInittab("fields", initfields);
	PyImport_AppendInittab("frequencies", initfrequencies);
	PyImport_AppendInittab("conversion", initconversion);
	PyImport_AppendInittab("nattype", initnattype);

	PyImport_AppendInittab("_sas", init_sas);

	PyImport_AppendInittab("_move", init_move);
}

void add_matplotlib(){
	PyImport_AppendInittab("_backend_agg", init_backend_agg);
	PyImport_AppendInittab("_contour", init_contour);
	PyImport_AppendInittab("_image", init_image);
	PyImport_AppendInittab("_path", init_path);
	PyImport_AppendInittab("_png", init_png);
	PyImport_AppendInittab("ft2font", initft2font);
	PyImport_AppendInittab("ttconv", initttconv);
	PyImport_AppendInittab("kiwisolver", initkiwisolver);
}

VM_Result* run_py(const char *py_code, char *result, size_t result_size){
	//sgx_thread_mutex_lock(&pyVMLock);

	if(errsbuf.buf==NULL){
	    errsbuf.buf=(char*)calloc(MSG_BUF_LEN, sizeof(char));
	}
	errsbuf.buf[MSG_BUF_LEN-1] = '\0';
	Result.cx=0;
	if (NULL == Result.result_type)
		Result.result_type=(char*)calloc(RESULT_TYPE_SIZE, sizeof(char));
	if (NULL == Result.result_buf)
		Result.result_buf=(char*)calloc(RESULT_BUF_SIZE, sizeof(char));

	inbuf.cx=0;
	outbuf.cx=0;
	errsbuf.cx=0;

	if(inbuf.buf==NULL){
	    inbuf.buf=(char*)calloc(MSG_BUF_LEN, sizeof(char));
	}
	if(outbuf.buf==NULL){
	    outbuf.buf=(char*)calloc(MSG_BUF_LEN, sizeof(char));
	}

	int val=setjmp(top_env);
//	if(!val){

//        add_native();
//        add_numpy();
//        add_scipy();
//        add_sklearn();
//        add_pandas();
//
//	    Py_Initialize();

	if(!val){
	    PyRun_SimpleString(py_code);
	    	//Py_Finalize();
	}
//	}
	outbuf.buf[MSG_BUF_LEN-1]='\0';
	errsbuf.buf[MSG_BUF_LEN-1]='\0';
	oc_print("1:",outbuf.buf);
	oc_print("2:",errsbuf.buf);

	do{
	    strcpy(Result.result_buf, "\nOUTPUT:\n");
	    Result.cx = 9;
	    size_t len1 = outbuf.cx;
	    size_t len2 = RESULT_BUF_SIZE-Result.cx-1;
	    size_t len = len1>len2?len2:len1;
	    strncpy(Result.result_buf+Result.cx,outbuf.buf,len);
	    Result.cx+=len;
	    if(Result.cx>=RESULT_BUF_SIZE-1)
	    	break;

	    if (errsbuf.cx){
		    char *errHeader="\nERRORMSG:\n";
		    len1 = errsbuf.cx+strnlen(errHeader, 32);
		    len2 = RESULT_BUF_SIZE-Result.cx-1;
		    len = len2<len1?len2:len1;
		    snprintf(Result.result_buf+Result.cx, len, "%s%s", errHeader, errsbuf.buf);
		    Result.cx+=len;
	    }
	}while(0);

	if(inbuf.buf!=NULL){
	    free(inbuf.buf);
	    inbuf.buf=NULL;
	}
	if(outbuf.buf!=NULL){
	    free(outbuf.buf);
	    outbuf.buf=NULL;
	}
	if(errsbuf.buf!=NULL){
	    free(errsbuf.buf);
	    errsbuf.buf=NULL;
	}

	if (NULL != result){
		snprintf(result, result_size, "%s\n", Result.result_buf);
	}
	free(Result.result_type);
	free(Result.result_buf);
	Result.result_type=NULL;
	Result.result_buf=NULL;
    //sgx_thread_mutex_unlock(&pyVMLock);

	return &Result;
}
