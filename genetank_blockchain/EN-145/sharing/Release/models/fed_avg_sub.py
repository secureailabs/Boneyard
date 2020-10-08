import csv
import federated as fd
import numpy as np
data_path = "/data/sharedin/migraine1-1.csv"
with open(data_path, 'r') as f:
    reader = csv.reader(f, delimiter=',')
    # get header from first row
    headers = next(reader)
    # get all the rows as a list
    data = list(reader)
    f.close()

a=[]
for row in range(len(data)):
  a.append(int(data[row][1]))
da=np.array(a)
res=[np.average(da), np.size(da)]
#res.append = np.average(da)
#res['size'] = np.size(da)

fd.submitData(res)
