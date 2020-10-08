let entries = {};
let url = "http://localhost:4100"
$(document).ready(function () {
   // Check if Web3 has been injected by the browser:
    if (typeof web3 !== 'undefined') {
      web3 = new Web3(web3.currentProvider);
      web3.eth.defaultAccount = web3.eth.accounts[0];
    } else {
      alert("Please Configure Metamask with this Browser. You will require it while validating a Sha3 Hash.")
    }

  // Retrieve first list of entries.
  getEntries();
  // Poll for new entries every 10 seconds.
  // window.setInterval(getEntries, 10 * 1000);
});

// Triggers when search button is clicked
function onSubmit() {
  let text = document.getElementById("searchbox").value;
  if (text !== ""){
    getSearchEntries(text);
  }
  else{
    alert("Please Enter Some Text in the Search Field!")
  }
}

// Triggers when validate button is clicked
function onValidate() {
  let transactionType = document.getElementById("transactionType").value;
  let inputHash = document.getElementById("sha3Box").value;
  if (inputHash !== ""){
    try{
      if (transactionType === "enclaveRegistration")
        validateEnclaveRegistrationTransaction(inputHash);
      if (transactionType === "enclaveDeRegistration")
        validateEnclaveDeRegistrationTransaction(inputHash);
      if (transactionType === "dataRegistration")
        validateDataRegistrationTransaction(inputHash);
      if (transactionType === "dataDeRegistration")
        validateDataDeRegistrationTransaction(inputHash);
      if (transactionType === "modelUpload")
        validateModelUploadTransaction(inputHash);
      if (transactionType === "modelDeletion")
        validateModelDeletionTransaction(inputHash);
      if (transactionType === "dataAccess")
        validateDataAccessTransaction(inputHash);
    } catch (e) {
      alert("There was some issue connecting to Web3, Please check if you have Metamask configured properly!")
    }
  }
  else{
    alert("Please Enter an SHA3 Hash in the Text Box")
  }
}


// Add click handlers for elements in the entries table.
function addEntriesClickHandlers() {
  $('#entries .delete-row i').on('click', function (e) {
    if ($(this).hasClass('fa-disabled')) {
      // Nothing happens if deletion is disabled.
      return;
    }

    // Get the key of the row.
    const data = JSON.stringify({
      key: $('#entries-' + $(this).data('index') + ' .entries-key').text()
    });

    // Send entry delete request.
    $.ajax({
      url: '/entry/delete',
      type: 'POST',
      data: data,
      contentType: 'application/json',
      success: function (data) {
        if (data.err) {
          alert('Sorry, something went wrong while deleting your entry. Please try again.');
        } else {
          alert('Success! Returned data: ' + JSON.stringify(data));
          // Refresh entries.
          getEntries();
        }
      },
      error: function (xhr, ajaxOptions, thrownError) {
        alert(JSON.stringify(xhr));
        // Refresh entries.
        getEntries();
      }
    });
  });
}

