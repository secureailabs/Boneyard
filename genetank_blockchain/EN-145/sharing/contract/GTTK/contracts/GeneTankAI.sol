pragma solidity ^0.4.18;
import "./GeneTankStructs.sol";
import "./SafeMath.sol";
import "./GTTToken.sol";

contract GeneTankAI {

    using SafeMath for uint256;

    address public owner;
    uint timedOut;
    uint PRICE;
    uint FRACTIONTOKENDONOR;

    /// @dev only allow owner
    modifier OwnerOnly() {
        require(msg.sender == owner);
        _;
    }

    /// @dev constructor
    constructor (uint access_code_timeout) public {
        owner = msg.sender;
        timedOut = access_code_timeout;

        // Cannot be changed better to have
        PRICE = 5;
        FRACTIONTOKENDONOR = 2;
    }

    /// @dev the owner of the contract is allowed to change the price
    function modifyPrice(uint amount) OwnerOnly public {
       PRICE = amount;
    }

    uint256 creator_index;
    mapping(uint256 => address) creator_address;
    mapping (address => GeneTankStructs.EnclavePackage) creators;

    //
    // ----------------------- Registration ------------------------------------
    //

    function registerDataOwner (uint256 hash_data) public {

          GeneTankStructs.DataRegistration storage data_owner = data_participants[msg.sender];

       //   assert(data_owner.data_owner_addr == 0);
       //   assert(data_owner.timestamp == 0);

            data_owner.hash_data_json[data_owner.data_index] = hash_data;
            data_owner.data_index += 1;
      }

    function getTotalDataOwner (address data_owner_address) public view returns (uint) {
       GeneTankStructs.DataRegistration storage data_owner = data_participants[data_owner_address];
       return data_owner.data_index;
   }

   function getDataOwnerByIndex (address data_owner_address, uint index) public view returns (uint) {
       GeneTankStructs.DataRegistration storage data_owner = data_participants[data_owner_address];
       return data_owner.hash_data_json[index];
   }


    /// @dev registration by creator of enclave.
    /// @notice Usage of assert is because it consumes more gas (attacker cannot change)
    function registerEnclaveByCreator  (  uint256 mrenclave,
                                          uint256 hash_enclave_json
                                        ) public {

        require(hash_enclave_json != 0);
        require(mrenclave != 0);

        creator_index = creator_index + 1;
        creator_address[creator_index] = msg.sender;

        // set the enclave information
        creators[msg.sender].mrenclave = mrenclave;
        creators[msg.sender].hash_package_json = hash_enclave_json;
    }

    uint256 trainer_index;
    mapping(uint256 => address) trainer_address;
    mapping (address => GeneTankStructs.EnclaveTrainer) trainers;

     /// @dev registration by trainer who has already trained a model
    /// @notice only trainer should be able to fill out the trainer details
    function registerModelByTrainer ( uint256 hash_model_json,
                                      uint256 fraction_token_to_allocate_donor
                                    ) public {

         // ensure that the fraction is not too large else
        require(fraction_token_to_allocate_donor < FRACTIONTOKENDONOR);
        require(hash_model_json != 0);
        require(trainers[msg.sender].hash_model_json == 0);

        trainer_address[trainer_index] = msg.sender;
        trainer_index = trainer_index + 1;


        //trainers[msg.sender].timestamp = now;
        trainers[msg.sender].hash_model_json = hash_model_json;
        //trainers[msg.sender].fraction_donors_token = fraction_token_to_allocate_donor;

       // return trainer_index;
    }

    // @dev Get the index of the trainer
    function getTotalTrainerIndex() public view returns(uint256) {
        return trainer_index;
    }

    function getTrainerByIndex (uint256 index) public view returns (uint256 hash_runner_json, address trainer_addr) {
        GeneTankStructs.EnclaveTrainer storage enclave_trainer = trainers[trainer_address[index]];
        return (enclave_trainer.hash_model_json, trainer_address[index]);
    }

    /// @dev registration by trainer who has already trained a model
    /// @notice only trainer should be able to fill out the trainer details
    function registerEnclaveRunnerByTrainer (address runner_address,
                                        uint256 hash_runner_json,
                                        string public_key
                                         ) public {

         // ensure that the fraction is not too large else
        require(hash_runner_json != 0);
        require(bytes(public_key).length != 0);
        // require(trainers[msg.sender].timestamp != 0);
        require(trainers[msg.sender].hash_model_json != 0);
        require(trainers[msg.sender].runners[runner_address].hash_runner_json == 0);
        //require(trainers[msg.sender].fraction_donors_token != 0);

        trainers[msg.sender].runner_address[trainers[msg.sender].runner_index] = runner_address;
        trainers[msg.sender].runner_index += 1;

        trainers[msg.sender].runners[runner_address].hash_runner_json = hash_runner_json;
        trainers[msg.sender].runners[runner_address].public_key = public_key;

    }

    function getTotalEnclaveIndexByTrainerAddress (address trainer_addr) public view returns (uint256 index) {
        return trainers[trainer_addr].runner_index;
    }

    function getEnclaveByTrainerAddressAndEnclaveIndex(address trainer_addr, uint256 index) public view returns (uint256 hash_runner_json, string public_key) {
        GeneTankStructs.EnclaveRunner storage enclave_runner = trainers[trainer_addr].runners[trainers[trainer_addr].runner_address[index]];
        return (enclave_runner.hash_runner_json, enclave_runner.public_key);
    }


    //verify trainers[trainer_address].runners[msg.sender].hash_runner_json is valid

    /// @dev Register the auditor details

    function registerReportByAuditor(  address creator_address,
                                        uint256 hash_json
                                        ) public returns(uint256 index) {

        require(creator_address != 0);
        require(hash_json != 0);

        // only if the name and version have not been set something wrong
        // required to have first registered the enclave
        require(creators[creator_address].hash_package_json != 0);
        require(creators[creator_address].mrenclave != 0);

        uint256 auditor_index = getAuditorIndex(creator_address);
        uint256 new_auditor_index = auditor_index + 1;

        creators[creator_address].auditor_index = new_auditor_index;
        creators[creator_address].auditor_address[new_auditor_index] = msg.sender;

        creators[creator_address].auditor_info[msg.sender].auditor_address = msg.sender;
        creators[creator_address].auditor_info[msg.sender].timestamp = now;
        return new_auditor_index;
    }


    /// @dev Runner will have the trainer do the attestation and place these in the struct
    function registerEnclaveByRunner(  address trainer_address,
                                       uint256 public_key,
                                       uint256 hash_runner_json) public {

        require(trainer_address != 0);
        require(hash_runner_json != 0);
        require(public_key != 0);

        // trainer needs to have filled the json. Timestamp already verified when registering trainer
        require(trainers[trainer_address].hash_model_json != 0);

        trainers[trainer_address].runners[msg.sender].hash_runner_json = hash_runner_json;
        trainers[trainer_address].runners[msg.sender].public_key = public_key;

    }

    /// @dev function to verify signature
    function verifySignature(bytes32 h, uint8 v, bytes32 r, bytes32 s) internal returns (address) {
        bytes memory prefix = "\x19Ethereum Signed Message:\n32";
        bytes32 prefixedHash = keccak256(prefix, h);
        address addr = ecrecover(prefixedHash, v, r, s);
        return addr;
    }

    uint256 data_participants_index;
    mapping(uint256 => address) data_participants_address;
    mapping (address => GeneTankStructs.DataRegistration) data_participants;
    /*
        In web3 the extraction of v,r,s:
         var r = `0x${sig.slice(0, 64)}`
         var s = `0x${sig.slice(64, 128)}`
         var v = web3.toDecimal(sig.slice(128, 130)) + 27
    */

    /// @dev Owners register their details
    function registerDataByOwner( uint256 hash_json
                                  /*  uint8 v,
                                    bytes32 r,
                                    bytes32 s,
                                    bytes32 message,
                                    uint256 hash_data, */
                                    ) public returns(uint256 index) {

      // @TODO verify the signature using ecrecover. If verification of signature failed skip

     //   address addr_sign = verifySignature(message, v, r, s);
     //   require(addr_sign == signature_address);
        require(hash_json != 0);
        uint256 data_index = getOwnerIndex(msg.sender);
        uint256 new_data_index = data_index + 1;
        data_participants[msg.sender].data_index = new_data_index;
        data_participants[msg.sender].hash_data_json[new_data_index] = hash_json;
        return new_data_index;
    }

  /*

    /// @dev Owner registers the verification report
    function registerVerificationReport ( uint256 hash_signature, address verifier_addr,
                                 uint256 hash_signed_verifier_report, uint8 score,
                                 uint256 hash_data) public {
        GeneTankStructs.DataRegistration storage data_owner = data_participants[hash_data];

        // if the owner address and the timestamp has not been set something is wrong
        assert(data_owner.data_owner_addr == msg.sender);
        assert(data_owner.timestamp != 0);

        // these should not have been set
        assert(data_owner.data_donor[hash_signed_verifier_report].data_donor == 0) ;
        assert(data_owner.data_donor[hash_signed_verifier_report].evaluation_score == 0);
        assert(data_owner.data_donor[hash_signed_verifier_report].hash_signature == 0);
        assert(data_owner.data_donor[hash_signed_verifier_report].timestamp == 0);
        assert( data_owner.data_donor[hash_signed_verifier_report].verifier == 0);

        data_owner.data_donor[hash_signed_verifier_report].data_donor = msg.sender;
        data_owner.data_donor[hash_signed_verifier_report].evaluation_score = score;
        data_owner.data_donor[hash_signed_verifier_report].hash_signature = hash_signature;
        data_owner.data_donor[hash_signed_verifier_report].timestamp = now;
        data_owner.data_donor[hash_signed_verifier_report].verifier = verifier_addr;
    }*/

    /// @dev First function: Owner becomes donor by registering the receipt
    function registerReceiptByDonor( address trainer_address
                                       /* address signature_address,
                                        uint8 v,
                                        bytes32 r,
                                        bytes32 s,
                                        bytes32 message,
                                        uint256 hash_data, */
                                        ) public returns(uint256 index) {

        //address addr_sign = verifySignature(message, v, r, s);
        //require(addr_sign == signature_address);

        require(trainer_address != 0);

        // We use now to get the time but also know that verification was successful
        // The miner could alter the value of the block and hack the verification
        require(now != 0);

        uint256 donor_index = getDonorIndex(trainer_address);
        uint256 new_donor_index = donor_index + 1;

        trainers[trainer_address].donor_index = new_donor_index;
        trainers[trainer_address].donors_address[new_donor_index] = msg.sender;

        trainers[trainer_address].donors_list[msg.sender].donor_address = msg.sender;
        trainers[trainer_address].donors_list[msg.sender].timestamp = now;

        return new_donor_index;
    }


    uint256 total_balances = 0;
    function registerAccessCode( address creator_address,
                                 address trainer_address,
                                 address runner_address,
                                 address user_address,
                                 address token_address,
                                 uint256 mrenclave,
                                 uint256 hash_user_data,
                                 uint256 access_code,
                                 uint256 amount ) payable public {

        /* need to hardcode main account
        address main_account;
        address user_account;
        gtoken.allowance(main_account, user_account);
        */

        require(creators[creator_address].hash_package_json != 0);

        // runner enclave will verify if this matches the one by the trainer
        /*creator.enclave_packages[mrenclave].
                trainers[trainer_address].
                runners[runner_address].
                user_info[msg.sender].
                access_code[hash_user_data].isValid = 1;

        creator.enclave_packages[mrenclave].
                trainers[trainer_address].
                runners[runner_address].
                user_info[msg.sender].
                access_code[hash_user_data].access_code = access_code;
          */
        GTTToken gtoken = GTTToken(token_address);
        // transfer the token from the user's account to the contract
        gtoken.transferFrom(user_address, this, amount);

        // store the balance that has not yet been redistributed
        total_balances = total_balances.add(amount);
    }

    /// @dev Will be executed by runner once it receives all the user's information

    /*function registerAccessCode(  	address creator_address,
                								address trainer_addr,
                								uint256 acccod,
                                                uint256 hash_user_input,
                                                uint256 mrenclave
                                       ) public {


        GeneTankStructs.EnclaveFamily storage creator = enclave_creators[creator_address];
     //   creator.enclave_packages[mrenclave].trainers[trainer_addr].runners[msg.sender].access_code[acccod].user_address = user_addr;
     //   creator.enclave_packages[mrenclave].trainers[trainer_addr].runners[msg.sender].access_code[acccod].hash_user_data = hash_user_input;
        creator.enclave_packages[mrenclave].trainers[trainer_addr].runners[msg.sender].access_code[ ].isValid = ;
    }*/

    // ----------------------- End Registration --------------------------------


    //
    // -------------------------- Getters --------------------------------------
    //

    // @dev Get the index of the auditor report
    function getAuditorIndex(address creator_address) internal returns(uint256 index) {
        return creators[creator_address].auditor_index;
    }

    // @dev Get the index of the owner
    function getOwnerIndex(address data_owner_address) internal returns(uint256 index) {
        return data_participants[data_owner_address].data_index;
    }

    // @dev Get the index of the donor
    function getDonorIndex(address trainer_address) internal returns(uint256 index) {
        return trainers[trainer_address].donor_index;
    }

    /// @dev Get the details about the enclave package
    function getEnclavePackage(address creator_address) view public
        returns( uint256 mrenclave,
                uint256 hash_json){

        return ( creators[creator_address].mrenclave, creators[creator_address].hash_package_json);
    }

    /// @dev Get the attestation report hash and signature hash from runner
    function getRunner(address trainer_address, address runner_address) view public
        returns(bool trainingOrNot,
                uint256 hash_json){

        return (trainers[trainer_address].runners[runner_address].is_training,
            trainers[trainer_address].runners[runner_address].hash_runner_json);
    }

    // -------------------------- End Getters --------------------------


    //
    // -------------------------- Payment --------------------------
    //

    /// @dev Transfer amount from user to contract

   /* function sendDepositAccessCode( address user_address,
                                    address token_address,
                                    uint256 user_token
                                     ) payable {

        // ensure that the price is the correct one
        require(user_token == PRICE);

        // instantiating the token contract
        GTTToken gtoken = GTTToken(token_address);

        // transfer tokens from user's account to contract
        gtoken.transferFrom(user_address, this, user_token);
    }*/

    /// @dev distribute tokens to the different different participants

    function distributeToken( uint256 amt_to_withdraw,
                            address runner_address,
                            address creator_address,
                            address trainer_address,
                            address token_address,
                            uint256 mrenclave )
                                 internal returns (bool) {
        // determine if the token the user has is more than the deposit
        if(amt_to_withdraw > owner.balance) {
            return false;
        }
        uint256 total_fraction = 3 + trainers[trainer_address].fraction_donors_token;

        // balances[owner] = balances[owner].sub(amt_to_withdraw);

        uint256 token = amt_to_withdraw.div( total_fraction );
        uint256 trainer_token = amt_to_withdraw.sub(token.add(token));

        GTTToken gtoken = GTTToken(token_address);
        // transfer the token
        gtoken.transferFrom(this, runner_address, token);
        gtoken.transferFrom(this, creator_address, token);
        gtoken.transferFrom(this, trainer_address, trainer_token);

        // subtract amount except donor. Will need this for donor to be able to withdraw amount already transferred
        total_balances = total_balances.sub(amt_to_withdraw);
        // rest goes to donor;
        return true;
    }

    // @TODO donor withdraws own token
    function withdrawTokenByDonor(  address creator_address,
                                    address trainer_address,
                                    address token_address,
                                    uint256 mrenclave,
                                    uint256 amt_claimed ) public {




   /*     uint256 total_donors = enclave_creators[creator_address].
                                    enclave_packages[mrenclave].
                                        trainers[trainer_address].
                                         total_donors;

        uint256 total_donor_amt = this.balance.div(total_donors);
        require(total_donor_amt > amt_claimed);

        enclave_creators[creator_address].enclave_packages[mrenclave].trainers[trainer_address].
                list_donors[msg.sender].amount_claimed = amt_claimed;

        GTTToken gtoken = GTTToken(token_address);

        // @TODO need to verify if successful then return true
        gtoken.transferFrom(this, msg.sender, amt_claimed);
      */
    }

   /*  function setDonorData(  address creator_address,
                            address trainer_address,
                            address donor_address,
                            uint256 mrenclave,
                            uint256 hash_data ) private {

        GeneTankStructs.DataRegistration storage data_owner = data_participants[donor_address];

        // if timestamp set then validation is completed
        require(enclave_creators[creator_address].enclave_packages[mrenclave].trainers[trainer_address].
                list_donors[donor_address].timestamp != 0);

        enclave_creators[creator_address].enclave_packages[mrenclave].trainers[trainer_address].
                list_donors[donor_address].weight = data_owner.verification_details[hash_data].weight;

    }*/

	/// @dev Cannot be called externally. runner_addr needs to be passed msg.sender
	function setConfirmationAddress( address creator_addr,
	                                 address trainer_addr,
	                                 address user_addr,
	                                 address runner_addr,
	                                 uint256 mrenclave,
	                                 uint256 accesscode ) private {

	    // set the runner as the one confirming
    /*    enclave_creators[creator_addr].
         enclave_packages[mrenclave].trainers[trainer_addr].
          runners[runner_addr].user_info[user_addr].
           access_code[accesscode].
            confirmed_address = runner_addr;
            */
	}

	/// @dev user confirms after receipt of prediction result
	function confirmationByUser( address creator_addr,
	                             address trainer_addr,
	                             address runner_addr,
	                             uint256 mrenclave,
	                             uint256 accesscode) public {

	    // set the user as the one confirming
        /*enclave_creators[creator_addr].
         enclave_packages[mrenclave].trainers[trainer_addr].
          runners[runner_addr].user_info[msg.sender].
           access_code[accesscode].
            confirmed_address = msg.sender;*/
	}

	/// @dev runner confirms after expiry
	function confirmationByRunner ( uint256 amt_to_withdraw,
	                  address creator_address,
	                  address runner_address,
	                  address trainer_address,
	                  address user_address,
	                  address token_address,
	                  uint256 mrenclave,
                      uint256 accesscode ) public returns (bool) {

	   // check that the userdata relates to the creator,runner, trainer

        // store whether confirmed or not
      /*  address is_confirmed = enclave_creators[creator_address].
                enclave_packages[mrenclave].trainers[trainer_address].
                    runners[runner_address].user_info[user_address].
                        access_code[accesscode].
                            confirmed_address;

        // if address has been set distribute token
        if (is_confirmed == 0) {
            distributeToken( amt_to_withdraw,
                             runner_address,
                             creator_address,
                             trainer_address,
                             token_address,
                             mrenclave);
        } else if (is_confirmed != 0) {
            uint when_code_paid = enclave_creators[creator_address].
                enclave_packages[mrenclave].trainers[trainer_address].
                    runners[runner_address].user_info[user_address].
                        access_code[accesscode].
                            validated_time;

            if( now > when_code_paid.add(timedOut)) { // timed out

                // after time out the runner is allowed to set confirmation
                setConfirmationAddress( creator_address,
	                                trainer_address,
	                                user_address,
	                                msg.sender,
	                                mrenclave,
	                                accesscode);
                return true;
            }
        }
        return false;*/
    }


     /// @dev to cancel the access code need to invalidate the access code

    function cancelAccessCode(  address creator_address,
                                address runner_address,
                                address trainer_address,
                                address user_address,
                                address token_address,
                                uint256 mrenclave,
                                uint256 accesscode
                                )  public returns (bool) {

     /*   address is_confirmed = enclave_creators[creator_address].
            enclave_packages[mrenclave].trainers[trainer_address].
                runners[runner_address].user_info[user_address].
                    access_code[accesscode].confirmed_address;

        if( is_confirmed != 0 ) {
            enclave_creators[creator_address].
                enclave_packages[mrenclave].trainers[trainer_address].
                    runners[runner_address].user_info[user_address].
                        access_code[accesscode].confirmed_address = 0;

            // Upon cancellation allow only 50% refund
            uint256 refund_amt = PRICE / 2;

            GTTToken gtoken = GTTToken(token_address);

            // @TODO need to verify if successful then return true
            gtoken.transferFrom(this, user_address, refund_amt);

            return true;
        }

        return false;
                */

    }

} // end contract
