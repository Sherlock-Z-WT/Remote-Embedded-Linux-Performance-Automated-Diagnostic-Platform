import psutil
import time
import os
import socket
import threading
import subprocess
from datetime import datetime

# 配置区
CPU_THRESHOLD = 50.0  # CPU 阈值
MEM_THRESHOLD = 40.0  # 内存阈值
CHECK_INTERVAL = 2    # 检查间隔（秒）
REPORT_DIR = "reports"  # 报告存储目录
EMBEDDED_TEST_PATH = "./test_framework"  

# 确保报告目录存在
if not os.path.exists(REPORT_DIR):
    os.makedirs(REPORT_DIR, exist_ok=True)

def log_event(message):
    """打印带时间戳的日志"""
    timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
    print(f"[{timestamp}] {message}")

def capture_performance(reason):
    """执行性能诊断并生成报告"""
    timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
    folder = f"{REPORT_DIR}/{timestamp}_{reason}"
    os.makedirs(folder, exist_ok=True)

    log_event(f"触发诊断！原因：{reason}")

    # 1. 抓取 CPU 火焰图
    log_event("正在录制内核堆栈...")
    # 尝试执行 perf 命令，如果失败则生成模拟火焰图
    try:
        # 检查 FlameGraph 目录是否存在
        if not os.path.exists("~/FlameGraph"):
            log_event("FlameGraph 目录不存在，正在克隆...")
            os.system("git clone https://github.com/brendangregg/FlameGraph ~/FlameGraph")
        
        # 执行 perf 命令
        perf_cmd = f"perf record -a -g -- sleep 5 && perf script | ~/FlameGraph/stackcollapse-perf.pl | ~/FlameGraph/flamegraph.pl > {folder}/cpu_flame.svg"
        log_event(f"执行命令: {perf_cmd}")
        result = os.system(perf_cmd)
        
        # 检查命令执行结果
        if result != 0:
            log_event("perf 命令执行失败，生成模拟火焰图")
            # 生成模拟火焰图
            with open(f"{folder}/cpu_flame.svg", "w") as f:
                f.write("<svg xmlns='http://www.w3.org/2000/svg' width='1000' height='500'><text x='500' y='250' text-anchor='middle' font-size='24'>模拟火焰图 - CPU 压测</text></svg>")
        else:
            log_event("CPU 火焰图生成成功")
    except Exception as e:
        log_event(f"生成火焰图时出错：{e}")
        # 生成模拟火焰图
        with open(f"{folder}/cpu_flame.svg", "w") as f:
            f.write("<svg xmlns='http://www.w3.org/2000/svg' width='1000' height='500'><text x='500' y='250' text-anchor='middle' font-size='24'>模拟火焰图 - CPU 压测</text></svg>")

    # 2. 抓取当前进程内存排行
    log_event("正在记录内存快照")
    mem_cmd = f"ps aux --sort=-%mem | head -n 11 > {folder}/memory_top.txt"
    os.system(mem_cmd)

    log_event(f"诊断报告已存入：{folder}")
    return folder

def run_embedded_tests(test_args=""):
    """执行旧项目的测试"""
    if os.path.exists(EMBEDDED_TEST_PATH):
        log_event(f"执行测试：{EMBEDDED_TEST_PATH} {test_args}")
        # 执行测试并获取输出
        result = os.popen(f"{EMBEDDED_TEST_PATH} {test_args}").read()
        log_event("测试执行完成")
        # 保存测试结果
        timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
        test_log = f"{REPORT_DIR}/test_{timestamp}.log"
        with open(test_log, "w") as f:
            f.write(result)
        return f"测试完成，结果已保存至：{test_log}"
    else:
        log_event("旧项目测试文件不存在")
        return "旧项目测试文件不存在"

def get_test_info():
    """获取旧项目测试框架的信息"""
    if os.path.exists(EMBEDDED_TEST_PATH):
        log_event("获取测试框架信息")
        # 执行测试框架并获取帮助信息
        result = os.popen(f"{EMBEDDED_TEST_PATH} --help").read()
        return result
    else:
        log_event("旧项目测试文件不存在")
        return "旧项目测试文件不存在"

def find_latest_flamegraph():
    """查找最新生成的火焰图文件"""
    if not os.path.exists(REPORT_DIR):
        return None
    
    # 获取所有报告文件夹
    folders = [f for f in os.listdir(REPORT_DIR) if os.path.isdir(os.path.join(REPORT_DIR, f))]
    if not folders:
        return None
    
    # 按时间戳排序，获取最新的文件夹
    folders.sort(reverse=True)
    latest_folder = os.path.join(REPORT_DIR, folders[0])
    
    # 查找火焰图文件
    flamegraph_file = os.path.join(latest_folder, "cpu_flame.svg")
    if os.path.exists(flamegraph_file):
        return flamegraph_file
    return None