// Get list of entries.
function getEntries() {
  $.ajax({
    url: '/entries',
    type: 'GET',
    success: function (data) {
      if (data.err) {
        // TODO: Surface this error better.
        alert('Sorry, something went wrong while loading the current ChainHub entries. Please try again.');
      } else {
        // Display entries.
        console.log('Success! Returned data: ' + JSON.stringify(data));
        if (entries.length === data.length) {
          var changed = false;
          for (var i = 0; i < data.length; i++) {
            if (!_.isEqual(entries[i], data[i])) {
              changed = true;
              break;
            }
          }
          if (!changed) {
            // The entries haven't changed; no need to refresh the table.
            return;
          }
        }
        entries = data;

        // Clear table.
        $('#entries tbody').empty();
        // Populate table.
        for (var i = 0; i < data.length; i++) {
          var protectedStr = '<i class="fas fa-';
          var protectedClass = '';
          if (data[i].protected) {
            protectedStr += 'check"></i>';
            protectedClass = ' fa-disabled';
          } else {
            protectedStr += 'times"></i>';
          }
	  // Post-process entries to fit better in the table.
	  var keyStr = data[i].key;
	  if (keyStr.length > 6) {
	    keyStr = keyStr.slice(0,6) + '...';
	  }
          $('#entries tbody').append($(
            '<tr id="entries-' + i + '">' +
            '<th scope="row" class="entries-key"><pre id="entries-key-' + i + '"></pre></th>' +
            '<td class="entries-value"><pre id="entries-value-' + i + '"></pre></td>' +
            '<td class="entries-keywords"><pre id="entries-keywords-' + i + '"></pre></td>' +
            '<td class="entries-protected">' + protectedStr + '</td>' +
            '<td class="delete-row"><i class="fas fa-trash' + protectedClass + '" data-index="' + i + '"></i></td>' +
            '</tr>'));
      $('#entries-key-' + i).html(data[i].key, undefined, 2);
      $('#entries-value-' + i).html(JSON.stringify(JSON.parse(data[i].value), undefined, 2));
	  $('#entries-keywords-' + i).html(data[i].keywords, undefined, 2);
        }
        addEntriesClickHandlers();
      }
    },
    error: function (xhr, ajaxOptions, thrownError) {
      alert(JSON.stringify(xhr));
    }
  });
}


// Get list of entries.
function getSearchEntries(keyword) {
  $.ajax({
    url: '/entries/'+keyword,
    type: 'GET',
    success: function (data) {
      if (data.err) {
        // TODO: Surface this error better.
        alert('Sorry, something went wrong while loading the current ChainHub entries. Please try again.');
      } else {
        // Display entries.
        console.log('Success! Returned data: ' + JSON.stringify(data));
        if (entries.length === data.length) {
          var changed = false;
          for (var i = 0; i < data.length; i++) {
            if (!_.isEqual(entries[i], data[i])) {
              changed = true;
              break;
            }
          }
          if (!changed) {
            // The entries haven't changed; no need to refresh the table.
            return;
          }
        }
        entries = data;

        // Clear table.
        $('#entries tbody').empty();
        // Populate table.
        for (var i = 0; i < data.length; i++) {
          var protectedStr = '<i class="fas fa-';
          var protectedClass = '';
          if (data[i].protected) {
            protectedStr += 'check"></i>';
            protectedClass = ' fa-disabled';
          } else {
            protectedStr += 'times"></i>';
          }
          // Post-process entries to fit better in the table.
          var keyStr = data[i].key;
          if (keyStr.length > 6) {
            keyStr = keyStr.slice(0,6) + '...';
          }
          $('#entries tbody').append($(
              '<tr id="entries-' + i + '">' +
              '<th scope="row" class="entries-key"><pre id="entries-key-' + i + '"></pre></th>' +
              '<td class="entries-value"><pre id="entries-value-' + i + '"></pre></td>' +
              '<td class="entries-keywords"><pre id="entries-keywords-' + i + '"></pre></td>' +
              '<td class="entries-protected">' + protectedStr + '</td>' +
              '<td class="delete-row"><i class="fas fa-trash' + protectedClass + '" data-index="' + i + '"></i></td>' +
              '</tr>'));
          $('#entries-key-' + i).html(data[i].key, undefined, 2);
          $('#entries-value-' + i).html(JSON.stringify(JSON.parse(data[i].value), undefined, 2));
          $('#entries-keywords-' + i).html(data[i].keywords, undefined, 2);
        }
        addEntriesClickHandlers();
      }
    },
    error: function (xhr, ajaxOptions, thrownError) {
      alert(JSON.stringify(xhr));
    }
  });
}

