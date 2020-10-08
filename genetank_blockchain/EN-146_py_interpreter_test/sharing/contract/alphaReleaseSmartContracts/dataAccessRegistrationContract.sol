pragma solidity ^0.6.1;

contract RegisterDataAccess {
	
	struct accessAdministration{
	uint256 dataId;
	string userId;
	string purpose;
	}
    
    struct auditAdministration{
    uint256 dataId;
    address auditorAddress;
    uint256 auditorJsonHash;
    }
    
    uint256 accessIndex;
    mapping(uint256 => accessAdministration) accessLogs;
    
    uint256 auditIndex;
    mapping(uint256 => auditAdministration) auditLogs;

    /// @dev registration by creator of enclave.
    function registerDataAccess (uint256 _dataId, string memory _userId, string memory _purpose) public {
        require(_dataId >= 0);
        require(bytes(_userId).length > 0);
        require(bytes(_purpose).length > 0);
        
        accessLogs[accessIndex].dataId = _dataId;
        accessLogs[accessIndex].userId = _userId;
        accessLogs[accessIndex].purpose = _purpose;
        accessIndex = accessIndex + 1;
    }
    
    function registerDataAudit (uint256 _dataId, address _auditorAddress, uint256 _auditorJsonHash) public {
        require(_dataId >= 0);
        require(_auditorAddress != address(0));
        require(_auditorJsonHash != 0);
        
        auditLogs[auditIndex].dataId = _dataId;
        auditLogs[auditIndex].auditorAddress = _auditorAddress;
        auditLogs[auditIndex].auditorJsonHash = _auditorJsonHash;
        auditIndex = auditIndex + 1;
    }

    function getAccessIndex () public view returns (uint) {
        return accessIndex;
    }    
    
    function getAuditIndex () public view returns (uint) {
        return auditIndex;
    }

    // index starts with 0
    function getAccessDataByIndex (uint index) public view returns (uint256, string memory, string memory) {
        return (accessLogs[index].dataId, accessLogs[index].userId, accessLogs[index].purpose);
    }
    
    function getAuditDataByIndex (uint index) public view returns (uint256, address, uint256) {
        return (auditLogs[index].dataId, auditLogs[index].auditorAddress, auditLogs[index].auditorJsonHash);
    }
}