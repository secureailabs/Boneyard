import React, { Component } from 'react';
import { EJSON } from 'meteor/ejson'

export const listEnclaves = (enclaves) =>{
  const res = enclaves.map((enclave) => {
      var infoJson = enclave.infoJson
      if ("object" !== typeof(infoJson)) {
        infoJson = EJSON.parse(infoJson)
      }
      return <option value={enclave.pubKey} key={enclave.pubKey.toString()}>
        Enclave: {infoJson.name === undefined || infoJson.name === "" ? enclave.pubKey :infoJson.name}
      </option>})

  var opList = ["Select"].map((i)=>(
    <option defaultValue="" value="" key={"select"}>Select</option>))
  var merged = opList.concat(res)
  return merged
}

export const initRunnerInfoJson =
{
    id: "EnclaveRunner",
    ethereumAddress: "", //The Ethereum address of the enclave owner
    enclavePackageHash: "", // The hash of the info Json of the enclave package that the enclave relying on.
    enclaveTrainerHash: "", // The hash of the info Json of the enclave trainer that the enclave running for
    name: "", //Name of the enclave such as “Cloud VM for DSaaS”, “Federated computing Server”, etc.
    attestationReport: "", //The report for IAS, there is a public key of the enclave in the report
    attSignature: "", //The signature of the attestation report
    attSignCert: "", //The certification of the key for the signature
    hardwareInfo: "", //Description of the hardware platform, such as CPU type, system name, etc.
  }
