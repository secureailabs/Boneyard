#! /bin/bash

./plink \
--noweb \
--bfile 1000G_20101123_v3_GIANT_chr1_23_minimacnames_CEU_MAF0.01 \
--indep 50 5 2
