pragma solidity ^0.6.1;

contract RegisterModelAccess {
	
	struct accessAdministration{
	uint256 modelId;
	string userId;
	string purpose;
	}
    
    struct auditAdministration{
    uint256 modelId;
    address auditorAddress;
    uint256 auditorJsonHash;
    }

    uint256 accessIndex;
    mapping(uint256 => accessAdministration) accessLogs;
    
    uint256 auditIndex;
    mapping(uint256 => auditAdministration) auditLogs;

    /// @dev registration by creator of enclave.
    function registerModelAccess (uint256 _modelId, string memory _userId, string memory _purpose) public {
        require(_modelId >= 0);
        require(bytes(_userId).length > 0);
        require(bytes(_purpose).length > 0);
        
        accessLogs[accessIndex].modelId = _modelId;
        accessLogs[accessIndex].userId = _userId;
        accessLogs[accessIndex].purpose = _purpose;
        accessIndex = accessIndex + 1;
    }
    
    function registerModelAudit (uint256 _modelId, address _auditorAddress, uint256 _auditorJsonHash) public {
        require(_modelId >= 0);
        require(_auditorAddress != address(0));
        require(_auditorJsonHash != 0);
        
        auditLogs[auditIndex].modelId = _modelId;
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
    function getModelAccessByIndex (uint index) public view returns (uint256, string memory, string memory) {
        return (accessLogs[index].modelId, accessLogs[index].userId, accessLogs[index].purpose);
    }
    
    function getModelAuditByIndex (uint index) public view returns (uint256, address, uint256) {
        return (auditLogs[index].modelId, auditLogs[index].auditorAddress, auditLogs[index].auditorJsonHash);
    }
}