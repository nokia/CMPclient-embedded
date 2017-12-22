## Synopsis

This client-side example implementation of RFC 4210 “Certificate Management
Protocol (CMP)” utilizes [Arm® Mbed™ TLS](https://tls.mbed.org/) as crypto
library. It makes use of and is the reference implementatio for the library
available in [CMPclient-embedded-lib](https://github.com/nokia/CMPclient-embedded-lib).

It is mainly intended for use on constrained embedded IoT devices, e.g. based
on [Arm Mbed OS](https://os.mbed.com/), but will also work on any other system
supported by the Mbed TLS library. It is commonly tested on the NXP
Semiconductors’ FRDM-K64F development platform utilizing Mbed OS, and on Debian
Linux®.

Having the goal to be lightweight, initial focus is on providing basic support
for Initial Registration (IR/IP), protected by means of MSG\_MAC\_ALG based on
reference and secret values, as well as MSG\_SIG\_ALG based on external identity
certificates (cf. RFC 4210, E.7). Later, other CMP functionality such as those
for Key Update (KUR/KUP) could be added.

## Code Example

N/A

## Motivation

A fully featured CMP client based on OpenSSL exists, but it is not possible to
use it on ARM Mbed devices.  Also developers of other constrained devices might
prefer to select Mbed TLS over OpenSSL.

## Installation

### Download

If the code is cloned from GitHub, one might prefer to use the --recurse flag
to immeditaly also clone the CMPclient library and Mbed TLS which are
referenced as git submodules.

    $ git clone --recursive https://github.com/nokia/CMPclient-embedded.git

### Building

The CMP client can be built  using e.g. GCC for Linux, or the Mbed CLI and the
GNU ARM Embedded Toolchain.

#### GCC for Linux

The library (including the dependencies) can be build using make

    $ make

This results in the **cmpclient** executable

#### Mbed CLI and GNU ARM Embedded Toolchain

The GNU ARM Embedded Toolchain is one way to build for Mbed OS.

##### Installation and preparation for K64F

Information about the needed Mbed CLI can be found in the [Mbed OS documentation] (https://docs.mbed.com/docs/mbed-os-handbook/en/5.1/dev\_tools/cli/).

On Debian the Mbed CLI can be installed like this:

    $ apt-get install python-pip
    $ pip install mbed-cli

The GNU ARM Embedded Toolchain can be downloaded from [here](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads).

Preparing to build

    $ mbed config --global GCC\_ARM\_PATH [path of Toolchain bin]
    $ mbed toolchain GCC\_ARM
    $ mbed target K64F

##### Preparation and building

First, the dependencies need to be deployed:

    $ mbed deploy

Then, the firmware can be built:

    $ mbed compile

## Configuration / CLI Reference

There are no CLI options as of now, all information needs to be coded into the
executable.  The following configuration options exist and must be taken into
account before building and trying any executable.

### mbed\_app.json - for Mbed OS

* CA\_HOST - the host name or IP of the CMP server
* USE\_EASYCONNECT - true/false whether to use easyconnect or default (ethernet)
* network-interface - whether to e.g. use WiFi ESP8266
* wifi-ssid - the WiFi SSID
* wifi-password - the WiFi password

### cmpcl\_certs.h for MbedOS and Linux

The certificates and chain in PEM format to use for:

* External Identity Certificate
  * also the key
* CMP server certificate

### cmpclient.cpp

* HARDCODED\_SERVER\_HOST - if *not* building for Mbed OS
* HARDCODED\_CA\_NAME - the CMP server name if MSG\_MAC\_ALG is used and no
server certificate exists
* HARDCODED\_SECRET - the secret to use for MSG\_MAC\_ALG, if applicable
* HARDCODED\_REFERENCE - the reference to use for MSG\_MAC\_ALG, if applicable
* the code for configuring using PBM / MSG\_MAC\_ALG, commented out by means of
precompiler condition (#if 0), if commented in, MSG\_MAC\_ALG will be used

## Tests

Currently there are no tests.

## Beware

This is not yet to be considered as production grade code.

## Contributors

Feel free to contact the main author if you are interested to contribute.

## License

Licensed under the Apache License, Version 2.0 (the "License"); you may
not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.


The CMP client contains code derived from examples and documentation for
mbedTLS by ARM
Copyright (C) 2006-2017, ARM Limited, All Rights Reserved
SPDX-License-Identifier: Apache-2.0


Arm® and Mbed™ are registered trademarks or trademarks of Arm Limited (or its
subsidiaries) in the US and/or elsewhere.

NXP is a trademark of NXP B.V.

Debian is a registered trademark owned by Software in the Public
Interest, Inc.

Linux® is the registered trademark of Linus Torvalds in the
U.S. and other countries
