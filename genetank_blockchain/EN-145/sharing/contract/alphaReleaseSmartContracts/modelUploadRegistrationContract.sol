pragma solidity ^0.5.1;

contract UploadModel {

    struct modelDetails{
    	string modelInfo;
    	uint256 enclaveId;
    	bool isValid;
    	uint256 modelJsonHash;
    }
    uint256 index;
    mapping(uint256 => modelDetails) modelRegistration;

    /// @dev registration by creator of enclave.
    function registerModelUploadByTrainer (string memory _modelInfo, uint256 _enclaveId, uint256 _modelJsonHash) public {
    
        require(_modelJsonHash != 0);
        require(bytes(_modelInfo).length > 0);
        require(_enclaveId >= 0);
        
        modelRegistration[index].modelInfo = _modelInfo;
        modelRegistration[index].enclaveId = _enclaveId;
        modelRegistration[index].isValid = true;
        modelRegistration[index].modelJsonHash = _modelJsonHash;
        index = index + 1;
    }

    function getTotalIndex () public view returns (uint) {
        return index;
    }

    // index starts with 0
    function getmodelDetailsByIndex (uint index) public view returns (string memory, uint256, bool, uint256) {
        return (modelRegistration[index].modelInfo, modelRegistration[index].enclaveId, modelRegistration[index].isValid, modelRegistration[index].modelJsonHash);
    }
    
    function updateValidityByIndex( uint _index, bool _isValid) public {
    	modelRegistration[_index].isValid = _isValid;
    }
    
    function returnValiditybyIndex( uint _index) public view returns (bool) {
    	return modelRegistration[_index].isValid;
    }    
}