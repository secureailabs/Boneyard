pragma solidity ^0.6.1;

contract RegisterEnclaveAccess {
	
	struct accessAdministration{
	uint256 enclaveId;
	string userId;
	string purpose;
	}
    
    struct auditAdministration{
    uint256 enclaveId;
    address auditorAddress;
    uint256 auditorJsonHash;
    }

    uint256 accessIndex;
    mapping(uint256 => accessAdministration) accessLogs;
    
    uint256 auditIndex;
    mapping(uint256 => auditAdministration) auditLogs;

    /// @dev registration by creator of enclave.
    function registerEnclaveAccess (uint256 _enclaveId, string memory _userId, string memory _purpose) public {
        require(_enclaveId >= 0);
        require(bytes(_userId).length > 0);
        require(bytes(_purpose).length > 0);
        
        accessLogs[accessIndex].enclaveId = _enclaveId;
        accessLogs[accessIndex].userId = _userId;
        accessLogs[accessIndex].purpose = _purpose;
        accessIndex = accessIndex + 1;
    }
    
    function registerEnclaveAudit (uint256 _enclaveId, address _auditorAddress, uint256 _auditorJsonHash) public {
        require(_enclaveId >= 0);
        require(_auditorAddress != address(0));
        require(_auditorJsonHash != 0);
        
        auditLogs[auditIndex].enclaveId = _enclaveId;
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
    function getEnclaveAccessByIndex (uint index) public view returns (uint256, string memory, string memory) {
        return (accessLogs[index].enclaveId, accessLogs[index].userId, accessLogs[index].purpose);
    }
    
    function getEnclaveAuditByIndex (uint index) public view returns (uint256, address, uint256) {
        return (auditLogs[index].enclaveId, auditLogs[index].auditorAddress, auditLogs[index].auditorJsonHash);
    }
}