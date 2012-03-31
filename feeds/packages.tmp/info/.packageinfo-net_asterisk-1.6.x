Source-Makefile: feeds/packages/net/asterisk-1.6.x/Makefile
Package: asterisk16
Submenu: asterisk16 (Complete Open Source PBX), v1.6.x
Version: 1.6.2.6-2
Depends: +libopenssl +libncurses +libpopt +libpthread +zlib @!TARGET_avr32 @!PACKAGE_asterisk14
Provides: 
Section: net
Category: Network
Title: Complete open source PBX
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.6.2.6.tar.gz
Type: ipkg
Description:  Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
http://www.asterisk.org/
@@

Package: asterisk16-voicemail
Submenu: asterisk16 (Complete Open Source PBX), v1.6.x
Version: 1.6.2.6-2
Depends: +asterisk16
Provides: 
Section: net
Category: Network
Title: Voicemail support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.6.2.6.tar.gz
Type: ipkg
Description:  Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
 This package contains voicemail related modules for Asterisk.
http://www.asterisk.org/
@@

Package: asterisk16-sounds
Submenu: asterisk16 (Complete Open Source PBX), v1.6.x
Version: 1.6.2.6-2
Depends: +asterisk16
Provides: 
Section: net
Category: Network
Title: Sound files
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.6.2.6.tar.gz
Type: ipkg
Description:  Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
 This package contains sound files for Asterisk.
http://www.asterisk.org/
@@

Package: asterisk16-chan-iax2
Submenu: asterisk16 (Complete Open Source PBX), v1.6.x
Version: 1.6.2.6-2
Depends: +asterisk16
Provides: 
Section: net
Category: Network
Title: IAX support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.6.2.6.tar.gz
Type: ipkg
Description:  Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
 This package provides IAX support to
 Asterisk.
http://www.asterisk.org/
@@

Package: asterisk16-cdr
Submenu: asterisk16 (Complete Open Source PBX), v1.6.x
Version: 1.6.2.6-2
Depends: +asterisk16
Provides: 
Section: net
Category: Network
Title: CDR support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.6.2.6.tar.gz
Type: ipkg
Description:  Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
 This package provides Call Detail Record support to
 Asterisk.
http://www.asterisk.org/
@@

Package: asterisk16-res-musiconhold
Submenu: asterisk16 (Complete Open Source PBX), v1.6.x
Version: 1.6.2.6-2
Depends: +asterisk16
Provides: 
Section: net
Category: Network
Title: MOH support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.6.2.6.tar.gz
Type: ipkg
Description:  Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
 This package provides Music On Hold support to
 Asterisk.
http://www.asterisk.org/
@@

Package: asterisk16-chan-gtalk
Submenu: asterisk16 (Complete Open Source PBX), v1.6.x
Version: 1.6.2.6-2
Depends: +asterisk16 +libiksemel
Provides: 
Section: net
Category: Network
Title: GTalk support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.6.2.6.tar.gz
Type: ipkg
Description:  Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
 This package provides the channel chan_gtalk and res_jabber for GTalk
 support to Asterisk.
http://www.asterisk.org/
@@

Package: asterisk16-app-authenticate
Submenu: asterisk16 (Complete Open Source PBX), v1.6.x
Version: 1.6.2.6-2
Depends: +asterisk16
Provides: 
Section: net
Category: Network
Title: Authenticate support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.6.2.6.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support support for executing arbitrary authenticate commands in Asterisk.
http://www.asterisk.org/
@@

Package: asterisk16-app-chanisavail
Submenu: asterisk16 (Complete Open Source PBX), v1.6.x
Version: 1.6.2.6-2
Depends: +asterisk16
Provides: 
Section: net
Category: Network
Title: Channel availability check support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.6.2.6.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support support for checking if a channel is available in Asterisk.
http://www.asterisk.org/
@@

Package: asterisk16-app-chanspy
Submenu: asterisk16 (Complete Open Source PBX), v1.6.x
Version: 1.6.2.6-2
Depends: +asterisk16
Provides: 
Section: net
Category: Network
Title: Channel listen in support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.6.2.6.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support support for listening in on any channel in Asterisk.
http://www.asterisk.org/
@@

Package: asterisk16-app-directed-pickup
Submenu: asterisk16 (Complete Open Source PBX), v1.6.x
Version: 1.6.2.6-2
Depends: +asterisk16
Provides: 
Section: net
Category: Network
Title: Directed call pickup support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.6.2.6.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support support for directed call pickup in Asterisk.
http://www.asterisk.org/
@@

Package: asterisk16-app-exec
Submenu: asterisk16 (Complete Open Source PBX), v1.6.x
Version: 1.6.2.6-2
Depends: +asterisk16
Provides: 
Section: net
Category: Network
Title: Exec application support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.6.2.6.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support support for application execution in Asterisk.
http://www.asterisk.org/
@@

Package: asterisk16-app-minivm
Submenu: asterisk16 (Complete Open Source PBX), v1.6.x
Version: 1.6.2.6-2
Depends: +asterisk16
Provides: 
Section: net
Category: Network
Title: Minimal voicemail system support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.6.2.6.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support a voicemail system in small building blocks working together based on the Comedian Mail voicemail system in Asterisk.
http://www.asterisk.org/
@@

