# 生产级嵌入式测试框架

[![License](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![Build Status](https://img.shields.io/badge/build-passing-brightgreen)](https://github.com/Sherlock-Z-WT/Embedded-Test-Framework/actions)
[![Language](https://img.shields.io/badge/language-C%2093.8%25-blue)](https://github.com/Sherlock-Z-WT/Embedded-Test-Framework)
[![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20Embedded-red)](https://github.com/Sherlock-Z-WT/Embedded-Test-Framework)

**一个高性能、企业级的嵌入式系统测试框架，专为半导体芯片验证和生产级可靠性测试设计。**

本框架为嵌入式设备测试、系统性能验证和硬件可靠性评估提供全面解决方案。支持大规模并发测试、系统稳定性验证以及硬件模块（CPU、内存、WiFi、电源、网络）测试，已针对 CI/CD 流水线和生产验证环境进行优化，适用于高通（Qualcomm）、联发科等芯片制造商的生产验证场景。

## 核心功能

- **高级并发调度**  
  智能作业调度器，支持可配置的最大作业数限制  
  动态负载均衡，实现资源最优利用  
  基于优先级的测试执行队列

- **可靠的超时控制**  
  使用 `waitpid(WNOHANG)` 轮询进行主动进程管理  
  通过 `kill(SIGKILL)` 实现优雅和强制终止  
  支持每测试独立超时配置

- **真实设备测试**  
  基于 SSH 的远程设备连接，支持安全认证  
  远程设备上的命令执行和输出捕获  
  连接池管理，高效处理多个设备

- **全面的系统指标监控**  
  实时监控 CPU、内存、磁盘使用情况  
  进程稳定性和资源利用跟踪  
  通过可扩展插件进行自定义指标收集

- **标准 JSON 测试报告**  
  兼容 CI/CD 的输出格式  
  详细的测试结果，集成指标数据  
  带时间戳的报告归档系统

- **交互式进度可视化**  
  实时测试进度条，显示完成百分比  
  预计完成时间（ETA）计算  
  彩色编码的状态指示器

- **灵活的日志系统**  
  多级日志（DEBUG/INFO/WARNING/ERROR）  
  可配置的详细程度模式  
  带文件轮转的异步日志

- **配置管理**  
  支持长选项的命令行参数解析  
  基于文件的配置，支持继承  
  容器化环境的环境变量支持

## 项目架构
Embedded-Test-Framework/
├── src/                 # 源代码目录
│   ├── main.c           # 主入口点和参数解析
│   ├── test_runner.c    # 并发测试调度器和执行器
│   ├── report.c         # JSON报告生成器
│   ├── logger.c         # 多级日志系统
│   ├── device.c         # SSH设备连接管理器
│   ├── device_test.c    # 远程设备测试实现
│   ├── config.c         # 配置解析器
│   ├── test_registry.c  # 测试注册和管理
│   └── utils.c          # 工具函数
├── include/             # 头文件
│   ├── test_runner.h    # 测试运行器API定义
│   ├── report.h         # 报告生成API
│   ├── logger.h         # 日志API
│   ├── device.h         # 设备管理API
│   ├── metrics.h        # 系统指标数据结构
│   ├── config.h         # 配置结构
│   ├── test_registry.h  # 测试注册表API
│   └── utils.h          # 工具函数声明
├── config/              # 配置文件
│   ├── default.conf     # 默认配置
│   └── test.conf        # 测试专用配置
├── report/              # 测试报告输出目录
├── logs/                # 日志文件目录
├── screenshots/         # 运行时截图（真实执行证据）
├── .github/workflows/   # CI/CD 流水线（GitHub Actions）
├── Dockerfile           # 生产级容器部署
├── Makefile             # 专业构建系统
├── README.md            # 英文文档
└── README_zh.md         # 中文文档
text## 技术栈

| 组件         | 技术              | 描述                     |
|--------------|-------------------|--------------------------|
| 编程语言     | C (C99标准)       | 核心框架实现             |
| 系统编程     | 多进程架构        | 进程隔离与并行执行       |
| 进程间通信   | 管道              | 高效数据传输             |
| 网络         | SSH协议           | 安全远程设备通信         |
| 构建系统     | GNU Make          | 依赖跟踪与并行构建       |
| 数据序列化   | JSON              | 标准化测试报告格式       |
| 并发         | 自定义作业调度器  | 智能任务管理             |
| 日志         | 异步日志          | 高性能日志处理           |
| 测试         | 单元测试与集成测试| 框架验证                 |

## 使用场景（针对高通类芯片验证）

- WiFi/5G Modem 稳定性和互操作性测试
- 电源管理耐久性和泄漏模拟
- 高负载下的 CPU/内存/IO 压力测试
- 远程嵌入式设备批量验证（SSH）
- 预生产芯片可靠性验证
- CI/CD 自动化回归测试

## 快速开始（Docker - 推荐生产部署）

```bash
# 1. 构建镜像
docker build -t embedded-test-framework .

# 2. 运行示例
docker run -it --rm \
  -v $(pwd)/report:/app/report \
  -v $(pwd)/logs:/app/logs \
  embedded-test-framework --jobs 16 --timeout 120
本地构建与运行
Bash# 构建框架
make

# 清理构建产物
make clean

# 使用默认配置运行测试
make run

# 启用调试日志运行测试
make run-debug

# 启用安静模式运行测试
make run-quiet

# 使用自定义配置运行测试
make run CONFIG=config/test.conf
命令行界面
Bash# 基本执行
./test_framework

# 自定义并发作业数和超时时间
./test_framework --jobs 8 --timeout 45

# 启用调试日志
./test_framework --debug

# 启用安静模式（最小输出）
./test_framework --quiet

# 从文件加载配置
./test_framework --config config/test.conf

# 指定报告输出目录
./test_framework --report-dir ./custom_reports

# 运行特定测试套件
./test_framework --test-suite cpu,memory
配置选项





















































短选项长选项描述默认值-j--jobs最大并发测试作业数2-t--timeout测试超时时间（秒）60-d--debug启用调试级别日志禁用-q--quiet启用安静模式禁用-c--config配置文件路径N/A-r--report-dir测试报告目录./report-s--test-suite逗号分隔的测试套件名称所有测试
配置文件格式
ini# config/test.conf - 嵌入式测试框架配置

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
测试报告
测试结果在指定的报告目录中生成为带时间戳命名的 JSON 文件：
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
生产注意事项

警告：本框架专为测试环境设计。请勿在生产系统上无监督运行。
完全兼容 GitHub Actions、Jenkins、GitLab CI
已准备好 Prometheus 指标端点，可直接接入监控系统
支持快速扩展新的硬件模块（WiFi、GPU、USB 等）

贡献指南
欢迎贡献！请参考 CONTRIBUTING.md 并提交 Issue/PR。
许可证
MIT License - 详见 LICENSE 文件。

专为半导体芯片制造商设计，加速可靠性测试并缩短产品上市时间。
最后更新：2026 年 3 月
