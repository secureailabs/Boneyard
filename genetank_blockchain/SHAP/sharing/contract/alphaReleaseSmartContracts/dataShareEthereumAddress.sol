pragma solidity ^0.5.1;

contract dataShareByEthAcc {
    struct DataRegistration {
            uint256 index;
            mapping(uint256 => uint256) index2HashJson;
    }

    mapping(address => DataRegistration) index2DataRegistration;

    function shareData (uint256 hash_data_json, address ethAcc) public {

        require(hash_data_json != 0);
        require(ethAcc != address(0));

        index2DataRegistration[ethAcc].index2HashJson[index2DataRegistration[ethAcc].index] = hash_data_json;
        index2DataRegistration[ethAcc].index = index2DataRegistration[ethAcc].index + 1;

    }

    function getTotalDataByEthAcc (address ethAcc) public view returns (uint) {
        return index2DataRegistration[ethAcc].index;
    }

    function getDataByEthAccAndIndex (address ethAcc, uint256 idx) public view returns (uint) {
        return index2DataRegistration[ethAcc].index2HashJson[idx];
    }

}
