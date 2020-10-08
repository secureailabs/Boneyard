pragma solidity ^0.6.1;

contract RegisterData {

    struct registration{
    string dataOwnerInfo;
    bool isValid;
    uint256 sha3Hash;
    }
    uint256 index;
    mapping(uint256 => registration) dataRegistration;

    /// @dev registration by creator of enclave.
    function registerDataByOwner (uint256 data_info_sha3, string memory _dataOwnerInfo) public {
        require(data_info_sha3 != 0);
        require(bytes(_dataOwnerInfo).length > 0);
        
        dataRegistration[index].sha3Hash = data_info_sha3;
        dataRegistration[index].isValid = true;
        dataRegistration[index].dataOwnerInfo = _dataOwnerInfo;
        index = index + 1;
    }

    function getTotalIndex () public view returns (uint) {
        return index;
    }

    // index starts with 0
    function getDataByIndex (uint _index) public view returns (string memory, bool, uint256) {
        return (dataRegistration[_index].dataOwnerInfo, dataRegistration[_index].isValid, dataRegistration[_index].sha3Hash);
    }
    
    function updateValidityByIndex( uint _index, bool _isValid) public {
    	dataRegistration[_index].isValid = _isValid;
    }
    
    function returnValiditybyIndex( uint _index) public view returns (bool) {
    	return dataRegistration[_index].isValid;
    }
}