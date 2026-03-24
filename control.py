import socket
import time
import os
from datetime import datetime

def download_file(sock, file_path, save_path):
    """下载文件"""
    # 发送下载请求
    sock.send(f"download:{file_path}".encode('utf-8'))
    
    # 接收响应
    response = sock.recv(1024).decode('utf-8')
    if response.startswith("FILE:"):
        file_size = int(response.split(":")[1])
        print(f"开始下载文件，大小: {file_size} bytes")
        
        # 发送确认
        sock.send("OK".encode('utf-8'))
        
        # 接收文件内容
        received_size = 0
        with open(save_path, "wb") as f:
            while received_size < file_size:
                chunk = sock.recv(1024)
                if not chunk:
                    break
                f.write(chunk)
                received_size += len(chunk)
                # 显示进度
                progress = (received_size / file_size) * 100
                print(f"下载进度: {progress:.2f}%", end="\r")
        print("\n文件下载完成")
        return True
    else:
        print(f"下载失败: {response}")
        return False

def control_client():
    """Windows 控制端客户端"""
    print("=== Remote Linux Performance Diagnostic Platform Control Client ===")
    
    while True:
        # 获取用户输入
        print("\n请选择操作:")
        print("1. 连接到 Agent")
        print("2. 发送 START_STRESS_CPU 指令")
        print("3. 发送 start_stress 指令")
        print("4. 发送 start_perf 指令")
        print("5. 查询系统状态")
        print("6. 执行旧项目测试")
        print("7. 退出")
        
        choice = input("请输入选项编号: ")
        
        if choice == "1":
            # 连接到 Agent
            try:
                host = input("请输入 Agent 服务器 IP: ")
                port = 8888
                
                client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                client.connect((host, port))
                print(f"成功连接到 Agent: {host}:{port}")
                
                # 保持连接，等待后续指令
                while True:
                    print("\n已连接到 Agent，可发送指令:")
                    print("1. 开始 CPU 压测 (START_STRESS_CPU)")
                    print("2. 开始综合压测 (start_stress)")
                    print("3. 开始性能诊断 (start_perf)")
                    print("4. 查询系统状态 (status)")
                    print("5. 执行旧项目测试 (run_tests)")
                    print("6. 执行旧项目测试（带参数）(run_tests_with_args)")
                    print("7. 获取测试框架信息 (get_test_info)")
                    print("8. 下载最新火焰图")
                    print("9. 断开连接")
                    
                    sub_choice = input("请输入选项编号: ")
                    
                    if sub_choice == "1":
                        # 发送 START_STRESS_CPU 指令
                        print("发送 START_STRESS_CPU 指令...")
                        client.send("START_STRESS_CPU".encode('utf-8'))
                        response = client.recv(1024).decode('utf-8')
                        print(f"Agent 响应: {response}")
                        
                        # 等待 15 秒
                        print("等待 15 秒...")
                        time.sleep(15)
                        
                        # 查找最新的火焰图文件
                        print("查找最新的火焰图文件...")
                        client.send("find_flamegraph".encode('utf-8'))
                        flamegraph_path = client.recv(1024).decode('utf-8')
                        
                        if flamegraph_path and not flamegraph_path == "FILE_NOT_FOUND":
                            print(f"找到火焰图文件: {flamegraph_path}")
                            
                            # 下载文件
                            save_path = f"cpu_flamegraph_{datetime.now().strftime('%Y%m%d_%H%M%S')}.svg"
                            print(f"下载文件到: {save_path}")
                            download_file(client, flamegraph_path, save_path)
                        else:
                            print("未找到火焰图文件")
                    elif sub_choice == "2":
                        # 发送 start_stress 指令
                        print("发送 start_stress 指令...")
                        client.send("start_stress".encode('utf-8'))
                        response = client.recv(1024).decode('utf-8')
                        print(f"Agent 响应: {response}")
                    elif sub_choice == "3":
                        # 发送 start_perf 指令
                        print("发送 start_perf 指令...")
                        client.send("start_perf".encode('utf-8'))
                        response = client.recv(1024).decode('utf-8')
                        print(f"Agent 响应: {response}")
                    elif sub_choice == "4":
                        # 查询系统状态
                        print("发送 status 指令...")
                        client.send("status".encode('utf-8'))
                        response = client.recv(1024).decode('utf-8')
                        print(f"系统状态: {response}")
                    elif sub_choice == "5":
                        # 执行旧项目测试
                        print("发送 run_tests 指令...")
                        client.send("run_tests".encode('utf-8'))
                        response = client.recv(1024).decode('utf-8')
                        print(f"Agent 响应: {response}")
                    elif sub_choice == "6":
                        # 执行旧项目测试（带参数）
                        test_args = input("请输入测试参数: ")
                        print(f"发送 run_tests_with_args:{test_args} 指令...")
                        client.send(f"run_tests_with_args:{test_args}".encode('utf-8'))
                        response = client.recv(1024).decode('utf-8')
                        print(f"Agent 响应: {response}")
                    elif sub_choice == "7":
                        # 获取测试框架信息
                        print("发送 get_test_info 指令...")
                        client.send("get_test_info".encode('utf-8'))
                        response = client.recv(4096).decode('utf-8')
                        print(f"测试框架信息:\n{response}")
                    elif sub_choice == "8":
                        # 下载最新火焰图
                        print("查找最新的火焰图文件...")
                        client.send("find_flamegraph".encode('utf-8'))
                        flamegraph_path = client.recv(1024).decode('utf-8')
                        
                        if flamegraph_path and not flamegraph_path == "FILE_NOT_FOUND":
                            print(f"找到火焰图文件: {flamegraph_path}")
                            
                            # 下载文件
                            save_path = f"cpu_flamegraph_{datetime.now().strftime('%Y%m%d_%H%M%S')}.svg"
                            print(f"下载文件到: {save_path}")
                            download_file(client, flamegraph_path, save_path)
                        else:
                            print("未找到火焰图文件")
                    elif sub_choice == "9":
                        # 断开连接
                        client.close()
                        print("已断开与 Agent 的连接")
                        break
                    else:
                        print("无效选项，请重新输入")
                        
            except Exception as e:
                print(f"连接错误: {e}")
                
        elif choice == "7":
            # 退出
            print("退出控制客户端")
            break
        else:
            print("无效选项，请重新输入")

if __name__ == "__main__":
    control_client()