// checks if the given hash is present in the given list
function validityCheck(sha3hashes, inputHash){
  let result = false;
    $.get(url+"/entry/"+inputHash)
      .then(object => {
        let eventInfo = JSON.parse(object.value).eventInfo;
        if (web3.utils.sha3(JSON.stringify(eventInfo)) === inputHash){
          for (let i in sha3hashes){
            if (sha3hashes[i] === inputHash)
              result = true;
          }
          if (result)
            alert("This is a Valid Hash");
          else
            alert("The Identity of this Hash could not be verified!");
        }
      });
}

// Smart Contract Functions

function validateEnclaveRegistrationTransaction(inputHash){
  let abiArray =
      [
        {
          "constant": false,
          "inputs": [
            {
              "name": "enclave_info_sha3",
              "type": "string"
            }
          ],
          "name": "registerEnclaveByCreator",
          "outputs": [],
          "payable": false,
          "stateMutability": "nonpayable",
          "type": "function"
        },
        {
          "inputs": [],
          "payable": false,
          "stateMutability": "nonpayable",
          "type": "constructor"
        },
        {
          "constant": true,
          "inputs": [],
          "name": "getRegisteredEnclaves",
          "outputs": [
            {
              "name": "",
              "type": "string[]"
            }
          ],
          "payable": false,
          "stateMutability": "view",
          "type": "function"
        },
        {
          "constant": true,
          "inputs": [],
          "name": "owner",
          "outputs": [
            {
              "name": "",
              "type": "address"
            }
          ],
          "payable": false,
          "stateMutability": "view",
          "type": "function"
        },
        {
          "constant": true,
          "inputs": [
            {
              "name": "",
              "type": "uint256"
            }
          ],
          "name": "sha3Hashes",
          "outputs": [
            {
              "name": "",
              "type": "string"
            }
          ],
          "payable": false,
          "stateMutability": "view",
          "type": "function"
        }
      ]
  let contractAddress = "0xDfA949f05afEF08AE24df720e400B1D1f5e43De5";
  return new web3.eth.Contract(abiArray, contractAddress)
      .methods.getRegisteredEnclaves().call()
      .then(sha3hashes => {
        validityCheck(sha3hashes, inputHash);
      });
}

function validateEnclaveDeRegistrationTransaction(inputHash){
  let abiArray =
      [
        {
          "constant": false,
          "inputs": [
            {
              "name": "enclave_dereg_info_sha3",
              "type": "string"
            }
          ],
          "name": "deRegisterEnclaveByCreator",
          "outputs": [],
          "payable": false,
          "stateMutability": "nonpayable",
          "type": "function"
        },
        {
          "inputs": [],
          "payable": false,
          "stateMutability": "nonpayable",
          "type": "constructor"
        },
        {
          "constant": true,
          "inputs": [],
          "name": "getDeRegisteredEnclaves",
          "outputs": [
            {
              "name": "",
              "type": "string[]"
            }
          ],
          "payable": false,
          "stateMutability": "view",
          "type": "function"
        },
        {
          "constant": true,
          "inputs": [],
          "name": "owner",
          "outputs": [
            {
              "name": "",
              "type": "address"
            }
          ],
          "payable": false,
          "stateMutability": "view",
          "type": "function"
        },
        {
          "constant": true,
          "inputs": [
            {
              "name": "",
              "type": "uint256"
            }
          ],
          "name": "sha3Hashes",
          "outputs": [
            {
              "name": "",
              "type": "string"
            }
          ],
          "payable": false,
          "stateMutability": "view",
          "type": "function"
        }
      ]
  let contractAddress = "0x698af182669F03996A9b3162Ec8E54E1DcB69B8f";
  return new web3.eth.Contract(abiArray, contractAddress)
      .methods.getDeRegisteredEnclaves().call()
      .then(sha3hashes => {
        validityCheck(sha3hashes, inputHash);
      });
}

