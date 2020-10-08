# Test
Files:
1. [testEnclave.js](https://github.com/herroannekim/genetank_blockchain/blob/phaseII/sharing/dapp/imports/ui/pages/test/testEnclave.js) - Renders the Test enclave page UI
2. [testenclave.js](https://github.com/herroannekim/genetank_blockchain/blob/phaseII/sharing/dapp/server/test/testenclave.js) - has functions called by the ui, in turn calls funcitons from test.js
3. [test.js](https://github.com/herroannekim/genetank_blockchain/blob/phaseII/sharing/dapp/server/model/test.js) - has functions to do the testing

---

## testenclave.js
Contain methods which are invoked by the front end to start the test
```javascript
Meteor.methods({
  runTestOnServer(param) {
	return sendData2EnclaveASync(params)
},
runTestOnServerSession(param){
	return testSessionASync(params);
},
})
```

---
## test.js
Contains definition of the functions being called above
```javascript

export function sendData2EnclaveASync(params){

}

export function testSessionASync(params) {

}

function pyVMTest(params){
  //called internally based on the test options
}

function dataUploadTest(params){
    //called internally based on the test options
}

function getInfoTest(params){

}
```
---
## testEnclave.js
Renders the page /testEnclave

```HTML
<div>
  <input text-align='left' type="Radio" name="testRadio" value="pyVM"/> PYVM Test
  <br/>
  <input text-align='left' type="Radio" name="testRadio" value="sessions"/> Sessions Test
  <br/>
  <input text-align='left' type="Radio" name="testRadio" value="dataUpload"/> Data Upload Test
  <br/>
  <button className="btn btn-success" id="runTest" onClick={this.onSubmit}>Run Test</button>
</div>
<div id="result"></div>
<div hidden id='hide-show' class="spinner-border" role="status">
  <span class="sr-only">Loading...</span>
</div>

```

Enclave functions tested and working :

:white_check_mark: Upload data

:white_check_mark: Run R Model

:white_check_mark: Get Info
