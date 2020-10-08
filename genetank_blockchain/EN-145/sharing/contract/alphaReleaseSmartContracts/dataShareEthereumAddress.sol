pragma solidity ^0.6.1;

contract dataShareByEthAcc {

	struct sharingDetails{
		uint256 dataId;
		uint256 sharingJsonHash;
		bool validity;
	}
    struct DataSharing {
            uint256 index;
            mapping(uint256 => sharingDetails) index2SharingDetails;
    }

    mapping(address => DataSharing) address2DataSharing;

    function shareDataToAddress (uint256 _dataId, uint256 _sharingJsonHash, address ethAcc) public {
        require(_sharingJsonHash != 0);
        require( _dataId >= 0);
        require(ethAcc != address(0));

        address2DataSharing[ethAcc].index2SharingDetails[address2DataSharing[ethAcc].index].sharingJsonHash = _sharingJsonHash;
        address2DataSharing[ethAcc].index2SharingDetails[address2DataSharing[ethAcc].index].dataId = _dataId;
        address2DataSharing[ethAcc].index2SharingDetails[address2DataSharing[ethAcc].index].validity = true;
        address2DataSharing[ethAcc].index = address2DataSharing[ethAcc].index + 1;
    }

    function getTotalDataByEthAcc (address ethAcc) public view returns (uint) {
        return address2DataSharing[ethAcc].index;
    }

    function getDataByEthAccAndIndex (address ethAcc, uint256 _index) public view returns (uint256, uint256, bool) {
        return (address2DataSharing[ethAcc].index2SharingDetails[_index].dataId, address2DataSharing[ethAcc].index2SharingDetails[_index].sharingJsonHash, address2DataSharing[ethAcc].index2SharingDetails[_index].validity);
    }
    
    function updateValidityByEthAccAndIndex(address ethAcc, uint _index, bool _validity) public {
    	address2DataSharing[ethAcc].index2SharingDetails[_index].validity = _validity;
    }
    
    function returnValidityByEthAccAndIndex( uint _index) public view returns (bool) {
    	return address2DataSharing[ethAcc].index2SharingDetails[_index].validity;
    }    

}