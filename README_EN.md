# Remote Embedded Linux Performance Automated Diagnostic Platform

## Project Introduction

The Remote Embedded Linux Performance Automated Diagnostic Platform is a comprehensive system based on multi-process architecture, integrating embedded test framework and kernel observability functions. It implements a complete workflow from Windows control terminal to remotely manage Linux devices for stress testing, performance diagnosis, and test execution. This platform aims to provide embedded device developers with an efficient, automated performance testing and diagnostic tool to help them quickly identify and resolve system performance bottlenecks.

## Technical Architecture

### System Architecture

- **Control-Agent Mode**: Adopts client-server architecture, where Windows control terminal communicates with Linux agent via Socket
- **Multi-threaded Concurrent Processing**: Agent uses multi-threaded model to handle concurrent connections, improving system response speed
- **Cross-language Integration**: Seamless integration of C language test framework and Python network services, fully leveraging their respective advantages
- **Real-time Monitoring and Automatic Diagnosis**: Continuously monitors system CPU, memory and other states, automatically triggering performance diagnosis when performance reaches thresholds

### Core Components

| Component                    | Function                                             | Implementation Language | Location      | Technical Features                                                   |
| ---------------------------- | ---------------------------------------------------- | ----------------------- | ------------- | -------------------------------------------------------------------- |
| Agent Server                 | Execute tests, performance diagnosis, return results | Python                  | agent.py      | Multi-threaded processing, real-time monitoring, automatic diagnosis |
| Control Client               | Send commands, receive results, display reports      | Python                  | control.py    | Interactive interface, file download, remote control                 |
| Embedded Test Framework      | CPU, memory, disk, device tests                      | C                       | src/          | High performance, low overhead, modular design                       |
| Network Communication Module | Socket server functionality                          | C                       | src/network.c | Low latency, high reliability, multi-client support                  |
| Performance Diagnosis Module | Flame graph generation, memory analysis              | Python/C                | agent.py      | Real-time analysis, visual display                                   |

## Features

### Agent Side (Linux Agent)

- **Real-time Monitoring**: Continuously monitors system status including CPU, memory, disk, network, with millisecond-level precision
- **Automatic Diagnosis**: Automatically executes performance diagnosis and generates reports when system performance reaches preset thresholds
- **Stress Testing**: Supports CPU stress testing, memory stress testing, IO stress testing, and comprehensive stress testing with customizable parameters
- **Performance Analysis**: Uses perf tool to collect CPU performance data, generates visual flame graphs, and analyzes memory usage
- **Test Execution**: Seamlessly integrates with embedded test framework to execute various tests including CPU, memory, disk, and device tests
- **Socket Server**: Supports concurrent multi-client connections, providing stable network communication services
- **Multi-threaded Processing**: Uses thread pool pattern to efficiently handle concurrent requests, improving system throughput
- **Permission Management**: Intelligently handles sudo permission issues to ensure stable system operation

### Control Side (Windows Control Terminal)

- **Remote Control**: Sends various commands to Agent via network to implement remote management
- **Status Query**: Real-time obtains system status information, including CPU usage, memory usage, etc.
- **Stress Test Trigger**: Remotely triggers Agent to execute various stress tests with configurable parameters
- **Performance Diagnosis**: Remotely triggers Agent to execute performance diagnosis and obtains detailed performance analysis reports
- **Test Execution**: Remotely executes tests from embedded test framework, supporting test execution with parameters
- **Result Acquisition**: Automatically downloads generated flame graphs, test reports and other files for local analysis
- **Interactive Interface**: Provides user-friendly command-line interface for easy operation
- **Error Handling**: Comprehensive error handling mechanism to ensure stable and reliable communication

## Technical Implementation

### Network Communication

- **Protocol Design**: Based on TCP protocol to implement reliable network communication
- **Data Transmission**: Adopts custom command format, supporting text commands and binary file transfer
- **Multi-threaded Processing**: Agent uses thread pool to handle concurrent connections, improving system response speed
- **Error Reconnection**: Implements automatic reconnection mechanism to ensure communication reliability

### Performance Diagnosis