Package: asterisk16-app-readexten
Submenu: asterisk16 (Complete Open Source PBX), v1.6.x
Version: 1.6.2.6-2
Depends: +asterisk16
Provides: 
Section: net
Category: Network
Title: Extension to variable support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.6.2.6.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support a trivial application to read an extension into a variable in Asterisk.
http://www.asterisk.org/
@@

Package: asterisk16-app-read
Submenu: asterisk16 (Complete Open Source PBX), v1.6.x
Version: 1.6.2.6-2
Depends: +asterisk16
Provides: 
Section: net
Category: Network
Title: Variable read support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.6.2.6.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support a trivial application to read a variable in Asterisk.
http://www.asterisk.org/
@@

Package: asterisk16-app-sayunixtime
Submenu: asterisk16 (Complete Open Source PBX), v1.6.x
Version: 1.6.2.6-2
Depends: +asterisk16
Provides: 
Section: net
Category: Network
Title: Say Unix time support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.6.2.6.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support an application to say Unix time in Asterisk.
http://www.asterisk.org/
@@

Package: asterisk16-app-sms
Submenu: asterisk16 (Complete Open Source PBX), v1.6.x
Version: 1.6.2.6-2
Depends: +asterisk16
Provides: 
Section: net
Category: Network
Title: SMS support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.6.2.6.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support SMS support (ETSI ES 201 912 protocol 1) in Asterisk.
http://www.asterisk.org/
@@

Package: asterisk16-app-stack
Submenu: asterisk16 (Complete Open Source PBX), v1.6.x
Version: 1.6.2.6-2
Depends: +asterisk16
Provides: 
Section: net
Category: Network
Title: Stack applications support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.6.2.6.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support  stack applications Gosub Return etc. in Asterisk.
http://www.asterisk.org/
@@

Package: asterisk16-app-system
Submenu: asterisk16 (Complete Open Source PBX), v1.6.x
Version: 1.6.2.6-2
Depends: +asterisk16
Provides: 
Section: net
Category: Network
Title: System exec support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.6.2.6.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support support for executing system commands in Asterisk.
http://www.asterisk.org/
@@

Package: asterisk16-app-talkdetect
Submenu: asterisk16 (Complete Open Source PBX), v1.6.x
Version: 1.6.2.6-2
Depends: +asterisk16
Provides: 
Section: net
Category: Network
Title: File playback with audio detect support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.6.2.6.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support for file playback with audio detect in Asterisk.
http://www.asterisk.org/
@@

Package: asterisk16-app-waituntil
Submenu: asterisk16 (Complete Open Source PBX), v1.6.x
Version: 1.6.2.6-2
Depends: +asterisk16
Provides: 
Section: net
Category: Network
Title: Sleep support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.6.2.6.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support support sleeping until the given epoch in Asterisk.
http://www.asterisk.org/
@@

Package: asterisk16-app-while
Submenu: asterisk16 (Complete Open Source PBX), v1.6.x
Version: 1.6.2.6-2
Depends: +asterisk16
Provides: 
Section: net
Category: Network
Title: While loop support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.6.2.6.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support a while loop implementation in Asterisk.
http://www.asterisk.org/
@@

Package: asterisk16-chan-agent
Submenu: asterisk16 (Complete Open Source PBX), v1.6.x
Version: 1.6.2.6-2
Depends: +asterisk16
Provides: 
Section: net
Category: Network
Title: Agents proxy channel support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.6.2.6.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support  an implementation of agents proxy channel in Asterisk.
http://www.asterisk.org/
@@

Package: asterisk16-chan-local
Submenu: asterisk16 (Complete Open Source PBX), v1.6.x
Version: 1.6.2.6-2
Depends: +asterisk16
Provides: 
Section: net
Category: Network
Title: Local proxy channel support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.6.2.6.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support  an implementation of local proxy channel in Asterisk.
http://www.asterisk.org/
@@

Package: asterisk16-codec-ulaw
Submenu: asterisk16 (Complete Open Source PBX), v1.6.x
Version: 1.6.2.6-2
Depends: +asterisk16
Provides: 
Section: net
Category: Network
Title: Signed linear to ulaw translation support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.6.2.6.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support translation between signed linear and ulaw codecs in Asterisk.
http://www.asterisk.org/
@@

Package: asterisk16-codec-a-mu
Submenu: asterisk16 (Complete Open Source PBX), v1.6.x
Version: 1.6.2.6-2
Depends: +asterisk16
Provides: 
Section: net
Category: Network
Title: Alaw to ulaw translation support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.6.2.6.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support translation between alaw and ulaw codecs in Asterisk.
http://www.asterisk.org/
@@

Package: asterisk16-codec-g726
Submenu: asterisk16 (Complete Open Source PBX), v1.6.x
Version: 1.6.2.6-2
Depends: +asterisk16
Provides: 
Section: net
Category: Network
Title: Signed linear to G.726 translation support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.6.2.6.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support translation between signed linear and ITU G.726-32kbps codecs in Asterisk.
http://www.asterisk.org/
@@

