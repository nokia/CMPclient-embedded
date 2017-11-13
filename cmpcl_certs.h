/*
 * Copyright (c) 2016-2017, Nokia, All rights reserved.
 */

/* propietary certificates which need to be updated according to used
   PKI hierarchies */

#ifndef CMPCL_CERTS_H
#define CMPCL_CERTS_H

#ifdef __cplusplus
extern "C" {
#endif

/* "factory-provisioned" device cert used as in RFC 4210 E.7 as external
   identity certificate for IR */
const unsigned char* vendor_cert_pem = (const unsigned char*)
"-----BEGIN CERTIFICATE-----\n"
"MIIBpTCCAUmgAwIBAgICHjQwDAYIKoZIzj0EAwIFADAqMRcwFQYDVQQDEw5GYWN0\n"
"b3J5IFN1YiBDQTEPMA0GA1UEChMGVmVuZG9yMB4XDTE3MDcxODIyNTMwNVoXDTMy\n"
"MDcxNzIzMjMwNVowRTEPMA0GA1UEAxMGRGV2aWNlMRMwEQYDVQQFEwowMDAwMDAw\n"
"MDAxMQ4wDAYDVQQKEwVOb2tpYTENMAsGA1UECxMEVGVzdDBZMBMGByqGSM49AgEG\n"
"CCqGSM49AwEHA0IABPiq03QgP0XE93KlBE/sNw4OTD6gxnu9BeHfh8p8z/jgQ/0k\n"
"F3W2h93Q5xEh0RiTz1KRrChE0zJQsBy0rNqWpn+jQjBAMB8GA1UdIwQYMBaAFB5B\n"
"Uy8WDClH+ypAPxLx8Tmc2/XuMB0GA1UdDgQWBBRUw0/YajVmwf6FDErNBshvrbZ9\n"
"oDAMBggqhkjOPQQDAgUAA0gAMEUCIQDd/2gfPLVX7Oel9thQzddKnAvRoOqe8wX/\n"
"mvSK3z8Y5QIgKEcEhdHpd20PPh0dZ4LLTAtEu66i+rJS7Ak96dYkxu8=\n"
"-----END CERTIFICATE-----\n";

/* "factory-provisioned" device key */
const unsigned char* vendor_cert_key_pem = (const unsigned char*)
"-----BEGIN EC PARAMETERS-----\n"
"BggqhkjOPQMBBw==\n"
"-----END EC PARAMETERS-----\n"
"-----BEGIN EC PRIVATE KEY-----\n"
"MHcCAQEEIFtKvCOn+MnNLyFdAhoRhyb987YmTHio6+kLdnzOyX9hoAoGCCqGSM49\n"
"AwEHoUQDQgAE+KrTdCA/RcT3cqUET+w3Dg5MPqDGe70F4d+HynzP+OBD/SQXdbaH\n"
"3dDnESHRGJPPUpGsKETTMlCwHLSs2pamfw==\n"
"-----END EC PRIVATE KEY-----\n";

/* vendor sub CA which issued the device certificate */
const unsigned char* vendor_sub_ca_cert_pem = (const unsigned char*)
"-----BEGIN CERTIFICATE-----\n"
"MIIBwTCCAWagAwIBAgICEd8wDAYIKoZIzj0EAwIFADArMRgwFgYDVQQDEw9GYWN0\n"
"b3J5IFJvb3QgQ0ExDzANBgNVBAoTBlZlbmRvcjAeFw0xNzA3MTgyMTM0MzZaFw0z\n"
"NzA3MTcyMTM0MzZaMCoxFzAVBgNVBAMTDkZhY3RvcnkgU3ViIENBMQ8wDQYDVQQK\n"
"EwZWZW5kb3IwWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAATvVtYfXt27XW5wlAoJ\n"
"0yTMRNrEBG04HwAerFzgIUErgPyxUJsPzVvfG1N00gObnYwXW6VbdEgzlFpSrs29\n"
"Zldlo3kwdzAfBgNVHSMEGDAWgBS4bBXHMYYCLJ7rC98l24/tzWhztjAdBgNVHQ4E\n"
"FgQUHkFTLxYMKUf7KkA/EvHxOZzb9e4wDgYDVR0PAQH/BAQDAgHGMBEGA1UdIAQK\n"
"MAgwBgYEVR0gADASBgNVHRMBAf8ECDAGAQH/AgEAMAwGCCqGSM49BAMCBQADRwAw\n"
"RAIgIUg2v2kcWUDHEKoyKjjR2frHNzP7O/lTENbEbJuwpHACIEkIaa8B5AjCF4Wq\n"
"3sCRoQMjJk273FA1SC99nBCN0vd7\n"
"-----END CERTIFICATE-----\n";

#if 0
/* vendor root CA which issued the device certificate */
/* not really needed by the CMP client, as it doesn't get included into
   extracerts */
const unsigned char* vendor_ca_cert_pem = (const unsigned char*)
"-----BEGIN CERTIFICATE-----\n"
"MIIBxDCCAWegAwIBAgICAwQwDAYIKoZIzj0EAwIFADArMRgwFgYDVQQDEw9GYWN0\n"
"b3J5IFJvb3QgQ0ExDzANBgNVBAoTBlZlbmRvcjAeFw0xNzA3MTgyMTIyMDlaFw00\n"
"NzA3MTgyMTIyMDlaMCsxGDAWBgNVBAMTD0ZhY3RvcnkgUm9vdCBDQTEPMA0GA1UE\n"
"ChMGVmVuZG9yMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEiwJvN3gRmXJPkD1d\n"
"au8SZ43laoGNuCdWyE7qjU1IPrO/ODARac9x6aNiH4VcuFsP5QgDBMMPAAjHCYM1\n"
"z79yMaN5MHcwHwYDVR0jBBgwFoAUuGwVxzGGAiye6wvfJduP7c1oc7YwHQYDVR0O\n"
"BBYEFLhsFccxhgIsnusL3yXbj+3NaHO2MA4GA1UdDwEB/wQEAwIBxjARBgNVHSAE\n"
"CjAIMAYGBFUdIAAwEgYDVR0TAQH/BAgwBgEB/wIBATAMBggqhkjOPQQDAgUAA0kA\n"
"MEYCIQCVSX0KFl8p9mpdhQQQ6JrDNw1DM/Kkke7DDZ/m93jBcgIhAKXT8YICGiab\n"
"tUdvJVnMlwBfBP16SbVvJPiXnldhNduc\n"
"-----END CERTIFICATE-----\n";
#endif /* 0 */

/* sub CA which acting as CMP server will issue the device certificate */
const unsigned char* operator_sub_ca_cert_pem = (const unsigned char*)
"-----BEGIN CERTIFICATE-----\n"
"MIIB3TCCAYKgAwIBAgICBCgwDAYIKoZIzj0EAwIFADBEMR8wHQYDVQQDExZjbXBj\n"
"bGllbnQgdGVzdCBSb290IENBMQ4wDAYDVQQKEwVOb2tpYTERMA8GA1UECxMITmV0\n"
"R3VhcmQwHhcNMTcxMjIwMTEyNjQxWhcNNDIxMjIwMTEyNjQxWjBDMR4wHAYDVQQD\n"
"ExVjbXBjbGllbnQgdGVzdCBTdWIgQ0ExDjAMBgNVBAoTBU5va2lhMREwDwYDVQQL\n"
"EwhOZXRHdWFyZDBZMBMGByqGSM49AgEGCCqGSM49AwEHA0IABACnZ+S0JvfzHtAU\n"
"vr+aBQNk7Cyh09UzBJkKsTYsJ8L5A5qzbSoZ7u3SNCippKSMNnhnG/faNn1Z+Om4\n"
"L7PeYZCjYzBhMB8GA1UdIwQYMBaAFMSx2S9UI86TcFNzC79BPmppFzNPMB0GA1Ud\n"
"DgQWBBRbYPL9s4+M0I24q6YYvljfOyseCDAOBgNVHQ8BAf8EBAMCAYYwDwYDVR0T\n"
"AQH/BAUwAwEB/zAMBggqhkjOPQQDAgUAA0cAMEQCIHahOtvllfNm2ISXwyJMHAe6\n"
"g3To7GoLwJUNiNhIl07aAiAN/ROG9tbw1//mswNTXLMn1Urq9QJFxc1nYTbsJVTD\n"
"mg==\n"
"-----END CERTIFICATE-----\n";


#if 0
/* not really needed when the server cert is known */
/* root CA of the PKI which will issue the device certificate */
const unsigned char* operator_ca_cert_pem = (const unsigned char*)
"-----BEGIN CERTIFICATE-----\n"
"MIIB3zCCAYOgAwIBAgICBCcwDAYIKoZIzj0EAwIFADBEMR8wHQYDVQQDExZjbXBj\n"
"bGllbnQgdGVzdCBSb290IENBMQ4wDAYDVQQKEwVOb2tpYTERMA8GA1UECxMITmV0\n"
"R3VhcmQwHhcNMTcxMjIwMTEyNDI3WhcNNDcxMjIwMTEyNDI3WjBEMR8wHQYDVQQD\n"
"ExZjbXBjbGllbnQgdGVzdCBSb290IENBMQ4wDAYDVQQKEwVOb2tpYTERMA8GA1UE\n"
"CxMITmV0R3VhcmQwWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAARtfyIZAsFGPmfA\n"
"L3DE3S98tFJLcVQmIDr2fT+RfLemQ8BhmstyA5dfw/HUQAZtf9lzfPjCYOszlxCS\n"
"nmHR5w4+o2MwYTAfBgNVHSMEGDAWgBTEsdkvVCPOk3BTcwu/QT5qaRczTzAdBgNV\n"
"HQ4EFgQUxLHZL1QjzpNwU3MLv0E+amkXM08wDgYDVR0PAQH/BAQDAgGGMA8GA1Ud\n"
"EwEB/wQFMAMBAf8wDAYIKoZIzj0EAwIFAANIADBFAiEA0F5VntCd+gTC517gOoNy\n"
"FgkzkjIGMwr45bwLclC75PMCICEoDK8MEQ7CzvNZ1QLZObFcB1OpSH3ON9T3Srjq\n"
"nZpv\n"
"-----END CERTIFICATE-----\n";

#endif /* 0 */

#ifdef __cplusplus
}
#endif

#endif /* CMPCL_CERTS_H */
