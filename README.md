# Production-Grade Embedded Test Framework

[![License](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![Build Status](https://img.shields.io/badge/build-passing-brightgreen)](https://github.com/Sherlock-Z-WT/Embedded-Test-Framework/actions)
[![Language](https://img.shields.io/badge/language-C%2093.8%25-blue)](https://github.com/Sherlock-Z-WT/Embedded-Test-Framework)
[![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20Embedded-red)](https://github.com/Sherlock-Z-WT/Embedded-Test-Framework)

**A high-performance, enterprise-grade embedded system test framework designed for semiconductor chipset validation and production reliability testing.**

This framework provides a comprehensive solution for testing embedded devices, system performance, and hardware reliability. It is built for large-scale concurrent testing of embedded devices, system stability, and hardware modules (CPU, memory, WiFi, power, network). It has been optimized for CI/CD pipelines and production verification environments, similar to those used by Qualcomm, MediaTek, and other chipset manufacturers.

## Key Features

- **Advanced Concurrent Scheduling**
  - Intelligent job dispatcher with configurable max jobs limit
  - Dynamic load balancing for optimal resource utilization
  - Priority-based test execution queue

- **Robust Timeout Control**
  - Proactive process management using `waitpid(WNOHANG)` polling
  - Graceful and forced termination with `kill(SIGKILL)`
  - Per-test timeout configuration

- **Real Device Testing**
  - SSH-based remote device connection with secure authentication
  - Command execution and output capture on remote devices
  - Connection pooling for efficient multiple device management

- **Comprehensive System Metrics**
  - Real-time monitoring of CPU, memory, disk usage
  - Process stability and resource utilization tracking
  - Custom metric collection via extensible plugins

- **Standard JSON Test Reports**
  - CI/CD compatible output format
  - Detailed test results with metrics integration
  - Timestamped report archiving system

- **Interactive Progress Visualization**
  - Real-time test progress bar with completion percentage
  - Estimated time of completion (ETA) calculation
  - Color-coded status indicators

- **Flexible Logging System**
  - Multi-level logging (debug/info/warning/error)
  - Configurable verbosity modes
  - Asynchronous logging with file rotation

- **Configuration Management**
  - Command-line argument parsing with long options
  - File-based configuration with inheritance
  - Environment variable support for containerized environments

## Project Architecture
Embedded-Test-Framework/
├── src/                 # Source code directory
│   ├── main.c           # Main entry point and argument parsing
│   ├── test_runner.c    # Concurrent test scheduler and executor
│   ├── report.c         # JSON report generator
│   ├── logger.c         # Multi-level logging system
│   ├── device.c         # SSH device connection manager
│   ├── device_test.c    # Remote device testing implementation
│   ├── config.c         # Configuration parser
│   ├── test_registry.c  # Test registration and management
│   └── utils.c          # Utility functions
├── include/             # Header files
│   ├── test_runner.h    # Test runner API definitions
│   ├── report.h         # Report generation API
│   ├── logger.h         # Logger API
│   ├── device.h         # Device management API
│   ├── metrics.h        # System metrics data structure
│   ├── config.h         # Configuration structure
│   ├── test_registry.h  # Test registry API
│   └── utils.h          # Utility function declarations
├── config/              # Configuration files
│   ├── default.conf     # Default configuration
│   └── test.conf        # Test-specific configuration
├── report/              # Test report output directory
├── logs/                # Log files directory
├── screenshots/         # Runtime screenshots (real execution evidence)
├── .github/workflows/   # CI/CD pipelines (GitHub Actions)
├── Dockerfile           # Production container deployment
├── Makefile             # Professional build system
├── README.md            # English documentation
└── README_zh.md         # Chinese documentation
text## Technical Stack

| Component | Technology | Description |
|-----------|------------|-------------|
| Programming Language | C (C99 standard) | Core framework implementation |
| System Programming | Multi-process architecture | Process isolation and parallel execution |
| Inter-process Communication | Pipes | Efficient data transfer between processes |
| Networking | SSH protocol | Secure remote device communication |
| Build System | GNU Make | Dependency tracking and parallel builds |
| Data Serialization | JSON | Standardized test report format |
| Concurrency | Custom job scheduler | Intelligent task management |
| Logging | Asynchronous logging | High-performance log processing |
| Testing | Unit tests & integration tests | Framework validation |

## Use Cases 

- WiFi/5G Modem stability and interoperability testing
- Power management endurance and leakage simulation
- CPU/Memory/IO stress testing under high load
- Remote embedded device batch verification (SSH)
- Pre-production chipset reliability validation
- CI/CD automated regression testing

## Quick Start (Docker - 推荐生产部署)

```bash
# 1. Build
docker build -t embedded-test-framework .

# 2. Run (example)
docker run -it --rm \
  -v $(pwd)/report:/app/report \
  -v $(pwd)/logs:/app/logs \
  embedded-test-framework --jobs 16 --timeout 120
Build & Run (本地)
Bash# Build the framework
make

# Clean build artifacts
make clean

# Run tests with default configuration
make run

# Run tests with debug logging
make run-debug

# Run tests with quiet mode
make run-quiet

# Run tests with custom configuration
make run CONFIG=config/test.conf
Command Line Interface
Bash# Basic execution
./test_framework

# Custom concurrent jobs and timeout
./test_framework --jobs 8 --timeout 45

# Enable debug logging
./test_framework --debug

# Enable quiet mode (minimal output)
./test_framework --quiet

# Load configuration from file
./test_framework --config config/test.conf

# Specify report output directory
./test_framework --report-dir ./custom_reports

# Run specific test suite
./test_framework --test-suite cpu,memory
Configuration Options





















































Short OptionLong OptionDescriptionDefault Value-j--jobsMaximum concurrent test jobs2-t--timeoutTest timeout in seconds60-d--debugEnable debug level loggingDisabled-q--quietEnable quiet modeDisabled-c--configPath to configuration fileN/A-r--report-dirDirectory for test reports./report-s--test-suiteComma-separated test suite namesAll tests
Configuration File Format
ini# config/test.conf - Embedded Test Framework Configuration

[concurrency]
max_jobs = 4
timeout = 30

[logging]
debug = true
quiet = false
log_file = logs/test.log

[tests]
cpu_test = true
mem_test = true
disk_test = true
network_test = false

[device]
host = 192.168.1.100
user = test
port = 22
key_file = ~/.ssh/id_rsa

[report]
dir = ./report
format = json
archive = true
Test Reports
Test results are generated in the specified report directory as JSON files with timestamp-based naming:
JSON{
  "metadata": {
    "framework_version": "1.0.0",
    "timestamp": "2026-03-19T10:30:45Z",
    "execution_time": 18.75,
    "hostname": "test-server-01",
    "os": "Linux 5.4.0-91-generic x86_64"
  },
  "summary": {
    "total": 4,
    "passed": 3,
    "failed": 1,
    "skipped": 0,
    "success_rate": 75.0
  },
  "tests": [
    {
      "name": "CPU Stress Test",
      "status": "PASSED",
      "duration": 5.2,
      "metrics": {
        "cpu_usage": 15.2,
        "memory_usage": 45.3,
        "disk_usage": 23.7,
        "process_count": 128,
        "load_average": [0.15, 0.22, 0.18]
      },
      "details": "CPU stress test completed successfully with no errors"
    },
    {
      "name": "Memory Test",
      "status": "PASSED",
      "duration": 3.8,
      "metrics": {
        "memory_usage": 67.2,
        "swap_usage": 12.5,
        "available_memory": 1536,
        "memory_utilization": 45.8
      },
      "details": "Memory test passed with no memory leaks detected"
    },
    {
      "name": "Disk I/O Test",
      "status": "PASSED",
      "duration": 6.2,
      "metrics": {
        "disk_usage": 23.7,
        "io_read_speed": 125.4,
        "io_write_speed": 98.7,
        "io_latency": 1.2,
        "inode_usage": 15.3
      },
      "details": "Disk I/O performance within expected range"
    },
    {
      "name": "Network Test",
      "status": "FAILED",
      "duration": 3.5,
      "metrics": {
        "ping_latency": 150.2,
        "packet_loss": 5.0,
        "bandwidth": 10.5
      },
      "details": "Network test failed: high packet loss detected"
    }
  ]
}
Production Notes

Warning: Designed for testing environments. Do not run on production systems without supervision.
Fully compatible with GitHub Actions, Jenkins, GitLab CI
Prometheus metrics endpoint ready for monitoring integration
Extensible for new hardware modules (WiFi, GPU, USB, etc.)

Contributing
Contributions are welcome! Please see CONTRIBUTING.md and open issues/PRs.
License
MIT License - see LICENSE file.

Made for semiconductor chipset manufacturers to accelerate reliability testing and reduce time-to-market.
Last updated: March 2026

*Built with for embedded systems testing*
