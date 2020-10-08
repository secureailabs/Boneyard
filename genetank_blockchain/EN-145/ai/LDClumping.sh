#! /bin/bash

./plink \
--noweb \
--bfile 1000G_20101123_v3_GIANT_chr1_23_minimacnames_CEU_MAF0.01 \
--clump IGAP_stage_1_reformatted.txt \
--clump-field P \
--clump-p1 0.0001 \
--clump-p2 0.01 \
--clump-r2 0.5 \
--clump-kb 250 \
--out IGAP_stage_1_Out
