#pragma once


#define SAMPLE_MAC_SIZE             16  // Message Authentication Code
// - 16 bytes
typedef uint8_t                     sample_mac_t[SAMPLE_MAC_SIZE];

#ifndef SAMPLE_FEBITSIZE
#define SAMPLE_FEBITSIZE        256
#endif

#define SAMPLE_NISTP256_KEY_SIZE    (SAMPLE_FEBITSIZE/ 8 /sizeof(uint32_t))

typedef struct sample_ec_sign256_t
{
	uint32_t x[SAMPLE_NISTP256_KEY_SIZE];
	uint32_t y[SAMPLE_NISTP256_KEY_SIZE];
} sample_ec_sign256_t;

#pragma pack(push,1)

#define SAMPLE_SP_TAG_SIZE          16

typedef struct sp_aes_gcm_data_t {
	uint32_t        payload_size;       //  0: Size of the payload which is
										//     encrypted
	uint8_t         reserved[12];       //  4: Reserved bits
	uint8_t	        payload_tag[SAMPLE_SP_TAG_SIZE];
	// 16: AES-GMAC of the plain text,
	//     payload, and the sizes
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning ( disable:4200 )
#endif
	uint8_t         payload[];          // 32: Ciphertext of the payload
										//     followed by the plain text
#ifdef _MSC_VER
#pragma warning(pop)
#endif
} sp_aes_gcm_data_t;


#define ISVSVN_SIZE 2
#define PSDA_SVN_SIZE 4
#define GID_SIZE 4
#define PSVN_SIZE 18

#define SAMPLE_PLATFORM_INFO_SIZE 101
typedef struct _ias_platform_info_blob_t
{
	uint8_t platform_info[SAMPLE_PLATFORM_INFO_SIZE];
} ias_platform_info_blob_t;


typedef struct sample_ra_att_result_msg_t {
	ias_platform_info_blob_t    platform_info_blob;
	sample_mac_t                mac;    // mac_smk(attestation_status)
	sp_aes_gcm_data_t           secret;
} sample_ra_att_result_msg_t;


#pragma pack(pop)

typedef enum {
	IAS_QUOTE_OK,
	IAS_QUOTE_SIGNATURE_INVALID,
	IAS_QUOTE_GROUP_REVOKED,
	IAS_QUOTE_SIGNATURE_REVOKED,
	IAS_QUOTE_KEY_REVOKED,
	IAS_QUOTE_SIGRL_VERSION_MISMATCH,
	IAS_QUOTE_GROUP_OUT_OF_DATE,
} ias_quote_status_t;

typedef enum {
	IAS_PSE_OK,
	IAS_PSE_DESC_TYPE_NOT_SUPPORTED,
	IAS_PSE_ISVSVN_OUT_OF_DATE,
	IAS_PSE_MISCSELECT_INVALID,
	IAS_PSE_ATTRIBUTES_INVALID,
	IAS_PSE_MRSIGNER_INVALID,
	IAS_PS_HW_GID_REVOKED,
	IAS_PS_HW_PRIVKEY_RLVER_MISMATCH,
	IAS_PS_HW_SIG_RLVER_MISMATCH,
	IAS_PS_HW_CA_ID_INVALID,
	IAS_PS_HW_SEC_INFO_INVALID,
	IAS_PS_HW_PSDA_SVN_OUT_OF_DATE,
} ias_pse_status_t;

// Revocation Reasons from RFC5280
typedef enum {
	IAS_REVOC_REASON_NONE,
	IAS_REVOC_REASON_KEY_COMPROMISE,
	IAS_REVOC_REASON_CA_COMPROMISED,
	IAS_REVOC_REASON_SUPERCEDED,
	IAS_REVOC_REASON_CESSATION_OF_OPERATION,
	IAS_REVOC_REASON_CERTIFICATE_HOLD,
	IAS_REVOC_REASON_PRIVILEGE_WITHDRAWN,
	IAS_REVOC_REASON_AA_COMPROMISE,
} ias_revoc_reason_t;

// These status should align with the definition in IAS API spec
#define ISVSVN_SIZE         2
#define PSDA_SVN_SIZE       4
#define GID_SIZE            4
#define PSVN_SIZE           18

#define SAMPLE_HASH_SIZE    32  // SHA256
#define SAMPLE_MAC_SIZE     16  // Message Authentication Code
// - 16 bytes

#define SAMPLE_REPORT_DATA_SIZE         64

