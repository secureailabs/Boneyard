pragma solidity ^0.4.18;

library GeneTankStructs {

    /*
    An "owner address" will initialize this struct
    */
    struct DataRegistration {
        /*
        One address maps to one hash of json data.
        Need to create many addresses for each json data
        */
        // key is an index which increases by one to a hash of json
        uint256 data_index;
        mapping(uint256 => uint256) hash_data_json;
    }
   
    /*
    A "creator address" will initialize this struct
    */
    struct EnclavePackage {
        /*
        The verifier and trainer will need to sign receipt/reports
        Instead of having public_key inside the verifier struct or trainer, we
        could have only one here since each enclave will be either a verifier 
        or a trainer
        */
       
        // we are assuming one ethereum address one enclave
        //uint256 mrenclave;
        uint256 hash_package_json;
        
        /*Not in Phase II
        Each verifier will set a bool to false or true. True means registered
        mapping(address => bool) verifiers;    
        */
        uint256 auditor_index;
        mapping(uint256 => address) auditor_address;
        //mapping(address => Auditor) auditor_info;
        mapping(address => uint256) hash_auditor_json;
    }

    /* All the information in the Auditor record can be in the info json */
    /* So the Auditor structure is not used */
    struct Auditor {
        address auditor_address;
        uint256 timestamp;
        uint256 hash_auditor_json;
    }
    
    
    struct EnclaveTrainer {
        /*
        timestamp is the only field that an untrained trainer will have to fill.
        A trained trainer will have to check if this value is set before 
        proceeding to fill out the remaining fields in this struct
        */
        /* We don't distinguish trained and untrained trainer anymore */
        //uint256 timestamp;
        
        uint256 hash_model_json;
        
        /*
        Each trainer could have some number of donors' data. In our case they will
        be hospitals or pharmaceutical companies. We are assuming this will be 
        a small number and easy to iterate. We are using a private blockchain
        so looping over a small array might be ok.
        Before we had a mapping because there could be millions of donors, and 
        should not loop over each
        */
        uint256 donor_index;
        mapping(uint256 => address) donors_address;
        mapping(address => DataDonation) donors_list;
        
        uint256 runner_index;
        mapping(uint256 => address) runner_address;
        mapping(address => EnclaveRunner) runners;

        uint256 auditor_index;
        mapping(uint256 => address) auditor_address;
        mapping(address => uint256) hash_auditor_json;

        // max_token_per_donor - donor.amount_claimed is the amount that the
        // donor can claim.
        uint256 max_token_per_donor;
        
        // this needs to be set by the trainer. lower means the lower the number
        // of tokens donor will get could be a fraction e.g. 0.5
        // The value divided by 1000 is the fraction
        uint32 fraction_donors_token;
        uint32 fraction_runner_token;
    }
    
    struct DataDonation {
        // if a donor decides to remove himself, set this to false
        bool is_still_donor;

        //address donor_address;

        //The index of the donated data under the the data donor structure
        //uint256 data_index; delete as it can be in the info json

        uint256 data_donor_json;
        
        // time when owner becomes donor
        // uint256 timestamp; It seems the block has the timestamp
        
        // each donor get a predefined amount of token
        // when the withdraw x amt of token need to add that to amt_claimed
        uint256 amount_claimed;
        
        //uint256 donors_token_left;
    }

    struct EnclaveRunner { // EnclaveInstance
        uint256 hash_runner_json;
        //bool is_training;

        // we need the public key to do verification inside the smartcontract
        uint256 public_key;
        
        // should points to the one who created the enclave (EnclavePackage)
        //address enclave_creator; delete as it can be in the info json
        
        /*
            Each user i.e. someone asking for prediction e.g. would need to know
            the runner's address and fill the below structs
        */
        uint256 access_code_index;
        mapping(uint256 => AccessCode) access_code;
    }
    
    /*
        Is the user going to pay for the accesscode using GTT token?
        We said the user should not be using GTT token  and it should be something
        internal. Do we know how the payment system will work? The data DataDonation
        and access code have been changing a few times and we are unsure how the 
        payment will work out.
        Let's say the user pays using stripe. From stripe's api we will know that
        user paid. We get that notification and then we call a function that will
        say how much user paid e.g. 5 dollars. Then we use that 5 and store it in 
        a variable. Could we use something like this instead?
    */
    
    struct AccessCode {
        address user_address;
        uint256 amount;
        /* We don't support transaction confirmation anymore 
        bool isValid; 
        uint256 validated_time; // in seconds
        address confirmed_address; // rcu : runner, creator, user
        */
    }
}
