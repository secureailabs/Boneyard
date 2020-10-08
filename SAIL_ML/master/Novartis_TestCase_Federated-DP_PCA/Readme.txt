iHMPall_metadata_df.tsv and iHMPall_norm_df.tsv have dupicate column names, although it is rare. For instance, PXS123456 can have 2 columns with same name. I think this is something wrong with data.

When dealing with dupicate column names, pandas read_csv renames the second column met, thus only taking first column's value into consideration to result.


I have 2 implementations about this situation. The only different of v1 and v2 is readCsv function.
All other functions are same. Also, you need to de-compress compressed TSVs to use my readCsv functions.

toNumpy-V2 notebook takes first column value into result, which has same final result from as orignal pandas notebook.

toNumpy-V1 notebook takes last(second) column value into result, which has has a bit different final result from orignal pandas notebook.

I recommend you to use  toNumpy-V2 notebook, and toNumpy-V2 notebook has better comments about functions.


All functions I defined have no side effects, e.g, they do not change input objects.


You can just run jupyter notebook to get results, or you can use my defined functions.


Environment: Python3, although I believe you can run my codes on python2.
Imports: numpy, python built-in csv and OrderedDict module
I also used Scipy.stats for the rankSum function, it is not in Numpy.


All my dataframes and final outputs are stored as list of OrderedDicts.
For instance, in toNumpy-V2, final output of Test UC x nonIBD is:
[OrderedDict([('msp_name', 'msp_0001'),
              ('module_name', 'core'),
              ('Nnotnull1', 8),
              ('Nnotnull2', 13),
              ('statistic', -0.95521850710367),
              ('pvalue', 0.33946720858371526)]),
 OrderedDict([('msp_name', 'msp_0002'),
              ('module_name', 'core'),
              ('Nnotnull1', 2),
              ('Nnotnull2', 8),
              ('statistic', -1.2012596377212819),
              ('pvalue', 0.2296505019618409)]),
....


By comparison with output of orignal pandas output of Test UC x nonIBD, you can see they are same:



msp_name  module_name   Nnotnull1  Nnotnull2  statistic   pvalue
msp_0001  core          8.0        13.0        -0.955219  0.339467
msp_0002  core          2.0         8.0         -1.201260  0.229651
....
