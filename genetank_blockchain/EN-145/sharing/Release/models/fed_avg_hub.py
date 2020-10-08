import federated as fd
import numpy as np
for iteration in range(1):
  fd.startCycle("fed_avg.py")
  avg=0;
  size=0;
  cnt=0;
  while cnt<3:
    result=fd.retrieveData()
    cnt = cnt + 1
    if result is not None:
      size = size + result[1]
      avg = avg + result[1]*result[0]
  if (size>0):
    avg = avg/size
  print("Total number of patients", size)
  print("Average age", avg)
  