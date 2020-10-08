pragma solidity ^0.5.1;

contract DeleteModel {

    uint256 index;
    mapping(uint256 => uint256) sha3Hash;

    /// @dev registration by creator of enclave.
    function registerModelDeletionByTrainer (uint256 data_info_sha3) public {
        require(data_info_sha3 != 0);
        sha3Hash[index] = data_info_sha3;
        index = index + 1;
    }

    function getTotalIndex () public view returns (uint) {
        return index;
    }

    // index starts with 0
    function getDataByIndex (uint index) public view returns (uint) {
        return sha3Hash[index];
    }
}