def handle_client(conn, addr):
    """处理客户端连接"""
    log_event(f"客户端 {addr} 已连接")
    try:
        while True:
            data = conn.recv(1024).decode('utf-8')
            if not data:
                break
            log_event(f"收到指令: {data}")
            
            # 处理指令
            if data.strip() == "START_STRESS_CPU":
                log_event("开始执行 CPU 压测...")
                # 执行 CPU 压测
                os.system("stress-ng --cpu 4 --timeout 60s")
                # 执行性能诊断
                capture_performance("CPU_Stress")
                conn.send("CPU 压测和性能诊断完成".encode('utf-8'))
            elif data.strip() == "start_stress":
                log_event("开始执行综合压测...")
                # 执行综合压测
                os.system("stress-ng --cpu 4 --io 2 --vm 1 --vm-bytes 1G --timeout 60s")
                log_event("压测完成")
                conn.send("压测完成".encode('utf-8'))
            elif data.strip() == "start_perf":
                log_event("开始性能诊断...")
                capture_performance("Manual_Trigger")
                conn.send("性能诊断完成".encode('utf-8'))
            elif data.strip() == "status":
                # 获取系统状态
                cpu_usage = psutil.cpu_percent(interval=1)
                mem_usage = psutil.virtual_memory().percent
                status = f"CPU: {cpu_usage}%, MEM: {mem_usage}%"
                conn.send(status.encode('utf-8'))
            elif data.strip() == "run_tests":
                log_event("开始执行旧项目测试...")
                # 执行旧项目的测试
                test_result = run_embedded_tests()
                conn.send(test_result.encode('utf-8'))
            elif data.strip().startswith("run_tests_with_args:"):
                # 执行旧项目的测试，带参数
                test_args = data.strip().split(":", 1)[1]
                log_event(f"开始执行旧项目测试（带参数）: {test_args}")
                test_result = run_embedded_tests(test_args)
                conn.send(test_result.encode('utf-8'))
            elif data.strip() == "get_test_info":
                # 获取测试框架信息
                log_event("获取测试框架信息...")
                test_info = get_test_info()
                conn.send(test_info.encode('utf-8'))
            elif data.strip() == "find_flamegraph":
                log_event("查找最新的火焰图文件...")
                flamegraph_path = find_latest_flamegraph()
                if flamegraph_path:
                    log_event(f"找到火焰图文件: {flamegraph_path}")
                    conn.send(flamegraph_path.encode('utf-8'))
                else:
                    log_event("未找到火焰图文件")
                    conn.send("FILE_NOT_FOUND".encode('utf-8'))
            elif data.strip().startswith("download:"):
                # 处理文件下载请求
                file_path = data.strip().split(":", 1)[1]
                if os.path.exists(file_path):
                    log_event(f"开始下载文件: {file_path}")
                    # 发送文件大小
                    file_size = os.path.getsize(file_path)
                    conn.send(f"FILE:{file_size}".encode('utf-8'))
                    # 等待客户端确认
                    conn.recv(1024)
                    # 发送文件内容
                    with open(file_path, "rb") as f:
                        while True:
                            chunk = f.read(1024)
                            if not chunk:
                                break
                            conn.send(chunk)
                    log_event(f"文件下载完成: {file_path}")
                else:
                    conn.send("FILE_NOT_FOUND".encode('utf-8'))
            else:
                conn.send("未知指令".encode('utf-8'))
    except Exception as e:
        log_event(f"客户端处理错误: {e}")
    finally:
        conn.close()
        log_event(f"客户端 {addr} 已断开")

def start_socket_server():
    """启动 Socket 服务器"""
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind(('0.0.0.0', 8888))
    server.listen(5)
    log_event("Socket 服务器已启动，监听端口 8888")
    
    while True:
        conn, addr = server.accept()
        client_thread = threading.Thread(target=handle_client, args=(conn, addr))
        client_thread.daemon = True
        client_thread.start()

def monitor_loop():
    """监控循环"""
    def capture_network_fault():
        """捕获网络故障"""
        timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
        log_file = f"{REPORT_DIR}/network_fault_{timestamp}.log"

        log_event(f"正在捕获网络故障...")
        # 记录当前网卡的统计信息和内核 dmesg 报错
        os.system(f"ip -s link show ens33 > {log_file}")
        os.system(f"dmesg | tail -n 20 >> {log_file}")
        log_event(f"网络快照已保存至：{log_file}")
        # 这里可以添加捕获网络故障的代码
        
    
    while True:
        try:
            # 获取 CPU 和内存的状态
            cpu_usage = psutil.cpu_percent(interval=1)
            mem_usage = psutil.virtual_memory().percent
            print(f"Debug: Current CPU: {cpu_usage}%, MEM: {mem_usage}%")
            print(f"--- 实时监控中: CPU: {cpu_usage}% | 内存: {mem_usage}% | 阈值: CPU>{CPU_THRESHOLD} MEM>{MEM_THRESHOLD} ---")
            # 策略判断
            if cpu_usage > CPU_THRESHOLD:
                capture_performance("High_CPU")
            elif mem_usage > MEM_THRESHOLD:
                capture_performance("High_MEM")
            
            time.sleep(CHECK_INTERVAL)
        except KeyboardInterrupt:
            log_event("监控停止")
            break

if __name__ == "__main__":
    # 启动 Socket 服务器线程
    server_thread = threading.Thread(target=start_socket_server)
    server_thread.daemon = True
    server_thread.start()
    
    # 启动监控循环
    monitor_loop()
