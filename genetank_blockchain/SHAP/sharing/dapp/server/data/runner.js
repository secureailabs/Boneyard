import { Meteor } from 'meteor/meteor';
import {HTTP} from "meteor/http";
import { myContract } from "/server/smartcontract.js";
import {chainhubUrl} from "/server/main.js";

Meteor.methods({
    getModels() {
        var Future = Npm.require('fibers/future');
        var resultArray = new Future();

        myContract.methods.getTotalAiModel().call()
            .then((total) => {
                var hashArray = [];
                var fut = new Future();
                console.log(total);

                if(total === "0") {
                    fut.return(hashArray);
                }

                for(var i=0; i<total; i++){

                    var futGetElement = new Future();
                    myContract.methods.getAiModelTrainerByIndex(i).call()
                        .then((response) => {
                            //console.log(web3.utils.toHex(response[0]));
                            //console.log(response);
                            futGetElement.return(web3.utils.toHex(response[0]));
                        });
                    hashArray.push(futGetElement.wait());
                    if(i===total-1) {
                        fut.return(hashArray)
                    }
                }

                var response = getDataGenehub(fut.wait());
                // console.log(response);
                resultArray.return(response);
            }).catch((error) => {
            console.log(error);
            resultArray.return();
        });
        return (resultArray.wait());
    },

    getRunnerFromTrainerAddress(trainerAddress) {
        var Future = Npm.require('fibers/future');
        var resultArray = new Future();

        myContract.methods.getTotalEnclaveByTrainerAddress(trainerAddress).call({from: trainerAddress})
            .then((total) => {
                //var total = response;
                let hashArray = [];
                let fut = new Future();

                if(total === "0") {
                   fut.return(hashArray);
                }

                for(let i=0; i<total; i++){

                    var futGetElement = new Future();
                    myContract.methods.getEnclaveByTrainerAndIndex(trainerAddress,i).call({from: trainerAddress})
                        .then((response) => {
                            var json = {hash: web3.utils.toHex(response[0]), publicKey: response[1]}
                            console.log(web3.utils.toHex(response[0]));
                            console.log(response[1]);
                            futGetElement.return(json);
                        });
                    hashArray.push(futGetElement.wait());
                    if(i===total-1) {
                        fut.return(hashArray)
                    }
                }

                var response = getEnclaveFromGenehub(fut.wait());
                console.log(response);
                resultArray.return(response);

            }).catch((error) => {
            console.log(error);
            resultArray.return();
        });

        return (resultArray.wait());
    }
});

export function getDataGenehub(hashArray) {

    var Future = Npm.require('fibers/future');
    var infoJsonArray = [];
    var fut = new Future();

    if(hashArray.length === 0) {
        fut.return([]);
    }
    console.log(hashArray)
    // const httpGetUrl = "https://hub.secureailabs.com/entry/";
    const httpGetUrl = chainhubUrl + "/entry/";
    for(var i=0; i<hashArray.length; i++){
        var futGetElement = new Future();
        HTTP.get(httpGetUrl+hashArray[i], {
            headers: {
                'Content-Type': 'application/json'
            }
        }, (error, result) => {
            if(!error){
                // console.log(result.data.value.infoJSON)

                const value = JSON.parse(result.data.value)
                console.log(value.infoJSON)

                var hashVerify = web3.utils.sha3(JSON.stringify(value.infoJSON));
                console.log(hashVerify);
                // console.log(result.data.value.infoJSON);
                if(hashVerify === hashArray[i]) {
                    futGetElement.return(value.infoJSON);
                }
                else
                    futGetElement.return();
            }
            else{
                console.log(error);
                futGetElement.return();
            }
        })
        const entry = futGetElement.wait();
        if (entry != null) {
            infoJsonArray.push(entry);
        }


        if(i === hashArray.length - 1){
            fut.return(infoJsonArray);
        }
    }
    return fut.wait();
}

function getEnclaveFromGenehub(hashArray) {

    var Future = Npm.require('fibers/future');
    var infoJsonArray = [];
    var fut = new Future();

    if(hashArray.length === 0) {
        fut.return([]);
    }

    httpGetUrl = "https://hub.secureailabs.com/entry/";
    for(var i=0; i<hashArray.length; i++){
        var futGetElement = new Future();
        HTTP.get(httpGetUrl+hashArray[i].hash, {
            headers: {
                'Content-Type': 'application/json'
            }
        }, (error, result) => {
            if(!error){
                var hashVerify = web3.utils.sha3(result.data.value.infoJSON);
                // console.log(hashVerify);
                // console.log(result.data.value.infoJSON);
                if(hashVerify === hashArray[i].hash) {
                    var json = JSON.parse(result.data.value.infoJSON);
                    json.publicKey = hashArray[i].publicKey;
                    futGetElement.return(json);
                }
                else
                    futGetElement.return();
            }
            else{
                console.log(error);
                futGetElement.return();
            }
        })
        infoJsonArray.push(futGetElement.wait());

        if(i === hashArray.length - 1){
            fut.return(infoJsonArray);
        }
    }
    return fut.wait();
}