typedef uint8_t             sample_measurement_t[SAMPLE_HASH_SIZE];
typedef uint8_t             sample_mac_t[SAMPLE_MAC_SIZE];
typedef uint8_t             sample_report_data_t[SAMPLE_REPORT_DATA_SIZE];
typedef uint16_t            sample_prod_id_t;

#define SAMPLE_CPUSVN_SIZE  16

typedef uint8_t             sample_cpu_svn_t[SAMPLE_CPUSVN_SIZE];
typedef uint16_t            sample_isv_svn_t;

typedef struct sample_attributes_t
{
	uint64_t                flags;
	uint64_t                xfrm;
} sample_attributes_t;

typedef struct sample_report_body_t {
	sample_cpu_svn_t        cpu_svn;        // (  0) Security Version of the CPU
	uint8_t                 reserved1[32];  // ( 16)
	sample_attributes_t     attributes;     // ( 48) Any special Capabilities
											//       the Enclave possess
	sample_measurement_t    mr_enclave;     // ( 64) The value of the enclave's
											//       ENCLAVE measurement
	uint8_t                 reserved2[32];  // ( 96)
	sample_measurement_t    mr_signer;      // (128) The value of the enclave's
											//       SIGNER measurement
	uint8_t                 reserved3[32];  // (160)
	sample_measurement_t    mr_reserved1;   // (192)
	sample_measurement_t    mr_reserved2;   // (224)
	sample_prod_id_t        isv_prod_id;    // (256) Product ID of the Enclave
	sample_isv_svn_t        isv_svn;        // (258) Security Version of the
											//       Enclave
	uint8_t                 reserved4[60];  // (260)
	sample_report_data_t    report_data;    // (320) Data provided by the user
} sample_report_body_t;

#pragma pack(push, 1)

typedef struct _ias_att_report_t
{
	uint32_t                id;
	ias_quote_status_t      status;
	uint32_t                revocation_reason;
	ias_platform_info_blob_t    info_blob;
	ias_pse_status_t        pse_status;
} ias_att_report_t;

typedef uint8_t epid_group_id_t[4];

typedef struct spid_t
{
	uint8_t                 id[16];
} spid_t;

typedef struct basename_t
{
	uint8_t                 name[32];
} basename_t;


typedef struct quote_nonce_t
{
	uint8_t                 rand[16];
} quote_nonce_t;

#define SAMPLE_QUOTE_UNLINKABLE_SIGNATURE 0
#define SAMPLE_QUOTE_LINKABLE_SIGNATURE   1

typedef struct quote_t {
	uint16_t                version;                // 0
	uint16_t                sign_type;              // 2
	epid_group_id_t  epid_group_id;          // 4
	sample_isv_svn_t        qe_svn;                 // 8
	sample_isv_svn_t        pce_svn;                // 10
	uint32_t                extended_epid_group_id; // 12
	basename_t       basename;               // 16
	sample_report_body_t    report_body;            // 48
	uint32_t                signature_len;          // 432
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning ( disable:4200 )
#endif
	uint8_t                 signature[];            // 436
#ifdef _MSC_VER
#pragma warning(pop)
#endif
} sample_quote_t;

#pragma pack(pop)

// Enum for all possible message types between the ISV app and
// the ISV SP. Requests and responses in the remote attestation
// sample.
typedef enum _ra_msg_type_t
{
	TYPE_RA_MSG0,
	TYPE_RA_MSG1,
	TYPE_RA_MSG2,
	TYPE_RA_MSG3,
	TYPE_RA_ATT_RESULT,
	TYPE_RA_SIMPLE,
	TYPE_RA_SRESP
}ra_msg_type_t;

// Enum for all possible message types between the SP and IAS.
// Network communication is not simulated in the remote
// attestation sample.  Currently these aren't used.
typedef enum _ias_msg_type_t
{
	TYPE_IAS_ENROLL,
	TYPE_IAS_GET_SIGRL,
	TYPE_IAS_SIGRL,
	TYPE_IAS_ATT_EVIDENCE,
	TYPE_IAS_ATT_RESULT,
}ias_msg_type_t;

#pragma pack(1)
typedef struct _ra_samp_request_header_t {
	uint8_t type;  // set to one of ra_msg_type_t
	uint32_t size; //size of request body,
	uint8_t align[3];
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning ( disable:4200 )
#endif
	uint8_t body[];
#ifdef _MSC_VER
#pragma warning(pop)
#endif
}ra_samp_request_header_t;

