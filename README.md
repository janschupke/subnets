# Subnets - IPv4 Subnet Calculator

A command-line IPv4 subnet calculator written in C++.

## Features

- **Interactive Mode**: Enter network information through prompts
- **File Input**: Load subnet configurations from text files
- **Automatic Calculations**: Computes network addresses, broadcast addresses, and subnet masks
- **Subnet Optimization**: Automatically allocates subnets based on required client counts
- **Simple Build**: Includes Makefile for easy compilation

## Installation

### Prerequisites
- C++ compiler (g++ recommended)
- Make (for building)

### Building from Source

1. Clone or download the project
2. Navigate to the project directory
3. Build the project:
   ```bash
   make
   ```

For Windows compilation:
```bash
make win
```

### Installation (Optional)
To install the binary system-wide:
```bash
sudo make install
```

To uninstall:
```bash
sudo make uninstall
```

## Usage

### Interactive Mode
Run without arguments to enter interactive mode:
```bash
./subnets
```

### File Input Mode
Provide a configuration file as an argument:
```bash
./subnets config.txt
```

### Help
Display usage information:
```bash
./subnets --help
./subnets -h
./subnets --usage
```

## Input File Format

The program accepts configuration files with the following format:

```
<network_ip>
<subnet_mask>
<subnet_name> <required_clients>
<subnet_name> <required_clients>
...
```

### Example Configuration File
```
192.168.0.0
255.255.0.0
net1 127
net2 17
net3 1500
net4 400
net5 42
net6 8
net7 800
```

### File Format Details

- **Line 1**: Network IP address (e.g., `192.168.0.0`)
- **Line 2**: Subnet mask (e.g., `255.255.0.0`)
- **Lines 3+**: Subnet definitions in format `<name> <client_count>`

## Example Output

The program calculates and displays:
- Network IP addresses for each subnet
- Subnet masks
- Broadcast addresses
- Number of available hosts per subnet

## Sample Files

The `files/` directory contains example configuration files:
- `ccna.txt` - CCNA-style subnetting exercise
- `range.txt` - Basic subnet range example
- `capacity.txt` - Capacity planning example
- `spaces.txt` - Space allocation example
- `bugTest.txt` - Test case for edge conditions
- `chars.txt` - Character handling test

## Building and Development

### Clean Build
```bash
make clean
make
```

### Code Statistics
```bash
make all  # Shows line count for all source files
```

## License

This project is licensed under the GNU Lesser General Public License v3.0. See the source code headers for details.

## Author

Created by Jan Schupke in 2013.
