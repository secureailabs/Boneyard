#for f1 in z*.c; do
#    sed -n '/\/\* Subroutine \*\//,$p' $f1 >> z_lapack.c
#    sed -i '$a\\r' z_lapack.c
#done

#for f2 in c*.c; do
#    sed -n '/\/\* Subroutine \*\//,$p' $f2 >> c_lapack.c
#    sed -i '$a\\r' c_lapack.c
#done

#for f3 in s*.c; do
#    sed -n '/\/\* Subroutine \*\//,$p' $f3 >> s_lapack.c
#    sed -i '$a\\r' s_lapack.c
#done

for f4 in d*.c; do
    sed -n '/\/\* Subroutine \*\//,$p' $f4 >> d_lapack.c
    sed -i '$a\\r' d_lapack.c
done