function validateDataRegistrationTransaction(inputHash){
  let abiArray =
      [
        {
          "constant": false,
          "inputs": [
            {
              "name": "data_info_sha3",
              "type": "string"
            }
          ],
          "name": "registerDataByOwner",
          "outputs": [],
          "payable": false,
          "stateMutability": "nonpayable",
          "type": "function"
        },
        {
          "inputs": [],
          "payable": false,
          "stateMutability": "nonpayable",
          "type": "constructor"
        },
        {
          "constant": true,
          "inputs": [],
          "name": "getRegisteredDataList",
          "outputs": [
            {
              "name": "",
              "type": "string[]"
            }
          ],
          "payable": false,
          "stateMutability": "view",
          "type": "function"
        },
        {
          "constant": true,
          "inputs": [],
          "name": "owner",
          "outputs": [
            {
              "name": "",
              "type": "address"
            }
          ],
          "payable": false,
          "stateMutability": "view",
          "type": "function"
        },
        {
          "constant": true,
          "inputs": [
            {
              "name": "",
              "type": "uint256"
            }
          ],
          "name": "sha3Hashes",
          "outputs": [
            {
              "name": "",
              "type": "string"
            }
          ],
          "payable": false,
          "stateMutability": "view",
          "type": "function"
        }
      ]
  let contractAddress = "0xE9AbB67050274578C0C97CC7544E92bCd2EE3c09";
  return new web3.eth.Contract(abiArray, contractAddress)
      .methods.getRegisteredDataList().call()
      .then(sha3hashes => {
        validityCheck(sha3hashes, inputHash);
      });
}

function validateDataDeRegistrationTransaction(inputHash){
  let abiArray =
      [
        {
          "constant": true,
          "inputs": [],
          "name": "getDeRegisteredDataList",
          "outputs": [
            {
              "name": "",
              "type": "string[]"
            }
          ],
          "payable": false,
          "stateMutability": "view",
          "type": "function"
        },
        {
          "constant": true,
          "inputs": [],
          "name": "owner",
          "outputs": [
            {
              "name": "",
              "type": "address"
            }
          ],
          "payable": false,
          "stateMutability": "view",
          "type": "function"
        },
        {
          "constant": false,
          "inputs": [
            {
              "name": "data_dereg_info_sha3",
              "type": "string"
            }
          ],
          "name": "deRegisterDataByOwner",
          "outputs": [],
          "payable": false,
          "stateMutability": "nonpayable",
          "type": "function"
        },
        {
          "constant": true,
          "inputs": [
            {
              "name": "",
              "type": "uint256"
            }
          ],
          "name": "sha3Hashes",
          "outputs": [
            {
              "name": "",
              "type": "string"
            }
          ],
          "payable": false,
          "stateMutability": "view",
          "type": "function"
        },
        {
          "inputs": [],
          "payable": false,
          "stateMutability": "nonpayable",
          "type": "constructor"
        }
      ]
  let contractAddress = "0x5338E06332275b3cAEcBcA2f0Da25784e0C219d0";
  return new web3.eth.Contract(abiArray, contractAddress)
      .methods.getDeRegisteredDataList().call()
      .then(sha3hashes => {
        validityCheck(sha3hashes, inputHash);
      });
}

function validateModelUploadTransaction(inputHash){
  let abiArray =
      [
        {
          "constant": false,
          "inputs": [
            {
              "name": "model_upload_info_sha3",
              "type": "string"
            }
          ],
          "name": "registerModelUploadByTrainer",
          "outputs": [],
          "payable": false,
          "stateMutability": "nonpayable",
          "type": "function"
        },
        {
          "inputs": [],
          "payable": false,
          "stateMutability": "nonpayable",
          "type": "constructor"
        },
        {
          "constant": true,
          "inputs": [],
          "name": "getRegisteredModels",
          "outputs": [
            {
              "name": "",
              "type": "string[]"
            }
          ],
          "payable": false,
          "stateMutability": "view",
          "type": "function"
        },
        {
          "constant": true,
          "inputs": [],
          "name": "owner",
          "outputs": [
            {
              "name": "",
              "type": "address"
            }
          ],
          "payable": false,
          "stateMutability": "view",
          "type": "function"
        },
        {
          "constant": true,
          "inputs": [
            {
              "name": "",
              "type": "uint256"
            }
          ],
          "name": "sha3Hashes",
          "outputs": [
            {
              "name": "",
              "type": "string"
            }
          ],
          "payable": false,
          "stateMutability": "view",
          "type": "function"
        }
      ]
  let contractAddress = "0x4a779DCCFf6B680E0d7480d53Ae6902cF5527254";
  return new web3.eth.Contract(abiArray, contractAddress)
      .methods.getRegisteredModels().call()
      .then(sha3hashes => {
        validityCheck(sha3hashes, inputHash);
      });
}