- **Data Collection**: Uses perf tool to collect CPU performance data, supporting multiple event types
- **Flame Graph Generation**: Uses FlameGraph tool to convert performance data into visual flame graphs
- **Memory Analysis**: Collects memory usage information, generates memory snapshots and analysis reports
- **System Load Analysis**: Collects system load, process status and other information to comprehensively analyze system performance

### Test Execution

- **Modular Design**: Embedded test framework adopts modular design for easy extension and maintenance
- **Multi-type Testing**: Supports multiple types of tests including CPU, memory, disk, device tests
- **Parameterized Testing**: Supports test execution with parameters to meet different test scenario requirements
- **Test Result Management**: Automatically saves test results and generates structured test reports

### Automatic Monitoring

- **Real-time Data Collection**: Uses psutil library to real-time collect system status data
- **Threshold Triggering**: Automatically triggers performance diagnosis when system performance reaches preset thresholds
- **Regular Reporting**: Regularly generates system performance reports to track system performance change trends
- **Anomaly Detection**: Intelligently detects system anomalies to timely discover potential problems

## Installation Instructions

### System Requirements

- **Agent Side**:
  - Linux system, kernel version 3.10+
  - Python 3.6+, with psutil library installed
  - perf tool (linux-tools-common)
  - stress-ng tool
  - FlameGraph tool
- **Control Side**:
  - Windows system
  - Python 3.6+, with socket library installed

### Dependency Installation

#### Agent Side Dependencies

```bash
# Install Python dependencies
pip3 install psutil

# Install system tools
sudo apt-get update
sudo apt-get install linux-tools-common linux-tools-$(uname -r) stress-ng git

# Install FlameGraph
git clone https://github.com/brendangregg/FlameGraph ~/FlameGraph

# Resolve perf permission issues
sudo sysctl kernel.perf_event_paranoid=-1

# Permanently modify perf permission settings
echo "kernel.perf_event_paranoid=-1" | sudo tee -a /etc/sysctl.conf
sudo sysctl -p
```

#### Control Side Dependencies

```bash
# Install Python dependencies
pip install socket
```

## Quick Start

### 1. Compile Embedded Test Framework

```bash
cd Remote-Linux-Performance-Diagnostic-Platform
make clean && make
```

### 2. Start Agent Server

```bash
# Use Python version of Agent server (recommended)
python3 agent.py

# Or use C language version of network server
./test_framework --server
```

### 3. Run Control Client

```bash
python3 control.py
```

### 4. Send Commands

- Connect to Agent server (enter server IP address)
- Select operation to execute:
  - Start CPU stress test
  - Start comprehensive stress test
  - Start performance diagnosis
  - Query system status
  - Execute embedded tests
  - Execute embedded tests with parameters
  - Get test framework information
  - Download latest flame graph

## Supported Commands

### Basic Commands

- **START\_STRESS\_CPU**: Execute CPU stress test and generate performance report
- **start\_stress**: Execute comprehensive stress test (CPU, IO, memory)
- **start\_perf**: Execute performance diagnosis
- **status**: Query system status
- **run\_tests**: Execute embedded tests
- **run\_tests\_with\_args:{args}**: Execute embedded tests with parameters
- **get\_test\_info**: Get test framework information

### Advanced Features

- **find\_flamegraph**: Find latest generated flame graph file
- **download:{file\_path}**: Download specified file

## Project Structure

```
Remote-Linux-Performance-Diagnostic-Platform/
├── src/               # Embedded test framework source code
│   ├── config.c       # Configuration management
│   ├── cpu_test.c     # CPU test
│   ├── device.c       # Device management
│   ├── device_test.c  # Device test
│   ├── disk_test.c    # Disk test
│   ├── logger.c       # Log management
│   ├── main.c         # Main program
│   ├── mem_test.c     # Memory test
│   ├── network.c      # Network communication module
│   ├── report.c       # Report generation
│   ├── test_registry.c # Test registry
│   └── test_runner.c  # Test runner
├── include/           # Header files
│   ├── config.h
│   ├── device.h
│   ├── logger.h
│   ├── metrics.h
│   ├── network.h
│   ├── report.h
│   ├── test_registry.h
│   ├── test_runner.h
│   └── tests.h
├── config/            # Configuration files
├── logs/              # Log files
├── report/            # Test reports
├── reports/           # Performance diagnosis reports
├── agent.py           # Python version of Agent server
├── control.py         # Python version of control client
├── Makefile           # Compilation configuration file
├── README.md          # Project documentation (Chinese)
└── README_EN.md       # Project documentation (English)
```

