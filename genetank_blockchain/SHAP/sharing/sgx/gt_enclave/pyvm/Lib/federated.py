import numpy as np
import cPickle as pickle
import _federated as fd

def submitData(data):
	buf = pickle.dumps(data)
	fd.submitResult(buf)

def retrieveData():
	buf = fd.collectSubResult()
	if buf is not None:
		return pickle.loads(buf)
	return

def startCycle(projectName):
	fd.startAllSub(projectName)
	
def runSubOPAL(subEnclaveName, OPAL_algo, secret_algo, dataset):
        res = fd.runSingleAlgo(secret_algo, dataset, subEnclaveName, OPAL_algo)
	return res

def runAlgoInSandbox(algo, dataPath):
        res = fd.runSingleAlgo(algo, dataPath, "", "")
	return res
