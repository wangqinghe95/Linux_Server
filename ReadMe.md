# Server

## Introduction

## Version

| Name | Link | brief | remake |
| CppServer | [30dayMakeCppServer](https://github.com/yuesong-feng/30dayMakeCppServer/blob/main/code/day07/src/Acceptor.cpp) | | None |

## Others
MySocket、MyEpoll、MyChannel 三个类中打开的 fd 会有内存泄露的风险
server.cpp 中的 client_socket 因为上述中对 fd 管辖权不明的原因无法 delete client_socket 指针