## Advantages

1. **Function Integration**: Successfully integrates two independent projects (embedded test framework and kernel observability platform) into a unified platform, achieving functional complementarity
2. **Automation**: Implements complete workflow automation from stress testing to diagnosis, reducing manual intervention and improving testing efficiency
3. **Real-time Performance**: Real-time monitors system status, timely discovers performance issues, and quickly responds to system anomalies
4. **Reliability**: Handles permission issues (such as sudo permissions) to ensure stable system operation and improve system reliability
5. **Scalability**: Modular design for easy addition of new features and support for new devices, adapting to different test scenarios
6. **Cross-platform**: Supports Windows control terminal to remotely manage Linux devices, breaking platform limitations
7. **Visualization**: Generates intuitive flame graphs and performance reports for easy analysis of system performance bottlenecks
8. **Low Overhead**: Embedded test framework is implemented in C language, featuring low overhead and high performance

## Application Scenarios

- **Embedded Device Development**: Remote testing and diagnosis of embedded Linux devices, reducing on-site debugging costs
- **System Performance Optimization**: Identify system performance bottlenecks, conduct targeted optimization, and improve system performance
- **Automated Testing**: Implements unattended automated testing and diagnosis, improving testing efficiency
- **Continuous Integration**: Integrate into CI/CD process to automatically conduct performance testing and ensure system quality
- **Production Environment Monitoring**: Real-time monitor production environment system status, timely discover and resolve performance issues
- **Performance Benchmarking**: Establish system performance benchmarks to provide reference for system optimization

## Troubleshooting

### Common Issues

1. **Unable to Generate Flame Graph**
   - Check if perf tool is installed correctly: `perf --version`
   - Check if FlameGraph directory exists: `ls -la ~/FlameGraph`
   - Check if user permissions are sufficient: `sysctl kernel.perf_event_paranoid`
   - Try running Agent server with sudo permissions
2. **Connection Failure**
   - Check if Agent server is running: `ps aux | grep agent.py`
   - Check if network connection is normal: `ping <agent_ip>`
   - Check if port is occupied: `netstat -tuln | grep 8888`
   - Check if firewall settings are blocking port access
3. **Test Execution Failure**
   - Check if embedded test framework is compiled successfully: `ls -la test_framework`
   - Check if test file permissions are correct: `chmod +x test_framework`
   - Check if test framework dependencies are installed completely

### Log Management

- Agent server logs are output to console for real-time viewing
- Test results are saved to reports/ directory, named by timestamp
- Performance diagnosis reports are saved to reports/ directory, including flame graphs and analysis data
- System logs are saved to logs/ directory for problem troubleshooting

##

## Technology Stack

| Category             | Technology/Tool | Purpose                                               |
| -------------------- | --------------- | ----------------------------------------------------- |
| Programming Language | Python 3        | Network services, control logic, performance analysis |
| Programming Language | C               | Embedded test framework, network communication module |
| System Tool          | perf            | CPU performance data collection                       |
| System Tool          | stress-ng       | System stress testing                                 |
| Tool Library         | FlameGraph      | Flame graph generation                                |
| Python Library       | psutil          | System status monitoring                              |
| Network Protocol     | TCP Socket      | Network communication                                 |
| Build Tool           | Make            | Compile embedded test framework                       |

## Contribution

Contributions are welcome through Issue submissions and Pull Requests. Contributors can:

- Report bugs and suggest feature requests
- Submit code improvements and new feature implementations
- Improve documentation and examples
- Participate in code review and testing

## License

This project is licensed under the MIT License. See LICENSE file for details.

## Contact

- Project Maintainer: \[Louis]
- Email: \[[zwt20051229@gmail.com](mailto:your.email@example.com)]

