      SUBROUTINE DSINQF (N,X,WSAVE)
      IMPLICIT NONE
      INTEGER N, K, KC, NS2
      DOUBLE PRECISION X(1), WSAVE(1), XHOLD
      IF (N .EQ. 1) RETURN
      NS2 = N/2
      DO 101 K=1,NS2
         KC = N-K
         XHOLD = X(K)
         X(K) = X(KC+1)
         X(KC+1) = XHOLD
  101 CONTINUE
      CALL DCOSQF (N,X,WSAVE)
      DO 102 K=2,N,2
         X(K) = -X(K)
  102 CONTINUE
      RETURN
      END
