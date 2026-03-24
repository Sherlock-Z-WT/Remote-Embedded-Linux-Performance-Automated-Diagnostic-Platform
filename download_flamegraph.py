#!/usr/bin/env python3
import socket
import os

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

def get_flamegraph():
    """获取火焰图"""
    try:
        # 创建 socket 连接
        client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client.connect(("127.0.0.1", 8888))
        print("连接成功！")
        
        # 查找最新的火焰图文件
        print("查找最新的火焰图文件...")
        client.send("find_flamegraph".encode('utf-8'))
        flamegraph_path = client.recv(1024).decode('utf-8')
        
        if flamegraph_path and not flamegraph_path == "FILE_NOT_FOUND":
            print(f"找到火焰图文件: {flamegraph_path}")
            
            # 下载文件
            save_path = "latest_flamegraph.svg"
            print(f"下载文件到: {save_path}")
            download_file(client, flamegraph_path, save_path)
            
            # 查看文件
            print(f"火焰图已保存为: {save_path}")
            print("您可以在浏览器中打开此文件查看火焰图")
        else:
            print("未找到火焰图文件")
        
        # 断开连接
        client.close()
        print("已断开与 Agent 的连接")
        
    except Exception as e:
        print(f"连接失败: {e}")

if __name__ == "__main__":
    get_flamegraph()