function validateModelDeletionTransaction(inputHash){
  let abiArray =
      [
        {
          "constant": false,
          "inputs": [
            {
              "name": "model_deletion_info_sha3",
              "type": "string"
            }
          ],
          "name": "registerModelDeletionByTrainer",
          "outputs": [],
          "payable": false,
          "stateMutability": "nonpayable",
          "type": "function"
        },
        {
          "inputs": [],
          "payable": false,
          "stateMutability": "nonpayable",
          "type": "constructor"
        },
        {
          "constant": true,
          "inputs": [],
          "name": "getRegisteredModelDeletions",
          "outputs": [
            {
              "name": "",
              "type": "string[]"
            }
          ],
          "payable": false,
          "stateMutability": "view",
          "type": "function"
        },
        {
          "constant": true,
          "inputs": [],
          "name": "owner",
          "outputs": [
            {
              "name": "",
              "type": "address"
            }
          ],
          "payable": false,
          "stateMutability": "view",
          "type": "function"
        },
        {
          "constant": true,
          "inputs": [
            {
              "name": "",
              "type": "uint256"
            }
          ],
          "name": "sha3Hashes",
          "outputs": [
            {
              "name": "",
              "type": "string"
            }
          ],
          "payable": false,
          "stateMutability": "view",
          "type": "function"
        }
      ]
  let contractAddress = "0xD8D1a9854DaE846828c36F847d446A1f91C143E1";
  return new web3.eth.Contract(abiArray, contractAddress)
      .methods.getRegisteredModelDeletions().call()
      .then(sha3hashes => {
        validityCheck(sha3hashes, inputHash);
      });
}

function validateDataAccessTransaction(inputHash){
  let abiArray =
      [
        {
          "constant": false,
          "inputs": [
            {
              "name": "data_access_info_sha3",
              "type": "string"
            }
          ],
          "name": "registerDataAccessByTrainer",
          "outputs": [],
          "payable": false,
          "stateMutability": "nonpayable",
          "type": "function"
        },
        {
          "inputs": [],
          "payable": false,
          "stateMutability": "nonpayable",
          "type": "constructor"
        },
        {
          "constant": true,
          "inputs": [],
          "name": "getRegisteredDataAccessList",
          "outputs": [
            {
              "name": "",
              "type": "string[]"
            }
          ],
          "payable": false,
          "stateMutability": "view",
          "type": "function"
        },
        {
          "constant": true,
          "inputs": [],
          "name": "owner",
          "outputs": [
            {
              "name": "",
              "type": "address"
            }
          ],
          "payable": false,
          "stateMutability": "view",
          "type": "function"
        },
        {
          "constant": true,
          "inputs": [
            {
              "name": "",
              "type": "uint256"
            }
          ],
          "name": "sha3Hashes",
          "outputs": [
            {
              "name": "",
              "type": "string"
            }
          ],
          "payable": false,
          "stateMutability": "view",
          "type": "function"
        }
      ];
  let contractAddress = "0xC6f710f276da233ade66E88500A1b412444e4fA1";
  return new web3.eth.Contract(abiArray, contractAddress)
      .methods.getRegisteredDataAccessList().call()
      .then(sha3hashes => {
        validityCheck(sha3hashes, inputHash);
      });
}