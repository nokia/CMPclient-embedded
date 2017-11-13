/*
 *  Copyright (c) 2016-2017, Nokia, All rights reserved.
 *
 *  This CMP client contains code derived from examples and documentation for
 *  mbedTLS by ARM
 *  Copyright (C) 2006-2017, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifdef __MBED__
#include "mbed.h"
#include "easy-connect.h"
#endif

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdlib.h>
#include <stdio.h>
#define mbedtls_free       free
#define mbedtls_calloc     calloc
#define mbedtls_snprintf   snprintf
#endif

#include <string.h>

#include "cmpcl.h"
#include "cmpcl_certs.h"

#include <mbedtls/x509_crt.h>
#include <mbedtls/asn1write.h>
#include <mbedtls/pk.h>

/* TODO: what needs to be done for entropy? */
#include <mbedtls/entropy.h>
#include <mbedtls/ctr_drbg.h> /* Random generator */

#ifdef __MBED__
/* Serial port */
Serial pc(USBTX, USBRX);
NetworkInterface* network;
#endif

/* **************************************************************** */
/* hardcoded settings TODO: make configurable by CLI */
/* key generation */
#define HARDCODED_ECCURVE "secp256r1" /* EC curve to use for key generation */
/* CMP server */
#define HARDCODED_SERVER_HOST "certifier.mynetwork"
/* only needed for PBM */
#define HARDCODED_CA_NAME "C=FI,ST=Uusimaa,L=Espoo,O=Nokia,OU=Security,CN=NetGuard Test CA"
#define HARDCODED_SECRET "9pp8-b35i-Xd3Q-udNR"
#define HARDCODED_REFERENCE "4787"

/* **************************************************************** */
/* HARDCODED CMP CTX-setting functions - TODO resolve eventually */
/* **************************************************************** */
static void cmp_ctx_init_hardcoded( cmp_ctx *ctx,
                                    mbedtls_ctr_drbg_context *ctr_drbg)
{
#ifdef __MBED__
    cmp_ctx_set_server_host( ctx, MBED_CONF_APP_CA_HOST );
#else
    cmp_ctx_set_server_host( ctx, HARDCODED_SERVER_HOST );
#endif

#if 0 /* uses PBM if enabled */
    cmp_ctx_set_recipient_name( ctx, HARDCODED_CA_NAME );
    cmp_ctx_set_pbm_secret( ctx, (const unsigned char*) HARDCODED_SECRET,
            strlen((const char*)HARDCODED_SECRET));
    cmp_ctx_set_pbm_reference( ctx, (const unsigned char*) HARDCODED_REFERENCE,
            strlen((const char*)HARDCODED_REFERENCE));

    cmp_PBMParameter *pbmp;
    pbmp = (cmp_PBMParameter*) mbedtls_calloc(1, sizeof(cmp_PBMParameter));
    cmp_PBMParameter_init( pbmp, ctr_drbg, 16,
                           MBEDTLS_MD_SHA256, 128,
                           MBEDTLS_MD_SHA1); /* TODO HARDCODED */
    cmp_ctx_set_pbmp( ctx, pbmp );
#endif
}


/* **************************************************************** */
/* global variables */
mbedtls_entropy_context entropy;
mbedtls_ctr_drbg_context ctr_drbg;

mbedtls_pk_context new_pk_ctx;
mbedtls_pk_context vendor_pk_ctx;
mbedtls_x509_crt vendor_crt;
mbedtls_x509_crt vendor_sub_ca_crt;
mbedtls_x509_crt operator_sub_ca_crt;
mbedtls_x509_crt service_crt;

/* **************************************************************** */
static void display_firmware_version(void)
{
    CMPDBGS(NEWLINE NEWLINE "Firmware built: " __DATE__ " " __TIME__);
}

/* **************************************************************** */
/* Parse a certificate */
static int parse_cert_from_pem(mbedtls_x509_crt *crt, const unsigned char *pem)
{
    int ret;

    mbedtls_x509_crt_init( crt );

    ret = mbedtls_x509_crt_parse( crt, pem, strlen((const char*)pem)+1 );

    if( ret != 0 )
        CMPERRV("parsing cert FAILED - mbedtls_x509_crt_parse returned %d", ret );

    return( ret);
}


/* **************************************************************** */
/* Parse a private key without password */
static int parse_key_from_pem( mbedtls_pk_context *pk_ctx,
                               const unsigned char* pem)
{
    int ret;

    mbedtls_pk_init( pk_ctx );

    ret = mbedtls_pk_parse_key( pk_ctx, pem, strlen((const char*)pem)+1, NULL, 0 );

    if( ret != 0 )
        CMPERRV("parsing key FAILED - mbedtls_pk_parse_key returned %d", ret );

    return( ret );
}


/* **************************************************************** */
static void init_entropy()
{
    /* initialize Entropy/DRBG */
    mbedtls_entropy_init( &entropy );
    unsigned char personalization[] = "TODO_add_better_entropy?";
    mbedtls_ctr_drbg_init( &ctr_drbg );
    if( mbedtls_ctr_drbg_seed( &ctr_drbg, mbedtls_entropy_func,
                               &entropy, personalization, 10 ) != 0 )
        printf("Error doing mbedtls_ctr_drbg_seed()\n");
    /* Unsure whether that brings an advantage
    mbedtls_ctr_drbg_set_prediction_resistance( &ctr_drbg,
                                                MBEDTLS_CTR_DRBG_PR_OFF );
    */
}

