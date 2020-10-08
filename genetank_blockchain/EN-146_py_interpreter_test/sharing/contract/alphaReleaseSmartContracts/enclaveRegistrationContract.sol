pragma solidity ^0.6.1;

contract RegisterEnclave {

    struct enclaveDetails{
    	string enclavePubkey;
    	string enclaveCreatorInfo;
    	bool isValid;
    	uint256 sha3Hash;
    }
    uint256 index;
    mapping(uint256 => enclaveDetails) enclaveRegistration;

    /// @dev registration by creator of enclave.
    function registerEnclaveByCreator (uint256 _sha3Hash, string memory _enclavePubkey, string memory _enclaveCreatorInfo) public {
        require(_sha3Hash != 0);
        enclaveRegistration[index].sha3Hash = _sha3Hash;
        enclaveRegistration[index].enclavePubkey = _enclavePubkey;
        enclaveRegistration[index].isValid = true;
        enclaveRegistration[index].enclaveCreatorInfo = _enclaveCreatorInfo;
        index = index + 1;
    }

    function getTotalIndex () public view returns (uint) {
        return index;
    }

    // index starts with 0
    function getEnclaveDetailsByIndex (uint _index) public view returns (string memory, string memory, uint256) {
        return (enclaveRegistration[_index].enclavePubkey, enclaveRegistration[_index].enclaveCreatorInfo, enclaveRegistration[_index].sha3Hash);
    }
    
    function updateValidityByIndex( uint _index, bool _isValid) public {
    	enclaveRegistration[_index].isValid = _isValid;
    }
    
    function returnValiditybyIndex( uint _index) public view returns (bool) {
    	return enclaveRegistration[_index].isValid;
    }
}