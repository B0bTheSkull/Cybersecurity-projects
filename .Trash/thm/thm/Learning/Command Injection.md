---
title: Command Injection — TryHackMe
tags:
slug: Command Injection
summary: Command injection is also often known as “Remote Code Execution” (RCE) because of the ability to remotely execute code within an application. These vulnerabilities are often the most lucrative to an attacker because it means that the attacker can directly interact with the vulnerable system. For example, an attacker may read system or user files, data, and things of that nature.
---
11-24-2025
10:23
No production, public, or third-party networks were accessed, disrupted, or monitored.
## Introduction

Command injection is also often known as “Remote Code Execution” (RCE) because of the ability to remotely execute code within an application. These vulnerabilities are often the most lucrative to an attacker because it means that the attacker can directly interact with the vulnerable system. For example, an attacker may read system or user files, data, and things of that nature.

SQL Injection, RCE, got all the names.

## Content

Command Injection can be detected in mostly one of two ways:
1) Blind command injection
2) Verbose command injection

| **Method** | **Description**                                                                                                                                                                                                                                                               |
| ---------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Blind      | This type of injection is where there is no direct output from the application when testing payloads. You will have to investigate the behaviours of the application to determine whether or not your payload was successful.                                                 |
| Verbose    | This type of injection is where there is direct feedback from the application once you have tested a payload. For example, running the `whoami` command to see what user the application is running under. The web application will output the username on the page directly. |

With blind command injection you'll need to utilize payloads that will cause some time delay. `ping` and `sleep` commands are significant payloads to test with. Via the use of `ping` the application will hang for x seconds in relation to how many pings you have specified. 

We can also leverage redirection operators such as `>`. For example, you can tell a web application to execute the command `whoami` and redirect that to a file. We can then utilize the command such as `cat` to read this newly created file's contents and display it on the web browser. This is confirmation that RCE or command injection has occurred.

The `curl` command is also a very useful tool for testing command injection. You are able to use `curl` to deliver data to and from an application in your payload. 

E.g., `curl http://bobtheskull.org/process.php%3Fsearch%3DThe%20Beatles%3B%20whoami`

Detecting *command injection*  this way is arguably the easiest method of the two. `Verbose command injection` is when the application gives you feedback or output as to what is happening or being executed.

For example the outputs of those `ping` or `whoami` commands are displayed directly on the web application itself.

**Useful payloads**

### Linux

| **Payload** | **Description**                                                                                                                                                                                                      |
| ----------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| whoami      | See what user the application is running under.                                                                                                                                                                      |
| ls          | List the contents of the current directory. You may be able to find files such as configuration files, environment files (tokens and application keys), and many more valuable things.                               |
| ping        | This command will invoke the application to hang. This will be useful in testing an application for blind command injection.                                                                                         |
| sleep       | This is another useful payload in testing an application for blind command injection, where the machine does not have `ping` installed.                                                                              |
| nc          | Netcat can be used to spawn a reverse shell onto the vulnerable application. You can use this foothold to navigate around the target machine for other services, files, or potential means of escalating privileges. |

### Windows

| **Payload** | **Description**                                                                                                                                                                        |
| ----------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| whoami      | See what user the application is running under.                                                                                                                                        |
| dir         | List the contents of the current directory. You may be able to find files such as configuration files, environment files (tokens and application keys), and many more valuable things. |
| ping        | This command will invoke the application to hang. This will be useful in testing an application for blind command injection.                                                           |
| timeout     | This command will also invoke the application to hang. It is also useful for testing an application for blind command injection if the `ping` command is not installed.                |
|             |                                                                                                                                                                                        |
### Vulnerable Functions

Command injection prevention can be seen in a variety of ways. Everything from minimal use of potentially dangerous functions or libraries in a programming language to filtering input without relying on a user's input.

For example in the PHP language:
- exec
- passthru
- system

All of these functions interact with the operating system to execute commands via shell. These functions take input such as a string or user data and will execute whatever is provided on the system. Any application that uses these functions without proper checks will be vulnerable to command injection.

### Input Sanitization

Sanitizing the input from a user is a great way to prevent command injection. This is a process of specifying the formats or types of data that a user can submit. For example, the input field can only accept numerical data or removes the special characters such as `>`, `&` and `/`. (This is my most common way of sanitizing data).

### Bypassing Filters

Applications will employ numerous techniques in filtering and sanitizing data that is taken from a user's input. These filters will restrict you to specific payloads; however, we can abuse the logic behind an application to bypass these filters. If an application strips out quotation marks; we can use the hexadecimal value of this to achieve the exact same result. 

When executed, although the data we give the application will be in a different format than what is expected, it can still be interpreted and will have the same result.

```
$payload = "\x2f\x65\x74\x63\x2f\x70\x61\x73\x73\x77\x64"
```

https://github.com/payloadbox/command-injection-payload-list

The way I was able to complete this room was by utilizing a `command & command`.
## Skills Learned