Package: asterisk16-format-g726
Submenu: asterisk16 (Complete Open Source PBX), v1.6.x
Version: 1.6.2.6-2
Depends: +asterisk16
Provides: 
Section: net
Category: Network
Title: G.726 support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.6.2.6.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support support for headerless G.726 16/24/32/40kbps data format in Asterisk.
http://www.asterisk.org/
@@

Package: asterisk16-format-g729
Submenu: asterisk16 (Complete Open Source PBX), v1.6.x
Version: 1.6.2.6-2
Depends: +asterisk16
Provides: 
Section: net
Category: Network
Title: G.729 support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.6.2.6.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support support for raw headerless G729 data in Asterisk.
http://www.asterisk.org/
@@

Package: asterisk16-format-sln
Submenu: asterisk16 (Complete Open Source PBX), v1.6.x
Version: 1.6.2.6-2
Depends: +asterisk16
Provides: 
Section: net
Category: Network
Title: Raw slinear format support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.6.2.6.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support support for raw slinear format in Asterisk.
http://www.asterisk.org/
@@

Package: asterisk16-format-sln16
Submenu: asterisk16 (Complete Open Source PBX), v1.6.x
Version: 1.6.2.6-2
Depends: +asterisk16
Provides: 
Section: net
Category: Network
Title: Raw slinear 16 format support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.6.2.6.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support support for Raw slinear 16 format in Asterisk.
http://www.asterisk.org/
@@

Package: asterisk16-func-db
Submenu: asterisk16 (Complete Open Source PBX), v1.6.x
Version: 1.6.2.6-2
Depends: +asterisk16
Provides: 
Section: net
Category: Network
Title: Database interaction support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.6.2.6.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support functions for interaction with the database in Asterisk.
http://www.asterisk.org/
@@

Package: asterisk16-func-devstate
Submenu: asterisk16 (Complete Open Source PBX), v1.6.x
Version: 1.6.2.6-2
Depends: +asterisk16
Provides: 
Section: net
Category: Network
Title: Blinky lights control support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.6.2.6.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support functions for manually controlled blinky lights in Asterisk.
http://www.asterisk.org/
@@

Package: asterisk16-func-vmcount
Submenu: asterisk16 (Complete Open Source PBX), v1.6.x
Version: 1.6.2.6-2
Depends: +asterisk16
Provides: 
Section: net
Category: Network
Title: vmcount dialplan support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.6.2.6.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support a vmcount dialplan function in Asterisk.
http://www.asterisk.org/
@@

Package: asterisk16-func-extstate
Submenu: asterisk16 (Complete Open Source PBX), v1.6.x
Version: 1.6.2.6-2
Depends: +asterisk16
Provides: 
Section: net
Category: Network
Title: Hinted extension state support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.6.2.6.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support retrieving the state of a hinted extension for dialplan control in Asterisk.
http://www.asterisk.org/
@@

Package: asterisk16-func-global
Submenu: asterisk16 (Complete Open Source PBX), v1.6.x
Version: 1.6.2.6-2
Depends: +asterisk16
Provides: 
Section: net
Category: Network
Title: Global variable support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.6.2.6.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support global variable dialplan functions in Asterisk.
http://www.asterisk.org/
@@

Package: asterisk16-func-shell
Submenu: asterisk16 (Complete Open Source PBX), v1.6.x
Version: 1.6.2.6-2
Depends: +asterisk16
Provides: 
Section: net
Category: Network
Title: Shell support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.6.2.6.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support support for shell execution in Asterisk.
http://www.asterisk.org/
@@

Package: asterisk16-pbx-ael
Submenu: asterisk16 (Complete Open Source PBX), v1.6.x
Version: 1.6.2.6-2
Depends: +asterisk16
Provides: 
Section: net
Category: Network
Title: Asterisk Extension Logic support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.6.2.6.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support support for symbolic Asterisk Extension Logic in Asterisk.
http://www.asterisk.org/
@@

Package: asterisk16-res-ael-share
Submenu: asterisk16 (Complete Open Source PBX), v1.6.x
Version: 1.6.2.6-2
Depends: +asterisk16
Provides: 
Section: net
Category: Network
Title: Shareable AEL code support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.6.2.6.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support support for shareable AEL code mainly between internal and external modules in Asterisk.
http://www.asterisk.org/
@@

Package: asterisk16-res-agi
Submenu: asterisk16 (Complete Open Source PBX), v1.6.x
Version: 1.6.2.6-2
Depends: +asterisk16
Provides: 
Section: net
Category: Network
Title: Asterisk Gateway Interface support
Maintainer: Hans Zandbelt <hans.zandbelt@gmail.com>
Source: asterisk-1.6.2.6.tar.gz
Type: ipkg
Description:    Asterisk is a complete PBX in software. It provides all of the features
 you would expect from a PBX and more. Asterisk does voice over IP in three
 protocols, and can interoperate with almost all standards-based telephony
 equipment using relatively inexpensive hardware.
    This package provides support support for the Asterisk Gateway Interface extension in Asterisk.
http://www.asterisk.org/
@@


