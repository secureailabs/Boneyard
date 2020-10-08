import cPickle as pickle
import _federated as fd

## Note: in case any change is made to the following list,
## ensure that the  corresponding changes are made to the
## simfl_data_t enum in msg_def.h as well
SIMFL_DATA_UNKNOWN = 0
SIMFL_DATA_LSH = 1
SIMFL_DATA_GHT = 2
SIMFL_TRAIN_PARAM = 3
SIMFL_DATA_GRADIENTS = 4
SIMFL_DATA_MODEL = 5

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


### SIMFL Sub Enclave Functions
def subInit():
	buf = fd.subCollectHubData(SIMFL_TRAIN_PARAM)
	if buf is not None:
		return pickle.loads(buf)
	return

def subAllReduce(local_hash):
	buf = pickle.dumps(local_hash)
	if buf is not None:
		fd.subSendDataToHub(buf, SIMFL_DATA_LSH)
	global_hash_table = fd.subCollectHubData(SIMFL_DATA_GHT)
	if global_hash_table is not None:
		return pickle.loads(global_hash_table)
	return

def subGetTrainParams():
	buf = fd.subCollectHubData(SIMFL_TRAIN_PARAM)
	if buf is not None:
		return pickle.loads(buf)
	return

def subGetGradients():
	buf = fd.subCollectHubData(SIMFL_DATA_GRADIENTS)
	if buf is not None:
		return pickle.loads(buf)
	return

def subSendModel(xgboost_model):
	buf = pickle.dumps(xgboost_model)
	if buf is not None:
		fd.subSendDataToHub(buf, SIMFL_DATA_MODEL)
	return

def subSendGradients(gradients):
	buf = pickle.dumps(gradients)
	if buf is not None:
		fd.subSendDataToHub(buf, SIMFL_DATA_GRADIENTS)
	return

def subGetModel():
	buf = fd.subCollectHubData(SIMFL_DATA_MODEL)
	if buf is not None:
		return pickle.loads(buf)
	return

### SIMFL Hub Enclave Functions
def hubInit():
	nodes = fd.getNumSubEnclaves()
	if nodes is None:
		return None
	for enclaveID in range(nodes):
		buf = pickle.dumps([enclaveID, nodes])
		if buf is not None:
			fd.hubSendDataToSub(enclaveID, buf, len(buf), SIMFL_TRAIN_PARAM)
	return nodes

def hubGetHashTable(enclaveID):
	buf = fd.hubCollectSubData(enclaveID, SIMFL_DATA_LSH)
	if buf is not None:
		return pickle.loads(buf)
	return

def hubSendGlobalHashTable(enclaveID, global_hash_table):
	nodes = fd.getNumSubEnclaves()
	buf = pickle.dumps(global_hash_table)
	if buf is not None:
		for enclaveID in range(nodes):
			fd.hubSendDataToSub(enclaveID, buf, len(buf), SIMFL_DATA_GHT)
	return

def hubSendTrainParams(training):
	nodes = fd.getNumSubEnclaves()
	buf = pickle.dumps(training)
	if buf is not None:
		for enclaveID in range(nodes):
			fd.hubSendDataToSub(enclaveID, buf, len(buf), SIMFL_TRAIN_PARAM)
	return

def hubGetGradients(enclaveID):
	buf = fd.hubCollectSubData(enclaveID, SIMFL_DATA_GRADIENTS)
	if buf is not None:
		return pickle.loads(buf)
	return

def hubSendGradients(enclaveID, gradients):
	buf = pickle.dumps(gradients)
	if buf is not None:
		fd.hubSendDataToSub(enclaveID, buf, len(buf), SIMFL_DATA_GRADIENTS)
	return

def hubGetModel(enclaveID):
	buf = fd.hubCollectSubData(enclaveID, SIMFL_DATA_MODEL)
	if buf is not None:
		return pickle.loads(buf)
	return

def hubSendModel(enclaveID, tree):
	buf = pickle.dumps(tree)
	if buf is not None:
		fd.hubSendDataToSub(enclaveID, buf, len(buf), SIMFL_DATA_MODEL)
	return
