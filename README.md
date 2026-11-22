# **ufedm-server \- An ImNAND OS Connector**

The ImNAND OS Connector is a standalone service responsible for providing hardware access to NAND flash devices on behalf of the ImNAND application.

It acts as the bridge between ImNAND's platform-independent NAND management functionality and the underlying operating system's hardware interfaces.

It acts as a TCP/IP server for ImNAND (as a client), hence the repository name is ufedm-server.

## **Overview**

ImNAND separates NAND management functionality from platform-specific hardware access.

The OS Connector is responsible for:

* Providing a communication interface for the ImNAND application.  
* Managing access to the underlying NAND device.  
* Exposing NAND-related operations through a unified interface.  
* Handling platform-specific hardware access.  
* Supporting local and remote connections.  
* Integrating with existing operating-system NAND infrastructure where available.

The goal is to keep the main ImNAND application independent of the hardware access mechanisms provided by individual operating systems.

## **Architecture**

The OS Connector is designed around a client-server architecture.

text

```
              ImNAND Application
                      |
                      |
                Client Protocol
                      |
                      v
              +---------------+
              | OS Connector |
              +---------------+
                      |
             Platform Backend
                      |
             +--------+--------+
             |                 |
          Linux              Windows
             |                 |
           ufedmd             WinUSB
             |                 |
           ufedm           NAND Device
             |
         NAND Device
```

Each connector instance represents a NAND storage device accessible through its respective platform backend.

Multiple instances can be deployed to manage multiple devices.

## **Supported Backends**

### **Linux**

The Linux backend integrates with the existing `ufedmd` userspace daemon and `ufedm` kernel module.

This allows the connector to provide access to NAND devices through the Linux MTD infrastructure.

Linux is also the target platform for MTD passthrough functionality, allowing NAND configurations to be applied to a device without requiring ImNAND to handle the actual NAND data transfer.

Related projects:

* ufedmd  
* ufedm

### **Windows**

The Windows backend is intended to provide direct NAND hardware access through WinUSB.

Unlike Linux, Windows does not provide an equivalent to the Linux MTD subsystem. Consequently, MTD passthrough is not applicable to this backend.

## **Communication**

The connector exposes a socket-based communication interface.

The intended transports are:

* **Unix domain sockets:** Preferred for local communication.  
* **TCP sockets:** Used for remote connections and compatibility with platforms where Unix domain sockets are unavailable or unsuitable.

The communication protocol is designed around CBOR serialization and length-prefixed messages.

### **Protocol characteristics**

| Property | Description |
| ----- | ----- |
| Serialization | CBOR |
| Message framing | Length-prefixed |
| Maximum message size | 16 MiB (initial design) |
| Text encoding | UTF-8 |
| Byte order | Big-endian |
| Protocol versioning | Explicit version field |
| Request identification | Request IDs |

The protocol is intended to support structured requests and responses while remaining independent of the underlying transport.

## **Security**

The connector may provide access to low-level NAND hardware operations, including operations capable of modifying persistent storage.

For this reason, network exposure must be considered carefully.

The intended deployment environments include:

* Local machine communication.  
* Trusted private networks.  
* VPN connections.

The initial design does not include an authentication mechanism.

Do not expose an unauthenticated connector directly to the public Internet. Remote deployments should use appropriate network isolation, firewall rules, and transport encryption.

## **Development Status**

The OS Connector is part of the broader ImNAND project and is under active development.

The architecture is intended to support multiple operating systems and hardware backends while maintaining a common communication interface.

Not all planned functionality or platform backends should be assumed to be available.

## **Relationship with ImNAND**

The OS Connector is a basically component of [ImNAND](https://github.com/nfiop/ImNAND), a cross-platform NAND flash programming and configuration utility.

The main application handles NAND configuration and user interaction, while the connector provides the platform-specific interface required to communicate with NAND hardware.

For more information about the overall project, its operating modes, and its architecture, see the main ImNAND README.

## **License**

This project is distributed under the licensing terms of ImNAND.

See the repository's LICENSE file for details.