typedef struct _ra_samp_response_header_t {
	uint8_t type;   // set to one of ra_msg_type_t
	uint8_t status[2];
	uint32_t size;  //size of the response body
	uint8_t align[1];
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning ( disable:4200 )
#endif
	uint8_t body[];
#ifdef _MSC_VER
#pragma warning(pop)
#endif
}ra_samp_response_header_t;

#pragma pack()

typedef enum {
	SP_OK,
	SP_UNSUPPORTED_EXTENDED_EPID_GROUP,
	SP_INTEGRITY_FAILED,
	SP_QUOTE_VERIFICATION_FAILED,
	SP_IAS_FAILED,
	SP_INTERNAL_ERROR,
	SP_PROTOCOL_ERROR,
	SP_QUOTE_VERSION_ERROR,
} sp_ra_msg_status_t;


#pragma pack(push,1)

#define SAMPLE_SP_TAG_SIZE       16
#define SAMPLE_SP_IV_SIZE        12

#ifndef SAMPLE_FEBITSIZE
#define SAMPLE_FEBITSIZE                    256
#endif

#ifndef SAMPLE_ECP_KEY_SIZE
#define SAMPLE_ECP_KEY_SIZE                     (SAMPLE_FEBITSIZE/8)
#endif

typedef struct sample_ec_pub_t
{
	uint8_t gx[SAMPLE_ECP_KEY_SIZE];
	uint8_t gy[SAMPLE_ECP_KEY_SIZE];
} sample_ec_pub_t;

//fixed length to align with internal structure
typedef struct sample_ps_sec_prop_desc_t
{
	uint8_t  sample_ps_sec_prop_desc[256];
} sample_ps_sec_prop_desc_t;

#pragma pack(pop)

typedef uint32_t                sample_ra_context_t;

typedef uint8_t                 sample_key_128bit_t[16];

typedef sample_key_128bit_t     sample_ra_key_128_t;

typedef struct sample_ra_msg0_t
{
	uint32_t                    extended_epid_group_id;
} sample_ra_msg0_t;


typedef struct sample_ra_msg1_t
{
	sample_ec_pub_t             g_a;        // the Endian-ness of Ga is
											// Little-Endian
	epid_group_id_t      gid;        // the Endian-ness of GID is
											// Little-Endian
} sample_ra_msg1_t;

//Key Derivation Function ID : 0x0001  AES-CMAC Entropy Extraction and Key Expansion
const uint16_t SAMPLE_AES_CMAC_KDF_ID = 0x0001;

typedef struct sample_ra_msg2_t
{
	sample_ec_pub_t             g_b;        // the Endian-ness of Gb is
											// Little-Endian
	spid_t               spid;       // In little endian
	uint16_t                    quote_type; /* unlinkable Quote(0) or linkable Quote(0) in little endian*/
	uint16_t                    kdf_id;     /* key derivation function id in little endian.
											0x0001 for AES-CMAC Entropy Extraction and Key Derivation */
	sample_ec_sign256_t         sign_gb_ga; // In little endian
	sample_mac_t                mac;        // mac_smk(g_b||spid||quote_type||
											//         sign_gb_ga)
	uint32_t                    sig_rl_size;
#ifdef _MSC_VER
#pragma warning(push)
	// Disable warning that array payload has size 0
#ifdef __INTEL_COMPILER
#pragma warning ( disable:94 )
#else
#pragma warning ( disable: 4200 )
#endif
#endif
	uint8_t                     sig_rl[];
#ifdef _MSC_VER
#pragma warning(pop)
#endif
} sample_ra_msg2_t;

typedef struct sample_ra_msg3_t
{
	sample_mac_t                mac;        // mac_smk(g_a||ps_sec_prop||quote)
	sample_ec_pub_t             g_a;        // the Endian-ness of Ga is
											// Little-Endian
	sample_ps_sec_prop_desc_t   ps_sec_prop;
#ifdef _MSC_VER
#pragma warning(push)
	// Disable warning that array payload has size 0
#ifdef __INTEL_COMPILER
#pragma warning ( disable:94 )
#else
#pragma warning ( disable: 4200 )
#endif
#endif
	uint8_t                     quote[];
#ifdef _MSC_VER
#pragma warning(pop)
#endif
} sample_ra_msg3_t;

//For Ethereum account address
#define MAX_ADDR_SIZE 50