/* **************************************************************** */
static void gen_ec_key(mbedtls_pk_context *key)
{
    int ret;
    const mbedtls_ecp_curve_info *curve_info;

    if( ( ret = mbedtls_pk_setup( key,
                    mbedtls_pk_info_from_type( MBEDTLS_PK_ECKEY ))) != 0 ) {
        CMPERRV( "Key setup failed, mbedtls_pk_setup returned -0x%04x", -ret );
    }

    curve_info = mbedtls_ecp_curve_info_from_name( HARDCODED_ECCURVE );
    ret = mbedtls_ecp_gen_key( curve_info->grp_id, mbedtls_pk_ec( *key ),
                               &mbedtls_ctr_drbg_random, &ctr_drbg );
    if( ret == 0 ) {
        CMPDBGS( "Generated Key" );
    } else {
        CMPERRV( "Key gen failed, mbedtls_ec_gen_key returned -0x%04x", -ret );
    }
}

static void print_certinfo( mbedtls_x509_crt *crt, const char* prefix )
{
    char *certbuf = (char*) mbedtls_calloc(1, 1024);
    mbedtls_x509_crt_info( certbuf, 1024, prefix, crt);
    printf("%s", certbuf);
    mbedtls_free(certbuf);
}


/* ************************************************************************** */
/* MAIN */
/* ************************************************************************** */
int main()
{
#ifdef __MBED__
    pc.baud(115200);
#endif /* __MBED__ */

    display_firmware_version();
    init_entropy();

#ifdef __MBED__
    network = easy_connect(true);
    if ( !network ) {
        CMPERRS("Cannot connect to the network, see serial output");
        return 1;
    }
#endif /* __MBED__ */

    /* TODO: make certs/key loadable through CLI */

    /* Vendor Device Cert */
    if( parse_cert_from_pem( &vendor_crt, vendor_cert_pem ) == 0) {
        CMPDBGS("Parsed Vendor Device Cert");
    } else {
        CMPERRS("Parsing Vendor Device Cert failed");
    }

    char mysubject[100];
    mbedtls_x509_dn_gets( mysubject, 100, &vendor_crt.subject );

    if( parse_key_from_pem( &vendor_pk_ctx, vendor_cert_key_pem ) == 0) {
        CMPDBGS("Parsed Vendor Device Key");
    } else {
        CMPERRS("Parsing Vendor Device Key failed");
    }

    /* Vendor subCA Cert */
    if( parse_cert_from_pem( &vendor_sub_ca_crt, vendor_sub_ca_cert_pem ) == 0) {
        CMPDBGS("Parsed Vendor Sub CA Cert");
    } else {
        CMPERRS("Parsing Vendor Sub CA Cert failed");
    }

    /* Operator Sub CA (=CMP Server) cert */
    if( parse_cert_from_pem( &operator_sub_ca_crt, operator_sub_ca_cert_pem ) == 0) {
        CMPDBGS("Parsed Operator Sub CA cert");
    } else {
        CMPERRS("Parsing Operator Sub CA cert");
    }

    char cmp_srv_subject[100];
    mbedtls_x509_dn_gets( cmp_srv_subject, 100, &operator_sub_ca_crt.subject );

    /* Load or generate key for new certificate */
    mbedtls_pk_init( &new_pk_ctx );
    //if( parse_key_from_pem( &new_pk_ctx, ecckey_pem) == 0) {
    //    CMPDBGS("parsed key");
    //}
    gen_ec_key( &new_pk_ctx );

    cmp_ctx ctx;

    /* create ctx (and body) settings */
    cmp_ctx_init( &ctx, &ctr_drbg );
    cmp_ctx_init_hardcoded( &ctx, &ctr_drbg );

    cmp_ctx_set_sender_name( &ctx, mysubject );
    cmp_ctx_set_subject_name( &ctx, mysubject );

    cmp_ctx_set_recipient_name( &ctx, cmp_srv_subject );
    /* TODO: also set the server cert to use for message validation */

    cmp_ctx_set_new_key( &ctx, &new_pk_ctx );
    cmp_ctx_set_cl_crt( &ctx, &vendor_crt );
    cmp_ctx_set_cl_crt_chain( &ctx, &vendor_sub_ca_crt );
    cmp_ctx_set_prot_key( &ctx, &vendor_pk_ctx );

    /* execute IR */
    if (cmpcl_ir(&ctx) == 1) {
        print_certinfo( ctx.new_cert, "recvd: ");
        /* this would be wherer one could store the cert */
    } else {
        CMPERRS("No Certificate received :-/");
    }
    cmp_ctx_free( &ctx );

/* cleanup */
    mbedtls_x509_crt_free( &operator_sub_ca_crt );
    mbedtls_x509_crt_free( &vendor_sub_ca_crt );
    mbedtls_pk_free( &vendor_pk_ctx );
    mbedtls_x509_crt_free( &vendor_crt );
    mbedtls_pk_free( &new_pk_ctx );
    mbedtls_ctr_drbg_free( &ctr_drbg );
    CMPDBGS("BYE");
    return 0;
}
